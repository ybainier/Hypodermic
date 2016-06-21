#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


namespace Hypodermic
{
namespace Testing
{

    BOOST_AUTO_TEST_SUITE(NestedContainerTests)

    BOOST_AUTO_TEST_CASE(should_not_resolve_dependency)
    {
        // Arrange
        ContainerBuilder builder;
        builder.registerType< TopLevelConstructor >();

        auto container = builder.build();

        // Act
        auto instance = container->resolve< TopLevelConstructor >();

        // Assert
        BOOST_REQUIRE(instance != nullptr);
        BOOST_CHECK(instance->dependency == nullptr);
    }

    BOOST_AUTO_TEST_CASE(should_resolve_dependency_in_nested_container)
    {
        // Arrange
        int expectedNumber = 42;

        ContainerBuilder builder;
        builder.registerType< TopLevelConstructor >();

        auto container = builder.build();

        // Act
        ContainerBuilder nestedContainerBuilder;
        nestedContainerBuilder.registerInstance(std::make_shared< NestedDependency >(expectedNumber));
        auto nestedContainer = nestedContainerBuilder.buildNestedContainerFrom(*container);

        // Assert
        auto instance = container->resolve< TopLevelConstructor >();
        BOOST_REQUIRE(instance != nullptr);
        BOOST_CHECK(instance->dependency == nullptr);

        instance = nestedContainer->resolve< TopLevelConstructor >();
        BOOST_REQUIRE(instance != nullptr);
        BOOST_REQUIRE(instance->dependency != nullptr);
        BOOST_CHECK_EQUAL(instance->dependency->i, expectedNumber);
    }

    BOOST_AUTO_TEST_CASE(should_keep_reference_on_top_level_registrations)
    {
        // Arrange
        int expectedNumber = 42;

        std::shared_ptr< Container > nestedContainer;
        {
            ContainerBuilder builder;
            builder.registerType< TopLevelConstructor >();

            auto container = builder.build();

            // Act
            ContainerBuilder nestedContainerBuilder;
            nestedContainerBuilder.registerInstance(std::make_shared< NestedDependency >(expectedNumber));
            nestedContainer = nestedContainerBuilder.buildNestedContainerFrom(*container);
        }

        // Assert
        BOOST_REQUIRE(nestedContainer != nullptr);

        auto instance = nestedContainer->resolve< TopLevelConstructor >();
        BOOST_REQUIRE(instance != nullptr);
        BOOST_REQUIRE(instance->dependency != nullptr);
        BOOST_CHECK_EQUAL(instance->dependency->i, expectedNumber);
    }

    BOOST_AUTO_TEST_CASE(should_resolve_all_registrations_of_a_type)
    {
        // Arrange
        std::shared_ptr< Container > nestedContainer;
        {
            ContainerBuilder builder;
            builder.registerType< DefaultConstructible1 >().as< DefaultConstructibleBase >();

            auto container = builder.build();

            ContainerBuilder nestedContainerBuilder;
            nestedContainerBuilder.registerType< DefaultConstructible2 >().as< DefaultConstructibleBase >();
            nestedContainer = nestedContainerBuilder.buildNestedContainerFrom(*container);
        }

        BOOST_REQUIRE(nestedContainer != nullptr);

        // Act
        auto instances = nestedContainer->resolveAll< DefaultConstructibleBase >();

        // Assert
        BOOST_REQUIRE_EQUAL(instances.size(), 2);
        BOOST_CHECK(instances[0] != nullptr);
        BOOST_CHECK(instances[1] != nullptr);
        BOOST_CHECK(instances[0] != instances[1]);
    }

    BOOST_AUTO_TEST_CASE(should_resolve_a_type_using_its_last_registration)
    {
        // Arrange
        std::shared_ptr< Container > nestedContainer;
        {
            ContainerBuilder builder;
            builder.registerType< DefaultConstructible1 >().as< DefaultConstructibleBase >();

            auto container = builder.build();

            ContainerBuilder nestedContainerBuilder;
            nestedContainerBuilder.registerType< DefaultConstructible2 >().as< DefaultConstructibleBase >();
            nestedContainer = nestedContainerBuilder.buildNestedContainerFrom(*container);
        }

        BOOST_REQUIRE(nestedContainer != nullptr);

        // Act
        auto instance = nestedContainer->resolve< DefaultConstructibleBase >();

        // Assert
        BOOST_CHECK(instance != nullptr);
        BOOST_CHECK(std::dynamic_pointer_cast< DefaultConstructible2 >(instance) == instance);
    }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace Testing
} // namespace Hypodermic