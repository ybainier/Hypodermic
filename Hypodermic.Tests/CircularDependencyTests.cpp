#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


namespace Hypodermic
{
namespace Testing
{

    BOOST_AUTO_TEST_SUITE(CircularDependencyTests)

    BOOST_AUTO_TEST_CASE(should_throw_when_detecting_a_circular_dependency)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< Type1 >().as< BaseType1 >();
        builder.registerType< Type2 >().as< BaseType2 >();

        auto container = builder.build();

        // Assert
        BOOST_CHECK_THROW(container->resolve< BaseType1 >(), std::exception);
        BOOST_CHECK_THROW(container->resolve< BaseType2 >(), std::exception);
    }

    BOOST_AUTO_TEST_CASE(should_not_throw_when_resolving_a_type_that_needs_an_instance_that_is_being_activated)
    {
        // Arrange
        ContainerBuilder builder;
        std::shared_ptr< TypeThatNeedsLoader > resolvedInstanceOnActivated;

        // Act
        builder.registerType< Loader >()
            .as< ILoader >()
            .singleInstance()
            .onActivated([&](ComponentContext& c, const std::shared_ptr< Loader >&)
            {
                resolvedInstanceOnActivated = c.resolve< TypeThatNeedsLoader >();
            });

        builder.registerType< TypeThatNeedsLoader >().singleInstance();
    
        builder.registerType< Initializer >();

        auto container = builder.build();

        // Assert
        std::shared_ptr< Initializer > initializer;

        BOOST_REQUIRE_NO_THROW(initializer = container->resolve< Initializer >());
        BOOST_REQUIRE(initializer != nullptr);
        BOOST_REQUIRE(initializer->dependency != nullptr);
        BOOST_CHECK(initializer->dependency == resolvedInstanceOnActivated);
        BOOST_CHECK(initializer->dependency->dependency == resolvedInstanceOnActivated->dependency);
    }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace Testing
} // namespace Hypodermic
