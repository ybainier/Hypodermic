#ifndef		REGISTRATION_BUILDER_H_
# define	REGISTRATION_BUILDER_H_

# include <vector>
# include <boost/type_traits.hpp>

# include <Hypodermic/ComponentRegistration.h>
# include <Hypodermic/DelegateActivator.h>
# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IInstanceActivator.h>
# include <Hypodermic/IRegistrationBuilder.h>
# include <Hypodermic/RegistrationData.h>
# include <Hypodermic/TypedService.h>


namespace Hypodermic
{
	template <class T>
	class RegistrationBuilder : public IRegistrationBuilder< T >
	{
	public:
		RegistrationBuilder(Service* defaultService, IInstanceActivator* activator)
			: registrationData_(defaultService), activator_(activator)
		{
		}

		RegistrationData& registrationData()
		{
			return registrationData_;
		}

		IInstanceActivator* activator()
		{
			return activator_;
		}

		template <class ServiceT>
		IRegistrationBuilder< T >* as()
		{
			registrationData_.addService(new TypedService(typeid(ServiceT)));
			return this;
		}

	private:
		RegistrationData registrationData_;
		IInstanceActivator* activator_;
	};


	class RegistrationBuilderFactory
	{
	public:
		template <class T>
		static IRegistrationBuilder< T >* forDelegate(Func< IComponentContext*, T > delegate)
		{
			return new RegistrationBuilder< T >(
				new TypedService(typeid(T)),
				new DelegateActivator< T >(typeid(T), delegate));
		}

		template <class T>
		static IRegistrationBuilder< T >* forType()
		{
			return new RegistrationBuilder< T >(
				new TypedService(typeid(T)),
				new DelegateActivator< T >(typeid(T), Func< IComponentContext*, T >(
					[](IComponentContext* c) -> T
					{
						return new boost::remove_pointer< T >::type;
					})));
		}

		template <class T>
		static void registerSingleComponent(IComponentRegistry* cr, IRegistrationBuilder< T >* rb)
		{
			auto registration = createRegistration< T >(rb);
			cr->addRegistration(registration);

			//var registeredEventArgs = new ComponentRegisteredEventArgs(cr, registration);
			//foreach (var rh in rb.RegistrationStyle.RegisteredHandlers)
			//	rh(cr, registeredEventArgs);
		}

		template <class T>
		static IComponentRegistration* createRegistration(IRegistrationBuilder< T >* rb)
		{
			return createRegistration(rb->registrationData(), rb->activator(), rb->registrationData().services());
		}

		static IComponentRegistration* createRegistration(RegistrationData& registrationData, IInstanceActivator* activator,
			std::vector< Service* >& services)
		{
			return new ComponentRegistration(activator, services);
		}
	};

} // namespace Hypodermic


#endif /* !REGISTRATION_BUILDER_H_ */