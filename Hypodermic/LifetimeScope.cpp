#include <stdexcept>

#include <boost/assert.hpp>
#include <boost/uuid/random_generator.hpp>

#include "ResolveOperation.h"
#include "LifetimeScope.h"


namespace Hypodermic
{
	const boost::uuids::uuid LifetimeScope::selfRegistrationId = boost::uuids::random_generator()();
	Action< ContainerBuilder* > LifetimeScope::noConfiguration_ = [](ContainerBuilder*) -> void { };

    LifetimeScope::LifetimeScope(IComponentRegistry* componentRegistry)
    {
        initialize(componentRegistry);
    }

    LifetimeScope::LifetimeScope( IComponentRegistry* componentRegistry, LifetimeScope* parent )
    {
        initialize(componentRegistry);

        BOOST_ASSERT(parent != nullptr);
        parent_ = parent;
        root_ = parent_->rootLifetimeScope();
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

    void* LifetimeScope::resolveComponent( IComponentRegistration* registration )
    {
        if (registration == nullptr)
            throw std::invalid_argument("registration");

        {
            boost::lock_guard< decltype(mutex_) > lock(mutex_);

            ResolveOperation operation(this);
            //ResolveOperationBeginning(this, new ResolveOperationBeginningEventArgs(operation));
            return operation.execute(registration);
        }
    }

    void* LifetimeScope::getOrCreateAndShare( const boost::uuids::uuid& id, Func< void, void* > creator )
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

    void LifetimeScope::initialize()
    {
        sharedInstances_[selfRegistrationId] = this;
    }

    void LifetimeScope::initialize( IComponentRegistry* componentRegistry )
    {
        initialize();

        BOOST_ASSERT(componentRegistry != nullptr);
        componentRegistry_ = componentRegistry;
        root_ = this;
    }

} // namespace Hypodermic