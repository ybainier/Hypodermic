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
	ServiceB(std::shared_ptr< IServiceA > serviceA)
        : serviceA_(serviceA)
	{
        BOOST_ASSERT(serviceA != nullptr);
	}

private:
    std::shared_ptr< IServiceA > serviceA_;
};

struct ServiceRunningWithScissors : IServiceB
{
    ServiceRunningWithScissors(std::shared_ptr< IRunWithScissors > serviceA)
        : serviceA_(serviceA)
    {
        BOOST_ASSERT(serviceA != nullptr);
    }

private:
    std::shared_ptr< IRunWithScissors > serviceA_;
};

struct ServiceBController
{
    ServiceBController(const std::vector< std::shared_ptr< IServiceB > >& serviceBs)
        : serviceBs_(serviceBs)
    {
        BOOST_ASSERT(!serviceBs.empty());
    }

private:
    std::vector< std::shared_ptr< IServiceB > > serviceBs_;
};

struct ContainerHolder
{
    ContainerHolder(std::shared_ptr< IContainer > container)
        : container_(container)
    {
        BOOST_ASSERT(container != nullptr);
    }

private:
    std::shared_ptr< IContainer > container_;
};


BOOST_AUTO_TEST_SUITE(ContainerBuilderTests);

BOOST_AUTO_TEST_CASE(can_register_and_resolve_concrete_type)
{
	ContainerBuilder builder;

	builder.registerType< ServiceA >();

	auto container = builder.build();
	auto serviceA = container->resolve< ServiceA >();

	BOOST_CHECK(serviceA != nullptr);
}

BOOST_AUTO_TEST_CASE(should_resolve_registered_type)
{
	ContainerBuilder builder;

	builder.registerType< ServiceA >()->as< IServiceA >();

	auto container = builder.build();

	auto serviceA = container->resolve< IServiceA >();

	BOOST_CHECK(serviceA != nullptr);
}

BOOST_AUTO_TEST_CASE(should_resolve_abstract_dependencies)
{
	ContainerBuilder builder;

	builder.registerType< ServiceA >()->as< IServiceA >();
	builder.registerType< ServiceB >(CREATE(new ServiceB(INJECT(IServiceA))))->as< IServiceB >();

	auto container = builder.build();

	auto serviceB = container->resolve< IServiceB >();

	BOOST_CHECK(serviceB != nullptr);
}

BOOST_AUTO_TEST_CASE(default_lifetime_should_be_transient)
{
	ContainerBuilder builder;

	builder.registerType< ServiceA >()->as< IServiceA >();

	auto container = builder.build();

	auto serviceA = container->resolve< IServiceA >();
	auto anotherServiceA = container->resolve< IServiceA >();

	BOOST_CHECK(serviceA != anotherServiceA);
}

BOOST_AUTO_TEST_CASE(as_method_should_override_default_type_registration)
{
	ContainerBuilder builder;

	builder.registerType< ServiceA >()->as< IServiceA >();

	auto container = builder.build();

	auto unresolvedServiceA = container->resolve< ServiceA >();
	auto serviceA = container->resolve< IServiceA >();

	BOOST_CHECK(unresolvedServiceA == nullptr);
	BOOST_CHECK(serviceA != nullptr);
}

BOOST_AUTO_TEST_CASE(registered_instance_should_be_shared)
{
	ContainerBuilder builder;

    auto registeredServiceA = std::make_shared< ServiceA >();
	builder.registerInstance(registeredServiceA);

	auto container = builder.build();

	auto serviceA = container->resolve< ServiceA >();
	auto sameServiceA = container->resolve< ServiceA >();

	BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(serviceA == registeredServiceA);
	BOOST_CHECK(serviceA == sameServiceA);
}

BOOST_AUTO_TEST_CASE(invoking_singleInstance_should_enable_instance_sharing)
{
    ContainerBuilder builder;

    builder.registerType< ServiceA >()->as< IServiceA >()->singleInstance();

    auto container = builder.build();

    auto serviceA = container->resolve< IServiceA >();
    auto sameServiceA = container->resolve< IServiceA >();

    BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(serviceA == sameServiceA);
}

BOOST_AUTO_TEST_CASE(polymorphic_resolution_should_be_available_through_polymorphic_registration)
{
    ContainerBuilder builder;

    builder.registerType< ServiceA >()->as< IServiceA >()->as< IRunWithScissors >();

    auto container = builder.build();

    auto serviceA = container->resolve< IServiceA >();
    auto anotherServiceA = container->resolve< IRunWithScissors >();

    BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(anotherServiceA != nullptr);
}

BOOST_AUTO_TEST_CASE(polymorphic_resolution_is_not_a_lie)
{
    ContainerBuilder builder;

    auto registeredServiceA = std::make_shared< ServiceA >();
    builder.registerInstance(registeredServiceA)->as< IServiceA >()->as< IRunWithScissors >();

    auto container = builder.build();

    auto serviceARunningWithScissors = container->resolve< IRunWithScissors >();
    auto serviceA = std::dynamic_pointer_cast< ServiceA >(serviceARunningWithScissors);

    BOOST_CHECK(serviceARunningWithScissors != nullptr);
    BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(serviceA == registeredServiceA);
}

BOOST_AUTO_TEST_CASE(polymorphic_resolution_can_be_used_to_express_dependencies)
{
    ContainerBuilder builder;

    auto serviceA = std::make_shared< ServiceA >();
    builder.registerInstance(serviceA)->as< IRunWithScissors >();

    builder.registerType< ServiceRunningWithScissors >(
        CREATE(new ServiceRunningWithScissors(INJECT(IRunWithScissors))))->as< IServiceB >();

    auto container = builder.build();

    auto serviceB = container->resolve< IServiceB >();

    BOOST_CHECK(serviceB != nullptr);
}

BOOST_AUTO_TEST_CASE(registerInstance_template_parameter_can_override_the_argument_type)
{
    ContainerBuilder builder;

    auto serviceA = std::make_shared< ServiceA >();
    builder.registerInstance< IRunWithScissors >(serviceA);
    builder.registerInstance< IServiceA >(serviceA);
    builder.registerType< IServiceB >(CREATE(new ServiceB(INJECT(IServiceA))));

    auto container = builder.build();

    auto serviceARunningWithScissors = container->resolve< IRunWithScissors >();
    auto serviceB = container->resolve< IServiceB >();

    BOOST_CHECK(serviceARunningWithScissors != nullptr);
    BOOST_CHECK(std::dynamic_pointer_cast< ServiceA >(serviceARunningWithScissors) != nullptr);
    BOOST_CHECK(std::dynamic_pointer_cast< ServiceA >(serviceARunningWithScissors) == serviceA);
    BOOST_CHECK(serviceB != nullptr);
}

BOOST_AUTO_TEST_CASE(resolveAll_should_collect_all_registrations_and_return_a_vector_of_instances)
{
    ContainerBuilder builder;

    auto serviceA = std::make_shared< ServiceA >();
    builder.registerInstance(serviceA)->as< IServiceA >()->as< IRunWithScissors >();

    builder.registerType< ServiceB >(CREATE(new ServiceB(INJECT(IServiceA))))->as< IServiceB >()->singleInstance();
    builder.registerType< ServiceRunningWithScissors >(
        CREATE(new ServiceRunningWithScissors(INJECT(IRunWithScissors))))->as< IServiceB >();

    auto container = builder.build();

    auto serviceB1 = container->resolveAll< IServiceB >();
    auto serviceB2 = container->resolveAll< IServiceB >();

    BOOST_CHECK(serviceB1.size() == serviceB2.size());
    BOOST_CHECK(serviceB1.size() == 2);
    BOOST_CHECK(serviceB1[0] == serviceB2[0]);
    BOOST_CHECK(serviceB1[1] != serviceB2[1]);
}

BOOST_AUTO_TEST_CASE(resolveAll_can_be_used_to_collect_dependencies)
{
    ContainerBuilder builder;

    auto serviceA = std::make_shared< ServiceA >();
    builder.registerInstance(serviceA)->as< IServiceA >()->as< IRunWithScissors >();

    builder.registerType< ServiceB >(CREATE(new ServiceB(INJECT(IServiceA))))->as< IServiceB >()->singleInstance();
    builder.registerType< ServiceRunningWithScissors >(
        CREATE(new ServiceRunningWithScissors(INJECT(IRunWithScissors))))->as< IServiceB >();

    builder.registerType< ServiceBController >(CREATE(new ServiceBController(INJECT_ALL(IServiceB))));

    auto container = builder.build();

    auto serviceBController = container->resolve< ServiceBController >();
    auto serviceBController2 = container->resolve< ServiceBController >();

    BOOST_CHECK(serviceBController != nullptr);
    BOOST_CHECK(serviceBController2 != nullptr);
    BOOST_CHECK(serviceBController != serviceBController2);
}

BOOST_AUTO_TEST_CASE(container_should_be_injectable_as_well)
{
    ContainerBuilder builder;

    builder.registerType< ContainerHolder >(CREATE(new ContainerHolder(INJECT(IContainer))));

    auto container = builder.build();

    auto containerHolder = container->resolve< ContainerHolder >();

    BOOST_CHECK(containerHolder != nullptr);
}

BOOST_AUTO_TEST_SUITE_END();