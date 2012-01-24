#ifdef      HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_
# ifndef    HYPODERMIC_REGISTRATION_BUILDER_FACTORY_HPP_
#  define   HYPODERMIC_REGISTRATION_BUILDER_FACTORY_HPP_

# include <Hypodermic/ProvidedInstanceActivator.h>
# include <Hypodermic/RegistrationBuilder.h>
# include <Hypodermic/TypedService.h>


namespace Hypodermic
{
    class IComponentRegistration;


    template <class T>
	std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > RegistrationBuilderFactory::forDelegate(std::function< T*(IComponentContext&) > delegate)
	{
        auto& typeInfo = typeid(T);
		return std::make_shared< RegistrationBuilder< T, SingleRegistrationStyle > >(
            std::make_shared< TypedService >(typeInfo),
			std::make_shared< DelegateActivator< T > >(typeInfo, delegate),
            SingleRegistrationStyle());
	}

	template <class T>
	std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > RegistrationBuilderFactory::forType()
	{
        auto& typeInfo = typeid(T);
		return std::make_shared< RegistrationBuilder< T, SingleRegistrationStyle > >(
            std::make_shared< TypedService >(typeInfo),
			std::make_shared< DelegateActivator< T > >(typeInfo,
                                       [](IComponentContext&) -> T*
			                           {
				                           return new T;
			                           }),
            SingleRegistrationStyle());
	}

	template <class T>
	std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > RegistrationBuilderFactory::forInstance(std::shared_ptr< T > instance)
	{
        return std::make_shared< RegistrationBuilder< T, SingleRegistrationStyle > >(
            std::make_shared< TypedService >(typeid(T)),
            std::make_shared< ProvidedInstanceActivator< T > >(instance),
            SingleRegistrationStyle());
	}

    template <class T, class RegistrationStyleT>
	void RegistrationBuilderFactory::registerSingleComponent(std::shared_ptr< IComponentRegistry > cr, std::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > rb)
	{
		auto registration = createRegistration< T, RegistrationStyleT >(rb);
		cr->addRegistration(registration);
	}

	template <class T, class RegistrationStyleT>
	std::shared_ptr< IComponentRegistration > RegistrationBuilderFactory::createRegistration(std::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > rb)
	{
		return createRegistration(rb->registrationStyle().id(),
                                  rb->registrationData(),
                                  rb->activator(),
                                  rb->registrationData().services(),
                                  rb->registrationStyle().target(),
                                  rb->typeCasters());
	}

} // namespace Hypodermic


# endif /* !HYPODERMIC_REGISTRATION_BUILDER_FACTORY_HPP_ */
#endif  /* HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_ */