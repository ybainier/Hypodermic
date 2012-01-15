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
                                                 const boost::unordered_map< std::type_index, ITypeCaster* >& typeCasters)
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
                                                 const boost::unordered_map< std::type_index, ITypeCaster* >& typeCasters,
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

    void* ComponentRegistration::castOrForward(const std::type_info& typeInfo, void* instance)
    {
        std::type_index typeIndex(typeInfo);
        if (instance == nullptr || typeCasters_.count(typeIndex) == 0)
            return instance;

        auto& typeCaster = typeCasters_[typeIndex];
        return typeCaster->cast(instance);
    }

    void ComponentRegistration::raisePreparing(IComponentContext* context)
    {
        //TODO
    }

    void ComponentRegistration::raiseActivating(IComponentContext* context)
    {
        //TODO
    }

    void ComponentRegistration::raiseActivated(IComponentContext* context)
    {
        //TODO
    }

    std::string ComponentRegistration::toString()
    {
        return std::string("Component Registration");
    }

} // namespace Hypodermic
