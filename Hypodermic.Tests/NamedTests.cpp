#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


namespace Hypodermic
{
namespace Testing
{

    BOOST_AUTO_TEST_SUITE(NamedTests)

    BOOST_AUTO_TEST_CASE(should_resolve_named_component)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< DefaultConstructible1 >().named< DefaultConstructibleBase >("default1");

        auto container = builder.build();

        // Assert
        auto instance = container->resolveNamed< DefaultConstructibleBase >("default1");
        BOOST_CHECK(instance != nullptr);
    }

    BOOST_AUTO_TEST_CASE(should_resolve_named_component_as_itself)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< DefaultConstructible1 >().named("default1");

        auto container = builder.build();

        // Assert
        auto instance = container->resolveNamed< DefaultConstructible1 >("default1");
        BOOST_CHECK(instance != nullptr);
    }

    BOOST_AUTO_TEST_CASE(should_not_resolve_named_component_without_its_name)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< DefaultConstructible1 >().named< DefaultConstructibleBase >("default1");

        auto container = builder.build();

        // Assert
        BOOST_CHECK_THROW(container->resolve< DefaultConstructibleBase >(), std::exception);
    }

    BOOST_AUTO_TEST_CASE(should_resolve_the_right_named_component)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< DefaultConstructible1 >().named< DefaultConstructibleBase >("default1");
        builder.registerType< DefaultConstructible2 >().named< DefaultConstructibleBase >("default2");

        auto container = builder.build();

        // Assert
        auto instance = container->resolveNamed< DefaultConstructibleBase >("default1");
        BOOST_CHECK(instance != nullptr);
        BOOST_CHECK(std::dynamic_pointer_cast< DefaultConstructible1 >(instance) == instance);
    }

    BOOST_AUTO_TEST_CASE(should_not_conflict_with_anonymous_registrations)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        auto instance = std::make_shared< DefaultConstructible1 >();
        builder.registerInstance(instance).as< DefaultConstructibleBase >();

        builder.registerType< DefaultConstructible1 >().named< DefaultConstructibleBase >("default");

        auto container = builder.build();

        // Assert
        auto resolvedInstance = container->resolve< DefaultConstructibleBase >();
        auto namedInstance1 = container->resolveNamed< DefaultConstructibleBase >("default");
        auto namedInstance2 = container->resolveNamed< DefaultConstructibleBase >("default");

        BOOST_CHECK(resolvedInstance == instance);

        BOOST_CHECK(namedInstance1 != nullptr);
        BOOST_CHECK(namedInstance1 != instance);

        BOOST_CHECK(namedInstance2 != nullptr);
        BOOST_CHECK(namedInstance2 != instance);

        BOOST_CHECK(namedInstance1 != namedInstance2);
    }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace Testing
} // namespace Hypodermic