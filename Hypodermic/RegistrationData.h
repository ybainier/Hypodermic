#ifndef		HYPODERMIC_REGISTRATION_DATA_H_
# define	HYPODERMIC_REGISTRATION_DATA_H_

# include <vector>

# include <Hypodermic/CurrentLifetimeScope.h>
# include <Hypodermic/InstanceOwnership.h>
# include <Hypodermic/InstanceSharing.h>
# include <Hypodermic/Service.h>


namespace Hypodermic
{

	class RegistrationData
	{
	public:
		RegistrationData(Service* defaultService)
			: defaultService_(defaultService)
            , defaultServiceOverriden_(false)
			, sharing_(InstanceSharing::None)
            , ownership_(InstanceOwnership::OwnedByLifetimeScope)
            , lifetime_(new CurrentLifetimeScope)
		{
		}

		InstanceSharing::Mode sharing() const
		{
			return sharing_;
		}

		void sharing(InstanceSharing::Mode value)
		{
			sharing_ = value;
		}

        InstanceOwnership::Mode ownership() const
        {
            return ownership_;
        }

        void ownership(InstanceOwnership::Mode value)
        {
            ownership_ = value;
        }

        IComponentLifetime* lifetime()
        {
            return lifetime_;
        }

        void lifetime(IComponentLifetime* value)
        {
            BOOST_ASSERT(value != nullptr);
            lifetime_ = value;
        }

		std::vector< Service* > services()
		{
			if (defaultServiceOverriden_)
				return services_;

			return std::vector< Service* >(1, defaultService_);
		}

		void addService(Service* service)
		{
			defaultServiceOverriden_ = true;
			services_.push_back(service);
		}

	private:
		Service* defaultService_;
		bool defaultServiceOverriden_;
		InstanceSharing::Mode sharing_;
        InstanceOwnership::Mode ownership_;
        IComponentLifetime* lifetime_;
		std::vector< Service* > services_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_REGISTRATION_DATA_H_ */