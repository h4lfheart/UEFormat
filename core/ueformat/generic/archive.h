#pragma once

#include <span>
#include <string>
#include <type_traits>
#include <vector>

#include "../infrastructure/numeric.h"

namespace UEFormat
{
    using FString = std::string;

    enum class EUEFormatVersion : u8
    {
        BeforeCustomVersionWasAdded = 0,
        SerializeBinormalSign = 1,
        AddMultipleVertexColors = 2,
        AddConvexCollisionGeom = 3,
        RestructureLevelOfDetailFormat = 4,
        SerializeVirtualBones = 5,
        SerializeMaterialPath = 6,
        SerializeAssetMetadata = 7,
        PreserveOriginalTransforms = 8,
        AddPoseExport = 9,
        AttributeSetFormat = 10,

        VersionPlusOne,
        LatestVersion = VersionPlusOne - 1
    };

    class FArchive
    {
    public:
        static FArchive Writer();
        static FArchive Reader(std::span<const u8> data);

        bool IsLoading() const { return _loading; }
        bool IsSaving() const { return !_loading; }
        bool AtEnd() const;
        usize Tell() const { return _offset; }
        usize TotalSize() const;
        usize Remaining() const;

        EUEFormatVersion FileVersion() const { return _fileVersion; }
        void SetFileVersion(EUEFormatVersion version) { _fileVersion = version; }

        FArchive& Serialize(void* data, usize size);

        template <typename T>
        FArchive& SerializePackedArray(std::vector<T>& values, i32 count)
        {
            values.resize(static_cast<usize>(count));
            for (auto& value : values)
            {
                *this << value;
            }
            return *this;
        }

        void Skip(usize size);

        [[nodiscard]] const std::vector<u8>& Bytes() const;
        [[nodiscard]] std::vector<u8> StealBytes();

        FArchive& operator<<(bool& value);
        FArchive& operator<<(u8& value);
        FArchive& operator<<(u16& value);
        FArchive& operator<<(i32& value);
        FArchive& operator<<(u32& value);
        FArchive& operator<<(f32& value);
        FArchive& operator<<(FString& value);

    private:
        FArchive(bool loading, std::span<const u8> readData);

        bool _loading = false;
        EUEFormatVersion _fileVersion = EUEFormatVersion::BeforeCustomVersionWasAdded;
        std::vector<u8> _writeBuffer;
        std::span<const u8> _readData{};
        usize _offset = 0;
    };

    template <typename Enum> requires std::is_enum_v<Enum>
    FArchive& operator<<(FArchive& archive, Enum& value)
    {
        using Underlying = std::underlying_type_t<Enum>;
        Underlying raw = static_cast<Underlying>(value);
        archive << raw;
        if (archive.IsLoading())
        {
            value = static_cast<Enum>(raw);
        }
        return archive;
    }
}
