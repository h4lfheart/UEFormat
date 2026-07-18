#pragma once

#include "ueformat/general/archive.h"

namespace UEFormat
{
    class UEModel
    {
    };

    inline FArchive& operator<<(FArchive& archive, UEModel& /*model*/)
    {
        return archive;
    }
}
