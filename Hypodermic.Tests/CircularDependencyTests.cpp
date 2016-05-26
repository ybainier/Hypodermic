#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


using namespace Hypodermic;


BOOST_AUTO_TEST_SUITE(CircularDependencyTests)

BOOST_AUTO_TEST_CASE(should_throw_when_detecting_a_circular_dependency)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< Testing::Type1 >().as< Testing::BaseType1 >();
    builder.registerType< Testing::Type2 >().as< Testing::BaseType2 >();

    auto container = builder.build();

    // Assert
    BOOST_CHECK_THROW(container->resolve< Testing::BaseType1 >(), std::exception);
    BOOST_CHECK_THROW(container->resolve< Testing::BaseType2 >(), std::exception);
}

BOOST_AUTO_TEST_CASE(should_not_throw_when_a_dependency_lol)
{
    // Arrange
    ContainerBuilder builder;
    std::shared_ptr< Testing::TypeThatNeedsLoader > resolvedInstanceOnActivated;

    // Act
    builder.registerType< Testing::Loader >()
        .as< Testing::ILoader >()
        .singleInstance()
        .onActivated([&](ComponentContext& c, const std::shared_ptr< Testing::Loader >&)
        {
            resolvedInstanceOnActivated = c.resolve< Testing::TypeThatNeedsLoader >();
        });

    builder.registerType< Testing::TypeThatNeedsLoader >().singleInstance();
    
    builder.registerType< Testing::Initializer >();

    auto container = builder.build();

    // Assert
    std::shared_ptr< Testing::Initializer > initializer;

    BOOST_REQUIRE_NO_THROW(initializer = container->resolve< Testing::Initializer >());
    BOOST_REQUIRE(initializer != nullptr);
    BOOST_REQUIRE(initializer->dependency != nullptr);
    BOOST_CHECK(initializer->dependency == resolvedInstanceOnActivated);
    BOOST_CHECK(initializer->dependency->dependency == resolvedInstanceOnActivated->dependency);
}

BOOST_AUTO_TEST_SUITE_END()