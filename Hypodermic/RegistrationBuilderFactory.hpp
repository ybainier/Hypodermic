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
	std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > RegistrationBuilderFactory::forDelegate(std::function< T*(IComponentContext*) > delegate)
	{
        auto& typeInfo = typeid(T);
		return std::make_shared< RegistrationBuilder< T, SingleRegistrationStyle > >(
            new TypedService(typeInfo),
			new DelegateActivator< T >(typeInfo, delegate),
            SingleRegistrationStyle());
	}

	template <class T>
	std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > RegistrationBuilderFactory::forType()
	{
        auto& typeInfo = typeid(T);
		return std::make_shared< RegistrationBuilder< T, SingleRegistrationStyle > >(
            new TypedService(typeInfo),
			new DelegateActivator< T >(typeInfo,
                                       [](IComponentContext* c) -> T*
			                           {
				                           return new T;
			                           }),
            SingleRegistrationStyle());
	}

	template <class T>
	std::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > RegistrationBuilderFactory::forInstance(T* instance)
	{
        return std::make_shared< RegistrationBuilder< T, SingleRegistrationStyle > >(
            new TypedService(typeid(T)),
            new ProvidedInstanceActivator< T >(instance),
            SingleRegistrationStyle());
	}

    template <class T, class RegistrationStyleT>
	void RegistrationBuilderFactory::registerSingleComponent(IComponentRegistry* cr, std::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > rb)
	{
		auto registration = createRegistration< T, RegistrationStyleT >(rb);
		cr->addRegistration(registration);
	}

	template <class T, class RegistrationStyleT>
	IComponentRegistration* RegistrationBuilderFactory::createRegistration(std::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > rb)
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