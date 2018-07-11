#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


namespace Hypodermic
{
namespace Testing
{

    BOOST_AUTO_TEST_SUITE(ContainerBuilderValidationTests)

    BOOST_AUTO_TEST_CASE(should_not_throw_when_validating_an_empty_builder)
    {
        // Arrange
        ContainerBuilder builder;

        // Act & Assert
        BOOST_CHECK_NO_THROW(builder.validate());
    }

    BOOST_AUTO_TEST_CASE(should_not_throw_when_everything_can_be_resolved)
    {
        // Arrange
        ContainerBuilder builder;
        builder.registerType< TypeWithOneDependency >();
        builder.registerType< ProvidedDependency >().as< ProvidedDependencyBase >();

        // Act & Assert
        BOOST_CHECK_NO_THROW(builder.validate());
    }

    BOOST_AUTO_TEST_CASE(should_throw_when_a_type_cannot_be_resolved)
    {
        // Arrange
        ContainerBuilder builder;
        builder.registerType< TypeWithOneDependency >();
        builder.registerType< ProvidedDependency >(); // since TypeWithOneDependency expects to be injected a ProvidedDependencyBase, it will fail

        // Act & Assert
        BOOST_CHECK_THROW(builder.validate(), std::exception);
    }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace Testing
} // namespace Hypodermic