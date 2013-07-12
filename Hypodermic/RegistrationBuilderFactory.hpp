#ifdef      HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_
# ifndef    HYPODERMIC_REGISTRATION_BUILDER_FACTORY_HPP_
#  define   HYPODERMIC_REGISTRATION_BUILDER_FACTORY_HPP_

# include <boost/foreach.hpp>

# include <Hypodermic/ComponentRegistration.h>
# include <Hypodermic/ProvidedInstanceActivator.h>
# include <Hypodermic/RegistrationBuilderFactory.h>
# include <Hypodermic/RegistrationData.h>
# include <Hypodermic/TypedService.h>


namespace Hypodermic
{
    class IComponentRegistration;


    template <template <class> class RegistrationBuilderInterfaceT>
    template <class T>
	std::shared_ptr< typename RegistrationBuilderInterfaceT< T >::Type >
    RegistrationBuilderFactory< RegistrationBuilderInterfaceT >::forDelegate(std::function< T*(IComponentContext&) > delegate)
	{
        auto& typeInfo = typeid(T);
		return std::make_shared< typename RegistrationBuilderInterfaceT< T >::ImplementationType >(
            std::make_shared< TypedService >(typeInfo),
			std::make_shared< DelegateActivator< T > >(typeInfo, delegate),
            typename RegistrationBuilderInterfaceT< T >::RegistrationStyleType());
	}

    template <template <class> class RegistrationBuilderInterfaceT>
	std::shared_ptr< typename RegistrationBuilderInterfaceT< void >::Type >
    RegistrationBuilderFactory< RegistrationBuilderInterfaceT >::forDelegate(const std::type_info& typeInfo,
                                                                             std::function
                                                                                <
                                                                                    std::shared_ptr< void >(IComponentContext&)
                                                                                >
                                                                                delegate)
	{
		return std::make_shared< typename RegistrationBuilderInterfaceT< void >::ImplementationType >(
            std::make_shared< TypedService >(typeInfo),
			std::make_shared< DelegateActivator< void > >(typeInfo, delegate),
            typename RegistrationBuilderInterfaceT< void >::RegistrationStyleType());
	}

	template <template <class> class RegistrationBuilderInterfaceT>
    template <class T>
    std::shared_ptr< typename RegistrationBuilderInterfaceT< T >::Type >
    RegistrationBuilderFactory< RegistrationBuilderInterfaceT >::forType()
	{
        auto& typeInfo = typeid(T);
		return std::make_shared< typename RegistrationBuilderInterfaceT< T >::ImplementationType >(
            std::make_shared< TypedService >(typeInfo),
			std::make_shared< DelegateActivator< T > >(typeInfo, [](IComponentContext&) -> T* { return new T; }),
            typename RegistrationBuilderInterfaceT< T >::RegistrationStyleType());
	}

	template <template <class> class RegistrationBuilderInterfaceT>
    template <class T>
    std::shared_ptr< typename RegistrationBuilderInterfaceT< T >::Type >
    RegistrationBuilderFactory< RegistrationBuilderInterfaceT >::forInstance(std::shared_ptr< T > instance)
	{
        return std::make_shared< typename RegistrationBuilderInterfaceT< T >::ImplementationType >(
            std::make_shared< TypedService >(typeid(T)),
            std::make_shared< ProvidedInstanceActivator< T > >(instance),
            typename RegistrationBuilderInterfaceT< T >::RegistrationStyleType());
	}

    template <template <class> class RegistrationBuilderInterfaceT>
    template <class T>
    void RegistrationBuilderFactory< RegistrationBuilderInterfaceT >::registerSingleComponent(std::shared_ptr< IComponentRegistry > cr,
                                                                                              std::shared_ptr< typename RegistrationBuilderInterfaceT< T >::Type > rb)
	{
		auto registration = createRegistration< T >(rb);
		cr->addRegistration(registration);
	}

	template <template <class> class RegistrationBuilderInterfaceT>
    template <class T>
    std::shared_ptr< IComponentRegistration >
    RegistrationBuilderFactory< RegistrationBuilderInterfaceT >::createRegistration(std::shared_ptr< typename RegistrationBuilderInterfaceT< T >::Type > rb)
	{
		return createRegistration(rb->registrationStyle().id(),
                                  rb->registrationData(),
                                  rb->activator(),
                                  rb->registrationData().services(),
                                  rb->registrationStyle().target(),
                                  rb->typeCasters());
	}

    template <template <class> class RegistrationBuilderInterfaceT>
    std::shared_ptr< IComponentRegistration >
    RegistrationBuilderFactory< RegistrationBuilderInterfaceT >::createRegistration(const boost::uuids::uuid& id,
                                                                                    RegistrationData& registrationData,
                                                                                    std::shared_ptr< IInstanceActivator > activator,
			                                                                        std::vector< std::shared_ptr< Service > > services,
                                                                                    std::shared_ptr< IComponentRegistration > target,
                                                                                    const std::unordered_map< std::type_index, std::shared_ptr< ITypeCaster > >& typeCasters)
	{
        std::shared_ptr< IComponentRegistration > registration;

        if (target == nullptr)
            registration = std::shared_ptr< ComponentRegistration >(new ComponentRegistration(id, activator, registrationData.lifetime(), registrationData.sharing(),
                                                                    registrationData.ownership(), services, typeCasters));
        else
            registration = std::shared_ptr< ComponentRegistration >(new ComponentRegistration(id, activator, registrationData.lifetime(), registrationData.sharing(),
                                                                    registrationData.ownership(), services, typeCasters, target));


        BOOST_FOREACH(auto preparingCallback, registrationData.preparingCallbacks())
            registration->preparing().connect(preparingCallback);

        BOOST_FOREACH(auto activatingCallback, registrationData.activatingCallbacks())
            registration->activating().connect(activatingCallback);

        BOOST_FOREACH(auto activatedCallback, registrationData.activatedCallbacks())
            registration->activated().connect(activatedCallback);
        
        return registration;
	}

} // namespace Hypodermic


# endif /* !HYPODERMIC_REGISTRATION_BUILDER_FACTORY_HPP_ */
#endif  /* HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_ */