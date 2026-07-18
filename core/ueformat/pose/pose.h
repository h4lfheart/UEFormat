#pragma once

#include "ueformat/general/archive.h"

namespace UEFormat
{
    class UEPose
    {
    };

    inline FArchive& operator<<(FArchive& archive, UEPose& /*pose*/)
    {
        return archive;
    }
}
