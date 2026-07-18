#pragma once

#include "archive.h"
#include "compression.h"

namespace UEFormat
{
    struct FSaveOptions
    {
        FString ObjectName;
        FString ObjectPath;
        EFileCompressionFormat Compression = EFileCompressionFormat::None;
        i32 CompressionLevel = DefaultCompressionLevel;
        EUEFormatVersion FileVersion = EUEFormatVersion::LatestVersion;
    };
}
