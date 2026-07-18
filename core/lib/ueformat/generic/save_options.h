#pragma once

#include "archive.h"

namespace UEFormat
{
    enum class EFileCompressionFormat : u8
    {
        None = 0,
        GZIP = 1,
        ZSTD = 2,
    };

    struct FSaveOptions
    {
        FString ObjectName;
        FString ObjectPath;
        EFileCompressionFormat Compression = EFileCompressionFormat::None;
        EUEFormatVersion FileVersion = EUEFormatVersion::LatestVersion;
    };
}
