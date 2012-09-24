#ifndef		HYPODERMIC_REGISTRATION_DATA_H_
# define	HYPODERMIC_REGISTRATION_DATA_H_

# include <functional>
# include <memory>
# include <vector>

# include <Hypodermic/ActivatedData.h>
# include <Hypodermic/ActivatingData.h>
# include <Hypodermic/InstanceOwnership.h>
# include <Hypodermic/InstanceSharing.h>
# include <Hypodermic/PreparingData.h>
# include <Hypodermic/Service.h>


namespace Hypodermic
{
    class IComponentContext;
    class IComponentLifetime;


	class RegistrationData
	{
	public:
        typedef std::vector< std::function< void(PreparingData&) > > PreparingCallbacks;
        typedef std::vector< std::function< void(ActivatingData< void >&) > > ActivatingCallbacks;
        typedef std::vector< std::function< void(ActivatedData< void >&) > > ActivatedCallbacks;

		RegistrationData(std::shared_ptr< Service > defaultService);

		InstanceSharing::Mode sharing() const;

		void sharing(InstanceSharing::Mode value);

        InstanceOwnership::Mode ownership() const;

        void ownership(InstanceOwnership::Mode value);

        std::shared_ptr< IComponentLifetime > lifetime();

        void lifetime(std::shared_ptr< IComponentLifetime > value);

		std::vector< std::shared_ptr< Service > > services();

		void addService(std::shared_ptr< Service > service);

        PreparingCallbacks& preparingCallbacks();
        ActivatingCallbacks& activatingCallbacks();
        ActivatedCallbacks& activatedCallbacks();

	private:
		std::shared_ptr< Service > defaultService_;
		bool defaultServiceOverriden_;
		InstanceSharing::Mode sharing_;
        InstanceOwnership::Mode ownership_;
        std::shared_ptr< IComponentLifetime > lifetime_;
		std::vector< std::shared_ptr< Service > > services_;
        PreparingCallbacks preparingCallbacks_;
        ActivatingCallbacks activatingCallbacks_;
        ActivatedCallbacks activatedCallbacks_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_REGISTRATION_DATA_H_ */