#include "anim.h"

#include "ueformat/general/attribute.h"

namespace UEFormat
{
    FArchive& operator<<(FArchive& archive, UEAnim& anim)
    {
        anim.Metadata = {};
        anim.Tracks.clear();
        anim.Curves.clear();

        FDataAttributeSet attrs;
        attrs.Bind("METADATA", anim.Metadata);
        attrs.Bind("TRACKS", anim.Tracks);
        attrs.Bind("CURVES", anim.Curves);
        archive << attrs;

        return archive;
    }
}
