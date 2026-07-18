#pragma once

#include "types.h"
#include "ueformat/generic/archive.h"
#include "ueformat/infrastructure/types.h"

namespace UEFormat
{
    class UEPose
    {
    public:
        TArray<FPoseData> Poses;
        TArray<FString> CurveNames;
    };

    FArchive& operator<<(FArchive& archive, UEPose& pose);
}
