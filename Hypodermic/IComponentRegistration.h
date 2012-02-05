#ifndef		HYPODERMIC_ICOMPONENT_REGISTRATION_H_
# define	HYPODERMIC_ICOMPONENT_REGISTRATION_H_

# include <memory>
# include <ostream>
# include <string>
# include <vector>

# include <boost/uuid/uuid.hpp>

# include <Hypodermic/InstanceOwnership.h>
# include <Hypodermic/InstanceSharing.h>
# include <Hypodermic/ITypeCaster.h>
# include <Hypodermic/Service.h>


namespace Hypodermic
{
    class IInstanceActivator;
    class IComponentLifetime;
    class IComponentRegistration;


	class IComponentRegistration
	{
	public:
        virtual ~IComponentRegistration() {}

		virtual std::shared_ptr< IInstanceActivator > activator() = 0;
        
        virtual std::shared_ptr< IComponentLifetime > lifetime() = 0;

        virtual std::shared_ptr< IComponentRegistration > target() = 0;

        virtual InstanceSharing::Mode sharing() = 0;

        virtual InstanceOwnership::Mode ownership() = 0;

        virtual std::vector< std::shared_ptr< Service > >& services() = 0;

        virtual std::shared_ptr< void > castOrForward(const std::type_info& typeInfo, std::shared_ptr< void > instance) = 0;

        virtual const boost::uuids::uuid& id() = 0;

        virtual std::string toString() = 0;
	};


    inline std::ostream& operator<<(std::ostream& os, IComponentRegistration& rhs)
    {
        return os << rhs.toString();
    }

} // namespace Hypodermic


#endif /* !HYPODERMIC_ICOMPONENT_REGISTRATION_H_ */