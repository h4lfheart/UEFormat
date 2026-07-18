#include "context.h"

#include "archive.h"
#include "container.h"
#include "header.h"

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
    container.Header.ObjectName = options.ObjectName;
    container.Header.ObjectPath = options.ObjectPath;
    container.Header.FileVersion = options.FileVersion;
    // TODO compression support GZIP/ZSTD
    container.Header.IsCompressed = false;

    auto archive = FArchive::Writer();
    archive << container;
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
