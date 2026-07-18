#pragma once

#include <span>

#include "archive.h"
#include "../infrastructure/types.h"

namespace UEFormat
{
    enum class EFileCompressionFormat : u8
    {
        None = 0,
        GZIP = 1,
        ZSTD = 2,
    };

    inline constexpr i32 DefaultCompressionLevel = 6;

    FString CompressionFormatName(EFileCompressionFormat format);
    EFileCompressionFormat CompressionFormatFromName(const FString& name);

    TArray<u8> Compress(std::span<const u8> data, EFileCompressionFormat format, i32 level = DefaultCompressionLevel);
    TArray<u8> Decompress(std::span<const u8> data, EFileCompressionFormat format, i32 uncompressedSize);
}
