#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


namespace Hypodermic
{
namespace Testing
{

    BOOST_AUTO_TEST_SUITE(NestedContainerTests)

    BOOST_AUTO_TEST_CASE(should_not_resolve_non_auto_registerable_unregistered_dependency)
    {
        // Arrange
        ContainerBuilder builder;
        builder.registerType< TopLevelConstructor >();

        auto container = builder.build();

        // Act & Assert
        BOOST_CHECK_THROW(container->resolve< TopLevelConstructor >(), std::exception);
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
        BOOST_CHECK_THROW(container->resolve< TopLevelConstructor >(), std::exception);

        auto instance = nestedContainer->resolve< TopLevelConstructor >();
        BOOST_REQUIRE(instance != nullptr);
        BOOST_REQUIRE(instance->dependency != nullptr);
        BOOST_CHECK_EQUAL(instance->dependency->i, expectedNumber);
    }

     BOOST_AUTO_TEST_CASE(should_resolve_dependency_in_nested_of_nested_container)
     {
         // Arrange
         int expectedNumber1 = 42;
         int expectedNumber2 = 43;
 
         ContainerBuilder builder;
         builder.registerType< TopLevelConstructor >();
 
         auto container = builder.build();
 
         ContainerBuilder nestedContainerBuilder1;
         nestedContainerBuilder1.registerInstance(std::make_shared< NestedDependency >(expectedNumber1));
         auto nestedContainer1 = nestedContainerBuilder1.buildNestedContainerFrom(*container);
 
         // Act
         ContainerBuilder nestedContainerBuilder2;
         nestedContainerBuilder2.registerInstance(std::make_shared< NestedDependency >(expectedNumber2));
         auto nestedContainer2 = nestedContainerBuilder2.buildNestedContainerFrom(*nestedContainer1);
 
         // Assert
         BOOST_CHECK_THROW(container->resolve< TopLevelConstructor >(), std::exception);
 
         auto instance = nestedContainer1->resolve< TopLevelConstructor >();
         BOOST_REQUIRE(instance != nullptr);
         BOOST_REQUIRE(instance->dependency != nullptr);
         BOOST_CHECK_EQUAL(instance->dependency->i, expectedNumber1);
 
         instance = nestedContainer2->resolve< TopLevelConstructor >();
         BOOST_REQUIRE(instance != nullptr);
         BOOST_REQUIRE(instance->dependency != nullptr);
         BOOST_CHECK_EQUAL(instance->dependency->i, expectedNumber2);
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
        BOOST_REQUIRE_EQUAL(instances.size(), 2u);
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

    BOOST_AUTO_TEST_CASE(should_no_longer_resolve_the_same_type_when_nested_container_is_released)
    {
        // Arrange
        std::shared_ptr< Container > container;

        std::shared_ptr< DefaultConstructibleBase > nestedInstance;
        {
            ContainerBuilder builder;
            builder.registerType< DefaultConstructible1 >().as< DefaultConstructibleBase >();

            container = builder.build();

            ContainerBuilder nestedContainerBuilder;
            nestedContainerBuilder.registerType< DefaultConstructible2 >().as< DefaultConstructibleBase >();
            auto nestedContainer = nestedContainerBuilder.buildNestedContainerFrom(*container);

            nestedInstance = nestedContainer->resolve< DefaultConstructibleBase >();
        }

        auto instance = container->resolve< DefaultConstructibleBase >();

        // Assert
        BOOST_CHECK(std::dynamic_pointer_cast< DefaultConstructible1 >(instance) == instance);
        BOOST_CHECK(std::dynamic_pointer_cast< DefaultConstructible2 >(nestedInstance) == nestedInstance);
    }

    BOOST_AUTO_TEST_CASE(should_no_longer_resolve_the_same_instance_when_nested_container_is_released)
    {
        // Arrange
        std::shared_ptr< Container > container;

        std::shared_ptr< DefaultConstructibleBase > nestedInstance1;
        std::shared_ptr< DefaultConstructibleBase > nestedInstance2;
        {
            ContainerBuilder builder;
            builder.registerType< DefaultConstructible1 >().as< DefaultConstructibleBase >().singleInstance();

            container = builder.build();

            ContainerBuilder nestedContainerBuilder;
            nestedContainerBuilder.registerType< DefaultConstructible2 >().as< DefaultConstructibleBase >().singleInstance();
            auto nestedContainer = nestedContainerBuilder.buildNestedContainerFrom(*container);

            nestedInstance1 = nestedContainer->resolve< DefaultConstructibleBase >();
            nestedInstance2 = nestedContainer->resolve< DefaultConstructibleBase >();
        }

        auto instance1 = container->resolve< DefaultConstructibleBase >();
        auto instance2 = container->resolve< DefaultConstructibleBase >();

        // Assert
        BOOST_CHECK(std::dynamic_pointer_cast< DefaultConstructible1 >(instance1) == instance2);
        BOOST_CHECK(std::dynamic_pointer_cast< DefaultConstructible2 >(nestedInstance1) == nestedInstance2);
        BOOST_CHECK(instance1 != nestedInstance1);
    }

    BOOST_AUTO_TEST_CASE(should_keep_using_resolved_instance_in_nested_container)
    {
        // Arrange
        std::shared_ptr< Container > container;

        std::shared_ptr< DefaultConstructibleBase > instance;
        std::shared_ptr< DefaultConstructibleBase > parentInstanceFromNestedContainer;
        {
            ContainerBuilder builder;
            builder.registerType< DefaultConstructible1 >().as< DefaultConstructibleBase >().singleInstance();

            container = builder.build();

            instance = container->resolve< DefaultConstructibleBase >();

            ContainerBuilder nestedContainerBuilder;
            nestedContainerBuilder.registerType< DefaultConstructible2 >();
            auto nestedContainer = nestedContainerBuilder.buildNestedContainerFrom(*container);

            auto nestedInstance = nestedContainer->resolve< DefaultConstructible2 >();
            BOOST_CHECK(nestedInstance != nullptr);

            parentInstanceFromNestedContainer = container->resolve< DefaultConstructibleBase >();
        }

        auto instanceAfterNestedContainerIsReleased = container->resolve< DefaultConstructibleBase >();

        // Assert
        BOOST_CHECK(std::dynamic_pointer_cast< DefaultConstructible1 >(instance) == instance);
        BOOST_CHECK(parentInstanceFromNestedContainer == instance);
        BOOST_CHECK(instanceAfterNestedContainerIsReleased == instance);
    }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace Testing
} // namespace Hypodermic