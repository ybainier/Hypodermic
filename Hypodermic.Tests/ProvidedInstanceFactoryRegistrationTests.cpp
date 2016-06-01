#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


namespace Hypodermic
{
namespace Testing
{

    BOOST_AUTO_TEST_SUITE(DefaultConstructibleRegistrationTests)

    BOOST_AUTO_TEST_CASE(should_register_manual_constructible_type_with_an_instance_factory)
    {
        // Arrange
        int expectedNumber = 42;
        ContainerBuilder builder;

        // Act
        builder.registerInstanceFactory([&](ComponentContext&) { return std::make_shared< ManualConstructible >(expectedNumber); })
               .singleInstance();

        auto container = builder.build();

        // Assert
        auto instance = container->resolve< ManualConstructible >();
        BOOST_CHECK(instance != nullptr);
        BOOST_CHECK_EQUAL(instance->i, expectedNumber);
    }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace Testing
} // namespace Hypodermic