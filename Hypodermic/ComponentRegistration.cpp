#include <boost/assert.hpp>

#include "IComponentLifetime.h"
#include "IInstanceActivator.h"
#include "ComponentRegistration.h"


namespace Hypodermic
{

    ComponentRegistration::ComponentRegistration(const boost::uuids::uuid& id,
                                                 IInstanceActivator* activator,
                                                 IComponentLifetime* lifetime,
                                                 InstanceSharing::Mode sharing,
                                                 InstanceOwnership::Mode ownership,
                                                 const std::vector< Service* >& services,
                                                 const std::vector< ITypeCaster* >& typeCasters)
        : id_(id)
        , activator_(activator)
        , sharing_(sharing)
        , ownership_(ownership)
        , lifetime_(lifetime)
        , services_(services)
        , typeCasters_(typeCasters)
        , target_(nullptr)
    {
        BOOST_ASSERT(activator != nullptr);
        BOOST_ASSERT(lifetime != nullptr);
    }

    ComponentRegistration::ComponentRegistration(const boost::uuids::uuid& id,
                                                 IInstanceActivator* activator,
                                                 IComponentLifetime* lifetime,
                                                 InstanceSharing::Mode sharing,
                                                 InstanceOwnership::Mode ownership,
                                                 const std::vector< Service* >& services,
                                                 const std::vector< ITypeCaster* >& typeCasters,
                                                 IComponentRegistration* target)
        : id_(id)
        , activator_(activator)
        , sharing_(sharing)
        , ownership_(ownership)
        , lifetime_(lifetime)
        , services_(services)
        , typeCasters_(typeCasters)
        , target_(target)
    {
        BOOST_ASSERT(activator != nullptr);
        BOOST_ASSERT(lifetime != nullptr);
        BOOST_ASSERT(target != nullptr);
    }

    IComponentRegistration* ComponentRegistration::target()
    {
        return target_ != nullptr ? target_ : this;
    }

    const boost::uuids::uuid& ComponentRegistration::id()
    {
        return id_;
    }

    IInstanceActivator* ComponentRegistration::activator()
    {
        return activator_;
    }

    IComponentLifetime* ComponentRegistration::lifetime()
    {
        return lifetime_;
    }

    InstanceSharing::Mode ComponentRegistration::sharing()
    {
        return sharing_;
    }

    InstanceOwnership::Mode ComponentRegistration::ownership()
    {
        return ownership_;
    }

    std::vector< Service* >& ComponentRegistration::services()
    {
        return services_;
    }

    const std::vector< ITypeCaster* >& ComponentRegistration::typeCasters() const
    {
        return typeCasters_;
    }

    void ComponentRegistration::raisePreparing(IComponentContext* context)
    {
        //var args = new PreparingEventArgs(context, this);
        //Preparing(this, args);
    }

    void ComponentRegistration::raiseActivating(IComponentContext* context)
    {
        //var args = new ActivatingEventArgs<object>(context, this, instance);
        //Activating(this, args);
        //instance = args.Instance;
    }

    void ComponentRegistration::raiseActivated(IComponentContext* context)
    {
        //Activated(this, new ActivatedEventArgs<object>(context, this, instance));
    }

    std::string ComponentRegistration::toString()
    {
        return std::string("Component Registration");
    }

} // namespace Hypodermic
