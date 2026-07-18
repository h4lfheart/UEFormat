#pragma once

#include <vector>

#include "../general/archive.h"
#include "exception.h"

namespace UEFormat
{
    struct FVector
    {
        f32 X = 0.0f;
        f32 Y = 0.0f;
        f32 Z = 0.0f;
    };

    struct FQuat
    {
        f32 X = 0.0f;
        f32 Y = 0.0f;
        f32 Z = 0.0f;
        f32 W = 1.0f;
    };

    struct FColor
    {
        u8 R = 0;
        u8 G = 0;
        u8 B = 0;
        u8 A = 255;
    };

    struct FMeshUVFloat
    {
        f32 U = 0.0f;
        f32 V = 0.0f;
    };

    template <typename T>
    using TArray = std::vector<T>;

    inline FArchive& operator<<(FArchive& archive, FVector& value)
    {
        archive << value.X << value.Y << value.Z;
        return archive;
    }

    inline FArchive& operator<<(FArchive& archive, FQuat& value)
    {
        archive << value.X << value.Y << value.Z << value.W;
        return archive;
    }

    inline FArchive& operator<<(FArchive& archive, FColor& value)
    {
        archive << value.R << value.G << value.B << value.A;
        return archive;
    }

    inline FArchive& operator<<(FArchive& archive, FMeshUVFloat& value)
    {
        archive << value.U << value.V;
        return archive;
    }

    template <typename T>
    FArchive& operator<<(FArchive& archive, TArray<T>& values)
    {
        i32 count = static_cast<i32>(values.size());
        archive << count;
        if (archive.IsLoading())
        {
            if (count < 0)
            {
                throw UEFormatException("Invalid TArray count");
            }
            values.resize(static_cast<usize>(count));
        }
        for (auto& value : values)
        {
            archive << value;
        }
        return archive;
    }
}
