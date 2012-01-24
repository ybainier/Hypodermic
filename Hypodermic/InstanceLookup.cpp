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

    InstanceLookup::InstanceLookup(IComponentRegistration* registration,
                                   IResolveOperation* context,
                                   ISharingLifetimeScope* mostNestedVisibleScope)
        : componentRegistration_(registration)
        , context_(context)
        , activationScope_(nullptr)
        , newInstance_(nullptr)
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

        std::shared_ptr< void > instance = nullptr;
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

    IComponentRegistry* InstanceLookup::componentRegistry()
    {
        return activationScope_->componentRegistry();
    }

    std::shared_ptr< void > InstanceLookup::resolveComponent(IComponentRegistration* registration)
    {
        return context_->getOrCreateInstance(activationScope_, registration);
    }

    IComponentRegistration* InstanceLookup::componentRegistration()
    {
        return componentRegistration_;
    }

    ILifetimeScope* InstanceLookup::activationScope()
    {
        return activationScope_;
    }

    bool InstanceLookup::newInstanceActivated()
    {
        return newInstance_ != nullptr;
    }

    std::shared_ptr< void > InstanceLookup::activate()
    {
        newInstance_ = std::shared_ptr< void >(componentRegistration_->activator()->activateInstance(this));

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
