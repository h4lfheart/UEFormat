#include "pose.h"

#include "ueformat/generic/attribute.h"

namespace UEFormat
{
    FArchive& operator<<(FArchive& archive, UEPose& pose)
    {
        FDataAttributeSet attrs;
        attrs.Bind("POSES", pose.Poses);
        attrs.Bind("CURVES", pose.CurveNames);
        archive << attrs;

        return archive;
    }
}
