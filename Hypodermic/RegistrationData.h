#ifndef		HYPODERMIC_REGISTRATION_DATA_H_
# define	HYPODERMIC_REGISTRATION_DATA_H_

# include <vector>

# include <Hypodermic/InstanceOwnership.h>
# include <Hypodermic/InstanceSharing.h>
# include <Hypodermic/Service.h>


namespace Hypodermic
{
    class IComponentLifetime;


	class RegistrationData
	{
	public:
		RegistrationData(Service* defaultService);

		InstanceSharing::Mode sharing() const;

		void sharing(InstanceSharing::Mode value);

        InstanceOwnership::Mode ownership() const;

        void ownership(InstanceOwnership::Mode value);

        IComponentLifetime* lifetime();

        void lifetime(IComponentLifetime* value);

		std::vector< Service* > services();

		void addService(Service* service);

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