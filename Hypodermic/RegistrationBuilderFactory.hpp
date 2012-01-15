#ifdef      HYPODERMIC_REGISTRATION_BUILDER_FACTORY_H_
# ifndef    HYPODERMIC_REGISTRATION_BUILDER_FACTORY_HPP_
#  define   HYPODERMIC_REGISTRATION_BUILDER_FACTORY_HPP_

# include <boost/make_shared.hpp>

# include <Hypodermic/RegistrationBuilder.h>


namespace Hypodermic
{
    class IComponentRegistration;


    template <class T>
	boost::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > RegistrationBuilderFactory::forDelegate(Func< IComponentContext*, T > delegate)
	{
        auto& typeInfo = typeid(T);
		return boost::make_shared< RegistrationBuilder< T, SingleRegistrationStyle > >(
            new TypedService(typeInfo),
			new DelegateActivator< T >(typeInfo, delegate),
            SingleRegistrationStyle());
	}

	template <class T>
	boost::shared_ptr< IRegistrationBuilder< T, SingleRegistrationStyle > > RegistrationBuilderFactory::forType()
	{
        auto& typeInfo = typeid(T);
		return boost::make_shared< RegistrationBuilder< T, SingleRegistrationStyle > >(
            new TypedService(typeInfo),
			new DelegateActivator< T >(typeInfo, Func< IComponentContext*, T >(
                [](IComponentContext* c) -> T
			    {
				    return new boost::remove_pointer< T >::type;
			    })),
            SingleRegistrationStyle());
	}

	template <class T, class RegistrationStyleT>
	void RegistrationBuilderFactory::registerSingleComponent(IComponentRegistry* cr, boost::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > rb)
	{
		auto registration = createRegistration< T, RegistrationStyleT >(rb);
		cr->addRegistration(registration);
	}

	template <class T, class RegistrationStyleT>
	IComponentRegistration* RegistrationBuilderFactory::createRegistration(boost::shared_ptr< IRegistrationBuilder< T, RegistrationStyleT > > rb)
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