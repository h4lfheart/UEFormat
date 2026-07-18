#include "container.h"

#include <format>
#include <type_traits>

#include "compression.h"
#include "../infrastructure/exception.h"

namespace UEFormat
{
    FString ObjectIdentifier(const UEFormatObject& object)
    {
        return std::visit([]<typename T0>(const T0&) -> FString
        {
            using T = std::decay_t<T0>;
            if constexpr (std::is_same_v<T, UEModel>)
            {
                return FString(ModelIdentifier);
            }
            else if constexpr (std::is_same_v<T, UEAnim>)
            {
                return FString(AnimIdentifier);
            }
            else
            {
                static_assert(std::is_same_v<T, UEPose>);
                return FString(PoseIdentifier);
            }
        }, object);
    }

    FArchive& SerializeObject(FArchive& archive, UEFormatObject& object)
    {
        std::visit([&archive](auto& value)
        {
            archive << value;
        }, object);
        return archive;
    }

    namespace
    {
        void EngageObjectForIdentifier(UEFormatContainer& container)
        {
            if (container.Header.Identifier == ModelIdentifier)
            {
                container.Object = UEModel();
            }
            else if (container.Header.Identifier == AnimIdentifier)
            {
                container.Object = UEAnim();
            }
            else if (container.Header.Identifier == PoseIdentifier)
            {
                container.Object = UEPose();
            }
            else
            {
                throw UEFormatException("Unknown identifier '" + container.Header.Identifier + "'");
            }
        }
    }

    FArchive& operator<<(FArchive& archive, UEFormatContainer& container)
    {
        if (archive.IsSaving())
        {
            container.Header.Identifier = ObjectIdentifier(container.Object);
            archive << container.Header;
            return SerializeObject(archive, container.Object);
        }

        archive << container.Header;
        EngageObjectForIdentifier(container);

        if (!container.Header.IsCompressed)
        {
            return SerializeObject(archive, container.Object);
        }

        if (container.Header.CompressedSize < 0
            || container.Header.UncompressedSize < 0
            || static_cast<usize>(container.Header.CompressedSize) > archive.Remaining())
        {
            throw UEFormatException("Invalid compression sizes");
        }

        TArray<u8> compressed(static_cast<usize>(container.Header.CompressedSize));
        if (!compressed.empty())
        {
            archive.Serialize(compressed.data(), compressed.size());
        }

        const EFileCompressionFormat format = CompressionFormatFromName(container.Header.CompressionFormat);
        if (format == EFileCompressionFormat::None)
        {
            throw UEFormatException("Unknown compression format '" + container.Header.CompressionFormat + "'");
        }

        TArray<u8> uncompressed = Decompress(compressed, format, container.Header.UncompressedSize);
        if (uncompressed.size() != static_cast<usize>(container.Header.UncompressedSize))
        {
            throw UEFormatException(std::format(
                "Uncompressed size mismatch: expected {}, got {}",
                container.Header.UncompressedSize,
                uncompressed.size()));
        }

        auto body = FArchive::Reader(uncompressed);
        body.SetFileVersion(container.Header.FileVersion);
        SerializeObject(body, container.Object);
        return archive;
    }
}
