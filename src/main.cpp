#include <iostream>
#include <fcntl.h>
#include <zconf.h>
#include "PGSStructures.h"

void generatePCS(int fd, int width, int height int composition_number) {
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
    compositionObject.object_vertical_position = 0;
    compositionObject.object_horizontal_position = 0;

    header.pts = 0;
    header.segment_type = 0x16;
    header.segment_size = sizeof(segment) + sizeof(compositionObject);

    write_struct(fd, header);
    write_struct(fd, segment);
    write_struct(fd, compositionObject);
}

int main() {
    int fd = open("out.sup", O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
    generatePCS(fd, 1920, 1080, 1);
    close(fd);
}