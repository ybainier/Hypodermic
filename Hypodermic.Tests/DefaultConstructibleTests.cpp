#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


using namespace Hypodermic;


BOOST_AUTO_TEST_SUITE(DefaultConstructibleTests)

BOOST_AUTO_TEST_CASE(should_register_default_constructible_type)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::DefaultConstructible1 >();

    auto container = builder.build();

    // Assert
    auto instance = container->resolve< Testing::DefaultConstructible1 >();
    BOOST_CHECK(instance != nullptr);
}

BOOST_AUTO_TEST_CASE(should_register_default_constructible_types)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::DefaultConstructible1 >();
    builder.registerType< Testing::DefaultConstructible2 >();

    auto container = builder.build();

    // Assert
    auto instance1 = container->resolve< Testing::DefaultConstructible1 >();
    auto instance2 = container->resolve< Testing::DefaultConstructible2 >();
    BOOST_CHECK(instance1 != nullptr);
    BOOST_CHECK(instance2 != nullptr);
}

BOOST_AUTO_TEST_SUITE_END()