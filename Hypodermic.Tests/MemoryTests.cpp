#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


namespace Hypodermic
{
namespace Testing
{

    BOOST_AUTO_TEST_SUITE(MemoryTests)

    BOOST_AUTO_TEST_CASE(container_should_be_freed)
    {
        // Arrange
        std::weak_ptr< Container > weakContainer;
        {
            ContainerBuilder builder;

            auto container = builder.build();

            weakContainer = container;

            // Act - exit from scope
        }

        // Assert
        auto container = weakContainer.lock();
        BOOST_CHECK(container == nullptr);
    }

    BOOST_AUTO_TEST_CASE(single_instance_should_be_freed)
    {
        // Arrange
        std::weak_ptr< DefaultConstructible1 > weakInstance;
        {
            std::shared_ptr< Container > container;
            {
                ContainerBuilder builder;
                builder.registerType< DefaultConstructible1 >().singleInstance();

                container = builder.build();
            }

            auto instance1 = container->resolve< DefaultConstructible1 >();
            auto instance2 = container->resolve< DefaultConstructible1 >();

            // Making sure this is not an on the fly registration
            BOOST_REQUIRE(instance1 != nullptr);
            BOOST_REQUIRE(instance1 == instance2);
            weakInstance = instance1;

            // Act - exit from scope
        }

        // Assert
        auto instance = weakInstance.lock();
        BOOST_CHECK(instance == nullptr);
    }

    BOOST_AUTO_TEST_CASE(provided_instance_should_be_freed)
    {
        // Arrange
        std::weak_ptr< DefaultConstructible1 > weakInstance;
        {
            std::shared_ptr< Container > container;
            {
                ContainerBuilder builder;
                builder.registerInstance(std::make_shared< DefaultConstructible1 >());

                container = builder.build();
            }

            auto instance1 = container->resolve< DefaultConstructible1 >();
            auto instance2 = container->resolve< DefaultConstructible1 >();

            // Making sure this is not an on the fly registration
            BOOST_REQUIRE(instance1 != nullptr);
            BOOST_REQUIRE(instance1 == instance2);
            weakInstance = instance1;

            // Act - exit from scope
        }

        // Assert
        auto instance = weakInstance.lock();
        BOOST_CHECK(instance == nullptr);
    }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace Testing
} // namespace Hypodermic