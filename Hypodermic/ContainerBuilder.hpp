#ifdef      HYPODERMIC_CONTAINER_BUILDER_H_
# ifndef    HYPODERMIC_CONTAINER_BUILDER_HPP_
#  define   HYPODERMIC_CONTAINER_BUILDER_HPP_

# include <type_traits>

# include <Hypodermic/RegistrationBuilderFactory.h>
# include <Hypodermic/RootScopeLifetime.h>


namespace Hypodermic
{

    template <class T>
    std::shared_ptr< typename ContainerBuilder::RegistrationBuilderInterface< T >::Type >
    ContainerBuilder::autowireType()
    {
        typedef typename T::AutowiredSignature AutowiredSignature;
        static_assert(AutowiredSignature::IsSignatureRecognized::value, "Unable to use this autowired constructor.");

        auto rb = RegistrationBuilderFactory< ContainerBuilder::RegistrationBuilderInterface >::forDelegate(AutowiredSignature::createDelegate());

        registerCallback(
            [rb](std::shared_ptr< IComponentRegistry > cr) -> void
            {
                RegistrationBuilderFactory< ContainerBuilder::RegistrationBuilderInterface >::registerSingleComponent< T >(cr, rb);
            });

        return rb;
    }

    template <class T>
    std::shared_ptr< typename ContainerBuilder::RegistrationBuilderInterface< T >::Type >
    ContainerBuilder::registerType(std::function< T*(IComponentContext&) > delegate)
    {
        static_assert(!std::is_pod< T >::value || std::is_empty< T >::value || std::is_class< T >::value,
                      "ContainerBuilder::registerType< T >() is incompatible with POD types.");

        auto rb = RegistrationBuilderFactory< ContainerBuilder::RegistrationBuilderInterface >::forDelegate(delegate);

        registerCallback(
            [rb](std::shared_ptr< IComponentRegistry > cr) -> void
            {
                RegistrationBuilderFactory< ContainerBuilder::RegistrationBuilderInterface >::registerSingleComponent< T >(cr, rb);
            });

        return rb;
    }

    template <class T>
    std::shared_ptr< typename ContainerBuilder::RegistrationBuilderInterface< T >::Type >
    ContainerBuilder::registerType()
    {
        static_assert(!std::is_pod< T >::value || std::is_empty< T >::value || std::is_class< T >::value,
                      "ContainerBuilder::registerType< T >() is incompatible with POD types.");

        auto rb = RegistrationBuilderFactory< ContainerBuilder::RegistrationBuilderInterface >::forType< T >();

        registerCallback(
            [rb](std::shared_ptr< IComponentRegistry > cr) -> void
            {
                RegistrationBuilderFactory< ContainerBuilder::RegistrationBuilderInterface >::registerSingleComponent< T >(cr, rb);
            });

        return rb;
    }

    template <class T>
    std::shared_ptr< typename ContainerBuilder::RegistrationBuilderInterface< T >::Type >
    ContainerBuilder::registerInstance(std::shared_ptr< T > instance)
    {
        static_assert(!std::is_pod< T >::value || std::is_empty< T >::value || std::is_class< T >::value,
                      "ContainerBuilder::registerType< T >(std::shared_ptr< T > instance) is incompatible with POD types.");

        auto rb = RegistrationBuilderFactory< ContainerBuilder::RegistrationBuilderInterface >::forInstance(instance);

        rb->singleInstance();

        registerCallback(
            [rb](std::shared_ptr< IComponentRegistry > cr) -> void
            {
                auto rootScopeLifetime = std::dynamic_pointer_cast< RootScopeLifetime >(rb->registrationData().lifetime());
                if (rootScopeLifetime == nullptr || rb->registrationData().sharing() != InstanceSharing::Shared)
                    throw std::logic_error("Instance registration is single instance only.");

                RegistrationBuilderFactory< ContainerBuilder::RegistrationBuilderInterface >::registerSingleComponent< T >(cr, rb);
            });

        return rb;
    }

} // namespace Hypodermic


# endif /* !HYPODERMIC_CONTAINER_BUILDER_HPP_ */
#endif /* HYPODERMIC_CONTAINER_BUILDER_H_ */