#ifndef		HYPODERMIC_CONTAINER_H_
# define	HYPODERMIC_CONTAINER_H_

# include <stdexcept>
# include <boost/assign.hpp>

# include <Hypodermic/ComponentRegistration.h>
# include <Hypodermic/ComponentRegistry.h>
# include <Hypodermic/CurrentLifetimeScope.h>
# include <Hypodermic/DelegateActivator.h>
# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IComponentRegistry.h>
# include <Hypodermic/IContainer.h>
# include <Hypodermic/ILifetimeScope.h>
# include <Hypodermic/LifetimeScope.h>
# include <Hypodermic/TypedService.h>


namespace Hypodermic
{
	class Container : public IContainer, public IComponentContext
	{
	public:
		Container()
		{
            using namespace boost::assign;

            std::vector< Service* > services = list_of(new TypedService(typeid(ILifetimeScope*)))
                                                      (new TypedService(typeid(IComponentContext*)));

			componentRegistry_ = new ComponentRegistry;

            componentRegistry_->addRegistration(new ComponentRegistration(
                LifetimeScope::selfRegistrationId(),
                new DelegateActivator< LifetimeScope* >(
                    typeid(LifetimeScope*), Func< IComponentContext*, LifetimeScope* >(
                        [](IComponentContext* c) -> LifetimeScope*
                        {
                            throw std::logic_error("Self registration cannot be activated");
                        })),
                new CurrentLifetimeScope,
                InstanceSharing::Shared,
                InstanceOwnership::ExternallyOwned,
                services));

            rootLifetimeScope_ = new LifetimeScope(componentRegistry_);
        }

		IComponentRegistry* componentRegistry()
		{
			return componentRegistry_;
		}

		void* resolveComponent(IComponentRegistration* registration)
		{
            return rootLifetimeScope_->resolveComponent(registration);
		}

		void* getOrCreateInstance(IComponentRegistration* registration)
		{
			auto i = registration->activator()->activateInstance(this);
			return i;
		}

	private:
		IComponentRegistry* componentRegistry_;
        ILifetimeScope* rootLifetimeScope_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_CONTAINER_H_ */