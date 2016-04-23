#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"

using namespace Hypodermic;

BOOST_AUTO_TEST_SUITE(MemotyTests)

BOOST_AUTO_TEST_CASE(container_should_be_freed)
{
    // Arrange
    std::weak_ptr<Container> weakContainer;
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
    std::weak_ptr<Testing::DefaultConstructible1> weakInstance;
    {
        ContainerBuilder builder;
        builder.registerType<Testing::DefaultConstructible1>().singleInstance();
        auto container = builder.build();
        auto instance = container->resolve<Testing::DefaultConstructible1>();
        weakInstance = instance;

        // Act - exit from scope
    }

    // Assert
    auto instance = weakInstance.lock();
    BOOST_CHECK(instance == nullptr);
}

BOOST_AUTO_TEST_SUITE_END()
