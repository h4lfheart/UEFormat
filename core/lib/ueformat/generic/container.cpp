#include "container.h"

#include <type_traits>

#include "../infrastructure/exception.h"

namespace UEFormat
{
    namespace
    {
        FString IdentifierForObject(const UEFormatObject& object)
        {
            return std::visit([]<typename T0>(const T0& value) -> FString
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
    }

    FArchive& operator<<(FArchive& archive, UEFormatContainer& container)
    {
        if (archive.IsSaving())
        {
            container.Header.Identifier = IdentifierForObject(container.Object);
            archive << container.Header;
            std::visit([&archive](auto& object)
            {
                archive << object;
            }, container.Object);
            return archive;
        }

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
