#include "LifetimeScope.h"

#include <stdexcept>
#include <vector>

#include <boost/uuid/random_generator.hpp>

#include "CopyOnWriteRegistry.h"
#include "ExternalRegistrySource.h"
#include "ResolveOperation.h"


namespace Hypodermic
{
	const boost::uuids::uuid LifetimeScope::selfRegistrationId = boost::uuids::random_generator()();
	std::function< void(ContainerBuilder&) > LifetimeScope::noConfiguration_ = [](ContainerBuilder&) -> void {};


    LifetimeScope::LifetimeScope(std::shared_ptr< IComponentRegistry > componentRegistry)
        : componentRegistry_(componentRegistry)
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
        return root_.lock();
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

        std::shared_ptr< void > result;
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
        auto sharedSelf = this->shared_from_this();
        root_ = parent_ != nullptr ? parent_ : std::static_pointer_cast< ISharingLifetimeScope >(sharedSelf);
    }

    std::shared_ptr< ILifetimeScope > LifetimeScope::createLifetimeScope()
    {
        auto registry = std::make_shared< CopyOnWriteRegistry >(componentRegistry_, [this]() { return this->createScopeRestrictedRegistry(noConfiguration_); });
        auto scope = std::shared_ptr< LifetimeScope >(new LifetimeScope(registry, this->shared_from_this()));

        return std::shared_ptr< ILifetimeScope >(scope);
    }

    std::shared_ptr< ScopeRestrictedRegistry > LifetimeScope::createScopeRestrictedRegistry(std::function< void(ContainerBuilder&) > configurationAction)
    {
        ContainerBuilder builder;

        std::vector< std::shared_ptr< IRegistrationSource > > parents;

        decltype(parentLifetimeScope()) s = this->shared_from_this();
        while (s != nullptr)
        {
            if (s->componentRegistry()->hasLocalComponents())
            {
                auto source = std::make_shared< ExternalRegistrySource >(s->componentRegistry());
                parents.push_back(source);
            }

            s = s->parentLifetimeScope();
        }

        std::for_each(parents.begin(), parents.end(),
            [&builder](std::shared_ptr< IRegistrationSource > external)
            {
                builder.registerSource(external);
            });

        configurationAction(builder);

        auto registry = std::make_shared< ScopeRestrictedRegistry >();
        builder.build(registry);
        return registry;
    }

} // namespace Hypodermic