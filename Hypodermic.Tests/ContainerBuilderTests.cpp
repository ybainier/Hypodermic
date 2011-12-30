#define BOOST_TEST_MODULE Hypodermic
#include <boost/test/unit_test.hpp>

#include <Hypodermic/ContainerBuilder.h>
#include <Hypodermic/Container.h>
#include <Hypodermic/Helpers.h>
#include <Hypodermic/IComponentContext.h>


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


BOOST_AUTO_TEST_SUITE(ContainerBuilderTests);

BOOST_AUTO_TEST_CASE(Concrete_type_can_be_setup_and_get_resolved)
{
	ContainerBuilder builder;

	builder.setup< ServiceA* >();

	auto container = builder.build();

	auto serviceA = container->resolve< ServiceA* >();

	BOOST_CHECK(serviceA != nullptr);
}


BOOST_AUTO_TEST_CASE(Should_be_setup_as_an_interface_and_get_resolved)
{
	ContainerBuilder builder;

	builder.setup< ServiceA* >()->as< IServiceA* >();

	auto container = builder.build();

	auto serviceA = container->resolve< IServiceA* >();

	BOOST_CHECK(serviceA != nullptr);
}

BOOST_AUTO_TEST_CASE(Should_be_setup_as_an_interface_and_resolve_abstract_dependencies)
{
	ContainerBuilder builder;

	builder.setup< ServiceA* >()->as< IServiceA* >();
	builder.setup(CREATE(ServiceB*, new ServiceB(INJECT(IServiceA*))))->as< IServiceB* >();

	auto container = builder.build();

	auto serviceB = container->resolve< IServiceB* >();

	BOOST_CHECK(serviceB != nullptr);
}


BOOST_AUTO_TEST_CASE(Default_lifetime_should_be_transient)
{
	ContainerBuilder builder;

	builder.setup< ServiceA* >()->as< IServiceA* >();

	auto container = builder.build();

	auto serviceA = container->resolve< IServiceA* >();
	auto anotherServiceA = container->resolve< IServiceA* >();

	BOOST_CHECK(serviceA != anotherServiceA);
}


BOOST_AUTO_TEST_CASE(Setup_as_interface_should_prevent_from_resolving_concrete_type)
{
	ContainerBuilder builder;

	builder.setup< ServiceA* >()->as< IServiceA* >();

	auto container = builder.build();

	auto unresolvedServiceA = container->resolve< ServiceA* >();
	auto serviceA = container->resolve< IServiceA* >();

	BOOST_CHECK(unresolvedServiceA == nullptr);
	BOOST_CHECK(serviceA != nullptr);
}


BOOST_AUTO_TEST_CASE(Registered_instance_should_be_shared)
{
	ContainerBuilder builder;

    auto registeredServiceA = new ServiceA;
	builder.setup(registeredServiceA);

	auto container = builder.build();

	auto serviceA = container->resolve< ServiceA* >();
	auto sameServiceA = container->resolve< ServiceA* >();

	BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(serviceA == registeredServiceA);
	BOOST_CHECK(serviceA == sameServiceA);
}


BOOST_AUTO_TEST_CASE(Invoking_singleInstance_should_enable_instance_sharing)
{
    ContainerBuilder builder;

    builder.setup< ServiceA* >()->as< IServiceA* >()->singleInstance();

    auto container = builder.build();

    auto serviceA = container->resolve< IServiceA* >();
    auto sameServiceA = container->resolve< IServiceA* >();

    BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(serviceA == sameServiceA);
}

BOOST_AUTO_TEST_CASE(Polymorphic_resolution_should_be_available_through_polymorphic_registration)
{
    ContainerBuilder builder;

    builder.setup< ServiceA* >()->as< IServiceA* >()->as< IRunWithScissors* >();

    auto container = builder.build();

    auto serviceA = container->resolve< IServiceA* >();
    auto anotherServiceA = container->resolve< IRunWithScissors* >();

    BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(anotherServiceA != nullptr);
}

BOOST_AUTO_TEST_SUITE_END();