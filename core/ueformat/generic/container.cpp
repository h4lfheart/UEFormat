#include "container.h"

#include "../infrastructure/exception.h"

namespace UEFormat
{
    FArchive& operator<<(FArchive& archive, UEFormatContainer& container)
    {
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

        return archive;
    }
}
