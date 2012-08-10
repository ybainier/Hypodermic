#define BOOST_TEST_MODULE Hypodermic
#include <boost/test/results_reporter.hpp>
#include <boost/test/unit_test.hpp>

#include <boost/config.hpp>

#if defined(BOOST_MSVC)
# pragma warning (disable: 4231) // nonstandard extension used : 'extern' before template explicit instantiation
#endif


struct GlobalFixture
{
    GlobalFixture()
    {
        boost::unit_test::results_reporter::set_level(boost::unit_test::SHORT_REPORT);
    }
};


BOOST_GLOBAL_FIXTURE(GlobalFixture);