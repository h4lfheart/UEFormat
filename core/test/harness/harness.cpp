#include "harness.h"

#include <atomic>
#include <chrono>
#include <format>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

#include "colors.h"

namespace UEFormat::Test
{
    namespace
    {
        std::vector<Case>& Registry()
        {
            static std::vector<Case> cases;
            return cases;
        }

        double ElapsedSeconds(std::chrono::steady_clock::time_point start)
        {
            return std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        }

        void PrintResult(std::string_view color, std::string_view status, double elapsed,
            std::string_view suite, std::string_view name)
        {
            std::cout << "\r\033[K"
                      << Color::Bold << color << status << Color::Reset
                      << Color::Gray << std::format(" [{:.3f}s] ", elapsed) << Color::Reset
                      << suite << "::" << name << '\n';
        }
    }

    void Fail(std::string_view message)
    {
        throw std::runtime_error(std::string(message));
    }

    void Check(bool condition, std::string_view message)
    {
        if (!condition)
        {
            Fail(message);
        }
    }

    Registrar::Registrar(std::string_view suite, std::string_view name, std::function<void()> run)
    {
        Registry().push_back(Case{suite, name, std::move(run)});
    }

    int RunAll()
    {
        auto& cases = Registry();
        std::cout << Color::Bold << "Running " << cases.size() << " tests" << Color::Reset << "\n\n";

        int passed = 0;
        int failed = 0;
        const auto suiteStart = std::chrono::steady_clock::now();

        for (const auto& test : cases)
        {
            const auto displayName = std::format("{}::{}", test.Suite, test.Name);
            std::atomic<bool> running{true};
            const auto t0 = std::chrono::steady_clock::now();

            std::thread timer([&]
            {
                while (running.load())
                {
                    std::cout << '\r' << Color::Bold << Color::Yellow << "RUNNING" << Color::Reset
                              << Color::Gray << std::format(" [{:.3f}s] ", ElapsedSeconds(t0))
                              << Color::Reset << displayName << std::flush;
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            });

            std::string failure;
            bool ok = true;
            try
            {
                test.Run();
            }
            catch (const std::exception& ex)
            {
                ok = false;
                failure = ex.what();
            }
            catch (...)
            {
                ok = false;
                failure = "unknown error";
            }

            running = false;
            timer.join();

            const auto elapsed = ElapsedSeconds(t0);
            if (ok)
            {
                ++passed;
                PrintResult(Color::Green, "PASS", elapsed, test.Suite, test.Name);
            }
            else
            {
                ++failed;
                PrintResult(Color::Red, "FAIL", elapsed, test.Suite, test.Name);
                std::cout << Color::Gray << "  " << failure << Color::Reset << '\n';
            }
        }

        const auto total = ElapsedSeconds(suiteStart);
        std::cout << '\n'
                  << Color::Bold << Color::Cyan << "SUMMARY" << Color::Reset
                  << Color::Gray << std::format(" [{:.3f}s] ", total) << Color::Reset
                  << cases.size() << " tests run, "
                  << Color::Green << passed << " passed" << Color::Reset << ", "
                  << (failed == 0 ? Color::Gray : Color::Red) << failed << " failed" << Color::Reset
                  << '\n';

        return failed == 0 ? 0 : 1;
    }
}
