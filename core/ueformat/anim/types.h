#pragma once

#include "../generic/archive.h"
#include "../infrastructure/types.h"

namespace UEFormat
{
    enum class EAdditiveAnimationType : u8
    {
        AAT_None = 0,
        AAT_LocalSpaceBase = 1,
        AAT_RotationOffsetMeshSpace = 2,
    };

    enum class EAdditiveBasePoseType : u8
    {
        ABPT_None = 0,
        ABPT_RefPose = 1,
        ABPT_AnimScaled = 2,
        ABPT_AnimFrame = 3,
        ABPT_LocalAnimFrame = 4,
    };

    struct FVectorKey
    {
        i32 Frame = 0;
        FVector Value;
    };

    struct FQuatKey
    {
        i32 Frame = 0;
        FQuat Value;
    };

    struct FFloatKey
    {
        i32 Frame = 0;
        f32 Value = 0.0f;
    };

    struct FAnimMetadata
    {
        i32 NumFrames = 0;
        f32 FramesPerSecond = 0.0f;
        FString RefPosePath;
        EAdditiveAnimationType AdditiveAnimType = EAdditiveAnimationType::AAT_None;
        EAdditiveBasePoseType RefPoseType = EAdditiveBasePoseType::ABPT_None;
        i32 RefFrameIndex = 0;
    };

    struct FTrack
    {
        FString BoneName;
        TArray<FVectorKey> PositionKeys;
        TArray<FQuatKey> RotationKeys;
        TArray<FVectorKey> ScaleKeys;
    };

    struct FCurve
    {
        FString CurveName;
        TArray<FFloatKey> Keys;
    };

    inline FArchive& operator<<(FArchive& archive, FVectorKey& value)
    {
        return archive << value.Frame << value.Value;
    }

    inline FArchive& operator<<(FArchive& archive, FQuatKey& value)
    {
        return archive << value.Frame << value.Value;
    }

    inline FArchive& operator<<(FArchive& archive, FFloatKey& value)
    {
        return archive << value.Frame << value.Value;
    }

    inline FArchive& operator<<(FArchive& archive, FAnimMetadata& value)
    {
        return archive << value.NumFrames << value.FramesPerSecond << value.RefPosePath
                       << value.AdditiveAnimType << value.RefPoseType << value.RefFrameIndex;
    }

    inline FArchive& operator<<(FArchive& archive, FTrack& value)
    {
        return archive << value.BoneName << value.PositionKeys << value.RotationKeys << value.ScaleKeys;
    }

    inline FArchive& operator<<(FArchive& archive, FCurve& value)
    {
        return archive << value.CurveName << value.Keys;
    }
}
