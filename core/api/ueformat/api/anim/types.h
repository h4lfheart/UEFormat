#pragma once

#include "ueformat/api/generic/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UEFormatVectorKeyDesc {
    int32_t frame;
    UEFormatVector value;
} UEFormatVectorKeyDesc;

typedef struct UEFormatQuatKeyDesc {
    int32_t frame;
    UEFormatQuat value;
} UEFormatQuatKeyDesc;

typedef struct UEFormatFloatKeyDesc {
    int32_t frame;
    float value;
} UEFormatFloatKeyDesc;

typedef struct UEFormatAnimMetadataDesc {
    int32_t num_frames;
    float frames_per_second;
    const char* ref_pose_path;
    uint8_t additive_anim_type;
    uint8_t ref_pose_type;
    int32_t ref_frame_index;
} UEFormatAnimMetadataDesc;

typedef struct UEFormatTrackDesc {
    const char* bone_name;
    const UEFormatVectorKeyDesc* position_keys;
    int32_t position_key_count;
    const UEFormatQuatKeyDesc* rotation_keys;
    int32_t rotation_key_count;
    const UEFormatVectorKeyDesc* scale_keys;
    int32_t scale_key_count;
} UEFormatTrackDesc;

typedef struct UEFormatCurveDesc {
    const char* curve_name;
    const UEFormatFloatKeyDesc* keys;
    int32_t key_count;
} UEFormatCurveDesc;

typedef struct UEFormatAnimDesc {
    UEFormatAnimMetadataDesc metadata;
    const UEFormatTrackDesc* tracks;
    int32_t track_count;
    const UEFormatCurveDesc* curves;
    int32_t curve_count;
} UEFormatAnimDesc;

#ifdef __cplusplus
}
#endif
