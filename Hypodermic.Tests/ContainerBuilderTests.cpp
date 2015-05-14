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


struct MoveConstructorObject
{
    typedef AutowiredConstructor< MoveConstructorObject(IServiceA*) > AutowiredSignature;

    MoveConstructorObject(std::shared_ptr< IServiceA >&& serviceA)
        : serviceA(std::move(serviceA))
    {
    }

    std::shared_ptr< IServiceA > serviceA;
};


BOOST_AUTO_TEST_SUITE(ContainerBuilderTests)

BOOST_AUTO_TEST_CASE(can_register_and_resolve_concrete_type)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< ServiceA >();
    auto container = builder.build();
    auto serviceA = container->resolve< ServiceA >();

    // Assert
    BOOST_CHECK(serviceA != nullptr);
}

BOOST_AUTO_TEST_CASE(should_resolve_registered_type)
{
    // Arrange
    ContainerBuilder builder;
    builder.registerType< ServiceA >().as< IServiceA >();

    auto container = builder.build();

    // Act
    auto serviceA = container->resolve< IServiceA >();

    // Assert
    BOOST_CHECK(serviceA != nullptr);
}

BOOST_AUTO_TEST_CASE(should_resolve_abstract_dependencies)
{
    // Arrange
    ContainerBuilder builder;

    builder.registerType< ServiceA >().as< IServiceA >();
    builder.registerType< ServiceB >(CREATE(std::make_shared< ServiceB >(INJECT(IServiceA)))).as< IServiceB >();

    auto container = builder.build();

    // Act
    auto serviceB = container->resolve< IServiceB >();

    // Assert
    BOOST_CHECK(serviceB != nullptr);
}

BOOST_AUTO_TEST_CASE(default_lifetime_should_be_transient)
{
    // Arrange
    ContainerBuilder builder;

    builder.registerType< ServiceA >().as< IServiceA >();

    auto container = builder.build();

    // Act
    auto serviceA = container->resolve< IServiceA >();
    auto anotherServiceA = container->resolve< IServiceA >();

    // Assert
    BOOST_CHECK(serviceA != anotherServiceA);
}

BOOST_AUTO_TEST_CASE(should_override_default_type_registration)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< ServiceA >().as< IServiceA >();

    // Assert
    auto container = builder.build();
    auto unresolvedServiceA = container->resolve< ServiceA >();
    auto serviceA = container->resolve< IServiceA >();

    BOOST_CHECK(unresolvedServiceA == nullptr);
    BOOST_CHECK(serviceA != nullptr);
}

BOOST_AUTO_TEST_CASE(should_not_override_default_type_registration_by_invoking_asSelf_method)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< ServiceA >().as< IServiceA >().asSelf();

    // Assert
    auto container = builder.build();
    auto serviceA = container->resolve< ServiceA >();
    auto serviceAInterface = container->resolve< IServiceA >();

    BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(serviceAInterface != nullptr);
}

BOOST_AUTO_TEST_CASE(registered_instance_should_be_shared)
{
    // Arrange
    ContainerBuilder builder;

    auto registeredServiceA = std::make_shared< ServiceA >();

    // Act
    builder.registerInstance(registeredServiceA);

    // Assert
    auto container = builder.build();
    auto serviceA = container->resolve< ServiceA >();
    auto sameServiceA = container->resolve< ServiceA >();

    BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(serviceA == registeredServiceA);
    BOOST_CHECK(serviceA == sameServiceA);
}

BOOST_AUTO_TEST_CASE(should_enable_instance_sharing)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< ServiceA >().as< IServiceA >().singleInstance();

    // Assert
    auto container = builder.build();
    auto serviceA = container->resolve< IServiceA >();
    auto sameServiceA = container->resolve< IServiceA >();

    BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(serviceA == sameServiceA);
}

BOOST_AUTO_TEST_CASE(polymorphic_resolution_should_be_available_through_polymorphic_registration)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< ServiceA >().as< IServiceA >().as< IRunWithScissors >();

    // Assert
    auto container = builder.build();
    auto serviceA = container->resolve< IServiceA >();
    auto anotherServiceA = container->resolve< IRunWithScissors >();

    BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(anotherServiceA != nullptr);
}

BOOST_AUTO_TEST_CASE(polymorphic_resolution_is_not_a_lie)
{
    // Arrange
    ContainerBuilder builder;

    auto registeredServiceA = std::make_shared< ServiceA >();
    builder.registerInstance(registeredServiceA).as< IServiceA >().as< IRunWithScissors >();

    auto container = builder.build();

    // Act
    auto serviceARunningWithScissors = container->resolve< IRunWithScissors >();
    auto serviceA = std::dynamic_pointer_cast< ServiceA >(serviceARunningWithScissors);

    // Assert
    BOOST_CHECK(serviceARunningWithScissors != nullptr);
    BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(serviceA == registeredServiceA);
}

BOOST_AUTO_TEST_CASE(polymorphic_resolution_can_be_used_to_express_dependencies)
{
    // Arrange
    ContainerBuilder builder;

    auto serviceA = std::make_shared< ServiceA >();
    builder.registerInstance(serviceA).as< IRunWithScissors >();

    // Act
    builder.registerType< ServiceRunningWithScissors >(CREATE(std::make_shared< ServiceRunningWithScissors >(INJECT(IRunWithScissors)))).as< IServiceB >();

    // Assert
    auto container = builder.build();
    auto serviceB = container->resolve< IServiceB >();

    BOOST_CHECK(serviceB != nullptr);
}

BOOST_AUTO_TEST_CASE(registerInstance_template_parameter_can_override_the_argument_type)
{
    // Arrange
    ContainerBuilder builder;
    builder.registerType< IServiceB >(CREATE(std::make_shared< ServiceB >(INJECT(IServiceA))));

    auto serviceA = std::make_shared< ServiceA >();

    // Act
    builder.registerInstance< IRunWithScissors >(serviceA);
    builder.registerInstance< IServiceA >(serviceA);

    // Assert
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
    // Arrange
    ContainerBuilder builder;

    auto serviceA = std::make_shared< ServiceA >();
    builder.registerInstance(serviceA).as< IServiceA >().as< IRunWithScissors >();

    builder.registerType< ServiceB >(CREATE(std::make_shared< ServiceB >(INJECT(IServiceA)))).as< IServiceB >().singleInstance();
    builder.registerType< ServiceRunningWithScissors >(CREATE(std::make_shared< ServiceRunningWithScissors >(INJECT(IRunWithScissors)))).as< IServiceB >();

    auto container = builder.build();

    // Act
    auto serviceB1 = container->resolveAll< IServiceB >();
    auto serviceB2 = container->resolveAll< IServiceB >();

    // Assert
    BOOST_CHECK(serviceB1.size() == serviceB2.size());
    BOOST_CHECK(serviceB1.size() == 2);
    BOOST_CHECK(serviceB1[0] == serviceB2[0]);
    BOOST_CHECK(serviceB1[1] != serviceB2[1]);
}

BOOST_AUTO_TEST_CASE(resolveAll_can_be_used_to_collect_dependencies)
{
    // Arrange
    ContainerBuilder builder;

    auto serviceA = std::make_shared< ServiceA >();
    builder.registerInstance(serviceA).as< IServiceA >().as< IRunWithScissors >();

    builder.registerType< ServiceB >(CREATE(std::make_shared< ServiceB >(INJECT(IServiceA)))).as< IServiceB >().singleInstance();
    builder.registerType< ServiceRunningWithScissors >(CREATE(std::make_shared< ServiceRunningWithScissors >(INJECT(IRunWithScissors)))).as< IServiceB >();

    builder.registerType< ServiceBController >(CREATE(std::make_shared< ServiceBController >(INJECT_ALL(IServiceB))));

    auto container = builder.build();

    // Act
    auto serviceBController = container->resolve< ServiceBController >();
    auto serviceBController2 = container->resolve< ServiceBController >();

    // Assert
    BOOST_CHECK(serviceBController != nullptr);
    BOOST_CHECK(serviceBController2 != nullptr);
    BOOST_CHECK(serviceBController != serviceBController2);
}

BOOST_AUTO_TEST_CASE(container_should_be_injectable_as_well)
{
    // Arrange
    ContainerBuilder builder;

    // Act
    builder.registerType< ContainerHolder >(CREATE(std::make_shared< ContainerHolder >(INJECT(IContainer))));

    // Assert
    auto container = builder.build();
    auto containerHolder = container->resolve< ContainerHolder >();

    BOOST_CHECK(containerHolder != nullptr);
}

BOOST_AUTO_TEST_CASE(named_registrations_should_not_conflict_with_anonymous_ones)
{
    // Arrange
    ContainerBuilder builder;

    auto serviceA = std::make_shared< ServiceA >();
    builder.registerInstance(serviceA).as< IServiceA >();

    // Act
    builder.registerType< ServiceA >().named< IServiceA >("whoami");

    // Assert
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
    // Arrange
    ContainerBuilder c;

    // Act
    c.autowireType< ServiceA >().as< IServiceA >();
    c.autowireType< ServiceB >().singleInstance();

    // Assert
    auto container = c.build();

    auto serviceB = container->resolve< ServiceB >();
    auto nullServiceB = container->resolve< IServiceB >();

    BOOST_CHECK(serviceB != nullptr);
    BOOST_CHECK(nullServiceB == nullptr);
    BOOST_CHECK(serviceB == container->resolve< ServiceB >());
}

BOOST_AUTO_TEST_CASE(autowired_registration_can_resolve_all_services)
{
    // Arrange
    ContainerBuilder c;
    c.autowireType< ServiceA >().as< IServiceA >().as< IRunWithScissors >();

    c.autowireType< ServiceB >().as< IServiceB >();
    c.autowireType< ServiceRunningWithScissors >().as< IServiceB >();

    c.autowireType< ServiceBController >();

    auto container = c.build();

    // Act
    auto serviceBController = container->resolve< ServiceBController >();

    // Assert
    BOOST_CHECK(serviceBController != nullptr);
}

BOOST_AUTO_TEST_CASE(registration_should_be_overridable)
{
    // Arrange
    ContainerBuilder c;

    std::shared_ptr< IServiceA > serviceA = std::make_shared< ServiceA >();
    c.registerInstance(serviceA);

    // Act
    c.autowireType< ServiceA >().as< IServiceA >().as< IRunWithScissors >().singleInstance();

    // Assert
    auto container = c.build();
    auto resolvedServiceA = container->resolve< IServiceA >();

    BOOST_CHECK(resolvedServiceA != nullptr);
    BOOST_CHECK(resolvedServiceA != serviceA);
}

BOOST_AUTO_TEST_CASE(registration_should_provide_instance_activating_data)
{
    // Arrange
    ContainerBuilder c;

    std::shared_ptr< IServiceA > serviceA = std::make_shared< ServiceA >();
    std::shared_ptr< IServiceA > anotherServiceA = std::make_shared< ServiceA >();

    // Act & Assert
    c.registerInstance(serviceA).onActivating
    (
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
    // Arrange
    ContainerBuilder c;

    std::shared_ptr< IServiceA > serviceA = std::make_shared< ServiceA >();
    bool onPreparingInvoked = false;

    // Act & Assert
    c.registerInstance(serviceA).onPreparing
    (
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
    // Arrange
    ContainerBuilder c;

    std::shared_ptr< IServiceA > serviceA = std::make_shared< ServiceA >();

    // Act & Assert
    c.registerInstance(serviceA).onActivated
    (
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
    // Arrange
    ContainerBuilder builder;

    int activatedCount = 0;
    auto registeredServiceA = std::make_shared< ServiceA >();

    // Act
    builder.registerInstance(registeredServiceA).onActivated
    (
        [&activatedCount](IActivatedData< ServiceA >&) -> void
        {
            ++activatedCount;
        }
    );

    // Assert
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
    // Arrange
    ContainerBuilder builder;

    int activatedCount = 0;

    // Act
    builder.registerType< ServiceA >().onActivated
    (
        [&activatedCount](IActivatedData< ServiceA >&) -> void
        {
            ++activatedCount;
        }
    );

    // Assert
    auto container = builder.build();

    auto serviceA = container->resolve< ServiceA >();
    auto sameServiceA = container->resolve< ServiceA >();

    BOOST_CHECK(activatedCount == 2);
}

BOOST_AUTO_TEST_CASE(registration_should_only_provide_instance_activated_data_once_for_shared_object)
{
    // Arrange
    ContainerBuilder builder;

    int activatedCount = 0;

    // Act
    builder.registerType< ServiceA >().onActivated
    (
        [&activatedCount](IActivatedData< ServiceA >&) -> void
        {
            ++activatedCount;
        }
    ).singleInstance();

    // Assert
    auto container = builder.build();
    auto serviceA = container->resolve< ServiceA >();
    auto sameServiceA = container->resolve< ServiceA >();

    BOOST_CHECK(activatedCount == 1);
}

BOOST_AUTO_TEST_CASE(should_use_move_constructor_with_shared_dependency)
{
    // Arrange
    ContainerBuilder builder;

    builder.registerType< ServiceA >().as< IServiceA >().singleInstance();
    builder.autowireType< MoveConstructorObject >();

    auto container = builder.build();

    // Act
    auto serviceA = container->resolve< IServiceA >();
    auto object = container->resolve< MoveConstructorObject >();
    auto serviceA2 = container->resolve< IServiceA >();

    // Assert
    BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(serviceA == serviceA2);

    BOOST_REQUIRE(object != nullptr);
    BOOST_CHECK(object->serviceA == serviceA);
}

BOOST_AUTO_TEST_CASE(should_use_move_constructor_with_transient_dependency)
{
    // Arrange
    ContainerBuilder builder;

    builder.registerType< ServiceA >().as< IServiceA >();
    builder.autowireType< MoveConstructorObject >();

    auto container = builder.build();

    // Act
    auto serviceA = container->resolve< IServiceA >();
    auto object = container->resolve< MoveConstructorObject >();
    auto serviceA2 = container->resolve< IServiceA >();

    // Assert
    BOOST_CHECK(serviceA != nullptr);
    BOOST_CHECK(serviceA2 != nullptr);
    BOOST_CHECK(serviceA != serviceA2);

    BOOST_REQUIRE(object != nullptr);
    BOOST_CHECK(object->serviceA != nullptr);
    BOOST_CHECK(object->serviceA != serviceA);
    BOOST_CHECK(object->serviceA != serviceA2);
}

BOOST_AUTO_TEST_CASE(should_create_scoped_container)
{
    // Arrange
    ContainerBuilder builder;

    builder.registerType< ServiceA >().as< IServiceA >();

    auto container = builder.build();

    auto serviceB = container->resolve< ServiceB >();
    BOOST_CHECK(serviceB == nullptr);

    {
        // Act
        auto lifetimeScope = container->createLifetimeScope();

        ContainerBuilder scopedBuilder;
        scopedBuilder.autowireType< ServiceB >();

        scopedBuilder.build(lifetimeScope->componentRegistry());

        // Assert
        serviceB = container->resolve< ServiceB >();
        BOOST_CHECK(serviceB == nullptr);

        serviceB = lifetimeScope->resolve< ServiceB >();
        BOOST_CHECK(serviceB != nullptr);
    }

    serviceB = container->resolve< ServiceB >();
    BOOST_CHECK(serviceB == nullptr);
}

BOOST_AUTO_TEST_CASE(should_create_scoped_container_with_single_instances)
{
    // Arrange
    ContainerBuilder builder;

    builder.registerType< ServiceA >().as< IServiceA >();

    auto container = builder.build();

    auto serviceB = container->resolve< IServiceB >();
    BOOST_CHECK(serviceB == nullptr);

    {
        // Act
        auto lifetimeScope = container->createLifetimeScope();

        ContainerBuilder scopedBuilder;
        scopedBuilder.autowireType< ServiceB >().as< IServiceB >().singleInstance();

        scopedBuilder.build(lifetimeScope->componentRegistry());

        // Assert
        serviceB = container->resolve< IServiceB >();
        BOOST_CHECK(serviceB == nullptr);

        serviceB = lifetimeScope->resolve< IServiceB >();
        BOOST_CHECK(serviceB != nullptr);

        auto sameServiceB = lifetimeScope->resolve< IServiceB >();
        BOOST_CHECK(sameServiceB == serviceB);
    }

    serviceB = container->resolve< IServiceB >();
    BOOST_CHECK(serviceB == nullptr);
}

BOOST_AUTO_TEST_CASE(should_create_scoped_container_with_scoped_dependencies)
{
    // Arrange

    struct RandomDependency
    {
        typedef Hypodermic::AutowiredConstructor< RandomDependency() > AutowiredSignature;

        RandomDependency() {}
    };

    struct CoolObject
    {
        typedef Hypodermic::AutowiredConstructor< CoolObject(RandomDependency*) > AutowiredSignature;

        CoolObject(std::shared_ptr< RandomDependency > randomDependency)
            : randomDependency(randomDependency)
        {
            if (randomDependency == nullptr)
                throw std::invalid_argument("randomDependency");
        }

        std::shared_ptr< RandomDependency > randomDependency;
    };

    ContainerBuilder builder;

    builder.autowireType< CoolObject >();

    auto container = builder.build();

    BOOST_CHECK_THROW(container->resolve< CoolObject >(), std::exception);

    {
        // Act
        auto lifetimeScope = container->createLifetimeScope();

        ContainerBuilder scopedBuilder;
        scopedBuilder.autowireType< RandomDependency >().singleInstance();

        scopedBuilder.build(lifetimeScope->componentRegistry());

        // Assert
        auto randomDependency = lifetimeScope->resolve< RandomDependency >();

        BOOST_CHECK_THROW(container->resolve< CoolObject >(), std::exception);
        BOOST_CHECK_NO_THROW(lifetimeScope->resolve< CoolObject >());

        auto coolObject = lifetimeScope->resolve< CoolObject >();
        BOOST_CHECK(coolObject != nullptr);
        BOOST_CHECK(coolObject->randomDependency == randomDependency);
    }

    BOOST_CHECK_THROW(container->resolve< CoolObject >(), std::exception);
}

BOOST_AUTO_TEST_CASE(should_resolve_factory_method)
{
    // Arrange
    struct RandomDependency
    {
        typedef Hypodermic::AutowiredConstructor< RandomDependency() > AutowiredSignature;

        RandomDependency() {}
    };


    struct FactoryMethodDependent
    {
        typedef Hypodermic::AutowiredConstructor< FactoryMethodDependent(std::function< RandomDependency*() >) > AutowiredSignature;

        FactoryMethodDependent(std::function< std::shared_ptr< RandomDependency >() > f)
        {
            BOOST_CHECK(f);
            BOOST_CHECK(f() != nullptr);
        }
    };

    ContainerBuilder c;

    c.autowireType< RandomDependency >();
    c.autowireType< FactoryMethodDependent >();

    auto container = c.build();

    // Act
    auto instance = container->resolve< FactoryMethodDependent >();

    // Assert
    BOOST_CHECK(instance != nullptr);
}

BOOST_AUTO_TEST_SUITE_END()