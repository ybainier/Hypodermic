#ifndef		HYPODERMIC_REGISTRATION_BUILDER_H_
# define	HYPODERMIC_REGISTRATION_BUILDER_H_

# include <vector>
# include <boost/type_traits.hpp>

# include <Hypodermic/ComponentRegistration.h>
# include <Hypodermic/CurrentLifetimeScope.h>
# include <Hypodermic/DelegateActivator.h>
# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IComponentRegistry.h>
# include <Hypodermic/IInstanceActivator.h>
# include <Hypodermic/IRegistrationBuilder.h>
# include <Hypodermic/RegistrationData.h>
# include <Hypodermic/RootScopeLifetime.h>
# include <Hypodermic/SingleRegistrationStyle.h>
# include <Hypodermic/TypedService.h>


namespace Hypodermic
{
	template <class T, class RegistrationStyleT>
	class RegistrationBuilder : public IRegistrationBuilder< T, RegistrationStyleT >
	{
	public:
		RegistrationBuilder(Service* defaultService, IInstanceActivator* activator,
                            const RegistrationStyleT& registrationStyle)
			: registrationData_(defaultService), activator_(activator), registrationStyle_(registrationStyle)
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

        RegistrationStyleT& registrationStyle()
        {
            return registrationStyle_;
        }

		IRegistrationBuilder< T, RegistrationStyleT >* singleInstance()
		{
            auto& rd = registrationData();
			rd.sharing(InstanceSharing::Shared);
			rd.lifetime(new RootScopeLifetime);
			return this;
		}

        IRegistrationBuilder< T, RegistrationStyleT >* instancePerLifetimeScope()
        {
            auto& rd = registrationData();
            rd.sharing(InstanceSharing::Shared);
            rd.lifetime(new CurrentLifetimeScope);
            return this;
        }

        IRegistrationBuilder< T, RegistrationStyleT >* instancePerDependency()
        {
            auto& rd = registrationData();
            rd.sharing(InstanceSharing::None);
            rd.lifetime(new CurrentLifetimeScope);
            return this;
        }

		template <class ServiceT>
		IRegistrationBuilder< T, RegistrationStyleT >* as()
		{
			registrationData_.addService(new TypedService(typeid(ServiceT)));
			return this;
		}

	private:
		RegistrationData registrationData_;
		IInstanceActivator* activator_;
        RegistrationStyleT registrationStyle_;
	};


	class RegistrationBuilderFactory
	{
	public:
		template <class T>
		static IRegistrationBuilder< T, SingleRegistrationStyle >* forDelegate(Func< IComponentContext*, T > delegate)
		{
			return new RegistrationBuilder< T, SingleRegistrationStyle >(
				new TypedService(typeid(T)),
				new DelegateActivator< T >(typeid(T), delegate),
                SingleRegistrationStyle());
		}

		template <class T>
		static IRegistrationBuilder< T, SingleRegistrationStyle >* forType()
		{
			return new RegistrationBuilder< T, SingleRegistrationStyle >(
				new TypedService(typeid(T)),
				new DelegateActivator< T >(typeid(T), Func< IComponentContext*, T >(
					[](IComponentContext* c) -> T
					{
						return new boost::remove_pointer< T >::type;
					})),
                SingleRegistrationStyle());
		}

		template <class T, class RegistrationStyleT>
		static void registerSingleComponent(IComponentRegistry* cr, IRegistrationBuilder< T, RegistrationStyleT >* rb)
		{
			auto registration = createRegistration< T, RegistrationStyleT >(rb);
			cr->addRegistration(registration);

			//var registeredEventArgs = new ComponentRegisteredEventArgs(cr, registration);
			//foreach (var rh in rb.RegistrationStyle.RegisteredHandlers)
			//	rh(cr, registeredEventArgs);
		}

		template <class T, class RegistrationStyleT>
		static IComponentRegistration* createRegistration(IRegistrationBuilder< T, RegistrationStyleT >* rb)
		{
			return createRegistration(rb->registrationStyle().id(),
                                      rb->registrationData(),
                                      rb->activator(),
                                      rb->registrationData().services(),
                                      rb->registrationStyle().target());
		}

		static IComponentRegistration* createRegistration(const boost::uuids::uuid& id,
                                                          RegistrationData& registrationData,
                                                          IInstanceActivator* activator,
			                                              std::vector< Service* >& services,
                                                          IComponentRegistration* target)
		{
            IComponentRegistration* registration;

            if (target == nullptr)
            {
                registration = new ComponentRegistration(id,
                                                         activator,
                                                         registrationData.lifetime(),
                                                         registrationData.sharing(),
                                                         registrationData.ownership(),
                                                         services);
            }
            else
            {
                registration = new ComponentRegistration(id,
                                                         activator,
                                                         registrationData.lifetime(),
                                                         registrationData.sharing(),
                                                         registrationData.ownership(),
                                                         services,
                                                         target);
            }

			return registration;
		}
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_REGISTRATION_BUILDER_H_ */