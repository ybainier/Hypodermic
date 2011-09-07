#define BOOST_TEST_MODULE Hypodermic
#include <boost/test/unit_test.hpp>
#include <Hypodermic/ContainerBuilder.h>
#include <Hypodermic/Container.h>
#include <Hypodermic/IComponentContext.h>


using namespace Hypodermic;


struct IDep
{
};

struct MyDep : public IDep
{
};

struct MyType
{
	MyType(IDep*)
	{
	}
};


BOOST_AUTO_TEST_SUITE(ContainerBuilderTests);


BOOST_AUTO_TEST_CASE(Should_be_funny)
{
	ContainerBuilder builder;

	builder.registerConstruction(Func< IComponentContext*, IDep* >(
		[](IComponentContext* c) -> IDep*
		{
			return new MyDep;
		}));

	builder.registerConstruction(Func< IComponentContext*, MyType* >(
		[](IComponentContext* c) -> MyType*
		{
			return new MyType(c->resolve< IDep* >());
		}));

	auto container = builder.build();

	auto myType = container->resolve< MyType* >();


	BOOST_CHECK(1 == 1);
}


BOOST_AUTO_TEST_SUITE_END();