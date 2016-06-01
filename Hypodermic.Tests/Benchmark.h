#include <chrono>
#include <functional>
#include <string>
#include <iomanip>

#include "DurationHumanizer.h"
#include "DurationUnit.h"
#include "PercentileCalculator.h"


namespace Hypodermic
{
namespace Testing
{
namespace Utils
{

    class Benchmark
    {
	    typedef std::conditional< std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock, std::chrono::steady_clock >::type Clock;
	    typedef Clock::time_point TimePoint;
	    typedef Clock::duration Duration;

    public:
        struct NormalizedDuration
        {
            NormalizedDuration() = default;
            explicit NormalizedDuration(Duration value, DurationUnits::DurationUnit unit)
                : value(value)
                , unit(unit)
            {}

            Duration value;
            DurationUnits::DurationUnit unit;
        };

        struct Result
        {
            struct IterationResult
            {
                NormalizedDuration average;
                NormalizedDuration min;
                NormalizedDuration max;
                NormalizedDuration percentile50;
                NormalizedDuration percentile95;
                NormalizedDuration percentile99;
                NormalizedDuration percentile999;
                NormalizedDuration percentile9999;
                NormalizedDuration percentile99999;
            };

            bool succeeded;
            std::string name;
            std::uint32_t iterationCount;
            TimePoint startDate;
            Duration elapsed;
            IterationResult iterationResult;

        };

        static Result measure(const std::string& name, const std::function< void(void) >& action, std::uint32_t warmUpCycleCount = 0)
        {
            return measure(name, 1, action, warmUpCycleCount);
        }

        static Result measure(const std::string& name, std::uint32_t iterationCount, const std::function< void(void) >& action, std::uint32_t warmUpCycleCount = 0)
        {
            PercentileCalculator< Duration > calculator;

            Result result;
            result.name = name;
            result.iterationCount = iterationCount;

            try
            {
                for (std::uint32_t i = 0; i < warmUpCycleCount; i++)
                    action();

                result.startDate = Clock::now();

                for (std::uint32_t i = 0; i < result.iterationCount; i++)
                {
                    auto iterationStartDate = Clock::now();
                    action();

                    calculator.pushLatency(Clock::now() - iterationStartDate);
                }

                result.succeeded = true;
            }
            catch (...)
            {
                result.succeeded = false;
            }

            result.elapsed = Clock::now() - result.startDate;

            auto&& iterationAverageDuration = calculator.computeAverage();
            auto&& humanIterationAverageDuration = DurationHumanizer::deduceHumanDuration(iterationAverageDuration);

            result.iterationResult.average = NormalizedDuration(iterationAverageDuration, humanIterationAverageDuration.unit);
            result.iterationResult.min = NormalizedDuration(calculator.computeMin(), humanIterationAverageDuration.unit);
            result.iterationResult.max = NormalizedDuration(calculator.computeMax(), humanIterationAverageDuration.unit);
            result.iterationResult.percentile50 = NormalizedDuration(calculator.computePercentile(50.0), humanIterationAverageDuration.unit);
            result.iterationResult.percentile95 = NormalizedDuration(calculator.computePercentile(95.0), humanIterationAverageDuration.unit);
            result.iterationResult.percentile99 = NormalizedDuration(calculator.computePercentile(99.0), humanIterationAverageDuration.unit);
            result.iterationResult.percentile999 = NormalizedDuration(calculator.computePercentile(99.9), humanIterationAverageDuration.unit);
            result.iterationResult.percentile9999 = NormalizedDuration(calculator.computePercentile(99.99), humanIterationAverageDuration.unit);
            result.iterationResult.percentile99999 = NormalizedDuration(calculator.computePercentile(99.999), humanIterationAverageDuration.unit);

            return result;
        }
    };

} // namespace Utils
} // namespace Testing
} // namespace Hypodermic

#include <iomanip>
#include <ostream>
#include <boost/format.hpp>


namespace std
{

    inline ostream& operator<<(ostream& stream, const Hypodermic::Testing::Utils::Benchmark::NormalizedDuration& duration)
    {
        auto&& humanDuration = Hypodermic::Testing::Utils::DurationHumanizer::createHumanDurationWithUnit(duration.value, duration.unit);

        return stream << (boost::format("%.f") % humanDuration.value) << " " << humanDuration.shortUnitName;
    }

    inline ostream& operator<<(ostream& stream, const Hypodermic::Testing::Utils::Benchmark::Result::IterationResult& result)
    {
        return stream <<
                "Average:           " << result.average << std::endl <<
                "Min:               " << result.min << std::endl <<
                "Max:               " << result.max << std::endl <<
                "99.999 percentile: " << result.percentile99999 << std::endl <<
                "99.99 percentile:  " << result.percentile9999 << std::endl <<
                "99.9 percentile:   " << result.percentile999 << std::endl <<
                "99 percentile:     " << result.percentile99 << std::endl <<
                "95 percentile:     " << result.percentile95 << std::endl <<
                "50 percentile:     " << result.percentile50;
    }

    inline ostream& operator<<(ostream& stream, const Hypodermic::Testing::Utils::Benchmark::Result& result)
    {
        auto&& readableElapsed = Hypodermic::Testing::Utils::DurationHumanizer::deduceHumanDuration(result.elapsed);

        stream << std::setprecision(3) <<
            "Run '" << result.name << "': " << readableElapsed.value << readableElapsed.shortUnitName << " over " << result.iterationCount << " iteration(s)";

        if (result.iterationCount > 1)
            stream << std::endl << result.iterationResult;

        return stream;
    }

} // namespace std