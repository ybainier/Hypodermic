#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


using namespace Hypodermic;


BOOST_AUTO_TEST_SUITE(ProvidedDependenciesTests)

BOOST_AUTO_TEST_CASE(should_register_type_with_autowired_provided_constructor)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::ProvidedDependency >().as< Testing::ProvidedDependencyBase >();
    builder.registerType< Testing::TypeWithOneDependency >();

    auto container = builder.build();

    // Assert
    auto instance = container->resolve< Testing::TypeWithOneDependency >();
    BOOST_REQUIRE(instance != nullptr);
    BOOST_CHECK(instance->dependency != nullptr);
}

BOOST_AUTO_TEST_CASE(should_register_type_with_provided_dependency)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::ProvidedDependency >();
    
    builder.registerType< Testing::TypeWithOneDependency >()
           .with< Testing::ProvidedDependencyBase, Testing::ProvidedDependency >();

    auto container = builder.build();

    // Assert
    auto instance = container->resolve< Testing::TypeWithOneDependency >();
    BOOST_REQUIRE(instance != nullptr);
    BOOST_CHECK(instance->dependency != nullptr);
}

BOOST_AUTO_TEST_CASE(should_register_type_with_provided_instance_dependency)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    auto dependency = std::make_shared< Testing::ProvidedDependency >();
    
    builder.registerType< Testing::TypeWithOneDependency >()
           .with< Testing::ProvidedDependencyBase >(dependency);

    auto container = builder.build();

    // Assert
    auto instance = container->resolve< Testing::TypeWithOneDependency >();
    BOOST_REQUIRE(instance != nullptr);
    BOOST_CHECK(instance->dependency != nullptr);
}

BOOST_AUTO_TEST_CASE(should_register_type_with_provided_instance_factory_dependency)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::ProvidedDependency >();

    builder.registerType< Testing::TypeWithOneDependency >()
           .with< Testing::ProvidedDependencyBase >([](Container& c)
            {
                return c.resolve< Testing::ProvidedDependency >();
            });

    auto container = builder.build();

    // Assert
    auto instance = container->resolve< Testing::TypeWithOneDependency >();
    BOOST_REQUIRE(instance != nullptr);
    BOOST_CHECK(instance->dependency != nullptr);
}

BOOST_AUTO_TEST_CASE(should_register_type_as_an_alias_with_autowired_provided_constructor)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::ProvidedDependency >().as< Testing::ProvidedDependencyBase >();
    builder.registerType< Testing::TypeWithOneDependency >().as< Testing::RandomTypeBase >();

    auto container = builder.build();

    // Assert
    std::shared_ptr< Testing::RandomTypeBase > instanceBase;
    BOOST_REQUIRE_NO_THROW(instanceBase = container->resolve< Testing::RandomTypeBase >());

    auto instance = std::dynamic_pointer_cast< Testing::TypeWithOneDependency >(instanceBase);

    BOOST_REQUIRE(instance != nullptr);
    BOOST_CHECK(instance->dependency != nullptr);
}

BOOST_AUTO_TEST_CASE(should_register_type_as_an_alias_with_provided_dependency)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::ProvidedDependency >();

    builder.registerType< Testing::TypeWithOneDependency >()
        .with< Testing::ProvidedDependencyBase, Testing::ProvidedDependency >()
        .as< Testing::RandomTypeBase >();

    auto container = builder.build();

    // Assert
    std::shared_ptr< Testing::RandomTypeBase > instanceBase;
    BOOST_REQUIRE_NO_THROW(instanceBase = container->resolve< Testing::RandomTypeBase >());

    auto instance = std::dynamic_pointer_cast< Testing::TypeWithOneDependency >(instanceBase);

    BOOST_REQUIRE(instance != nullptr);
    BOOST_CHECK(instance->dependency != nullptr);
}

BOOST_AUTO_TEST_CASE(should_register_type_as_an_alias_with_provided_instance_dependency)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    auto dependency = std::make_shared< Testing::ProvidedDependency >();

    builder.registerType< Testing::TypeWithOneDependency >()
        .with< Testing::ProvidedDependencyBase >(dependency)
        .as< Testing::RandomTypeBase >();

    auto container = builder.build();

    // Assert
    std::shared_ptr< Testing::RandomTypeBase > instanceBase;
    BOOST_REQUIRE_NO_THROW(instanceBase = container->resolve< Testing::RandomTypeBase >());

    auto instance = std::dynamic_pointer_cast< Testing::TypeWithOneDependency >(instanceBase);

    BOOST_REQUIRE(instance != nullptr);
    BOOST_CHECK(instance->dependency != nullptr);
}

BOOST_AUTO_TEST_CASE(should_register_type_as_an_alias_with_provided_instance_factory_dependency)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::ProvidedDependency >();

    builder.registerType< Testing::TypeWithOneDependency >()
        .with< Testing::ProvidedDependencyBase >([](Container& c)
        {
            return c.resolve< Testing::ProvidedDependency >();
        })
        .as< Testing::RandomTypeBase >();

    auto container = builder.build();

    // Assert
    std::shared_ptr< Testing::RandomTypeBase > instanceBase;
    BOOST_REQUIRE_NO_THROW(instanceBase = container->resolve< Testing::RandomTypeBase >());

    auto instance = std::dynamic_pointer_cast< Testing::TypeWithOneDependency >(instanceBase);

    BOOST_REQUIRE(instance != nullptr);
    BOOST_CHECK(instance->dependency != nullptr);
}

BOOST_AUTO_TEST_SUITE_END()