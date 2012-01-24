#include <stdexcept>

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
                                                 const std::unordered_map< std::type_index, ITypeCaster* >& typeCasters)
        : id_(id)
        , activator_(activator)
        , sharing_(sharing)
        , ownership_(ownership)
        , lifetime_(lifetime)
        , services_(services)
        , typeCasters_(typeCasters)
        , target_(nullptr)
    {
        if (activator == nullptr)
            throw std::invalid_argument("activator");
        if (lifetime == nullptr)
            throw std::invalid_argument("lifetime");
    }

    ComponentRegistration::ComponentRegistration(const boost::uuids::uuid& id,
                                                 IInstanceActivator* activator,
                                                 IComponentLifetime* lifetime,
                                                 InstanceSharing::Mode sharing,
                                                 InstanceOwnership::Mode ownership,
                                                 const std::vector< Service* >& services,
                                                 const std::unordered_map< std::type_index, ITypeCaster* >& typeCasters,
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
        if (activator == nullptr)
            throw std::invalid_argument("activator");
        if (lifetime == nullptr)
            throw std::invalid_argument("lifetime");
        if (target == nullptr)
            throw std::invalid_argument("target");
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

    std::shared_ptr< void > ComponentRegistration::castOrForward(const std::type_info& typeInfo, std::shared_ptr< void > instance)
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
