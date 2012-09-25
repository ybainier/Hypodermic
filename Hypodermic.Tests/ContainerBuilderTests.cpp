#include <boost/test/unit_test.hpp>

#include <Hypodermic/ActivatingData.h>
#include <Hypodermic/AutowiredConstructor.h>
#include <Hypodermic/ContainerBuilder.h>
#include <Hypodermic/Container.h>
#include <Hypodermic/Helpers.h>
#include <Hypodermic/IComponentContext.h>
#include <Hypodermic/NullptrWorkaround.h>


using namespace Hypodermic;


struct IServiceA
{
    virtual ~IServiceA() {}
    virtual void act() = 0;
};

struct IRunWithScissors
{
    virtual ~IRunWithScissors() {}
    virtual void run() = 0;
};

struct ServiceA : IServiceA, IRunWithScissors
{
    typedef AutowiredConstructor< ServiceA() > AutowiredSignature;

    void act() {}
    void run() {}
};

struct IServiceB
{
    virtual ~IServiceB() {}
    virtual void act() = 0;
};

struct ServiceB : IServiceB
{
    typedef AutowiredConstructor< ServiceB(IServiceA*) > AutowiredSignature;

    ServiceB(std::shared_ptr< IServiceA > serviceA)
        : serviceA_(serviceA)
	{
        BOOST_ASSERT(serviceA != nullptr);
	}

    void act() {}

private:
    std::shared_ptr< IServiceA > serviceA_;
};

struct ServiceRunningWithScissors : IServiceB
{
    typedef AutowiredConstructor< ServiceRunningWithScissors(IRunWithScissors*) > AutowiredSignature;

    ServiceRunningWithScissors(std::shared_ptr< IRunWithScissors > serviceA)
        : serviceA_(serviceA)
    {
        BOOST_ASSERT(serviceA != nullptr);
    }

    void act() {}

private:
    std::shared_ptr< IRunWithScissors > serviceA_;
};

struct ServiceBController
{
    typedef AutowiredConstructor< ServiceBController(std::vector< IServiceB* >) > AutowiredSignature;

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

BOOST_AUTO_TEST_CASE(as_method_should_not_override_default_type_registration_by_invoking_asSelf_method)
{
    ContainerBuilder builder;

    builder.registerType< ServiceA >()->as< IServiceA >()->asSelf();

    auto container = builder.build();

    auto serviceA = container->resolve< ServiceA >();
    auto serviceAInterface = container->resolve< IServiceA >();

    BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(serviceAInterface != nullptr);
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

BOOST_AUTO_TEST_CASE(named_registrations_should_not_conflict_with_anonymous_ones)
{
    ContainerBuilder builder;

    auto serviceA = std::make_shared< ServiceA >();
    builder.registerInstance(serviceA)->as< IServiceA >();

    builder.registerType< ServiceA >()->named< IServiceA >("whoami");

    auto container = builder.build();

    auto resolvedServiceA = container->resolve< IServiceA >();
    auto whoami1 = container->resolveNamed< IServiceA >("whoami");
    auto whoami2 = container->resolveNamed< IServiceA >("whoami");

    BOOST_CHECK(resolvedServiceA != nullptr);
    BOOST_CHECK(resolvedServiceA == serviceA);

    BOOST_CHECK(whoami1 != nullptr);
    BOOST_CHECK(whoami1 != serviceA);

    BOOST_CHECK(whoami2 != nullptr);
    BOOST_CHECK(whoami2 != serviceA);

    BOOST_CHECK(whoami1 != whoami2);
}

BOOST_AUTO_TEST_CASE(autowired_registration_follows_the_usual_registration_rules)
{
    ContainerBuilder c;
    c.autowireType< ServiceA >()->as< IServiceA >();
    c.autowireType< ServiceB >()->singleInstance();

    auto container = c.build();

    auto serviceB = container->resolve< ServiceB >();
    auto nullServiceB = container->resolve< IServiceB >();

    BOOST_CHECK(serviceB != nullptr);
    BOOST_CHECK(nullServiceB == nullptr);
    BOOST_CHECK(serviceB == container->resolve< ServiceB >());
}

BOOST_AUTO_TEST_CASE(autowired_registration_can_resolved_all_services)
{
    ContainerBuilder c;
    c.autowireType< ServiceA >()->as< IServiceA >()->as< IRunWithScissors >();

    c.autowireType< ServiceB >()->as< IServiceB >();
    c.autowireType< ServiceRunningWithScissors >()->as< IServiceB >();

    c.autowireType< ServiceBController >();

    auto container = c.build();

    auto serviceBController = container->resolve< ServiceBController >();

    BOOST_CHECK(serviceBController != nullptr);
}

BOOST_AUTO_TEST_CASE(registration_should_be_overridable)
{
    ContainerBuilder c;

    std::shared_ptr< IServiceA > serviceA = std::make_shared< ServiceA >();
    c.registerInstance(serviceA);

    c.autowireType< ServiceA >()->as< IServiceA >()->as< IRunWithScissors >()->singleInstance();

    auto container = c.build();

    auto resolvedServiceA = container->resolve< IServiceA >();

    BOOST_CHECK(resolvedServiceA != nullptr);
    BOOST_CHECK(resolvedServiceA != serviceA);
}

BOOST_AUTO_TEST_CASE(registration_should_provide_instance_activating_data)
{
    ContainerBuilder c;

    std::shared_ptr< IServiceA > serviceA = std::make_shared< ServiceA >();
    std::shared_ptr< IServiceA > anotherServiceA = std::make_shared< ServiceA >();
    
    c.registerInstance(serviceA)->onActivating(
        [serviceA, anotherServiceA](IActivatingData< IServiceA >& data) -> void
        {
            BOOST_CHECK(data.componentContext() != nullptr);
            BOOST_CHECK(data.componentRegistration() != nullptr);
            BOOST_CHECK(data.instance() == serviceA);

            static_cast< ActivatingData< IServiceA >& >(data).instance(anotherServiceA);
        }
    );

    auto container = c.build();

    auto resolvedServiceA = container->resolve< IServiceA >();

    BOOST_CHECK(resolvedServiceA == anotherServiceA);
}

BOOST_AUTO_TEST_CASE(registration_should_provide_instance_preparing_data)
{
    ContainerBuilder c;

    std::shared_ptr< IServiceA > serviceA = std::make_shared< ServiceA >();
    bool onPreparingInvoked = false;

    c.registerInstance(serviceA)->onPreparing(
        [&onPreparingInvoked](IPreparingData& data) -> void
        {
            BOOST_CHECK(data.componentContext() != nullptr);
            BOOST_CHECK(data.componentRegistration() != nullptr);

            onPreparingInvoked = true;
        }
    );

    auto container = c.build();

    auto resolvedServiceA = container->resolve< IServiceA >();

    BOOST_CHECK(onPreparingInvoked);
}

BOOST_AUTO_TEST_CASE(registration_should_provide_instance_activated_data)
{
    ContainerBuilder c;

    std::shared_ptr< IServiceA > serviceA = std::make_shared< ServiceA >();

    c.registerInstance(serviceA)->onActivated(
        [serviceA](IActivatedData< IServiceA >& data) -> void
        {
            BOOST_CHECK(data.componentContext() != nullptr);
            BOOST_CHECK(data.componentRegistration() != nullptr);
            BOOST_CHECK(data.instance() == serviceA);
        }
    );

    auto container = c.build();

    auto resolvedServiceA = container->resolve< IServiceA >();

    BOOST_CHECK(resolvedServiceA == serviceA);
}

BOOST_AUTO_TEST_CASE(registration_should_not_provide_instance_activated_data_for_already_activated_instance)
{
    ContainerBuilder builder;

    int activatedCount = 0;
    auto registeredServiceA = std::make_shared< ServiceA >();

    builder.registerInstance(registeredServiceA)->onActivated(
        [&activatedCount](IActivatedData< ServiceA >&) -> void
        {
            ++activatedCount;
        }
    );

    auto container = builder.build();

    auto serviceA = container->resolve< ServiceA >();
    auto sameServiceA = container->resolve< ServiceA >();

    BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(serviceA == registeredServiceA);
    BOOST_CHECK(serviceA == sameServiceA);
    BOOST_CHECK(activatedCount == 1);
}

BOOST_AUTO_TEST_CASE(registration_should_provide_instance_activated_data_for_every_time_a_transient_object_is_activated)
{
    ContainerBuilder builder;

    int activatedCount = 0;

    builder.registerType< ServiceA >()->onActivated(
        [&activatedCount](IActivatedData< ServiceA >&) -> void
        {
            ++activatedCount;
        }
    );

    auto container = builder.build();

    auto serviceA = container->resolve< ServiceA >();
    auto sameServiceA = container->resolve< ServiceA >();

    BOOST_CHECK(activatedCount == 2);
}

BOOST_AUTO_TEST_CASE(registration_should_only_provide_instance_activated_data_once_for_shared_object)
{
    ContainerBuilder builder;

    int activatedCount = 0;

    builder.registerType< ServiceA >()->onActivated(
        [&activatedCount](IActivatedData< ServiceA >&) -> void
        {
            ++activatedCount;
        }
    )->singleInstance();

    auto container = builder.build();

    auto serviceA = container->resolve< ServiceA >();
    auto sameServiceA = container->resolve< ServiceA >();

    BOOST_CHECK(activatedCount == 1);
}

BOOST_AUTO_TEST_SUITE_END();