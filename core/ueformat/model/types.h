#pragma once

#include "../general/archive.h"
#include "../infrastructure/types.h"

namespace UEFormat
{
    struct FNormal
    {
        f32 BinormalSign = 1.0f;
        FVector Normal;
    };

    struct FTexCoordEntry
    {
        FString Name;
        TArray<FMeshUVFloat> UVs;
    };

    struct FVertexColor
    {
        FString Name;
        TArray<FColor> Data;
    };

    struct FMaterial
    {
        FString MaterialName;
        FString MaterialPath;
        i32 FirstIndex = 0;
        i32 NumFaces = 0;
    };

    struct FWeight
    {
        u16 Bone = 0;
        i32 VertexIndex = 0;
        f32 Weight = 0.0f;
    };

    struct FMorphData
    {
        FVector PositionDelta;
        FVector TangentZDelta;
        u32 VertexIndex = 0;
    };

    struct FMorphTarget
    {
        FString MorphName;
        TArray<FMorphData> MorphData;
    };

    struct FBone
    {
        FString BoneName;
        i32 ParentIndex = -1;
        FVector Position;
        FQuat Orientation;
    };

    struct FSocket
    {
        FString SocketName;
        FString BoneName;
        FVector RelativeLocation;
        FQuat RelativeRotation;
        FVector RelativeScale{1.0f, 1.0f, 1.0f};
    };

    struct FVirtualBone
    {
        FString SourceBoneName;
        FString TargetBoneName;
        FString VirtualBoneName;
    };

    struct FConvexMeshCollision
    {
        FString Name;
        TArray<FVector> VertexData;
        TArray<i32> IndexData;
    };

    struct UEModelLOD
    {
        FString Name;
        TArray<FVector> Vertices;
        TArray<FNormal> Normals;
        TArray<FVector> Tangents;
        TArray<FTexCoordEntry> TextureCoordinates;
        TArray<u32> Indices;
        TArray<FVertexColor> VertexColors;
        TArray<FMaterial> Materials;
        TArray<FWeight> Weights;
        TArray<FMorphTarget> MorphTargets;
    };

    struct FSkeletonMetadata
    {
        FString Path;
    };

    struct UEModelSkeleton
    {
        FSkeletonMetadata Metadata;
        TArray<FBone> Bones;
        TArray<FSocket> Sockets;
        TArray<FVirtualBone> VirtualBones;
    };

    inline FArchive& operator<<(FArchive& archive, FNormal& value)
    {
        return archive << value.BinormalSign << value.Normal;
    }

    inline FArchive& operator<<(FArchive& archive, FTexCoordEntry& value)
    {
        return archive << value.Name << value.UVs;
    }

    inline FArchive& operator<<(FArchive& archive, FVertexColor& value)
    {
        return archive << value.Name << value.Data;
    }

    inline FArchive& operator<<(FArchive& archive, FMaterial& value)
    {
        return archive << value.MaterialName << value.MaterialPath << value.FirstIndex << value.NumFaces;
    }

    inline FArchive& operator<<(FArchive& archive, FWeight& value)
    {
        return archive << value.Bone << value.VertexIndex << value.Weight;
    }

    inline FArchive& operator<<(FArchive& archive, FMorphData& value)
    {
        return archive << value.PositionDelta << value.TangentZDelta << value.VertexIndex;
    }

    inline FArchive& operator<<(FArchive& archive, FMorphTarget& value)
    {
        return archive << value.MorphName << value.MorphData;
    }

    inline FArchive& operator<<(FArchive& archive, FBone& value)
    {
        return archive << value.BoneName << value.ParentIndex << value.Position << value.Orientation;
    }

    inline FArchive& operator<<(FArchive& archive, FSocket& value)
    {
        return archive << value.SocketName << value.BoneName << value.RelativeLocation << value.RelativeRotation
                       << value.RelativeScale;
    }

    inline FArchive& operator<<(FArchive& archive, FVirtualBone& value)
    {
        return archive << value.SourceBoneName << value.TargetBoneName << value.VirtualBoneName;
    }

    inline FArchive& operator<<(FArchive& archive, FSkeletonMetadata& value)
    {
        return archive << value.Path;
    }

    inline FArchive& operator<<(FArchive& archive, FConvexMeshCollision& value)
    {
        return archive << value.Name << value.VertexData << value.IndexData;
    }

    FArchive& operator<<(FArchive& archive, UEModelLOD& lod);
    FArchive& operator<<(FArchive& archive, UEModelSkeleton& skeleton);
}
