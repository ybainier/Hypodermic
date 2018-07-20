#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


namespace Hypodermic
{
namespace Testing
{

    BOOST_AUTO_TEST_SUITE(FactoryTests)

#if !defined(VS2013)

    BOOST_AUTO_TEST_CASE(should_inject_instance_factory)
    {
        // Arrange
        ContainerBuilder builder;
        builder.registerType< Loader >().as< ILoader >();
    
        auto container = builder.build();

        // Act
        auto instance = container->resolve< TypeWithInjectedFactory >();

        // Assert
        BOOST_REQUIRE(instance != nullptr);
        BOOST_REQUIRE(instance->factory);

        auto loader1 = instance->factory();
        BOOST_CHECK(loader1 != nullptr);

        auto loader2 = instance->factory();
        BOOST_CHECK(loader2 != nullptr);

        BOOST_CHECK(loader1 != loader2);
    }

    BOOST_AUTO_TEST_CASE(should_inject_instance_factory_that_keeps_registration_configuration)
    {
        // Arrange
        ContainerBuilder builder;
        builder.registerType< Loader >().as< ILoader >().singleInstance();
    
        auto container = builder.build();

        // Act
        auto instance = container->resolve< TypeWithInjectedFactory >();

        // Assert
        BOOST_REQUIRE(instance != nullptr);
        BOOST_REQUIRE(instance->factory);

        auto loader1 = instance->factory();
        BOOST_CHECK(loader1 != nullptr);

        auto loader2 = instance->factory();
        BOOST_CHECK(loader2 != nullptr);

        BOOST_CHECK(loader1 == loader2);
    }

    BOOST_AUTO_TEST_CASE(should_not_keep_container_in_injected_instance_factory)
    {
        // Arrange
        std::shared_ptr< TypeWithInjectedFactory > instance;

        {
            ContainerBuilder builder;
            builder.registerType< Loader >().as< ILoader >().singleInstance();
    
            auto container = builder.build();

            // Act
            instance = container->resolve< TypeWithInjectedFactory >();
        }

        // Assert
        BOOST_REQUIRE(instance != nullptr);
        BOOST_REQUIRE(instance->factory);

        BOOST_CHECK_THROW(instance->factory(), std::exception);
    }

#endif // !defined(VS2013)


    BOOST_AUTO_TEST_CASE(should_inject_instance_factory_builder)
    {
        // Arrange
        ContainerBuilder builder;
        builder.registerType< Loader >().as< ILoader >();
    
        auto container = builder.build();

        // Act
        auto instance = container->resolve< TypeWithInjectedFactoryWrapper >();

        // Assert
        BOOST_REQUIRE(instance != nullptr);
        BOOST_REQUIRE(instance->factory);

        auto loader1 = instance->factory();
        BOOST_CHECK(loader1 != nullptr);

        auto loader2 = instance->factory();
        BOOST_CHECK(loader2 != nullptr);

        BOOST_CHECK(loader1 != loader2);
    }

    BOOST_AUTO_TEST_CASE(should_inject_instance_factory_builder_that_keeps_registration_configuration)
    {
        // Arrange
        ContainerBuilder builder;
        builder.registerType< Loader >().as< ILoader >().singleInstance();
    
        auto container = builder.build();

        // Act
        auto instance = container->resolve< TypeWithInjectedFactoryWrapper >();

        // Assert
        BOOST_REQUIRE(instance != nullptr);
        BOOST_REQUIRE(instance->factory);

        auto loader1 = instance->factory();
        BOOST_CHECK(loader1 != nullptr);

        auto loader2 = instance->factory();
        BOOST_CHECK(loader2 != nullptr);

        BOOST_CHECK(loader1 == loader2);
    }

    BOOST_AUTO_TEST_CASE(should_not_keep_container_in_injected_instance_factory_builder)
    {
        // Arrange
        std::shared_ptr< TypeWithInjectedFactoryWrapper > instance;

        {
            ContainerBuilder builder;
            builder.registerType< Loader >().as< ILoader >().singleInstance();
    
            auto container = builder.build();

            // Act
            instance = container->resolve< TypeWithInjectedFactoryWrapper >();
        }

        // Assert
        BOOST_REQUIRE(instance != nullptr);
        BOOST_REQUIRE(instance->factory);

        BOOST_CHECK_THROW(instance->factory(), std::exception);
    }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace Testing
} // namespace Hypodermic