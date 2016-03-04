#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


using namespace Hypodermic;


BOOST_AUTO_TEST_SUITE(NestedContainerTests)

BOOST_AUTO_TEST_CASE(should_not_resolve_dependency)
{
    // Arrange
    ContainerBuilder builder;
    builder.registerType< Testing::TopLevelConstructor >();

    auto container = builder.build();

    // Act
    auto instance = container->resolve< Testing::TopLevelConstructor >();

    // Assert
    BOOST_REQUIRE(instance != nullptr);
    BOOST_CHECK(instance->dependency == nullptr);
}

BOOST_AUTO_TEST_CASE(should_resolve_dependency_in_nested_container)
{
    // Arrange
    int expectedNumber = 42;

    ContainerBuilder builder;
    builder.registerType< Testing::TopLevelConstructor >();

    auto container = builder.build();

    // Act
    ContainerBuilder nestedContainerBuilder;
    nestedContainerBuilder.registerInstance(std::make_shared< Testing::NestedDependency >(expectedNumber));
    auto nestedContainer = nestedContainerBuilder.buildNestedContainerFrom(*container);

    // Assert
    auto instance = container->resolve< Testing::TopLevelConstructor >();
    BOOST_REQUIRE(instance != nullptr);
    BOOST_CHECK(instance->dependency == nullptr);

    instance = nestedContainer->resolve< Testing::TopLevelConstructor >();
    BOOST_REQUIRE(instance != nullptr);
    BOOST_REQUIRE(instance->dependency != nullptr);
    BOOST_CHECK_EQUAL(instance->dependency->i, expectedNumber);
}

BOOST_AUTO_TEST_SUITE_END()