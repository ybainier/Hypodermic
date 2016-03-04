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

BOOST_AUTO_TEST_CASE(should_resolve_all_registrations_of_a_type)
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

BOOST_AUTO_TEST_CASE(should_create_a_new_registration_when_resolving_a_type_registered_by_its_base_type)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::DefaultConstructible1 >().as< Testing::DefaultConstructibleBase >();

    auto container = builder.build();

    // Assert
    auto instance = container->resolve< Testing::DefaultConstructible1 >();
    BOOST_CHECK(instance != nullptr);
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

BOOST_AUTO_TEST_CASE(should_resolve_container_as_a_dependency)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::ContainerDependent >();

    auto container = builder.build();

    // Assert
    std::shared_ptr< Testing::ContainerDependent > instance;
    
    BOOST_REQUIRE_NO_THROW(instance = container->resolve< Testing::ContainerDependent >());

    BOOST_CHECK(instance->container == container);
}

BOOST_AUTO_TEST_CASE(should_call_activation_handlers_everytime_an_instance_is_activated)
{
    // Arrange
    ContainerBuilder builder;
    std::shared_ptr< Testing::DefaultConstructible1 > activatedInstance;
    int activationCount = 0;

    // Act
    builder.registerType< Testing::DefaultConstructible1 >()
        .onActivated([&](Container&, const std::shared_ptr< Testing::DefaultConstructible1 >& instance)
        {
            activatedInstance = instance;
        })
        .onActivated([&](Container&, const std::shared_ptr< Testing::DefaultConstructible1 >&)
        {
            activationCount++;
        });

    auto container = builder.build();

    // Assert
    auto resolvedInstance = container->resolve< Testing::DefaultConstructible1 >();
    BOOST_CHECK(resolvedInstance != nullptr);
    BOOST_CHECK(activatedInstance == resolvedInstance);
    BOOST_CHECK_EQUAL(activationCount, 1);

    auto resolvedInstance2 = container->resolve< Testing::DefaultConstructible1 >();
    BOOST_CHECK(resolvedInstance2 != nullptr);
    BOOST_CHECK(resolvedInstance2 != resolvedInstance);
    BOOST_CHECK(activatedInstance == resolvedInstance2);
    BOOST_CHECK_EQUAL(activationCount, 2);
}

BOOST_AUTO_TEST_CASE(should_call_activation_handler_only_once_when_a_single_instance_is_activated)
{
    // Arrange
    ContainerBuilder builder;
    std::shared_ptr< Testing::DefaultConstructible1 > activatedInstance;
    int activationCount = 0;

    // Act
    builder.registerType< Testing::DefaultConstructible1 >()
        .singleInstance()
        .onActivated([&](Container&, const std::shared_ptr< Testing::DefaultConstructible1 >& instance)
        {
            activatedInstance = instance;
            activationCount++;
        });

    auto container = builder.build();

    // Assert
    auto resolvedInstance = container->resolve< Testing::DefaultConstructible1 >();
    BOOST_CHECK(resolvedInstance != nullptr);
    BOOST_CHECK(activatedInstance == resolvedInstance);
    BOOST_CHECK_EQUAL(activationCount, 1);

    auto resolvedInstance2 = container->resolve< Testing::DefaultConstructible1 >();
    BOOST_CHECK(resolvedInstance2 != nullptr);
    BOOST_CHECK(resolvedInstance2 == resolvedInstance);
    BOOST_CHECK(activatedInstance == resolvedInstance2);
    BOOST_CHECK_EQUAL(activationCount, 1);
}

BOOST_AUTO_TEST_CASE(should_throw_when_detecting_a_circular_dependency)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::Type1 >().as< Testing::BaseType1 >();
    builder.registerType< Testing::Type2 >().as< Testing::BaseType2 >();

    auto container = builder.build();

    // Assert
    BOOST_CHECK_THROW(container->resolve< Testing::BaseType1 >(), std::exception);
    BOOST_CHECK_THROW(container->resolve< Testing::BaseType2 >(), std::exception);
}

BOOST_AUTO_TEST_CASE(should_resolve_non_registered_types)
{
    // Arrange
    ContainerBuilder builder;

    builder.registerType< Testing::ProvidedDependency >().as< Testing::ProvidedDependencyBase >();

    auto container = builder.build();

    // Act
    auto instance = container->resolve< Testing::AutowiredProvidedConstructor >();

    // Assert
    BOOST_REQUIRE(instance != nullptr);
    BOOST_CHECK(instance->dependency != nullptr);
}

BOOST_AUTO_TEST_CASE(should_resolve_non_registered_types_recursively)
{
    // Arrange
    ContainerBuilder builder;
    auto container = builder.build();

    // Act
    auto instance = container->resolve< Testing::MissingConstructor >();

    // Assert
    BOOST_REQUIRE(instance != nullptr);
    BOOST_CHECK(instance->dependency != nullptr);
}

BOOST_AUTO_TEST_SUITE_END()