#include <iostream>
#include <fcntl.h>
#include <zconf.h>
#include "PGSStructures.h"

void generatePCS(int fd, int width, int height, int composition_number) {
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
    compositionObject.object_y = 0;
    compositionObject.object_x = 0;

    header.pts = 100;
    header.segment_type = SEGMENT_TYPE_PCS;
    header.segment_size = sizeof(segment) + sizeof(compositionObject);

    write_struct(fd, header);
    write_struct(fd, segment);
    write_struct(fd, compositionObject);
}

void generateWDS(int fd) {
    PGSHeader_t  header;
    WDSSegment_t window;

    window.id = 0;
    //TODO FILL
    window.y = 0;
    window.x = 0;
    window.width = 100;
    window.height = 100;

    header.pts = 100;
    header.segment_type = SEGMENT_TYPE_WDS;
    header.segment_size = sizeof(window);

    write_struct(fd, header);
    write_struct(fd, window);
}

void generatePDS(int fd) {
    PGSHeader_t  header;
    PDSSegment_t pds;
    PDSPalette_t palette;

    pds.palette_id = 0;
    pds.palette_version = 0;

    palette.palette_entry_id = 0;
    palette.y = 10;
    palette.cb = 80;
    palette.cr = 80;
    palette.alpha = 0;

    header.pts = 100;
    header.segment_type = SEGMENT_TYPE_PDS;
    header.segment_size = sizeof(pds) + sizeof(palette);

    write_struct(fd, header);
    write_struct(fd, pds);
    write_struct(fd, palette);
}

void generateODS(int fd) {
    PGSHeader_t  header;
    ODSSegment_t object;

    object.id = 0;
    object.version = 0;
    object.sequence_flag = ODS_SEQUENCE_BOTH;
    object.width = 100;
    object.height = 100;

    header.pts = 100;
    header.segment_type = SEGMENT_TYPE_ODS;
    header.segment_size = sizeof(object) + (600);

    write_struct(fd, header);
    write_struct(fd, object);

    for (int i = 0; i < 100; i ++) {
        be_uint8_t code;

        code = 0;
        write_struct(fd, code);

        //================
        code = 0xC0;
        write_struct(fd, code);

        code = 0x64;
        write_struct(fd, code);

        code = 0;
        write_struct(fd, code);

        //================
        code = 0;
        write_struct(fd, code);

        code = 0;
        write_struct(fd, code);
    }
}

void generateEND(int fd) {
    PGSHeader_t header;

    header.pts = 100000;
    header.segment_type = SEGMENT_TYPE_END;
    header.segment_size = 0;

    write_struct(fd, header);
}

int main() {
    int fd = open("out.sup", O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
    generatePCS(fd, 1920, 1080, 1);
    generateWDS(fd);
    generatePDS(fd);
    generateODS(fd);
    generateEND(fd);
    close(fd);
}