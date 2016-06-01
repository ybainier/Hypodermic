#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


namespace Hypodermic
{
namespace Testing
{

    BOOST_AUTO_TEST_SUITE(ContainerBuilderTests)

    BOOST_AUTO_TEST_CASE(should_add_registrations)
    {
        // Arrange
        int expectedNumber = 42;
        ContainerBuilder builder;
        builder.registerInstanceFactory([&](ComponentContext&) { return std::make_shared< ManualConstructible >(expectedNumber); });

        // Act
        ContainerBuilder builder2;
        builder2.addRegistrations(builder);

        // Assert
        auto container = builder2.build();
        auto instance = container->resolve< ManualConstructible >();
        BOOST_REQUIRE(instance != nullptr);
        BOOST_CHECK_EQUAL(instance->i, expectedNumber);
    }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace Testing
} // namespace Hypodermic