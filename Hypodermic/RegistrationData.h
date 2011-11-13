#ifndef		HYPODERMIC_REGISTRATION_DATA_H_
# define	HYPODERMIC_REGISTRATION_DATA_H_

# include <vector>

# include <Hypodermic/InstanceSharing.h>
# include <Hypodermic/Service.h>


namespace Hypodermic
{

	class RegistrationData
	{
	public:
		RegistrationData(Service* defaultService)
			: defaultService_(defaultService), defaultServiceOverriden_(false)
			, sharing_(InstanceSharing::None)
		{
		}

		const InstanceSharing::Mode& sharing() const
		{
			return sharing_;
		}

		void sharing(InstanceSharing::Mode value)
		{
			sharing_ = value;
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
		std::vector< Service* > services_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_REGISTRATION_DATA_H_ */