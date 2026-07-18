#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UEFORMAT_API
#define UEFORMAT_API
#endif

typedef enum UEFormatStatus {
    UEFORMAT_OK = 0,
    UEFORMAT_INVALID_ARGUMENT = 1,
    UEFORMAT_OUT_OF_MEMORY = 2,
    UEFORMAT_SERIALIZE_ERROR = 3,
    UEFORMAT_TYPE_MISMATCH = 4,
} UEFormatStatus;

typedef enum UEFormatCompression {
    UEFORMAT_COMPRESSION_NONE = 0,
    UEFORMAT_COMPRESSION_GZIP = 1,
    UEFORMAT_COMPRESSION_ZSTD = 2,
} UEFormatCompression;

typedef struct UEFormatBuffer {
    uint8_t* data;
    size_t size;
} UEFormatBuffer;

typedef struct UEFormatSaveOptions {
    const char* object_name;
    const char* object_path;
    uint8_t compression;
    int32_t compression_level;
} UEFormatSaveOptions;

typedef struct UEFormatBufferResult {
    UEFormatStatus status;
    UEFormatBuffer buffer;
    const char* error;
} UEFormatBufferResult;

typedef struct UEFormatVector {
    float x, y, z;
} UEFormatVector;

typedef struct UEFormatQuat {
    float x, y, z, w;
} UEFormatQuat;

typedef struct UEFormatColor {
    uint8_t r, g, b, a;
} UEFormatColor;

typedef struct UEFormatMeshUV {
    float u, v;
} UEFormatMeshUV;

typedef struct UEFormatNormal {
    float binormal_sign;
    UEFormatVector normal;
} UEFormatNormal;

UEFORMAT_API void ueformat_buffer_free(UEFormatBuffer* buffer);
UEFORMAT_API const char* ueformat_status_string(UEFormatStatus status);
UEFORMAT_API uint8_t ueformat_latest_version(void);

#ifdef __cplusplus
}
#endif
