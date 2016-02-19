#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


using namespace Hypodermic;


BOOST_AUTO_TEST_SUITE(ProvidedConstructorRegistrationTests)

BOOST_AUTO_TEST_CASE(should_register_type_with_autowired_provided_constructor)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::ProvidedDependency >().as< Testing::ProvidedDependencyBase >();
    builder.registerType< Testing::AutowiredProvidedConstructor >();

    auto container = builder.build();

    // Assert
    auto instance = container->resolve< Testing::AutowiredProvidedConstructor >();
    BOOST_REQUIRE(instance != nullptr);
    BOOST_REQUIRE(instance->dependency != nullptr);
}

BOOST_AUTO_TEST_CASE(should_register_type_with_provided_constructor_just_as_if_they_were_autowired)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::ProvidedDependency >().as< Testing::ProvidedDependencyBase >();
    
    builder.registerType< Testing::NonAutowiredProvidedConstructor >()
           .usingConstructor< Testing::NonAutowiredProvidedConstructor(Testing::ProvidedDependencyBase*) >();

    auto container = builder.build();

    // Assert
    auto instance = container->resolve< Testing::NonAutowiredProvidedConstructor >();
    BOOST_REQUIRE(instance != nullptr);
    BOOST_REQUIRE(instance->dependency != nullptr);
}

BOOST_AUTO_TEST_CASE(should_register_type_with_provided_constructor_and_dependency)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::ProvidedDependency >();
    
    builder.registerType< Testing::NonAutowiredProvidedConstructor >()
           .usingConstructor< Testing::NonAutowiredProvidedConstructor(Testing::ProvidedDependencyBase*) >()
           .with< Testing::ProvidedDependencyBase, Testing::ProvidedDependency >();

    auto container = builder.build();

    // Assert
    auto instance = container->resolve< Testing::NonAutowiredProvidedConstructor >();
    BOOST_REQUIRE(instance != nullptr);
    BOOST_REQUIRE(instance->dependency != nullptr);
}

BOOST_AUTO_TEST_CASE(should_register_type_with_provided_constructor_and_instance_dependency)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    auto dependency = std::make_shared< Testing::ProvidedDependency >();
    
    builder.registerType< Testing::NonAutowiredProvidedConstructor >()
           .usingConstructor< Testing::NonAutowiredProvidedConstructor(Testing::ProvidedDependencyBase*) >()
           .with< Testing::ProvidedDependencyBase >(dependency);

    auto container = builder.build();

    // Assert
    auto instance = container->resolve< Testing::NonAutowiredProvidedConstructor >();
    BOOST_REQUIRE(instance != nullptr);
    BOOST_REQUIRE(instance->dependency != nullptr);
}

BOOST_AUTO_TEST_CASE(should_register_type_with_provided_constructor_and_instance_factory_dependency)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::ProvidedDependency >();

    builder.registerType< Testing::NonAutowiredProvidedConstructor >()
           .usingConstructor< Testing::NonAutowiredProvidedConstructor(Testing::ProvidedDependencyBase*) >()
           .with< Testing::ProvidedDependencyBase >([](Container& c)
            {
                return c.resolve< Testing::ProvidedDependency >();
            });

    auto container = builder.build();

    // Assert
    auto instance = container->resolve< Testing::NonAutowiredProvidedConstructor >();
    BOOST_REQUIRE(instance != nullptr);
    BOOST_REQUIRE(instance->dependency != nullptr);
}

BOOST_AUTO_TEST_SUITE_END()