#pragma once

#include "container.h"
#include "../infrastructure/types.h"

namespace UEFormat
{
    class Context
    {
    public:
        Context();
        ~Context();

        UEFormatContainer LoadContainer(const TArray<u8>& data);
    };
}
