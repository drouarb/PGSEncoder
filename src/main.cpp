#include <iostream>
#include <fcntl.h>
#include <zconf.h>
#include "PGSStructures.h"
#include "PNGImage.h"
#include "Palette.h"

void generatePCS(int fd, unsigned int pts, int width, int height, int x, int y, int composition_number) {
    PGSHeader_t         header;
    PCSSegment_t        segment;
    CompositionObject_t compositionObject;

    segment.width = width;
    segment.height = height;
    segment.composition_number = composition_number;
    segment.composition_state = COMPOSITION_STATE_EPOCH_START;
    segment.palette_update_flag = PALETTE_UPDATE_FLAG_FALSE;
    segment.palette_id = 0;
    segment.composition_object_count = 1;

    compositionObject.object_id = 0;
    compositionObject.window_id = 0;
    compositionObject.object_cropped_flag = OBJECT_CROPPED_FLAG_OFF;
    //TODO fill
    compositionObject.object_y = y;
    compositionObject.object_x = x;

    header.pts = pts;
    header.segment_type = SEGMENT_TYPE_PCS;
    header.segment_size = sizeof(segment) + sizeof(compositionObject);

    write_struct(fd, header);
    write_struct(fd, segment);
    write_struct(fd, compositionObject);
}

void generateWDS(int fd, unsigned int pts, int x, int y, int width, int height) {
    PGSHeader_t  header;
    WDSSegment_t window;

    header.pts = pts;
    header.segment_type = SEGMENT_TYPE_WDS;
    header.segment_size = sizeof(window);

    window.x = x;
    window.y = y;
    window.width = width;
    window.height = height;

    write_struct(fd, header);
    write_struct(fd, window);
}

void RLEEncode(unsigned char color, int run, std::vector<unsigned char> &data_buffer, Palette &palette) {
    if (run > 1) {
        data_buffer.emplace_back(0);
        //If it is not the default color we need to apply the mask 0b10000000
        unsigned char colorFlag = (color > 0 ? 0x80 : 0x00);

        //If it is longer than 64 (more that 5 bits) we need to apply the mask 0b01000000
        if (run < 64) {
            data_buffer.emplace_back((run & 0x3F) | 0x00 | colorFlag);
        } else {
            data_buffer.emplace_back(((run >> 8) & 0x3F) | 0x40 | colorFlag);
            data_buffer.emplace_back(run & 0xFF);
        }

        //We apply the color
        if (color > 0)
            data_buffer.emplace_back(color);
    } else {
        //We have only one pixel
        if (color == 0) {
            //It's our default color, need to find a substitute
            data_buffer.emplace_back(palette.getDefaultColorReplacement());
        } else {
            data_buffer.emplace_back(color);
        }
    }
}

void generateODS(int fd, unsigned int timestamp, PNGImage &image, Palette &palette) {
    PGSHeader_t  header;
    ODSSegment_t object;
    std::vector<unsigned char> data_buffer;

    header.pts = timestamp;
    header.segment_type = SEGMENT_TYPE_ODS;

    object.id = 0;
    object.version = 0;
    object.sequence_flag = ODS_SEQUENCE_BOTH;
    object.width = image.getWidth();
    object.height = image.getHeight();

    for (int y = 0; y < image.getHeight(); y++) {
        int run = 0;
        unsigned int lastpx = 0;
        png_bytep row = image.getPixels()[y];

        for (int x = 0; x < image.getWidth(); x++) {
            png_bytep pngpx = &(row[x * 4]);
            unsigned int px = getRGBA(pngpx[0], pngpx[1], pngpx[2], pngpx[3]);

            if (x == 0 || px != lastpx) {
                if (x != 0) {
                    RLEEncode(palette.getColor(lastpx), run, data_buffer, palette);
                }
                run = 0;
                lastpx = px;
            }
            run++;
        }
        //Flush last
        RLEEncode(palette.getColor(lastpx), run, data_buffer, palette);
        data_buffer.emplace_back(0);
        data_buffer.emplace_back(0);
    }

    header.segment_size = sizeof(object) + data_buffer.size();
    object.data_length = data_buffer.size() + 4; // WTF -4 VLC

    write_struct(fd, header);
    write_struct(fd, object);
    write(fd, data_buffer.data(), data_buffer.size());
}

void generateEND(int fd, unsigned int pts) {
    PGSHeader_t header;

    header.pts = pts;
    header.segment_type = SEGMENT_TYPE_END;
    header.segment_size = 0;

    write_struct(fd, header);
}

void cleanScreen(int fd, int pts, int width, int height, int composition_number) {
    PGSHeader_t         header;
    PCSSegment_t        segment;

    segment.width = width;
    segment.height = height;
    segment.composition_number = composition_number;

    segment.composition_state = COMPOSITION_STATE_NORMAL;
    segment.palette_update_flag = PALETTE_UPDATE_FLAG_FALSE;
    segment.palette_id = 0;
    segment.composition_object_count = 0;

    header.pts = pts;
    header.segment_type = SEGMENT_TYPE_PCS;
    header.segment_size = sizeof(segment);

    write_struct(fd, header);
    write_struct(fd, segment);
}

int main(int argc, char **argv) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " output.sup width height" << std::endl;
        std::cout << "\tInsert one frame per line on the following format:" << std::endl;
        std::cout << "\t<timestamp> <x position> <y position> <path to image|clean>" << std::endl;
        return 0;
    }

    int fd = open(argv[1], O_WRONLY | O_CREAT, S_IRWXU);
    int width = std::stoi(argv[2]);
    int height = std::stoi(argv[3]);

    if (fd < 0) {
        std::cout << "Can't open " << argv[1] << std::endl;
        return 0;
    }

    int x;
    int y;
    int window_width;
    int window_height;
    unsigned int timestamp;
    std::string image_path;
    int composition_number = 0;

    while (std::cin >> timestamp >> x >> y >> image_path) {
        std::cout << timestamp << " " << x << " " << y << " " << image_path << std::endl;

        if (image_path != "clean") {
            PNGImage image(image_path);
            Palette palette(image);

            generatePCS(fd, timestamp, width, height, x, y, composition_number);
            generateWDS(fd, timestamp, x, y, image.getWidth(), image.getHeight());
            palette.generatePalette(fd, timestamp);
            generateODS(fd, timestamp, image, palette);
            generateEND(fd, timestamp);

            window_width = image.getWidth();
            window_height = image.getHeight();
        } else {
            cleanScreen(fd, timestamp, width, height, composition_number);
            generateWDS(fd, timestamp, x, y, window_width, window_height);
            generateEND(fd, timestamp);
            composition_number++;
        }
    }

    /*generatePCS(fd, 0x43DFFA, 1920, 1080, 0);
    generateWDS(fd, 0x43DFFA);
    generatePDS(fd, 0x43DFFA, 0x80, 0xFF);
    generateODS(fd, 0x43DFFA, 0);
    generateEND(fd, 0x43DFFA);*/
    close(fd);
}