#include "ScopeRestrictedRegistry.h"

#include <stdexcept>

#include "ComponentRegistrationLifetimeDecorator.h"
#include "IComponentRegistration.h"
#include "MatchingScopeLifetime.h"
#include "RootScopeLifetime.h"


namespace Hypodermic
{

    ScopeRestrictedRegistry::ScopeRestrictedRegistry()
        : restrictedRootScopeLifetime_(std::make_shared< MatchingScopeLifetime >())
    {
    }

    void ScopeRestrictedRegistry::addRegistration(std::shared_ptr< IComponentRegistration > registration, bool preserveDefaults)
    {
        if (registration == nullptr)
            throw std::invalid_argument("registration");

        auto toRegister = registration;

        auto rootScopeLifetime = std::dynamic_pointer_cast< RootScopeLifetime >(registration->lifetime());
        if (rootScopeLifetime != nullptr)
            toRegister = std::make_shared< ComponentRegistrationLifetimeDecorator >(registration, restrictedRootScopeLifetime_);

        ComponentRegistry::addRegistration(toRegister, preserveDefaults);
    }

} // namespace Hypodermic