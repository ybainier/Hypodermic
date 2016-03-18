#pragma once

#include <chrono>
#include <string>


namespace Testing
{
namespace Utils
{

    template <class TDuration>
    struct LitteralDurationUnit;

    template <>
    struct LitteralDurationUnit< std::chrono::nanoseconds >
    {
        static std::string name() { return "nanoseconds"; }
        static std::string shortName() { return "ns"; }
    };

    template <>
    struct LitteralDurationUnit< std::chrono::microseconds >
    {
        static std::string name() { return "microseconds"; }
        static std::string shortName() { return "us"; }
    };

    template <>
    struct LitteralDurationUnit< std::chrono::milliseconds >
    {
        static std::string name() { return "milliseconds"; }
        static std::string shortName() { return "ms"; }
    };

    template <>
    struct LitteralDurationUnit< std::chrono::seconds >
    {
        static std::string name() { return "seconds"; }
        static std::string shortName() { return "s"; }
    };

    template <>
    struct LitteralDurationUnit< std::chrono::minutes >
    {
        static std::string name() { return "minutes"; }
        static std::string shortName() { return "m"; }
    };

    template <>
    struct LitteralDurationUnit< std::chrono::hours >
    {
        static std::string name() { return "hours"; }
        static std::string shortName() { return "h"; }
    };
    
} // namespace Utils
} // namespace Testing
