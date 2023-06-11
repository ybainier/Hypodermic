#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"


namespace Hypodermic
{
namespace Testing
{
namespace
{

    class Foo
    {
    public:
        int value() const
        {
            return 42;
        }
    };

    class Bar
    {
    public:
        explicit Bar(std::shared_ptr<Foo> foo) noexcept : _foo(std::move(foo))
        { }

        const std::shared_ptr<Foo>& foo() const noexcept
        {
            return _foo;
        }

    private:
        std::shared_ptr<Foo> _foo;
    };

    class Baz
    {
    public:
        explicit Baz(std::shared_ptr<const Foo> foo) noexcept : _foo(std::move(foo))
        { }

        const std::shared_ptr<const Foo>& foo() const noexcept
        {
            return _foo;
        }

    private:
        std::shared_ptr<const Foo> _foo;
    };

} // namespace

    BOOST_AUTO_TEST_SUITE(ResolveConstPtrTests)

    BOOST_AUTO_TEST_CASE(should_resolve_non_const_ptr)
    {
        // Arrange
        ContainerBuilder builder;
        builder.registerType<Foo>();
        builder.registerType<Bar>();
        auto container = builder.build();

        // Act
        const auto bar = container->resolve<Bar>();
        auto value = bar->foo()->value();

        // Assert
        BOOST_CHECK_EQUAL(value, 42);
    }

    BOOST_AUTO_TEST_CASE(should_resolve_const_ptr)
    {
        // Arrange
        ContainerBuilder builder;
        builder.registerType<Foo>();
        builder.registerType<Baz>();
        auto container = builder.build();

        // Act
        const auto baz = container->resolve<Baz>();
        auto value = baz->foo()->value();

        // Assert
        BOOST_CHECK_EQUAL(value, 42);
    }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace Testing
} // namespace Hypodermic