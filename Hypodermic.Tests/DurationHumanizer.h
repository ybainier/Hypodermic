#pragma once

#include <string>

#include "LitteralDurationUnit.h"


namespace Testing
{
namespace Utils
{

    struct HumanDuration
    {
        double duration;
        std::string unit;
        std::string shortUnit;
    };


    struct DurationHumanizer
    {
    public:
        template <class TDuration>
        static HumanDuration makeReadable(const TDuration& duration, std::uint32_t iterationCount = 1)
        {
            if (isReadable< std::chrono::nanoseconds >(duration, iterationCount))
                return createHumanDuration< std::chrono::nanoseconds >(duration, iterationCount);

            if (isReadable< std::chrono::microseconds >(duration, iterationCount))
                return createHumanDuration< std::chrono::microseconds >(duration, iterationCount);

            if (isReadable< std::chrono::milliseconds >(duration, iterationCount))
                return createHumanDuration< std::chrono::milliseconds >(duration, iterationCount);

            if (isReadable< std::chrono::seconds >(duration, iterationCount))
                return createHumanDuration< std::chrono::seconds >(duration, iterationCount);

            if (isReadable< std::chrono::minutes >(duration, iterationCount))
                return createHumanDuration< std::chrono::minutes >(duration, iterationCount);

            return createHumanDuration< std::chrono::hours >(duration, iterationCount);
        }

    private:
        template <class TClosestDuration, class TDuration>
        static bool isReadable(const TDuration& duration, std::uint32_t iterationCount)
        {
            return std::chrono::duration_cast< TClosestDuration >(duration).count() / static_cast< double >(iterationCount) < 1000;
        }

        template <class TClosestDuration, class TDuration>
        static HumanDuration createHumanDuration(const TDuration& duration, std::uint32_t iterationCount)
        {
            auto count = std::chrono::duration_cast< TClosestDuration >(duration).count() / static_cast< double >(iterationCount);

            HumanDuration result;
            result.duration = static_cast< double >(count);
            result.unit = LitteralDurationUnit< TClosestDuration >::name();
            result.shortUnit = LitteralDurationUnit< TClosestDuration >::shortName();

            return result;
        }
    };
    
} // namespace Utils
} // namespace Testing
