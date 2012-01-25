#include <functional>
#include <stdexcept>

#include "IComponentLifetime.h"
#include "IComponentRegistration.h"
#include "IInstanceActivator.h"
#include "IResolveOperation.h"
#include "ISharingLifetimeScope.h"
#include "InstanceLookup.h"


namespace Hypodermic
{

    InstanceLookup::InstanceLookup(std::shared_ptr< IComponentRegistration > registration,
                                   std::shared_ptr< IResolveOperation > context,
                                   std::shared_ptr< ISharingLifetimeScope > mostNestedVisibleScope)
        : componentRegistration_(registration)
        , context_(context)
        , executed_(false)
    {
        if (registration == nullptr)
            throw std::invalid_argument("registration");
        if (context == nullptr)
            throw std::invalid_argument("context");
        if (mostNestedVisibleScope == nullptr)
            throw std::invalid_argument("mostNestedVisibleScope");
        activationScope_ = componentRegistration_->lifetime()->findScope(mostNestedVisibleScope);
    }

    std::shared_ptr< void > InstanceLookup::execute()
    {
        if (executed_)
            throw std::logic_error("Activation already executed");

        executed_ = true;

        std::shared_ptr< void > instance;
        if (componentRegistration_->sharing() == InstanceSharing::None)
            instance = activate();
        else
        {
            instance = activationScope_->getOrCreateAndShare(
                componentRegistration_->id(),
                std::bind(&InstanceLookup::activate, this));
        }

        return instance;
    }

    std::shared_ptr< IComponentRegistry > InstanceLookup::componentRegistry()
    {
        return activationScope_->componentRegistry();
    }

    std::shared_ptr< void > InstanceLookup::resolveComponent(std::shared_ptr< IComponentRegistration > registration)
    {
        return context_->getOrCreateInstance(activationScope_, registration);
    }

    std::shared_ptr< IComponentRegistration > InstanceLookup::componentRegistration()
    {
        return componentRegistration_;
    }

    std::shared_ptr< ILifetimeScope > InstanceLookup::activationScope()
    {
        return activationScope_;
    }

    bool InstanceLookup::newInstanceActivated()
    {
        return newInstance_ != nullptr;
    }

    std::shared_ptr< void > InstanceLookup::activate()
    {
        newInstance_ = std::shared_ptr< void >(componentRegistration_->activator()->activateInstance(shared_from_this()));

        //TODO
        //if (componentRegistration_->ownership() == InstanceOwnership::OwnedByLifetimeScope)
        //{
        //}

        return newInstance_;
    }

    void InstanceLookup::complete()
    {
        //TODO
        //if (newInstanceActivated())
        //{
        //}
    }

} // namespace Hypodermic
