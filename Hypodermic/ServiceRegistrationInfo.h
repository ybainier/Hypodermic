#ifndef		HYPODERMIC_SERVICE_REGISTRATION_INFO_H_
# define	HYPODERMIC_SERVICE_REGISTRATION_INFO_H_

# include <memory>
# include <vector>

# include <Hypodermic/IComponentRegistration.h>


namespace Hypodermic
{
    class Service;


	class ServiceRegistrationInfo
	{
	public:
		ServiceRegistrationInfo(std::shared_ptr< Service > service);

		std::vector< IComponentRegistration* >& implementations();

		void addImplementation(IComponentRegistration* registration);

		IComponentRegistration* getRegistration();

		bool isRegistered();


	private:
		std::shared_ptr< Service > service_;
		bool isInitialized_;
		std::vector< IComponentRegistration* > implementations_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_SERVICE_REGISTRATION_INFO_H_ */