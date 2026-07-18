#pragma once

#include <span>

#include "types.h"
#include "ueformat/api/generic/result.h"
#include "ueformat/anim/anim.h"

#ifdef __cplusplus

namespace UEFormat::API
{
    UEAnim FromDesc(const UEFormatAnimDesc& desc);

    Result<ByteBuffer> Save(const UEFormatAnimDesc& desc, const FSaveOptions& options);
    Result<ByteBuffer> Save(const UEAnim& anim, const FSaveOptions& options);
    Result<UEAnim> LoadAnim(std::span<const u8> data);
}

#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UEFormatAnim UEFormatAnim;

typedef struct UEFormatAnimResult {
    UEFormatStatus status;
    UEFormatAnim* anim;
    const char* error;
} UEFormatAnimResult;

UEFORMAT_API UEFormatBufferResult ueformat_save_anim(
    const UEFormatAnimDesc* anim,
    const UEFormatSaveOptions* options);

UEFORMAT_API UEFormatAnimResult ueformat_load_anim(
    const uint8_t* data,
    size_t size);

UEFORMAT_API void ueformat_anim_free(UEFormatAnim* anim);

UEFORMAT_API UEFormatBufferResult ueformat_anim_save(
    const UEFormatAnim* anim,
    const UEFormatSaveOptions* options);

#ifdef __cplusplus
}
#endif
