#pragma once

#include <optional>

#include "types.h"
#include "ueformat/generic/archive.h"
#include "ueformat/infrastructure/types.h"

namespace UEFormat
{
    class UEModel
    {
    public:
        TArray<UEModelLOD> LODs;
        std::optional<UEModelSkeleton> Skeleton;
        TArray<FConvexMeshCollision> Collisions;
    };

    FArchive& operator<<(FArchive& archive, UEModel& model);
}
