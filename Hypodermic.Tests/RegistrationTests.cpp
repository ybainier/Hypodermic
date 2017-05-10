#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


namespace Hypodermic
{
namespace Testing
{

    BOOST_AUTO_TEST_SUITE(RegistrationTests)

    BOOST_AUTO_TEST_CASE(should_call_activation_handler_everytime_an_instance_is_activated_through_resolution)
    {
        // Arrange
        ContainerBuilder builder;

        std::vector< std::shared_ptr< DefaultConstructible1 > > activatedInstances;

        // Act
        builder.registerType< DefaultConstructible1 >().onActivated([&activatedInstances](ComponentContext&, const std::shared_ptr< DefaultConstructible1 >& instance)
        {
            activatedInstances.push_back(instance);
        });

        auto container = builder.build();

        // Assert
        auto instance1 = container->resolve< DefaultConstructible1 >();
        BOOST_CHECK(instance1 != nullptr);

        auto instance2 = container->resolve< DefaultConstructible1 >();
        BOOST_CHECK(instance2 != nullptr);

        BOOST_CHECK(instance1 != instance2);

        BOOST_REQUIRE_EQUAL(activatedInstances.size(), 2u);
        BOOST_CHECK(instance1 == activatedInstances[0]);
        BOOST_CHECK(instance2 == activatedInstances[1]);
    }

    BOOST_AUTO_TEST_CASE(should_call_activation_handler_only_once_for_single_instance_mode)
    {
        // Arrange
        ContainerBuilder builder;

        std::vector< std::shared_ptr< DefaultConstructible1 > > activatedInstances;

        // Act
        builder.registerType< DefaultConstructible1 >()
               .singleInstance()
               .onActivated([&activatedInstances](ComponentContext&, const std::shared_ptr< DefaultConstructible1 >& instance)
                {
                    activatedInstances.push_back(instance);
                });

        auto container = builder.build();

        // Assert
        auto instance1 = container->resolve< DefaultConstructible1 >();
        BOOST_CHECK(instance1 != nullptr);

        auto instance2 = container->resolve< DefaultConstructible1 >();
        BOOST_CHECK(instance2 != nullptr);

        BOOST_CHECK(instance1 == instance2);

        BOOST_REQUIRE_EQUAL(activatedInstances.size(), 1u);
        BOOST_CHECK(instance1 == activatedInstances[0]);
    }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace Testing
} // namespace Hypodermic