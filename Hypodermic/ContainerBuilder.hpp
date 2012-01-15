#ifdef      HYPODERMIC_CONTAINER_BUILDER_H_
# ifndef    HYPODERMIC_CONTAINER_BUILDER_HPP_
#  define   HYPODERMIC_CONTAINER_BUILDER_HPP_

# include <Hypodermic/ProvidedInstanceActivator.h>
# include <Hypodermic/RegistrationBuilder.h>
# include <Hypodermic/RegistrationBuilderFactory.h>
# include <Hypodermic/RootScopeLifetime.h>


namespace Hypodermic
{

    template <class T>
    IRegistrationBuilder< T, SingleRegistrationStyle >* ContainerBuilder::registerType(Func< IComponentContext*, T > delegate)
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
    IRegistrationBuilder< T, SingleRegistrationStyle >* ContainerBuilder::registerType()
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
    IRegistrationBuilder< T, SingleRegistrationStyle >* ContainerBuilder::registerType(T instance)
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

                RegistrationBuilderFactory::registerSingleComponent(cr, rb);
            }));

        return rb;
    }

} // namespace Hypodermic


# endif /* !HYPODERMIC_CONTAINER_BUILDER_HPP_ */
#endif /* HYPODERMIC_CONTAINER_BUILDER_H_ */