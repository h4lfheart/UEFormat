#pragma once

#include "ueformat/general/archive.h"

namespace UEFormat
{
    class UEAnim
    {
    };

    inline FArchive& operator<<(FArchive& archive, UEAnim& /*anim*/)
    {
        return archive;
    }
}
