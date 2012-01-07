#ifdef      HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_
# ifndef    HYPODERMIC_REGISTRATION_BUILDER_FACTORY_HPP_
#  define   HYPODERMIC_REGISTRATION_BUILDER_FACTORY_HPP_

# include <Hypodermic/RegistrationBuilder.h>


namespace Hypodermic
{
    class IComponentRegistration;


    template <class T>
	IRegistrationBuilder< T, SingleRegistrationStyle >* RegistrationBuilderFactory::forDelegate(Func< IComponentContext*, T > delegate)
	{
        auto& typeInfo = typeid(T);
		return new RegistrationBuilder< T, SingleRegistrationStyle >(
            new TypedService(typeInfo),
			new DelegateActivator< T >(typeInfo, delegate),
            SingleRegistrationStyle());
	}

	template <class T>
	IRegistrationBuilder< T, SingleRegistrationStyle >* RegistrationBuilderFactory::forType()
	{
        auto& typeInfo = typeid(T);
		return new RegistrationBuilder< T, SingleRegistrationStyle >(
            new TypedService(typeInfo),
			new DelegateActivator< T >(typeInfo, Func< IComponentContext*, T >(
                [](IComponentContext* c) -> T
			    {
				    return new boost::remove_pointer< T >::type;
			    })),
            SingleRegistrationStyle());
	}

	template <class T, class RegistrationStyleT>
	void RegistrationBuilderFactory::registerSingleComponent(IComponentRegistry* cr, IRegistrationBuilder< T, RegistrationStyleT >* rb)
	{
		auto registration = createRegistration< T, RegistrationStyleT >(rb);
		cr->addRegistration(registration);
	}

	template <class T, class RegistrationStyleT>
	IComponentRegistration* RegistrationBuilderFactory::createRegistration(IRegistrationBuilder< T, RegistrationStyleT >* rb)
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