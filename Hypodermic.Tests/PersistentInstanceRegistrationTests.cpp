#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


namespace Hypodermic
{
namespace Testing
{

    BOOST_AUTO_TEST_SUITE(PersistentInstanceRegistrationTests)

    BOOST_AUTO_TEST_CASE(should_resolve_the_same_instance)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< DefaultConstructible1 >().singleInstance();

        auto container = builder.build();

        // Assert
        auto instance1 = container->resolve< DefaultConstructible1 >();
        auto instance2 = container->resolve< DefaultConstructible1 >();
        BOOST_CHECK(instance1 != nullptr);
        BOOST_CHECK(instance1 == instance2);
    }

    BOOST_AUTO_TEST_CASE(should_resolve_the_same_instance_when_using_an_alias)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< DefaultConstructible1 >()
               .as< DefaultConstructibleBase >()
               .asSelf()
               .singleInstance();

        auto container = builder.build();

        // Assert
        auto instance1 = container->resolve< DefaultConstructible1 >();
        auto instance2 = container->resolve< DefaultConstructibleBase >();
        BOOST_CHECK(instance1 != nullptr);
        BOOST_CHECK(std::static_pointer_cast< DefaultConstructibleBase >(instance1) == instance2);
    }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace Testing
} // namespace Hypodermic