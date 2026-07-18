#include "context.h"

#include "archive.h"
#include "container.h"

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

}
