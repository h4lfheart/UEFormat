#pragma once

#include <variant>

#include "archive.h"
#include "header.h"
#include "ueformat/anim/anim.h"
#include "ueformat/model/model.h"
#include "ueformat/pose/pose.h"

namespace UEFormat
{
    using UEFormatObject = std::variant<UEModel, UEAnim, UEPose>;

    struct UEFormatContainer
    {
        FUEFormatHeader Header;
        UEFormatObject Object;
    };

    FArchive& operator<<(FArchive& archive, UEFormatContainer& container);
}
