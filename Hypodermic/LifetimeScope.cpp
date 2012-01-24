#include <stdexcept>

#include <boost/uuid/random_generator.hpp>

#include "ResolveOperation.h"
#include "LifetimeScope.h"


namespace Hypodermic
{
	const boost::uuids::uuid LifetimeScope::selfRegistrationId = boost::uuids::random_generator()();
	std::function< void(ContainerBuilder*) > LifetimeScope::noConfiguration_ = [](ContainerBuilder*) -> void {};


    LifetimeScope::LifetimeScope(std::shared_ptr< IComponentRegistry > componentRegistry)
        : componentRegistry_(componentRegistry)
        , parent_(nullptr)
    {
        if (componentRegistry == nullptr)
            throw std::invalid_argument("componentRegistry");
    }

    LifetimeScope::LifetimeScope(std::shared_ptr< IComponentRegistry > componentRegistry, std::shared_ptr< LifetimeScope > parent)
        : componentRegistry_(componentRegistry)
        , parent_(parent)
    {
        if (componentRegistry == nullptr)
            throw std::invalid_argument("componentRegistry");
        if (parent == nullptr)
            throw std::invalid_argument("parent");
    }

    std::shared_ptr< ISharingLifetimeScope > LifetimeScope::parentLifetimeScope()
    {
        return parent_;
    }

    std::shared_ptr< ISharingLifetimeScope > LifetimeScope::rootLifetimeScope()
    {
        return root_;
    }

    std::shared_ptr< IComponentRegistry > LifetimeScope::componentRegistry()
    {
        return componentRegistry_;
    }

    std::shared_ptr< void > LifetimeScope::resolveComponent(std::shared_ptr< IComponentRegistration > registration)
    {
        if (registration == nullptr)
            throw std::invalid_argument("registration");

        {
            boost::lock_guard< decltype(mutex_) > lock(mutex_);

            auto operation = std::make_shared< ResolveOperation >(shared_from_this());
            return operation->execute(registration);
        }
    }

    std::shared_ptr< void > LifetimeScope::getOrCreateAndShare(const boost::uuids::uuid& id,
                                                               std::function< std::shared_ptr< void >() > creator)
    {
        boost::lock_guard< decltype(mutex_) > lock(mutex_);

        std::shared_ptr< void > result = nullptr;
        if (sharedInstances_.count(id) == 0)
        {
            result = creator();
            sharedInstances_[id] = result;
        }
        else
            result = sharedInstances_[id];

        return result;
    }

    void LifetimeScope::initialize()
    {
        auto sharedSelf = shared_from_this();
        root_ = parent_ != nullptr ? parent_ : sharedSelf;
        sharedInstances_[selfRegistrationId] = sharedSelf;
    }

} // namespace Hypodermic