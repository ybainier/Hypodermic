#ifndef		REGISTRATION_DATA_H_
# define	REGISTRATION_DATA_H_

# include <vector>

# include <Hypodermic/Service.h>


namespace Hypodermic
{

	class RegistrationData
	{
	public:
		RegistrationData(Service* defaultService)
			: defaultService_(defaultService), defaultServiceOverriden_(false)
		{
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
		std::vector< Service* > services_;
	};

} // namespace Hypodermic


#endif /* !REGISTRATION_DATA_H_ */