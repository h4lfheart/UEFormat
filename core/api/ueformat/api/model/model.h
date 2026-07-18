#pragma once

#include <span>

#include "types.h"
#include "ueformat/api/generic/result.h"
#include "ueformat/model/model.h"

#ifdef __cplusplus

namespace UEFormat::API
{
    UEModel FromDesc(const UEFormatModelDesc& desc);

    Result<ByteBuffer> Save(const UEFormatModelDesc& desc, const FSaveOptions& options);
    Result<ByteBuffer> Save(const UEModel& model, const FSaveOptions& options);
    Result<UEModel> LoadModel(std::span<const u8> data);
}

#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UEFormatModel UEFormatModel;

typedef struct UEFormatModelResult {
    UEFormatStatus status;
    UEFormatModel* model;
    const char* error;
} UEFormatModelResult;

UEFORMAT_API UEFormatBufferResult ueformat_save_model(
    const UEFormatModelDesc* model,
    const UEFormatSaveOptions* options);

UEFORMAT_API UEFormatModelResult ueformat_load_model(
    const uint8_t* data,
    size_t size);

UEFORMAT_API void ueformat_model_free(UEFormatModel* model);

UEFORMAT_API UEFormatBufferResult ueformat_model_save(
    const UEFormatModel* model,
    const UEFormatSaveOptions* options);

#ifdef __cplusplus
}
#endif
