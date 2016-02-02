#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


using namespace Hypodermic;


BOOST_AUTO_TEST_SUITE(ContainerTests)

BOOST_AUTO_TEST_CASE(should_resolve_a_type_using_its_last_registration)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::DefaultConstructible1 >().as< Testing::DefaultConstructibleBase >();
    builder.registerType< Testing::DefaultConstructible2 >().as< Testing::DefaultConstructibleBase >();

    auto container = builder.build();

    // Assert
    auto instance = container->resolve< Testing::DefaultConstructibleBase >();
    BOOST_CHECK(instance != nullptr);
    BOOST_CHECK(std::dynamic_pointer_cast<Testing::DefaultConstructible2>(instance) == instance);
}

BOOST_AUTO_TEST_CASE(should_resolve_all_registration_of_a_type)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::DefaultConstructible1 >().as< Testing::DefaultConstructibleBase >();
    builder.registerType< Testing::DefaultConstructible2 >().as< Testing::DefaultConstructibleBase >();

    auto container = builder.build();

    // Assert
    auto instances = container->resolveAll< Testing::DefaultConstructibleBase >();
    BOOST_REQUIRE_EQUAL(instances.size(), 2);
    BOOST_CHECK(instances[0] == std::dynamic_pointer_cast<Testing::DefaultConstructible1>(instances[0]));
    BOOST_CHECK(instances[1] == std::dynamic_pointer_cast<Testing::DefaultConstructible2>(instances[1]));
}

BOOST_AUTO_TEST_CASE(should_not_resolve_a_type_registered_by_its_base_type)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::DefaultConstructible1 >().as< Testing::DefaultConstructibleBase >();

    auto container = builder.build();

    // Assert
    auto instance = container->resolve< Testing::DefaultConstructible1 >();
    BOOST_CHECK(instance == nullptr);
}

BOOST_AUTO_TEST_CASE(should_resolve_a_type_registered_by_its_base_type_and_by_itself)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::DefaultConstructible1 >().as< Testing::DefaultConstructibleBase >().asSelf();

    auto container = builder.build();

    // Assert
    auto instance = container->resolve< Testing::DefaultConstructible1 >();
    BOOST_CHECK(instance != nullptr);
}

BOOST_AUTO_TEST_CASE(should_resolve_different_instances_of_a_registered_type)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::DefaultConstructible1 >().as< Testing::DefaultConstructibleBase >();

    auto container = builder.build();

    // Assert
    auto instance1 = container->resolve< Testing::DefaultConstructibleBase >();
    BOOST_CHECK(instance1 != nullptr);

    auto instance2 = container->resolve< Testing::DefaultConstructibleBase >();
    BOOST_CHECK(instance2 != nullptr);

    BOOST_CHECK(instance1 != instance2);
}

BOOST_AUTO_TEST_CASE(should_resolve_the_same_instance_of_a_type_registered_as_a_single_instance_1)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::DefaultConstructible1 >().as< Testing::DefaultConstructibleBase >().singleInstance();

    auto container = builder.build();

    // Assert
    auto instance1 = container->resolve< Testing::DefaultConstructibleBase >();
    BOOST_CHECK(instance1 != nullptr);

    auto instance2 = container->resolve< Testing::DefaultConstructibleBase >();
    BOOST_CHECK(instance2 != nullptr);

    BOOST_CHECK(instance1 == instance2);
}

BOOST_AUTO_TEST_CASE(should_resolve_the_same_instance_of_a_type_registered_as_a_single_instance_2)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::DefaultConstructible1 >().as< Testing::DefaultConstructibleBase >().asSelf().singleInstance();

    auto container = builder.build();

    // Assert
    auto instance1 = container->resolve< Testing::DefaultConstructible1 >();
    BOOST_CHECK(instance1 != nullptr);

    auto instance2 = container->resolve< Testing::DefaultConstructible1 >();
    BOOST_CHECK(instance2 != nullptr);

    BOOST_CHECK(instance1 == instance2);
}

BOOST_AUTO_TEST_CASE(should_resolve_the_same_instance_of_a_type_registered_as_a_single_instance_3)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::DefaultConstructible1 >().singleInstance();

    auto container = builder.build();

    // Assert
    auto instance1 = container->resolve< Testing::DefaultConstructible1 >();
    BOOST_CHECK(instance1 != nullptr);

    auto instance2 = container->resolve< Testing::DefaultConstructible1 >();
    BOOST_CHECK(instance2 != nullptr);

    BOOST_CHECK(instance1 == instance2);
}

BOOST_AUTO_TEST_SUITE_END()