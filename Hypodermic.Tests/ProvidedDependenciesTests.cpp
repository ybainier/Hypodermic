#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


namespace Hypodermic
{
namespace Testing
{

    BOOST_AUTO_TEST_SUITE(ProvidedDependenciesTests)

    BOOST_AUTO_TEST_CASE(should_register_type_with_autowired_provided_constructor)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< ProvidedDependency >().as< ProvidedDependencyBase >();
        builder.registerType< TypeWithOneDependency >();

        auto container = builder.build();

        // Assert
        auto instance = container->resolve< TypeWithOneDependency >();
        BOOST_REQUIRE(instance != nullptr);
        BOOST_CHECK(instance->dependency != nullptr);
    }

    BOOST_AUTO_TEST_CASE(should_register_type_with_provided_dependency)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< ProvidedDependency >();
    
        builder.registerType< TypeWithOneDependency >()
               .with< ProvidedDependencyBase, ProvidedDependency >();

        auto container = builder.build();

        // Assert
        auto instance = container->resolve< TypeWithOneDependency >();
        BOOST_REQUIRE(instance != nullptr);
        BOOST_CHECK(instance->dependency != nullptr);
    }

    BOOST_AUTO_TEST_CASE(should_register_type_with_provided_instance_dependency)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        auto dependency = std::make_shared< ProvidedDependency >();
    
        builder.registerType< TypeWithOneDependency >()
               .with< ProvidedDependencyBase >(dependency);

        auto container = builder.build();

        // Assert
        auto instance = container->resolve< TypeWithOneDependency >();
        BOOST_REQUIRE(instance != nullptr);
        BOOST_CHECK(instance->dependency != nullptr);
    }

    BOOST_AUTO_TEST_CASE(should_register_type_with_provided_instance_factory_dependency)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< ProvidedDependency >();

        builder.registerType< TypeWithOneDependency >()
               .with< ProvidedDependencyBase >([](ComponentContext& c)
                {
                    return c.resolve< ProvidedDependency >();
                });

        auto container = builder.build();

        // Assert
        auto instance = container->resolve< TypeWithOneDependency >();
        BOOST_REQUIRE(instance != nullptr);
        BOOST_CHECK(instance->dependency != nullptr);
    }

    BOOST_AUTO_TEST_CASE(should_register_type_as_an_alias_with_autowired_provided_constructor)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< ProvidedDependency >().as< ProvidedDependencyBase >();
        builder.registerType< TypeWithOneDependency >().as< RandomTypeBase >();

        auto container = builder.build();

        // Assert
        std::shared_ptr< RandomTypeBase > instanceBase;
        BOOST_REQUIRE_NO_THROW(instanceBase = container->resolve< RandomTypeBase >());

        auto instance = std::dynamic_pointer_cast< TypeWithOneDependency >(instanceBase);

        BOOST_REQUIRE(instance != nullptr);
        BOOST_CHECK(instance->dependency != nullptr);
    }

    BOOST_AUTO_TEST_CASE(should_register_type_as_an_alias_with_provided_dependency)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< ProvidedDependency >();

        builder.registerType< TypeWithOneDependency >()
            .with< ProvidedDependencyBase, ProvidedDependency >()
            .as< RandomTypeBase >();

        auto container = builder.build();

        // Assert
        std::shared_ptr< RandomTypeBase > instanceBase;
        BOOST_REQUIRE_NO_THROW(instanceBase = container->resolve< RandomTypeBase >());

        auto instance = std::dynamic_pointer_cast< TypeWithOneDependency >(instanceBase);

        BOOST_REQUIRE(instance != nullptr);
        BOOST_CHECK(instance->dependency != nullptr);
    }

    BOOST_AUTO_TEST_CASE(should_register_type_as_an_alias_with_provided_instance_dependency)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        auto dependency = std::make_shared< ProvidedDependency >();

        builder.registerType< TypeWithOneDependency >()
            .with< ProvidedDependencyBase >(dependency)
            .as< RandomTypeBase >();

        auto container = builder.build();

        // Assert
        std::shared_ptr< RandomTypeBase > instanceBase;
        BOOST_REQUIRE_NO_THROW(instanceBase = container->resolve< RandomTypeBase >());

        auto instance = std::dynamic_pointer_cast< TypeWithOneDependency >(instanceBase);

        BOOST_REQUIRE(instance != nullptr);
        BOOST_CHECK(instance->dependency != nullptr);
    }

    BOOST_AUTO_TEST_CASE(should_register_type_as_an_alias_with_provided_instance_factory_dependency)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< ProvidedDependency >();

        builder.registerType< TypeWithOneDependency >()
            .with< ProvidedDependencyBase >([](ComponentContext& c)
            {
                return c.resolve< ProvidedDependency >();
            })
            .as< RandomTypeBase >();

        auto container = builder.build();

        // Assert
        std::shared_ptr< RandomTypeBase > instanceBase;
        BOOST_REQUIRE_NO_THROW(instanceBase = container->resolve< RandomTypeBase >());

        auto instance = std::dynamic_pointer_cast< TypeWithOneDependency >(instanceBase);

        BOOST_REQUIRE(instance != nullptr);
        BOOST_CHECK(instance->dependency != nullptr);
    }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace Testing
} // namespace Hypodermic