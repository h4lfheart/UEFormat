#pragma once

#include "archive.h"

namespace UEFormat
{
    struct FDataAttribute
    {
        FString Name;
        i32 ByteSize = 0;
    };

    inline FArchive& operator<<(FArchive& archive, FDataAttribute& attribute)
    {
        return archive << attribute.Name << attribute.ByteSize;
    }

    template <typename Handler>
    void IterateDataAttributes(FArchive& archive, Handler&& handler)
    {
        i32 count = 0;
        archive << count;
        for (i32 i = 0; i < count; ++i)
        {
            FDataAttribute attribute;
            archive << attribute;
            if (!handler(attribute))
            {
                archive.Skip(static_cast<usize>(attribute.ByteSize));
            }
        }
    }
}
