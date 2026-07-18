#pragma once

#include "ueformat/api/generic/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UEFormatPoseKeyDesc {
    const char* bone_name;
    UEFormatVector location;
    UEFormatQuat rotation;
    UEFormatVector scale;
} UEFormatPoseKeyDesc;

typedef struct UEFormatPoseCurveInfluenceDesc {
    int32_t curve_index;
    float influence;
} UEFormatPoseCurveInfluenceDesc;

typedef struct UEFormatPoseDataDesc {
    const char* pose_name;
    const UEFormatPoseKeyDesc* keys;
    int32_t key_count;
    const UEFormatPoseCurveInfluenceDesc* curves;
    int32_t curve_count;
} UEFormatPoseDataDesc;

typedef struct UEFormatPoseDesc {
    const UEFormatPoseDataDesc* poses;
    int32_t pose_count;
    const char* const* curve_names;
    int32_t curve_name_count;
} UEFormatPoseDesc;

#ifdef __cplusplus
}
#endif
