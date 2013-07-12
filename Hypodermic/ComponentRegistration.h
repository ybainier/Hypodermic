#ifndef		HYPODERMIC_COMPONENT_REGISTRATION_H_
# define	HYPODERMIC_COMPONENT_REGISTRATION_H_

# include <memory>
# include <unordered_map>
# include <vector>

# include <boost/uuid/uuid.hpp>

# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IComponentRegistration.h>
# include <Hypodermic/InstanceOwnership.h>
# include <Hypodermic/InstanceSharing.h>
# include <Hypodermic/Service.h>
# include <Hypodermic/TypeIndexWorkaround.h>


namespace Hypodermic
{
    class IComponentLifetime;
    class IInstanceActivator;


	class ComponentRegistration : public std::enable_shared_from_this< ComponentRegistration >, public IComponentRegistration
	{
	public:
        ComponentRegistration(const boost::uuids::uuid& id,
                              std::shared_ptr< IInstanceActivator > activator,
                              std::shared_ptr< IComponentLifetime > lifetime,
                              InstanceSharing::Mode sharing,
                              InstanceOwnership::Mode ownership,
                              const std::vector< std::shared_ptr< Service > >& services,
                              const std::unordered_map< std::type_index,
                              std::shared_ptr< ITypeCaster > >& typeCasters);

        ComponentRegistration(const boost::uuids::uuid& id,
                              std::shared_ptr< IInstanceActivator > activator,
                              std::shared_ptr< IComponentLifetime > lifetime,
                              InstanceSharing::Mode sharing,
                              InstanceOwnership::Mode ownership,
                              const std::vector< std::shared_ptr< Service > >& services,
                              const std::unordered_map< std::type_index,
                              std::shared_ptr< ITypeCaster > >& typeCasters,
                              std::shared_ptr< IComponentRegistration > target);

        std::shared_ptr< IComponentRegistration > target();

        const boost::uuids::uuid& id();

        std::shared_ptr< IInstanceActivator > activator();

        std::shared_ptr< IComponentLifetime > lifetime();

        InstanceSharing::Mode sharing();

        InstanceOwnership::Mode ownership();

        std::vector< std::shared_ptr< Service > >& services();

        std::shared_ptr< void > castOrForward(const std::type_info& typeInfo, std::shared_ptr< void > instance);
        std::shared_ptr< ITypeCaster > getTypeCaster(const std::type_info& typeInfo);

        std::string toString();

        Preparing& preparing();

        void raisePreparing(std::shared_ptr< IComponentContext > componentContext);

        Activating& activating();

        void raiseActivating(std::shared_ptr< IComponentContext > componentContext, std::shared_ptr< void >& instance);

        Activated& activated();

        void raiseActivated(std::shared_ptr< IComponentContext > componentContext, std::shared_ptr< void > instance);

	private:
        boost::uuids::uuid id_;
        std::shared_ptr< IInstanceActivator > activator_;
        InstanceSharing::Mode sharing_;
        InstanceOwnership::Mode ownership_;
        std::shared_ptr< IComponentLifetime > lifetime_;
        std::vector< std::shared_ptr< Service > > services_;
        std::unordered_map< std::type_index, std::shared_ptr< ITypeCaster > > typeCasters_;
        std::shared_ptr< IComponentRegistration > target_;
        Preparing preparingSignal_;
        Activating activatingSignal_;
        Activated activatedSignal_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_COMPONENT_REGISTRATION_H_ */