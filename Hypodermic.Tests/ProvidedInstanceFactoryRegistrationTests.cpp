#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


using namespace Hypodermic;


BOOST_AUTO_TEST_SUITE(DefaultConstructibleRegistrationTests)

BOOST_AUTO_TEST_CASE(should_register_manual_constructible_type_with_an_instance_factory)
{
    // Arrange
    int expectedNumber = 42;
    ContainerBuilder builder;

    // Act
    builder.registerInstanceFactory([&](Container&) { return std::make_shared< Testing::ManualConstructible >(expectedNumber); });

    auto container = builder.build();

    // Assert
    auto instance = container->resolve< Testing::ManualConstructible >();
    BOOST_CHECK(instance != nullptr);
    BOOST_CHECK_EQUAL(instance->i, expectedNumber);
}

BOOST_AUTO_TEST_SUITE_END()