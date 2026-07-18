#pragma once

#include "types.h"
#include "ueformat/generic/archive.h"
#include "ueformat/infrastructure/types.h"

namespace UEFormat
{
    class UEAnim
    {
    public:
        FAnimMetadata Metadata;
        TArray<FTrack> Tracks;
        TArray<FCurve> Curves;
    };

    FArchive& operator<<(FArchive& archive, UEAnim& anim);
}
