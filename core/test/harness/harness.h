#pragma once

#include <functional>
#include <string_view>

namespace UEFormat::Test
{
    struct Case
    {
        std::string_view Suite;
        std::string_view Name;
        std::function<void()> Run;
    };

    struct Registrar
    {
        Registrar(std::string_view suite, std::string_view name, std::function<void()> run);
    };

    void Fail(std::string_view message);
    void Check(bool condition, std::string_view message);

    int RunAll();
}

#define UEFORMAT_TEST(SuiteName, TestName)                                                         \
    static void SuiteName##_##TestName();                                                          \
    static const ::UEFormat::Test::Registrar SuiteName##_##TestName##_registrar{                   \
        #SuiteName, #TestName, &SuiteName##_##TestName};                                           \
    static void SuiteName##_##TestName()
