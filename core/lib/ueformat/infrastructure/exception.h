#pragma once

#include <stdexcept>
#include <string>

namespace UEFormat
{
    class UEFormatException : public std::runtime_error
    {
    public:
        explicit UEFormatException(const std::string& message)
            : std::runtime_error(message)
        {
        }

        explicit UEFormatException(const char* message)
            : std::runtime_error(message)
        {
        }
    };
}
