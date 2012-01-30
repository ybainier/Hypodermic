#ifdef      HYPODERMIC_CONTAINER_BUILDER_H_
# ifndef    HYPODERMIC_CONTAINER_BUILDER_HPP_
#  define   HYPODERMIC_CONTAINER_BUILDER_HPP_

# include <type_traits>
# include <boost/mpl/for_each.hpp>

# include <Hypodermic/RegistrationBuilder.h>
# include <Hypodermic/RegistrationBuilderFactory.h>
# include <Hypodermic/RootScopeLifetime.h>


namespace Hypodermic
{

    template <class T>
    struct AutowiringHelper
    {
        AutowiringHelper(std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > rb)
            : rb_(rb)
        {
        }

        template <class U> void operator()(U& /* dummy */)
        {
            rb_->as< U >();
        }

    private:
        std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > rb_;
    };


    template <class T>
    std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > ContainerBuilder::autowireType()
    {
        typedef T::AutowiredTypeRegistration AutowiredTypeRegistration;
        auto rb = RegistrationBuilderFactory::forDelegate(typename AutowiredTypeRegistration::ConstructorFactory::createDelegate());

        AutowiringHelper< T > helper(rb);
        boost::mpl::for_each< AutowiredTypeRegistration::Services >(helper);

        registerCallback(
            [rb](std::shared_ptr< IComponentRegistry > cr) -> void
            {
                RegistrationBuilderFactory::registerSingleComponent(cr, rb);
            });

        return rb;
    }

    template <class T>
    std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > ContainerBuilder::registerType(std::function< T*(IComponentContext&) > delegate)
    {
        static_assert(!std::is_pod< T >::value || std::is_empty< T >::value || std::is_class< T >::value,
                      "ContainerBuilder::registerType< T >() is incompatible with POD types");

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
        static_assert(!std::is_pod< T >::value || std::is_empty< T >::value || std::is_class< T >::value,
                      "ContainerBuilder::registerType< T >() is incompatible with POD types");

        auto rb = RegistrationBuilderFactory::forType< T >();

        registerCallback(
            [rb](std::shared_ptr< IComponentRegistry > cr) -> void
            {
                RegistrationBuilderFactory::registerSingleComponent(cr, rb);
            });

        return rb;
    }

    template <class T>
    std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > ContainerBuilder::registerInstance(std::shared_ptr< T > instance)
    {
        static_assert(!std::is_pod< T >::value || std::is_empty< T >::value || std::is_class< T >::value,
                      "ContainerBuilder::registerType< T >(std::shared_ptr< T > instance) is incompatible with POD types");

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