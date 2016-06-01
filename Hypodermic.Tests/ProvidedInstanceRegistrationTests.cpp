#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


namespace Hypodermic
{
namespace Testing
{

    BOOST_AUTO_TEST_SUITE(ProvidedInstanceRegistrationTests)

    BOOST_AUTO_TEST_CASE(should_register_an_instance)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        auto instance = std::make_shared< DefaultConstructible1 >();
        builder.registerInstance(instance);

        auto container = builder.build();

        // Assert
        auto resolvedInstnace = container->resolve< DefaultConstructible1 >();
        BOOST_CHECK(resolvedInstnace != nullptr);
        BOOST_CHECK(resolvedInstnace == instance);
    }

    BOOST_AUTO_TEST_CASE(should_register_an_instance_as_a_base_class)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        auto instance = std::make_shared< DefaultConstructible1 >();
        builder.registerInstance(instance).as< DefaultConstructibleBase >();

        auto container = builder.build();

        // Assert
        auto resolvedInstnace = container->resolve< DefaultConstructibleBase >();
        BOOST_CHECK(resolvedInstnace != nullptr);
        BOOST_CHECK(resolvedInstnace == instance);
    }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace Testing
} // namespace Hypodermic