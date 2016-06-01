#pragma once

#include <chrono>
#include <string>
#include <type_traits>


namespace Hypodermic
{
namespace Testing
{
namespace Utils
{

    namespace DurationUnits
    {
        enum DurationUnit
        {
            Nanoseconds,
            Microseconds,
            Milliseconds,
            Seconds,
            Minutes,
            Hours,
        };
    }

    template <DurationUnits::DurationUnit>
    struct DurationUnitInfo;

    template <>
    struct DurationUnitInfo< DurationUnits::Nanoseconds >
    {
        static std::string name() { return "nanoseconds"; }
        static std::string shortName() { return "ns"; }
    };

    template <>
    struct DurationUnitInfo< DurationUnits::Microseconds >
    {
        static std::string name() { return "microseconds"; }
        static std::string shortName() { return "us"; }
    };

    template <>
    struct DurationUnitInfo< DurationUnits::Milliseconds >
    {
        static std::string name() { return "milliseconds"; }
        static std::string shortName() { return "ms"; }
    };

    template <>
    struct DurationUnitInfo< DurationUnits::Seconds >
    {
        static std::string name() { return "seconds"; }
        static std::string shortName() { return "s"; }
    };

    template <>
    struct DurationUnitInfo< DurationUnits::Minutes >
    {
        static std::string name() { return "minutes"; }
        static std::string shortName() { return "m"; }
    };

    template <>
    struct DurationUnitInfo< DurationUnits::Hours >
    {
        static std::string name() { return "hours"; }
        static std::string shortName() { return "h"; }
    };


    template <class TDuration>
    struct DurationToUnit;

    template <>
    struct DurationToUnit< std::chrono::nanoseconds > : std::integral_constant< DurationUnits::DurationUnit, DurationUnits::Nanoseconds >
    {};

    template <>
    struct DurationToUnit< std::chrono::microseconds > : std::integral_constant< DurationUnits::DurationUnit, DurationUnits::Microseconds >
    {};

    template <>
    struct DurationToUnit< std::chrono::milliseconds > : std::integral_constant< DurationUnits::DurationUnit, DurationUnits::Milliseconds >
    {};

    template <>
    struct DurationToUnit< std::chrono::seconds > : std::integral_constant< DurationUnits::DurationUnit, DurationUnits::Seconds >
    {};

    template <>
    struct DurationToUnit< std::chrono::minutes > : std::integral_constant< DurationUnits::DurationUnit, DurationUnits::Minutes >
    {};

    template <>
    struct DurationToUnit< std::chrono::hours > : std::integral_constant< DurationUnits::DurationUnit, DurationUnits::Hours >
    {};
    
} // namespace Utils
} // namespace Testing
} // namespace Hypodermic
