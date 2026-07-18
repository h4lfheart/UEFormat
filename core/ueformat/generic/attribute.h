#pragma once

#include <functional>
#include <optional>
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
                },
                [] { return true; }
            });
        }

        template <typename T>
        void Bind(std::string_view name, std::optional<T>& dest)
        {
            _bindings.push_back(Binding{
                FString(name),
                [&dest](FArchive& archive)
                {
                    archive << dest;
                },
                [&dest] { return dest.has_value(); }
            });
        }

        friend FArchive& operator<<(FArchive& archive, FDataAttributeSet& attrs)
        {
            if (archive.IsSaving())
            {
                std::vector<Binding*> bindingsToSave;
                bindingsToSave.reserve(attrs._bindings.size());
                for (auto& binding : attrs._bindings)
                {
                    if (binding.ShouldSave())
                    {
                        bindingsToSave.push_back(&binding);
                    }
                }

                i32 count = static_cast<i32>(bindingsToSave.size());
                archive << count;

                for (const Binding* binding : bindingsToSave)
                {
                    auto payload = FArchive::Writer();
                    binding->Serialize(payload);
                    auto bytes = payload.StealBytes();

                    FDataAttribute attribute;
                    attribute.Name = binding->Name;
                    attribute.ByteSize = static_cast<i32>(bytes.size());
                    archive << attribute;
                    if (!bytes.empty())
                    {
                        archive.Serialize(bytes.data(), bytes.size());
                    }
                }

                return archive;
            }

            IterateDataAttributes(archive, [&attrs, &archive](const FDataAttribute& attribute) -> bool
            {
                for (const auto& binding : attrs._bindings)
                {
                    if (binding.Name == attribute.Name)
                    {
                        binding.Serialize(archive);
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
            std::function<void(FArchive&)> Serialize;
            std::function<bool()> ShouldSave;
        };

        std::vector<Binding> _bindings;
    };
}
