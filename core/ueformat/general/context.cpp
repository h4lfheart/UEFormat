#include "context.h"

#include "container.h"
#include "../infrastructure/exception.h"
#include "header.h"

namespace UEFormat
{

Context::Context() = default;
Context::~Context() = default;

UEFormatContainer Context::LoadContainer(const TArray<u8>& data)
{
    auto archive = FArchive::Reader(data);

    UEFormatContainer container;
    archive << container.Header;

    if (container.Header.Identifier == ModelIdentifier)
    {
        container.Object = UEModel();
        archive << std::get<UEModel>(container.Object);
    }
    else if (container.Header.Identifier == AnimIdentifier)
    {
        container.Object = UEAnim();
        archive << std::get<UEAnim>(container.Object);
    }
    else if (container.Header.Identifier == PoseIdentifier)
    {
        container.Object = UEPose();
        archive << std::get<UEPose>(container.Object);
    }
    else
    {
        throw UEFormatException("Unknown identifier '" + container.Header.Identifier + "'");
    }

    return container;
}

}
