#include "ComponentRegistrationLifetimeDecorator.h"

#include <functional>
#include <stdexcept>

#include "ActivatedData.h"
#include "ActivatingData.h"
#include "IComponentLifetime.h"
#include "IInstanceActivator.h"
#include "NullptrWorkaround.h"
#include "PreparingData.h"


namespace Hypodermic
{

    ComponentRegistrationLifetimeDecorator::ComponentRegistrationLifetimeDecorator(std::shared_ptr< IComponentRegistration > inner, std::shared_ptr< IComponentLifetime > lifetime)
        : lifetime_(lifetime)
        , inner_(inner)
    {
        if (lifetime_ == nullptr)
            throw std::invalid_argument("lifetime");
        if (inner_ == nullptr)
            throw std::invalid_argument("inner");
    }

    std::shared_ptr< IComponentRegistration > ComponentRegistrationLifetimeDecorator::target()
    {
        if (inner_->isAdapting())
            return inner_->target();
            
        return this->shared_from_this();
    }

    const boost::uuids::uuid& ComponentRegistrationLifetimeDecorator::id()
    {
        return inner_->id();
    }

    std::shared_ptr< IInstanceActivator > ComponentRegistrationLifetimeDecorator::activator()
    {
        return inner_->activator();
    }

    std::shared_ptr< IComponentLifetime > ComponentRegistrationLifetimeDecorator::lifetime()
    {
        return lifetime_;
    }

    InstanceSharing::Mode ComponentRegistrationLifetimeDecorator::sharing()
    {
        return inner_->sharing();
    }

    InstanceOwnership::Mode ComponentRegistrationLifetimeDecorator::ownership()
    {
        return inner_->ownership();
    }

    std::vector< std::shared_ptr< Service > >& ComponentRegistrationLifetimeDecorator::services()
    {
        return inner_->services();
    }

    std::shared_ptr< void > ComponentRegistrationLifetimeDecorator::castOrForward(const std::type_info& typeInfo, std::shared_ptr< void > instance)
    {
        return inner_->castOrForward(typeInfo, instance);
    }

    std::string ComponentRegistrationLifetimeDecorator::toString()
    {
        return std::string("ComponentRegistrationLifetimeDecorator");
    }

    IComponentRegistration::Preparing& ComponentRegistrationLifetimeDecorator::preparing()
    {
        return inner_->preparing();
    }

    void ComponentRegistrationLifetimeDecorator::raisePreparing(std::shared_ptr< IComponentContext > componentContext)
    {
        inner_->raisePreparing(componentContext);
    }

    IComponentRegistration::Activating& ComponentRegistrationLifetimeDecorator::activating()
    {
        return inner_->activating();
    }

    void ComponentRegistrationLifetimeDecorator::raiseActivating(std::shared_ptr< IComponentContext > componentContext, std::shared_ptr< void >& instance)
    {
        inner_->raiseActivating(componentContext, instance);
    }

    IComponentRegistration::Activated& ComponentRegistrationLifetimeDecorator::activated()
    {
        return inner_->activated();
    }

    void ComponentRegistrationLifetimeDecorator::raiseActivated(std::shared_ptr< IComponentContext > componentContext, std::shared_ptr< void > instance)
    {
        inner_->raiseActivated(componentContext, instance);
    }

    std::shared_ptr< ITypeCaster > ComponentRegistrationLifetimeDecorator::getTypeCaster(const std::type_info& typeInfo)
    {
        return inner_->getTypeCaster(typeInfo);
    }

} // namespace Hypodermic