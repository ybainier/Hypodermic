#include <stdexcept>
#include <boost/assign.hpp>

#include "ComponentRegistration.h"
#include "ComponentRegistry.h"
#include "CurrentLifetimeScope.h"
#include "DelegateActivator.h"
#include "IComponentRegistry.h"
#include "ILifetimeScope.h"
#include "LifetimeScope.h"
#include "TypedService.h"
#include "Container.h"


namespace Hypodermic
{

    Container::Container()
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

    IComponentRegistry* Container::componentRegistry()
    {
        return componentRegistry_;
    }

    void* Container::resolveComponent(IComponentRegistration* registration)
    {
        return rootLifetimeScope_->resolveComponent(registration);
    }

    void* Container::getOrCreateInstance(IComponentRegistration* registration)
    {
        auto i = registration->activator()->activateInstance(this);
        return i;
    }

} // namespace Hypodermic
