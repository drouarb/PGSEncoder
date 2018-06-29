#include <iostream>
#include <fcntl.h>
#include <zconf.h>
#include "PGSStructures.h"

void generatePCS(int fd, int pts, int width, int height, int composition_number) {
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
    compositionObject.object_y = 100;
    compositionObject.object_x = 100;

    header.pts = pts;
    header.segment_type = SEGMENT_TYPE_PCS;
    header.segment_size = sizeof(segment) + sizeof(compositionObject);

    write_struct(fd, header);
    write_struct(fd, segment);
    write_struct(fd, compositionObject);
}

void generateWDS(int fd, int pts) {
    PGSHeader_t  header;
    WDSSegment_t window;

    //TODO FILL
    window.x = 100;
    window.y = 100;
    window.width = 100;
    window.height = 100;

    header.pts = pts;
    header.segment_type = SEGMENT_TYPE_WDS;
    header.segment_size = sizeof(window);

    write_struct(fd, header);
    write_struct(fd, window);
}

void generatePDS(int fd, int pts, int y1, int y2) {
    PGSHeader_t  header;
    PDSSegment_t pds;
    PDSPalette_t palette1;
    PDSPalette_t palette2;

    pds.palette_id = 0;
    pds.palette_version = 0;

    palette1.palette_entry_id = 0;
    palette1.y = y1;
    palette1.cb = 0x80;
    palette1.cr = 0x80;
    palette1.alpha = 0xFF;

    palette2.palette_entry_id = 1;
    palette2.y = y2;
    palette2.cb = 0x80;
    palette2.cr = 0x80;
    palette2.alpha = 0xFF;

    header.pts = pts;
    header.segment_type = SEGMENT_TYPE_PDS;
    header.segment_size = sizeof(pds) + sizeof(palette1) * 2;

    write_struct(fd, header);
    write_struct(fd, pds);
    write_struct(fd, palette1);
    write_struct(fd, palette2);
}

void generateODS(int fd, int pts, int color) {
    PGSHeader_t  header;
    ODSSegment_t object;

    object.id = 0;
    object.version = 0;
    object.sequence_flag = ODS_SEQUENCE_BOTH;
    object.width = 945;
    object.height = 61;
    object.data_length = (61 * 6) + 4; //Should add 4 for width & height shorts

    header.pts = pts;
    header.segment_type = SEGMENT_TYPE_ODS;
    header.segment_size = sizeof(object) + (61 * 6);

    write_struct(fd, header);
    write_struct(fd, object);

    for (int i = 0; i < 61; i ++) {
        be_uint8_t code;

        code = 0;
        write_struct(fd, code);

        //================
        code = 0xC0 | 0x03;
        write_struct(fd, code);

        code = 0xB1;
        write_struct(fd, code);

        code = 2;
        write_struct(fd, code);

        //================
        code = 0;
        write_struct(fd, code);

        code = 0;
        write_struct(fd, code);
    }
}

void generateEND(int fd, int pts) {
    PGSHeader_t header;

    header.pts = pts;
    header.segment_type = SEGMENT_TYPE_END;
    header.segment_size = 0;

    write_struct(fd, header);
}

void cleanScreen(int fd, int pts, int composition_number) {
    PGSHeader_t         header;
    PCSSegment_t        segment;

    segment.width = 1280;
    segment.height = 720;
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
    generateWDS(fd, pts);
    generateEND(fd, pts);
}

int main() {
    int fd = open("out.sup", O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
    /*generatePCS(fd, 100, 1280, 720, 0);
    generateWDS(fd, 100);
    generatePDS(fd, 100, 0x80, 0xFF);
    generateODS(fd, 100, 0);
    generateEND(fd, 100);
    cleanScreen(fd, 10000000, 1);*/

    generateODS(fd, 0x43DFFA, 0);
    generateEND(fd, 0x43DFFA);

    /*generatePCS(fd, 20000500, 1280, 720, 2);
    generateWDS(fd, 20000500);
    generatePDS(fd, 20000500, 0xFE, 0x81);
    generateODS(fd, 20000500, 1);
    generateEND(fd, 20000500);
    cleanScreen(fd, 30000000, 3);

    generatePCS(fd, 40000500, 1280, 720, 4);
    generateWDS(fd, 40000500);
    generatePDS(fd, 40000500, 0x82, 0xFD);
    generateODS(fd, 40000500, 0);
    generateEND(fd, 40000500);
    cleanScreen(fd, 50000000, 5);*/

    close(fd);
}