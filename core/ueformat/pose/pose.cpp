#include "pose.h"

#include "ueformat/general/attribute.h"

namespace UEFormat
{
    FArchive& operator<<(FArchive& archive, UEPose& pose)
    {
        pose.Poses.clear();
        pose.CurveNames.clear();

        IterateDataAttributes(archive, [&](const FDataAttribute& section) -> bool
        {
            if (section.Name == "POSES")
            {
                archive << pose.Poses;
            }
            else if (section.Name == "CURVES")
            {
                archive << pose.CurveNames;
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
