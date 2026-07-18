#pragma once

#include <cstring>
#include <format>
#include <string_view>

#include "archive.h"
#include "../infrastructure/exception.h"

namespace UEFormat
{
    inline constexpr std::string_view Magic = "UEFORMAT";
    inline constexpr std::string_view ModelIdentifier = "UEMODEL";
    inline constexpr std::string_view AnimIdentifier = "UEANIM";
    inline constexpr std::string_view PoseIdentifier = "UEPOSE";

    inline bool IsValidIdentifier(std::string_view identifier)
    {
        return identifier == ModelIdentifier
            || identifier == AnimIdentifier
            || identifier == PoseIdentifier;
    }

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

        archive << header.Identifier;

        if (archive.IsLoading() && !IsValidIdentifier(header.Identifier))
        {
            throw UEFormatException("Unknown identifier '" + header.Identifier + "'");
        }

        archive << header.FileVersion;

        if (archive.IsLoading() && header.FileVersion < EUEFormatVersion::AttributeSetFormat)
        {
            throw UEFormatException(std::format(
                "File version {} too low, earliest supported version by the core library is {}",
                static_cast<int>(header.FileVersion),
                static_cast<int>(EUEFormatVersion::AttributeSetFormat)
            ));
        }

        archive << header.ObjectName << header.ObjectPath << header.IsCompressed;

        if (header.IsCompressed)
        {
            archive << header.CompressionFormat << header.UncompressedSize << header.CompressedSize;
        }

        archive.SetFileVersion(header.FileVersion);

        return archive;
    }
}
