#include "anim.h"

#include "ueformat/general/attribute.h"

namespace UEFormat
{
    FArchive& operator<<(FArchive& archive, UEAnim& anim)
    {
        anim.Metadata = {};
        anim.Tracks.clear();
        anim.Curves.clear();

        IterateDataAttributes(archive, [&](const FDataAttribute& section) -> bool
        {
            if (section.Name == "METADATA")
            {
                archive << anim.Metadata;
            }
            else if (section.Name == "TRACKS")
            {
                archive << anim.Tracks;
            }
            else if (section.Name == "CURVES")
            {
                archive << anim.Curves;
            }
            else
            {
                return false;
            }
            return true;
        });

        return archive;
    }
}
