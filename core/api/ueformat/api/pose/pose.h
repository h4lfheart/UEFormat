#pragma once

#include <span>

#include "types.h"
#include "ueformat/api/generic/result.h"
#include "ueformat/pose/pose.h"

#ifdef __cplusplus

namespace UEFormat::API
{
    UEPose FromDesc(const UEFormatPoseDesc& desc);

    Result<ByteBuffer> Save(const UEFormatPoseDesc& desc, const FSaveOptions& options);
    Result<ByteBuffer> Save(const UEPose& pose, const FSaveOptions& options);
    Result<UEPose> LoadPose(std::span<const u8> data);
}

#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UEFormatPose UEFormatPose;

typedef struct UEFormatPoseResult {
    UEFormatStatus status;
    UEFormatPose* pose;
    const char* error;
} UEFormatPoseResult;

UEFORMAT_API UEFormatBufferResult ueformat_save_pose(
    const UEFormatPoseDesc* pose,
    const UEFormatSaveOptions* options);

UEFORMAT_API UEFormatPoseResult ueformat_load_pose(
    const uint8_t* data,
    size_t size);

UEFORMAT_API void ueformat_pose_free(UEFormatPose* pose);

UEFORMAT_API UEFormatBufferResult ueformat_pose_save(
    const UEFormatPose* pose,
    const UEFormatSaveOptions* options);

#ifdef __cplusplus
}
#endif
