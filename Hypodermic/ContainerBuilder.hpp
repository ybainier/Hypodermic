#ifdef      HYPODERMIC_CONTAINER_BUILDER_H_
# ifndef    HYPODERMIC_CONTAINER_BUILDER_HPP_
#  define   HYPODERMIC_CONTAINER_BUILDER_HPP_

# include <type_traits>

# include <Hypodermic/RegistrationBuilder.h>
# include <Hypodermic/RegistrationBuilderFactory.h>
# include <Hypodermic/RootScopeLifetime.h>


namespace Hypodermic
{

    template <class T>
    std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > ContainerBuilder::registerType(std::function< T*(IComponentContext&) > delegate)
    {
        static_assert(!std::is_pod< T >::value || std::is_empty< T >::value || std::is_class< T >::value, "ContainerBuilder::registerType< T >() is incompatible with POD types");

        auto rb = RegistrationBuilderFactory::forDelegate(delegate);

        registerCallback(
            [rb](std::shared_ptr< IComponentRegistry > cr) -> void
            {
                RegistrationBuilderFactory::registerSingleComponent(cr, rb);
            });

        return rb;
    }

    template <class T>
    std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > ContainerBuilder::registerType()
    {
        static_assert(!std::is_pod< T >::value || std::is_empty< T >::value || std::is_class< T >::value, "ContainerBuilder::registerType< T >() is incompatible with POD types");

        auto rb = RegistrationBuilderFactory::forType< T >();

        registerCallback(
            [rb](std::shared_ptr< IComponentRegistry > cr) -> void
            {
                RegistrationBuilderFactory::registerSingleComponent(cr, rb);
            });

        return rb;
    }

    template <class T>
    std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > ContainerBuilder::registerType(T* instance)
    {
        static_assert(!std::is_pod< T >::value || std::is_empty< T >::value || std::is_class< T >::value, "ContainerBuilder::registerType< T >(T* instance) is incompatible with POD types");

        auto rb = RegistrationBuilderFactory::forInstance(instance);

        rb->singleInstance();

        registerCallback(
            [rb](std::shared_ptr< IComponentRegistry > cr) -> void
            {
                auto rootScopeLifetime = std::dynamic_pointer_cast< RootScopeLifetime >(rb->registrationData().lifetime());
                if (rootScopeLifetime == nullptr || rb->registrationData().sharing() != InstanceSharing::Shared)
                    throw std::logic_error("Instance registration is single instance only");

                RegistrationBuilderFactory::registerSingleComponent(cr, rb);
            });

        return rb;
    }

} // namespace Hypodermic


# endif /* !HYPODERMIC_CONTAINER_BUILDER_HPP_ */
#endif /* HYPODERMIC_CONTAINER_BUILDER_H_ */