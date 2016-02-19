#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


using namespace Hypodermic;


BOOST_AUTO_TEST_SUITE(MissingConstructorRegistrationTests)

BOOST_AUTO_TEST_CASE(should_when_building_container_which_has_at_least_one_non_constructible_type)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::MissingConstructorDependency >().singleInstance();
    builder.registerType< Testing::MissingConstructor >();

    // Assert
    std::shared_ptr< Container > container;
    BOOST_CHECK_THROW(container = builder.build(), std::exception);
}

BOOST_AUTO_TEST_CASE(should_build_container_when_constructor_is_described)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::MissingConstructorDependency >().singleInstance();
    builder.registerType< Testing::MissingConstructor >().usingConstructor< Testing::MissingConstructor(Testing::MissingConstructorDependency*) >();

    // Assert
    std::shared_ptr< Container > container;
    BOOST_REQUIRE_NO_THROW(container = builder.build());

    auto dependency = container->resolve< Testing::MissingConstructorDependency >();
    auto instance = container->resolve< Testing::MissingConstructor >();

    BOOST_REQUIRE(instance != nullptr);
    BOOST_CHECK(instance->dependency == dependency);
}

BOOST_AUTO_TEST_SUITE_END()