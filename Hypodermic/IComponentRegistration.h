#ifndef		HYPODERMIC_ICOMPONENT_REGISTRATION_H_
# define	HYPODERMIC_ICOMPONENT_REGISTRATION_H_

# include <memory>
# include <ostream>
# include <string>
# include <vector>

# include <boost/signals.hpp>
# include <boost/uuid/uuid.hpp>

# include <Hypodermic/InstanceOwnership.h>
# include <Hypodermic/InstanceSharing.h>
# include <Hypodermic/ITypeCaster.h>
# include <Hypodermic/Service.h>


namespace Hypodermic
{
    class PreparingData;
    template <class> class ActivatingData;
    template <class> class ActivatedData;

    class IInstanceActivator;
    class IComponentContext;
    class IComponentLifetime;


	class IComponentRegistration
	{
	public:
        typedef boost::signal< void(PreparingData&) > Preparing;
        typedef boost::signal< void(ActivatingData< void >&) > Activating;
        typedef boost::signal< void(ActivatedData< void >&) > Activated;

        virtual ~IComponentRegistration() {}

		virtual std::shared_ptr< IInstanceActivator > activator() = 0;
        
        virtual std::shared_ptr< IComponentLifetime > lifetime() = 0;

        virtual std::shared_ptr< IComponentRegistration > target() = 0;

        virtual InstanceSharing::Mode sharing() = 0;

        virtual InstanceOwnership::Mode ownership() = 0;

        virtual std::vector< std::shared_ptr< Service > >& services() = 0;

        virtual std::shared_ptr< void > castOrForward(const std::type_info& typeInfo, std::shared_ptr< void > instance) = 0;
        virtual std::shared_ptr< ITypeCaster > getTypeCaster(const std::type_info& typeInfo) = 0;

        virtual const boost::uuids::uuid& id() = 0;

        virtual std::string toString() = 0;

        virtual Preparing& preparing() = 0;

        virtual void raisePreparing(std::shared_ptr< IComponentContext > componentContext) = 0;

        virtual Activating& activating() = 0;

        virtual void raiseActivating(std::shared_ptr< IComponentContext > componentContext, std::shared_ptr< void >& instance) = 0;

        virtual Activated& activated() = 0;

        virtual void raiseActivated(std::shared_ptr< IComponentContext > componentContext, std::shared_ptr< void > instance) = 0;

        bool isAdapting() { return target().get() != this; }
	};

    inline std::ostream& operator<<(std::ostream& os, IComponentRegistration& rhs)
    {
        return os << rhs.toString();
    }

} // namespace Hypodermic


#endif /* !HYPODERMIC_ICOMPONENT_REGISTRATION_H_ */