#pragma once

#include <string>

#include "DurationUnit.h"


namespace Hypodermic
{
namespace Testing
{
namespace Utils
{

    struct HumanDuration
    {
        double value;
        DurationUnits::DurationUnit unit;
        std::string unitName;
        std::string shortUnitName;
    };


    class DurationHumanizer
    {
    public:
        template <class TDuration>
        static HumanDuration deduceHumanDuration(const TDuration& duration)
        {
            if (isReadable< std::chrono::nanoseconds >(duration))
                return createHumanDuration< std::chrono::nanoseconds >(duration);

            if (isReadable< std::chrono::microseconds >(duration))
                return createHumanDuration< std::chrono::microseconds >(duration);

            if (isReadable< std::chrono::milliseconds >(duration))
                return createHumanDuration< std::chrono::milliseconds >(duration);

            if (isReadable< std::chrono::seconds >(duration))
                return createHumanDuration< std::chrono::seconds >(duration);

            if (isReadable< std::chrono::minutes >(duration))
                return createHumanDuration< std::chrono::minutes >(duration);

            return createHumanDuration< std::chrono::hours >(duration);
        }

        template <class TClosestDuration, class TDuration>
        static HumanDuration createHumanDuration(const TDuration& duration)
        {
            auto count = std::chrono::duration_cast< TClosestDuration >(duration).count();

            HumanDuration result;
            result.value = static_cast< double >(count);
            result.unit = DurationToUnit< TClosestDuration >::value;
            result.unitName = DurationUnitInfo< DurationToUnit< TClosestDuration >::value >::name();
            result.shortUnitName = DurationUnitInfo< DurationToUnit< TClosestDuration >::value >::shortName();

            return result;
        }

        template <class TDuration>
        static HumanDuration createHumanDurationWithUnit(const TDuration& duration, DurationUnits::DurationUnit durationUnit)
        {
            switch (durationUnit)
            {
            case DurationUnits::Nanoseconds:
                return createHumanDuration< std::chrono::nanoseconds, TDuration >(duration);
            case DurationUnits::Microseconds:
                return createHumanDuration< std::chrono::microseconds, TDuration >(duration);
            case DurationUnits::Milliseconds:
                return createHumanDuration< std::chrono::milliseconds, TDuration >(duration);
            case DurationUnits::Seconds:
                return createHumanDuration< std::chrono::seconds, TDuration >(duration);
            case DurationUnits::Minutes:
                return createHumanDuration< std::chrono::minutes, TDuration >(duration);
            case DurationUnits::Hours:
                return createHumanDuration< std::chrono::hours, TDuration >(duration);
            default:;
                throw std::exception();
            }
        }

    private:
        template <class TClosestDuration, class TDuration>
        static bool isReadable(const TDuration& duration)
        {
            return std::chrono::duration_cast< TClosestDuration >(duration).count() < 1000;
        }
    };
    
} // namespace Utils
} // namespace Testing
} // namespace Hypodermic
