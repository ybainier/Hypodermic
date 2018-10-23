#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


namespace Hypodermic
{
namespace Testing
{

    struct RuntimeRegistrationFixture
    {
        ~RuntimeRegistrationFixture()
        {
            Behavior::configureRuntimeRegistration(true);
        }
    };

    BOOST_FIXTURE_TEST_SUITE(RuntimeRegistrationTests, RuntimeRegistrationFixture)
    
    BOOST_AUTO_TEST_CASE(should_resolve_non_registered_types)
    {
        // Arrange
        ContainerBuilder builder;

        builder.registerType< ProvidedDependency >().as< ProvidedDependencyBase >();

        auto container = builder.build();

        // Act
        auto instance = container->resolve< TypeWithOneDependency >();

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
        auto instance = container->resolve< MissingConstructor >();

        // Assert
        BOOST_REQUIRE(instance != nullptr);
        BOOST_CHECK(instance->dependency != nullptr);
    }

    BOOST_AUTO_TEST_CASE(should_not_resolve_non_registered_types_when_runtime_registration_is_disabled)
    {
        // Arrange
        ContainerBuilder builder;

        builder.registerType< ProvidedDependency >().as< ProvidedDependencyBase >();

        auto container = builder.build();

        // Act
        Behavior::configureRuntimeRegistration(false);

        // Assert
        BOOST_CHECK_THROW(container->resolve< TypeWithOneDependency >(), std::exception);
    }

    BOOST_AUTO_TEST_CASE(should_not_resolve_non_registered_types_recursively_when_runtime_registration_is_disabled)
    {
        // Arrange
        ContainerBuilder builder;

        builder.registerType< MissingConstructor >();

        auto container = builder.build();

        // Act
        Behavior::configureRuntimeRegistration(false);

        // Assert
        BOOST_CHECK_THROW(container->resolve< MissingConstructor >(), std::exception);
    }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace Testing
} // namespace Hypodermic