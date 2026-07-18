#pragma once

#include <cstring>
#include <string>
#include <vector>

#include "ueformat/api/generic/types.h"
#include "ueformat/infrastructure/types.h"

namespace UEFormat::API::Detail
{
    inline FString ToString(const char* utf8)
    {
        return utf8 ? FString(utf8) : FString{};
    }

    inline FVector ToVector(const UEFormatVector& v)
    {
        return FVector{v.x, v.y, v.z};
    }

    inline UEFormatVector FromVector(const FVector& v)
    {
        return UEFormatVector{v.X, v.Y, v.Z};
    }

    inline FQuat ToQuat(const UEFormatQuat& q)
    {
        return FQuat{q.x, q.y, q.z, q.w};
    }

    inline UEFormatQuat FromQuat(const FQuat& q)
    {
        return UEFormatQuat{q.X, q.Y, q.Z, q.W};
    }

    inline FColor ToColor(const UEFormatColor& c)
    {
        return FColor{c.r, c.g, c.b, c.a};
    }

    inline FMeshUVFloat ToUV(const UEFormatMeshUV& uv)
    {
        return FMeshUVFloat{uv.u, uv.v};
    }

    template <typename TDst, typename TSrc, typename Convert>
    std::vector<TDst> MapArray(const TSrc* data, int32_t count, Convert&& convert)
    {
        std::vector<TDst> out;
        if (!data || count <= 0)
        {
            return out;
        }
        out.reserve(static_cast<size_t>(count));
        for (int32_t i = 0; i < count; ++i)
        {
            out.push_back(convert(data[i]));
        }
        return out;
    }

    template <typename T>
    std::vector<T> CopyArray(const T* data, int32_t count)
    {
        if (!data || count <= 0)
        {
            return {};
        }
        return std::vector<T>(data, data + count);
    }
}
