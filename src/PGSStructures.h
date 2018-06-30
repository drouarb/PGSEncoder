//
// Created by Benjamin DROUARD on 18/06/2018.
//

#ifndef PGSENCODER_PGSSTRUCTURES_H
#define PGSENCODER_PGSSTRUCTURES_H

#include <unistd.h>
#include "BigEdianTypes.h"

#define write_struct(fd, obj) write(fd, &obj, sizeof(obj))

/**
 * PGS Segment Header
 */

typedef struct __attribute__ ((packed)) {
    be_uint16_t magic = 0x5047;
    be_uint32_t pts;
    be_uint32_t dts = 0;
    be_uint8_t  segment_type;
    be_uint16_t segment_size;
} PGSHeader_t;

#define SEGMENT_TYPE_PDS 0x14
#define SEGMENT_TYPE_ODS 0x15
#define SEGMENT_TYPE_PCS 0x16
#define SEGMENT_TYPE_WDS 0x17
#define SEGMENT_TYPE_END 0x80

/**
 * Presentation Composition Segment
 */

typedef struct __attribute__ ((packed)) {
    be_uint16_t width;
    be_uint16_t height;
    be_uint8_t  framerate = 0x10;
    be_uint16_t composition_number;
    be_uint8_t  composition_state;
    be_uint8_t  palette_update_flag;
    be_uint8_t  palette_id;
    be_uint8_t  composition_object_count;
} PCSSegment_t;

#define COMPOSITION_STATE_NORMAL            0x00
#define COMPOSITION_STATE_ACQUISITION_POINT 0x40
#define COMPOSITION_STATE_EPOCH_START       0x80

#define PALETTE_UPDATE_FLAG_TRUE  0x80
#define PALETTE_UPDATE_FLAG_FALSE 0x00

typedef struct __attribute__ ((packed)) {
    be_uint16_t object_id;
    be_uint8_t  window_id;
    be_uint8_t  object_cropped_flag;
    be_uint16_t object_x;
    be_uint16_t object_y;
} CompositionObject_t;

#define OBJECT_CROPPED_FLAG_FORCE 0x40
#define OBJECT_CROPPED_FLAG_OFF   0x00

typedef struct __attribute__ ((packed)) {
    be_uint16_t object_cropping_horizontal;
    be_uint16_t object_cropping_vertical;
    be_uint16_t object_cropping_width;
    be_uint16_t object_cropping_height;
} CompositionObjectCropping_t;

/**
 * Window Definition Segment
 */

typedef struct __attribute__ ((packed)) {
    be_uint8_t  window_count = 1;
    be_uint8_t  id = 0;
    be_uint16_t x;
    be_uint16_t y;
    be_uint16_t width;
    be_uint16_t height;
} WDSSegment_t;

/**
 * Palette Definition Segment
 */

typedef struct __attribute__ ((packed)) {
    be_uint8_t palette_id;
    be_uint8_t palette_version = 0;
} PDSSegment_t;

typedef struct __attribute__ ((packed)) {
    be_uint8_t palette_entry_id;
    be_uint8_t y;
    be_uint8_t cr;
    be_uint8_t cb;
    be_uint8_t alpha;
} PDSPalette_t;

/**
 * Object Definition Segment
 */

typedef struct __attribute__ ((packed)) {
    be_uint16_t id;
    be_uint8_t  version;
    be_uint8_t  sequence_flag;
    be_uint24_t data_length;
    be_uint16_t width;
    be_uint16_t height;
} ODSSegment_t;

#define ODS_SEQUENCE_LAST  0x40
#define ODS_SEQUENCE_FIRST 0x80
#define ODS_SEQUENCE_BOTH  (ODS_SEQUENCE_LAST | ODS_SEQUENCE_FIRST)

#endif //PGSENCODER_PGSSTRUCTURES_H
