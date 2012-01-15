#define BOOST_TEST_MODULE Hypodermic
#include <boost/test/unit_test.hpp>

#include <Hypodermic/ContainerBuilder.h>
#include <Hypodermic/Container.h>
#include <Hypodermic/Helpers.h>
#include <Hypodermic/IComponentContext.h>

#include <boost/date_time.hpp>

using namespace Hypodermic;


struct IServiceA
{
    virtual ~IServiceA()
    {
    }
};

struct IRunWithScissors
{
    virtual ~IRunWithScissors()
    {
    }
};

struct ServiceA : IServiceA, IRunWithScissors
{
};

struct IServiceB
{
    virtual ~IServiceB()
    {
    }
};

struct ServiceB : IServiceB
{
	ServiceB(IServiceA* serviceA)
	{
        BOOST_ASSERT(serviceA != nullptr);
	}
};

struct ServiceRunningWithScissors : IServiceB
{
    ServiceRunningWithScissors(IRunWithScissors* serviceA)
    {
        BOOST_ASSERT(serviceA != nullptr);
    }
};


BOOST_AUTO_TEST_SUITE(ContainerBuilderTests);

BOOST_AUTO_TEST_CASE(can_register_and_resolve_concrete_type)
{
	ContainerBuilder builder;

	builder.registerType< ServiceA* >();

	auto container = builder.build();

	auto serviceA = container->resolve< ServiceA* >();

	BOOST_CHECK(serviceA != nullptr);
}


BOOST_AUTO_TEST_CASE(should_resolve_registered_type)
{
	ContainerBuilder builder;

	builder.registerType< ServiceA* >()->as< IServiceA* >();

	auto container = builder.build();

	auto serviceA = container->resolve< IServiceA* >();

	BOOST_CHECK(serviceA != nullptr);
}

BOOST_AUTO_TEST_CASE(should_resolve_abstract_dependencies)
{
	ContainerBuilder builder;

	builder.registerType< ServiceA* >()->as< IServiceA* >();
	builder.registerType(CREATE(ServiceB*, new ServiceB(INJECT(IServiceA*))))->as< IServiceB* >();

	auto container = builder.build();

	auto serviceB = container->resolve< IServiceB* >();

	BOOST_CHECK(serviceB != nullptr);
}


BOOST_AUTO_TEST_CASE(default_lifetime_should_be_transient)
{
	ContainerBuilder builder;

	builder.registerType< ServiceA* >()->as< IServiceA* >();

	auto container = builder.build();

	auto serviceA = container->resolve< IServiceA* >();
	auto anotherServiceA = container->resolve< IServiceA* >();

	BOOST_CHECK(serviceA != anotherServiceA);
}


BOOST_AUTO_TEST_CASE(as_method_should_override_default_type_registration)
{
	ContainerBuilder builder;

	builder.registerType< ServiceA* >()->as< IServiceA* >();

	auto container = builder.build();

	auto unresolvedServiceA = container->resolve< ServiceA* >();
	auto serviceA = container->resolve< IServiceA* >();

	BOOST_CHECK(unresolvedServiceA == nullptr);
	BOOST_CHECK(serviceA != nullptr);
}


BOOST_AUTO_TEST_CASE(registered_instance_should_be_shared)
{
	ContainerBuilder builder;

    auto registeredServiceA = new ServiceA;
	builder.registerType(registeredServiceA);

	auto container = builder.build();

	auto serviceA = container->resolve< ServiceA* >();
	auto sameServiceA = container->resolve< ServiceA* >();

	BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(serviceA == registeredServiceA);
	BOOST_CHECK(serviceA == sameServiceA);
}


BOOST_AUTO_TEST_CASE(invoking_singleInstance_should_enable_instance_sharing)
{
    ContainerBuilder builder;

    builder.registerType< ServiceA* >()->as< IServiceA* >()->singleInstance();

    auto container = builder.build();

    auto serviceA = container->resolve< IServiceA* >();
    auto sameServiceA = container->resolve< IServiceA* >();

    BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(serviceA == sameServiceA);
}

BOOST_AUTO_TEST_CASE(polymorphic_resolution_should_be_available_through_polymorphic_registration)
{
    ContainerBuilder builder;

    builder.registerType< ServiceA* >()->as< IServiceA* >()->as< IRunWithScissors* >();

    auto container = builder.build();

    auto serviceA = container->resolve< IServiceA* >();
    auto anotherServiceA = container->resolve< IRunWithScissors* >();

    BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(anotherServiceA != nullptr);
}

BOOST_AUTO_TEST_CASE(polymorphic_resolution_is_not_a_lie)
{
    ContainerBuilder builder;

    auto registeredServiceA = new ServiceA;
    builder.registerType(registeredServiceA)->as< IServiceA* >()->as< IRunWithScissors* >();

    auto container = builder.build();

    auto serviceARunningWithScissors = container->resolve< IRunWithScissors* >();
    auto serviceA = dynamic_cast< ServiceA* >(serviceARunningWithScissors);

    BOOST_CHECK(serviceARunningWithScissors != nullptr);
    BOOST_CHECK(serviceA == registeredServiceA);
}

BOOST_AUTO_TEST_CASE(polymorphic_resolution_can_be_used_to_express_dependencies)
{
    ContainerBuilder builder;

    auto serviceA = new ServiceA;
    builder.registerType(serviceA)->as< IRunWithScissors* >();

    builder.registerType(CREATE(ServiceRunningWithScissors*,
                                     new ServiceRunningWithScissors(INJECT(IRunWithScissors*))))->as< IServiceB* >();

    auto container = builder.build();

    auto serviceB = container->resolve< IServiceB* >();

    BOOST_CHECK(serviceB != nullptr);
}

BOOST_AUTO_TEST_CASE(registerType_with_an_instance_argument_can_bypass_argument_type_with_a_template_parameter)
{
    ContainerBuilder builder;

    ServiceA* serviceA = new ServiceA;
    builder.registerType< IRunWithScissors* >(serviceA);

    auto container = builder.build();

    auto serviceARunningWithScissors = container->resolve< IRunWithScissors* >();

    BOOST_CHECK(serviceARunningWithScissors != nullptr);
    BOOST_CHECK(dynamic_cast< ServiceA* >(serviceARunningWithScissors) != nullptr);
    BOOST_CHECK(dynamic_cast< ServiceA* >(serviceARunningWithScissors) == serviceA);
}


BOOST_AUTO_TEST_SUITE_END();