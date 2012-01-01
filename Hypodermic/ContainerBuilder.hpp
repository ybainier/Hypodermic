#ifdef      HYPODERMIC_CONTAINER_BUILDER_H_
# ifndef    HYPODERMIC_CONTAINER_BUILDER_HPP_
#  define   HYPODERMIC_CONTAINER_BUILDER_HPP_

# include <Hypodermic/ProvidedInstanceActivator.h>
# include <Hypodermic/RegistrationBuilder.h>
# include <Hypodermic/RootScopeLifetime.h>


namespace Hypodermic
{

    template <class T>
    IRegistrationBuilder< T, SingleRegistrationStyle >* ContainerBuilder::setup(Func< IComponentContext*, T > delegate)
    {
        auto rb = RegistrationBuilderFactory::forDelegate(delegate);

        registerCallback(ConfigurationCallback(
            [rb](IComponentRegistry* cr) -> void
            {
                RegistrationBuilderFactory::registerSingleComponent(cr, rb);
            }));

        return rb;
    }

    template <class T>
    IRegistrationBuilder< T, SingleRegistrationStyle >* ContainerBuilder::setup()
    {
        auto rb = RegistrationBuilderFactory::forType< T >();

        registerCallback(ConfigurationCallback(
            [rb](IComponentRegistry* cr) -> void
            {
                RegistrationBuilderFactory::registerSingleComponent(cr, rb);
            }));

        return rb;
    }

    template <class T>
    IRegistrationBuilder< T, SingleRegistrationStyle >* ContainerBuilder::setup(T instance)
    {
        auto activator = new ProvidedInstanceActivator< T >(instance);

        auto rb = new RegistrationBuilder< T, SingleRegistrationStyle >(
            new TypedService(typeid(T)),
            activator,
            SingleRegistrationStyle());

        rb->singleInstance();

        registerCallback(ConfigurationCallback(
            [rb, activator](IComponentRegistry* cr) -> void
            {
                auto rootScopeLifetime = dynamic_cast< RootScopeLifetime* >(rb->registrationData().lifetime());
                if (rootScopeLifetime == nullptr || rb->registrationData().sharing() != InstanceSharing::Shared)
                    throw std::logic_error("Instance registration are single instance only");

                //activator.disposeInstance(rb.registrationData().ownership() == InstanceOwnership.OwnedByLifetimeScope);
                RegistrationBuilderFactory::registerSingleComponent(cr, rb);
            }));

        return rb;
    }

} // namespace Hypodermic


# endif /* !HYPODERMIC_CONTAINER_BUILDER_HPP_ */
#endif /* HYPODERMIC_CONTAINER_BUILDER_H_ */