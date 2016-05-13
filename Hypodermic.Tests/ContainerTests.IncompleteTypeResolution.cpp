#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.ForwardDeclaredDependency.h"


using namespace Hypodermic;


BOOST_AUTO_TEST_SUITE(ContainerTests)

BOOST_AUTO_TEST_CASE(should_not_resolve_incomplete_dependencies)
{
    // Arrange
    ContainerBuilder builder;
    auto container = builder.build();

    // Act
    auto instance = container->resolve< Testing::TypeWithOneForwardDeclaredDependency >();

    // Assert
    BOOST_REQUIRE(instance != nullptr);
    BOOST_CHECK(instance->dependency == nullptr);
}

BOOST_AUTO_TEST_CASE(should_resolve_registered_incomplete_dependencies)
{
    // Arrange
    ContainerBuilder builder;

    Testing::TypeWithOneForwardDeclaredDependency::registerDependency(builder);

    auto container = builder.build();

    // Act
    auto instance = container->resolve< Testing::TypeWithOneForwardDeclaredDependency >();

    // Assert
    BOOST_REQUIRE(instance != nullptr);
    BOOST_CHECK(instance->dependency != nullptr);
}

BOOST_AUTO_TEST_SUITE_END()