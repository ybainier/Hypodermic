#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


using namespace Hypodermic;


BOOST_AUTO_TEST_SUITE(PersistentInstanceRegistrationTests)

BOOST_AUTO_TEST_CASE(should_resolve_the_same_instance)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::DefaultConstructible1 >().singleInstance();

    auto container = builder.build();

    // Assert
    auto instance1 = container->resolve< Testing::DefaultConstructible1 >();
    auto instance2 = container->resolve< Testing::DefaultConstructible1 >();
    BOOST_CHECK(instance1 != nullptr);
    BOOST_CHECK(instance1 == instance2);
}

BOOST_AUTO_TEST_CASE(should_resolve_the_same_instance_when_using_an_alias)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::DefaultConstructible1 >()
           .as< Testing::DefaultConstructibleBase >()
           .asSelf()
           .singleInstance();

    auto container = builder.build();

    // Assert
    auto instance1 = container->resolve< Testing::DefaultConstructible1 >();
    auto instance2 = container->resolve< Testing::DefaultConstructibleBase >();
    BOOST_CHECK(instance1 != nullptr);
    BOOST_CHECK(std::static_pointer_cast< Testing::DefaultConstructibleBase >(instance1) == instance2);
}

BOOST_AUTO_TEST_SUITE_END()