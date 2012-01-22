#include <stdexcept>

#include <boost/uuid/random_generator.hpp>

#include "ResolveOperation.h"
#include "LifetimeScope.h"


namespace Hypodermic
{
	const boost::uuids::uuid LifetimeScope::selfRegistrationId = boost::uuids::random_generator()();
	std::function< void(ContainerBuilder*) > LifetimeScope::noConfiguration_ = [](ContainerBuilder*) -> void {};


    LifetimeScope::LifetimeScope(IComponentRegistry* componentRegistry)
        : componentRegistry_(componentRegistry)
        , parent_(nullptr)
    {
        if (componentRegistry == nullptr)
            throw std::invalid_argument("componentRegistry");
        root_ = this;
        sharedInstances_[selfRegistrationId] = this;
    }

    LifetimeScope::LifetimeScope(IComponentRegistry* componentRegistry, LifetimeScope* parent)
        : componentRegistry_(componentRegistry)
        , parent_(parent)
    {
        if (componentRegistry == nullptr)
            throw std::invalid_argument("componentRegistry");
        if (parent == nullptr)
            throw std::invalid_argument("parent");
        root_ = parent_->rootLifetimeScope();
        sharedInstances_[selfRegistrationId] = this;
    }

    ISharingLifetimeScope* LifetimeScope::parentLifetimeScope()
    {
        return parent_;
    }

    ISharingLifetimeScope* LifetimeScope::rootLifetimeScope()
    {
        return root_;
    }

    IComponentRegistry* LifetimeScope::componentRegistry()
    {
        return componentRegistry_;
    }

    void* LifetimeScope::resolveComponent(IComponentRegistration* registration)
    {
        if (registration == nullptr)
            throw std::invalid_argument("registration");

        {
            boost::lock_guard< decltype(mutex_) > lock(mutex_);

            ResolveOperation operation(this);
            return operation.execute(registration);
        }
    }

    void* LifetimeScope::getOrCreateAndShare(const boost::uuids::uuid& id, std::function< void*() > creator)
    {
        boost::lock_guard< decltype(mutex_) > lock(mutex_);

        void* result = nullptr;
        if (sharedInstances_.count(id) == 0)
        {
            result = creator();
            sharedInstances_[id] = result;
        }
        else
            result = sharedInstances_[id];

        return result;
    }

} // namespace Hypodermic