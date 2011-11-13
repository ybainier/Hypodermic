#ifndef		HYPODERMIC_SERVICE_REGISTRATION_INFO_H_
# define	HYPODERMIC_SERVICE_REGISTRATION_INFO_H_

# include <deque>

# include <Hypodermic/Service.h>


namespace Hypodermic
{

	class ServiceRegistrationInfo
	{
	public:
		ServiceRegistrationInfo(Service* service)
			: service_(service), isInitialized_(false)
		{
		}

		std::vector< IComponentRegistration* >& implementations()
		{
			return implementations_;
		}

		void addImplementation(IComponentRegistration* registration)
		{
			implementations_.push_back(registration);
		}

		IComponentRegistration* getRegistration()
		{
			if (implementations_.size() > 0)
				return implementations_[0];
			return nullptr;
		}

		bool isRegistered() 
		{
			return implementations_.size() > 0 && implementations_[0] != nullptr;
		}


	private:
		Service* service_;
		bool isInitialized_;
		std::vector< IComponentRegistration* > implementations_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_SERVICE_REGISTRATION_INFO_H_ */