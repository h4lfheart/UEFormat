#include "context.h"

#include "archive.h"
#include "container.h"
#include "header.h"

namespace UEFormat
{

Context::Context() = default;
Context::~Context() = default;

UEFormatContainer Context::LoadContainer(const TArray<u8>& data)
{
    auto archive = FArchive::Reader(data);

    UEFormatContainer container;
    archive << container;

    return container;
}

TArray<u8> Context::SaveContainer(
    const UEFormatObject& object,
    FString objectName,
    FString objectPath)
{
    UEFormatContainer container;
    container.Object = object;
    container.Header.ObjectName = std::move(objectName);
    container.Header.ObjectPath = std::move(objectPath);
    container.Header.FileVersion = EUEFormatVersion::LatestVersion;
    container.Header.IsCompressed = false;

    auto archive = FArchive::Writer();
    archive << container;
    return archive.StealBytes();
}

}
