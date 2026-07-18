#pragma once

#include "../generic/archive.h"
#include "../infrastructure/types.h"

namespace UEFormat
{
    struct FPoseKey
    {
        FString BoneName;
        FVector Location;
        FQuat Rotation;
        FVector Scale{1.0f, 1.0f, 1.0f};
    };

    struct FPoseCurveInfluence
    {
        i32 CurveIndex = 0;
        f32 Influence = 0.0f;
    };

    struct FPoseData
    {
        FString PoseName;
        TArray<FPoseKey> Keys;
        TArray<FPoseCurveInfluence> Curves;
    };

    inline FArchive& operator<<(FArchive& archive, FPoseKey& value)
    {
        return archive << value.BoneName << value.Location << value.Rotation << value.Scale;
    }

    inline FArchive& operator<<(FArchive& archive, FPoseCurveInfluence& value)
    {
        return archive << value.CurveIndex << value.Influence;
    }

    inline FArchive& operator<<(FArchive& archive, FPoseData& value)
    {
        return archive << value.PoseName << value.Keys << value.Curves;
    }
}
