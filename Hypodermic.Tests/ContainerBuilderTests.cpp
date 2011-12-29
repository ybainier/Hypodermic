#define BOOST_TEST_MODULE Hypodermic
#include <boost/test/unit_test.hpp>

#include <Hypodermic/ContainerBuilder.h>
#include <Hypodermic/Container.h>
#include <Hypodermic/Helpers.h>
#include <Hypodermic/IComponentContext.h>


using namespace Hypodermic;


struct IDep
{
};

struct MyDep : public IDep
{
};

struct IMyType
{
};

struct MyType : public IMyType
{
	MyType(IDep* dep)
	{
        BOOST_ASSERT(dep != nullptr);
	}
};


BOOST_AUTO_TEST_SUITE(ContainerBuilderTests);

BOOST_AUTO_TEST_CASE(Concrete_type_can_be_setup_and_get_resolved)
{
	ContainerBuilder builder;

	builder.setup< MyDep* >();

	auto container = builder.build();

	auto myDep = container->resolve< MyDep* >();

	BOOST_CHECK(myDep != nullptr);
}


BOOST_AUTO_TEST_CASE(Should_be_setup_as_an_interface_and_get_resolved)
{
	ContainerBuilder builder;

	builder.setup< MyDep* >()->as< IDep* >();

	auto container = builder.build();

	auto myDep = container->resolve< IDep* >();

	BOOST_CHECK(myDep != nullptr);
}


BOOST_AUTO_TEST_CASE(Should_be_setup_as_an_interface_and_resolve_abstract_dependencies)
{
	ContainerBuilder builder;

	builder.setup< MyDep* >()->as< IDep* >();
	builder.setup(CREATE(MyType*, new MyType(INJECT(IDep*))))->as< IMyType* >();

	auto container = builder.build();

	auto myType = container->resolve< IMyType* >();

	BOOST_CHECK(myType != nullptr);
}


BOOST_AUTO_TEST_CASE(Default_lifetime_should_be_transient)
{
	ContainerBuilder builder;

	builder.setup< MyDep* >()->as< IDep* >();

	auto container = builder.build();

	auto myDep = container->resolve< IDep* >();
	auto myDep2 = container->resolve< IDep* >();

	BOOST_CHECK(myDep != myDep2);
}


BOOST_AUTO_TEST_CASE(Setup_as_interface_should_prevent_from_resolving_concrete_type)
{
	ContainerBuilder builder;

	builder.setup< MyDep* >()->as< IDep* >();

	auto container = builder.build();

	auto myDep = container->resolve< MyDep* >();
	auto myDep2 = container->resolve< IDep* >();

	BOOST_CHECK(myDep == nullptr);
	BOOST_CHECK(myDep2 != nullptr);
}


BOOST_AUTO_TEST_CASE(Registered_instance_should_be_shared)
{
	ContainerBuilder builder;

	builder.setup(new MyDep);

	auto container = builder.build();

	auto myDep = container->resolve< MyDep* >();
	auto myDep2 = container->resolve< MyDep* >();

	BOOST_CHECK(myDep != nullptr);
	BOOST_CHECK(myDep == myDep2);
}


BOOST_AUTO_TEST_CASE(Invoking_singleInstance_should_enable_instance_sharing)
{
    ContainerBuilder builder;

    auto rb = builder.setup< MyDep* >()->as< IDep* >();

    rb->singleInstance();

    auto container = builder.build();

    auto myDep = container->resolve< IDep* >();
    auto myDep2 = container->resolve< IDep* >();

    BOOST_CHECK(myDep == myDep2);
}


BOOST_AUTO_TEST_SUITE_END();