#include "stdafx.h"

#include "Hypodermic/IsComplete.h"

#include "TestingTypes.h"


using namespace Hypodermic;


BOOST_AUTO_TEST_SUITE(IsCompleteTests)

BOOST_AUTO_TEST_CASE(should_be_incomplete)
{
    // Assert
    BOOST_CHECK_EQUAL(Traits::IsComplete< Testing::IncompleteType >::value, false);
}

BOOST_AUTO_TEST_CASE(should_be_complete)
{
    // Assert
    BOOST_CHECK_EQUAL(Traits::IsComplete< Testing::CompleteType >::value, true);
}

BOOST_AUTO_TEST_CASE(should_be_complete_even_with_abstract_type)
{
    // Assert
    BOOST_CHECK_EQUAL(Traits::IsComplete< Testing::DefaultConstructibleBase >::value, true);
}

BOOST_AUTO_TEST_SUITE_END()
