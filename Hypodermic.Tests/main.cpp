#define BOOST_TEST_MODULE Hypodermic

#include <boost/test/results_reporter.hpp>
#include <boost/test/unit_test.hpp>

#include <boost/config.hpp>

#if defined(BOOST_MSVC)
# pragma warning (disable: 4231) // nonstandard extension used : 'extern' before template explicit instantiation
#endif

#include "Hypodermic/Behavior.h"
#include "Hypodermic/ConsoleLoggerSink.h"
#include "Hypodermic/Logger.h"


struct GlobalFixture
{
    GlobalFixture()
    {
        boost::unit_test::results_reporter::set_level(boost::unit_test::SHORT_REPORT);
        boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_successful_tests);
        
        Hypodermic::Logger::configureLogLevel(Hypodermic::LogLevels::Debug);
        Hypodermic::Logger::configureSink(std::make_shared< Hypodermic::ConsoleLoggerSink >());
    }
};


BOOST_GLOBAL_FIXTURE(GlobalFixture);
