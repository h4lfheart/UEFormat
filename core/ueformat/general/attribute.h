#pragma once

#include <functional>
#include <string_view>
#include <vector>

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

    struct FDataAttributeSet
    {
        template <typename T>
        void Bind(std::string_view name, T& dest)
        {
            _bindings.push_back(Binding{
                FString(name),
                [&dest](FArchive& archive)
                {
                    archive << dest;
                }
            });
        }

        friend FArchive& operator<<(FArchive& archive, FDataAttributeSet& attrs)
        {
            IterateDataAttributes(archive, [&attrs, &archive](const FDataAttribute& attribute) -> bool
            {
                for (const auto& binding : attrs._bindings)
                {
                    if (binding.Name == attribute.Name)
                    {
                        binding.Read(archive);
                        return true;
                    }
                }
                return false;
            });
            return archive;
        }

    private:
        struct Binding
        {
            FString Name;
            std::function<void(FArchive&)> Read;
        };

        std::vector<Binding> _bindings;
    };
}
