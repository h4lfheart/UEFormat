#pragma once

#include <cstring>
#include <string_view>

#include "archive.h"
#include "../infrastructure/exception.h"

namespace UEFormat
{
    inline constexpr std::string_view Magic = "UEFORMAT";
    inline constexpr std::string_view ModelIdentifier = "UEMODEL";
    inline constexpr std::string_view AnimIdentifier = "UEANIM";
    inline constexpr std::string_view PoseIdentifier = "UEPOSE";

    struct FUEFormatHeader
    {
        FString Identifier;
        EUEFormatVersion FileVersion = EUEFormatVersion::LatestVersion;
        FString ObjectName;
        FString ObjectPath;
        bool IsCompressed = false;

        FString CompressionFormat;
        i32 UncompressedSize = 0;
        i32 CompressedSize = 0;
    };

    inline FArchive& operator<<(FArchive& archive, FUEFormatHeader& header)
    {
        constexpr usize magicSize = 8;
        char magic[magicSize] = {};

        if (archive.IsSaving())
        {
            std::memcpy(magic, Magic.data(), magicSize);
        }

        archive.Serialize(magic, magicSize);

        if (archive.IsLoading() && std::string_view(magic, magicSize) != Magic)
        {
            throw UEFormatException("Invalid magic");
        }

        archive << header.Identifier << header.FileVersion << header.ObjectName << header.ObjectPath; << header.IsCompressed;

        if (header.IsCompressed)
        {
            archive << header.CompressionFormat << header.UncompressedSize << header.CompressedSize;
        }

        if (archive.IsLoading())
        {
            archive.SetFileVersion(header.FileVersion);
        }

        return archive;
    }
}
