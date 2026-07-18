#include "context.h"

#include "archive.h"
#include "compression.h"
#include "container.h"
#include "header.h"
#include "../infrastructure/exception.h"

#include <limits>

namespace UEFormat
{

Context::Context() = default;
Context::~Context() = default;

UEFormatContainer Context::Load(const TArray<u8>& data)
{
    auto archive = FArchive::Reader(data);

    UEFormatContainer container;
    archive << container;

    return container;
}

TArray<u8> Context::Save(const UEFormatObject& object, const FSaveOptions& options)
{
    UEFormatContainer container;
    container.Object = object;
    container.Header.Identifier = ObjectIdentifier(object);
    container.Header.ObjectName = options.ObjectName;
    container.Header.ObjectPath = options.ObjectPath;
    container.Header.FileVersion = options.FileVersion;
    container.Header.IsCompressed = options.Compression != EFileCompressionFormat::None;

    auto bodyArchive = FArchive::Writer();
    bodyArchive.SetFileVersion(options.FileVersion);
    SerializeObject(bodyArchive, container.Object);
    TArray<u8> body = bodyArchive.StealBytes();

    TArray<u8> payload;
    if (container.Header.IsCompressed)
    {
        if (body.size() > static_cast<usize>(std::numeric_limits<i32>::max()))
        {
            throw UEFormatException("Uncompressed body too large");
        }

        container.Header.CompressionFormat = CompressionFormatName(options.Compression);
        container.Header.UncompressedSize = static_cast<i32>(body.size());
        payload = Compress(body, options.Compression, options.CompressionLevel);

        if (payload.size() > static_cast<usize>(std::numeric_limits<i32>::max()))
        {
            throw UEFormatException("Compressed body too large");
        }

        container.Header.CompressedSize = static_cast<i32>(payload.size());
    }
    else
    {
        payload = std::move(body);
    }

    auto archive = FArchive::Writer();
    archive << container.Header;
    if (!payload.empty())
    {
        archive.Serialize(payload.data(), payload.size());
    }
    return archive.StealBytes();
}

TArray<u8> Context::Save(const UEModel& model, const FSaveOptions& options)
{
    return Save(UEFormatObject{model}, options);
}

TArray<u8> Context::Save(const UEAnim& anim, const FSaveOptions& options)
{
    return Save(UEFormatObject{anim}, options);
}

TArray<u8> Context::Save(const UEPose& pose, const FSaveOptions& options)
{
    return Save(UEFormatObject{pose}, options);
}

}
