#ifndef		HYPODERMIC_COMPONENT_REGISTRATION_LIFETIME_DECORATOR_H_
# define	HYPODERMIC_COMPONENT_REGISTRATION_LIFETIME_DECORATOR_H_

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


	class ComponentRegistrationLifetimeDecorator : public std::enable_shared_from_this< ComponentRegistrationLifetimeDecorator >, public IComponentRegistration
	{
	public:
        ComponentRegistrationLifetimeDecorator(std::shared_ptr< IComponentRegistration > inner, std::shared_ptr< IComponentLifetime > lifetime);

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
        std::shared_ptr< IComponentLifetime > lifetime_;
        std::shared_ptr< IComponentRegistration > inner_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_COMPONENT_REGISTRATION_LIFETIME_DECORATOR_H_ */