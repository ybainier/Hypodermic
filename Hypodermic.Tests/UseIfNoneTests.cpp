#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


namespace Hypodermic
{
namespace Testing
{

    BOOST_AUTO_TEST_SUITE(UseIfNoneTests)

    BOOST_AUTO_TEST_CASE(should_resolve_a_type_using_a_fallback_registration)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< DefaultConstructible1 >().useIfNone();

        auto container = builder.build();

        // Assert
        auto instance = container->resolve< DefaultConstructible1 >();
        BOOST_CHECK(instance != nullptr);
    }

    BOOST_AUTO_TEST_CASE(should_resolve_a_type_using_the_first_fallback_registration)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< DefaultConstructible1 >().as< DefaultConstructibleBase >().useIfNone();
        builder.registerType< DefaultConstructible2 >().as< DefaultConstructibleBase >().useIfNone();

        auto container = builder.build();

        // Assert
        auto instance = container->resolve< DefaultConstructibleBase >();
        BOOST_CHECK(instance != nullptr);
        BOOST_CHECK(std::dynamic_pointer_cast< DefaultConstructible1 >(instance) == instance);
    }

    BOOST_AUTO_TEST_CASE(should_not_override_default_registration)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< DefaultConstructible1 >().as< DefaultConstructibleBase >();
        builder.registerType< DefaultConstructible2 >().as< DefaultConstructibleBase >().useIfNone();

        auto container = builder.build();

        // Assert
        auto instance = container->resolve< DefaultConstructibleBase >();
        BOOST_CHECK(instance != nullptr);
        BOOST_CHECK(std::dynamic_pointer_cast< DefaultConstructible1 >(instance) == instance);
    }

    BOOST_AUTO_TEST_CASE(should_resolve_all_registration)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< DefaultConstructible1 >().as< DefaultConstructibleBase >().useIfNone();
        builder.registerType< DefaultConstructible2 >().as< DefaultConstructibleBase >().useIfNone();

        auto container = builder.build();

        // Assert
        auto instances = container->resolveAll< DefaultConstructibleBase >();
        BOOST_REQUIRE_EQUAL(instances.size(), 2u);
        BOOST_CHECK(instances[0] == std::dynamic_pointer_cast< DefaultConstructible2 >(instances[0]));
        BOOST_CHECK(instances[1] == std::dynamic_pointer_cast< DefaultConstructible1 >(instances[1]));
    }

    BOOST_AUTO_TEST_CASE(should_override_registration)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< DefaultConstructible1 >().as< DefaultConstructibleBase >();
        builder.registerType< DefaultConstructible2 >().as< DefaultConstructibleBase >().useIfNone();
        builder.registerType< DefaultConstructible3 >().as< DefaultConstructibleBase >();

        auto container = builder.build();

        // Assert
        auto instance = container->resolve< DefaultConstructibleBase >();
        BOOST_CHECK(instance != nullptr);
        BOOST_CHECK(std::dynamic_pointer_cast< DefaultConstructible3 >(instance) == instance);
    }

    BOOST_AUTO_TEST_CASE(should_fallback_to_parent_registration)
    {
        // Arrange
        ContainerBuilder builder;
        builder.registerType< DefaultConstructible1 >().as< DefaultConstructibleBase >();

        auto container = builder.build();

        // Act
        ContainerBuilder nestedContainerBuilder;
        nestedContainerBuilder.registerType< DefaultConstructible2 >().as< DefaultConstructibleBase >().useIfNone();
        auto nestedContainer = nestedContainerBuilder.buildNestedContainerFrom(*container);

        // Assert
        auto instance = nestedContainer->resolve< DefaultConstructibleBase >();
        BOOST_CHECK(instance != nullptr);
        BOOST_CHECK(std::dynamic_pointer_cast< DefaultConstructible1 >(instance) == instance);
    }

    BOOST_AUTO_TEST_CASE(should_ignore_parent_registration)
    {
        // Arrange
        ContainerBuilder builder;
        builder.registerType< DefaultConstructible1 >().as< DefaultConstructibleBase >().useIfNone();

        auto container = builder.build();

        // Act
        ContainerBuilder nestedContainerBuilder;
        nestedContainerBuilder.registerType< DefaultConstructible2 >().as< DefaultConstructibleBase >();
        auto nestedContainer = nestedContainerBuilder.buildNestedContainerFrom(*container);

        // Assert
        auto instance = nestedContainer->resolve< DefaultConstructibleBase >();
        BOOST_CHECK(instance != nullptr);
        BOOST_CHECK(std::dynamic_pointer_cast< DefaultConstructible2 >(instance) == instance);
    }

    BOOST_AUTO_TEST_CASE(should_not_override_parent_fallback_registration)
    {
        // Arrange
        ContainerBuilder builder;
        builder.registerType< DefaultConstructible1 >().as< DefaultConstructibleBase >().useIfNone();

        auto container = builder.build();

        // Act
        ContainerBuilder nestedContainerBuilder;
        nestedContainerBuilder.registerType< DefaultConstructible2 >().as< DefaultConstructibleBase >().useIfNone();
        auto nestedContainer = nestedContainerBuilder.buildNestedContainerFrom(*container);

        // Assert
        auto instance = nestedContainer->resolve< DefaultConstructibleBase >();
        BOOST_CHECK(instance != nullptr);
        BOOST_CHECK(std::dynamic_pointer_cast< DefaultConstructible1 >(instance) == instance);
    }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace Testing
} // namespace Hypodermic