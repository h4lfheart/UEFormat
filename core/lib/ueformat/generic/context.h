#pragma once

#include "container.h"
#include "save_options.h"
#include "../infrastructure/types.h"

namespace UEFormat
{
    class Context
    {
    public:
        Context();
        ~Context();

        Context(const Context&) = delete;
        Context& operator=(const Context&) = delete;

        UEFormatContainer Load(const TArray<u8>& data);

        TArray<u8> Save(const UEFormatObject& object, const FSaveOptions& options);
        TArray<u8> Save(const UEModel& model, const FSaveOptions& options);
        TArray<u8> Save(const UEAnim& anim, const FSaveOptions& options);
        TArray<u8> Save(const UEPose& pose, const FSaveOptions& options);

        // Legacy names — thin wrappers over Load / Save.
        UEFormatContainer LoadContainer(const TArray<u8>& data);

        TArray<u8> SaveContainer(
            const UEFormatObject& object,
            FString objectName,
            FString objectPath = {});
    };
}
