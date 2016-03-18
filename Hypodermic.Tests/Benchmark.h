#include <chrono>
#include <functional>
#include <string>
#include <iomanip>


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
        struct Result
        {
            bool succeeded;
            std::string name;
            std::uint32_t iterationCount;
            TimePoint startDate;
            Duration elapsed;
        };

        static Result measure(const std::string& name, const std::function< void(void) >& action)
        {
            return measure(name, 1, action);
        }

        static Result measure(const std::string& name, std::uint32_t iterationCount, const std::function< void(void) >& action)
        {
            Result result;
            result.name = name;
            result.iterationCount = iterationCount;
            result.startDate = Clock::now();

            try
            {
                for (std::uint32_t i = 0; i < result.iterationCount; i++)
                    action();

                result.succeeded = true;
            }
            catch (...)
            {
                result.succeeded = false;
            }

            result.elapsed = Clock::now() - result.startDate;

            return result;
        }
    };

} // namespace Utils
} // namespace Testing

#include <iomanip>
#include <ostream>
#include "DurationHumanizer.h"


namespace std
{
    inline ostream& operator<<(ostream& stream, const Testing::Utils::Benchmark::Result& result)
    {
        auto&& readableElapsed = Testing::Utils::DurationHumanizer::makeReadable(result.elapsed);

        stream << std::setprecision(3) <<
            "Run '" << result.name << "': " << readableElapsed.duration << readableElapsed.shortUnit << " over " << result.iterationCount << " iteration(s)";

        if (result.iterationCount > 1)
        {
            auto readableElapsedPerIteration = Testing::Utils::DurationHumanizer::makeReadable(result.elapsed, result.iterationCount);

            stream <<
                " [" << readableElapsedPerIteration.duration << readableElapsedPerIteration.shortUnit << " per iteration]";
        }

        return stream;
    }
}