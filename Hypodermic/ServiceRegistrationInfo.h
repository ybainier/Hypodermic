#ifndef		HYPODERMIC_SERVICE_REGISTRATION_INFO_H_
# define	HYPODERMIC_SERVICE_REGISTRATION_INFO_H_

# include <deque>
# include <memory>
# include <vector>

# include <Hypodermic/IComponentRegistration.h>


namespace Hypodermic
{
    class Service;
    class IRegistrationSource;


	class ServiceRegistrationInfo
	{
	public:
		ServiceRegistrationInfo(std::shared_ptr< Service > service);

		std::vector< std::shared_ptr< IComponentRegistration > >& implementations();

		void addImplementation(std::shared_ptr< IComponentRegistration > registration);

		std::shared_ptr< IComponentRegistration > getRegistration();

        bool isInitialized() const;
        bool isInitializing() const;

		bool isRegistered();
        void completeInitialization();

        void include(std::shared_ptr< IRegistrationSource > source);

        void beginInitialization(const std::deque< std::shared_ptr< IRegistrationSource > >& sources);
        
        bool hasSourcesToQuery() const;

        std::shared_ptr< IRegistrationSource > dequeueNextSource();
        void skipSource(std::shared_ptr< IRegistrationSource > source);

	private:
		std::shared_ptr< Service > service_;
		bool isInitialized_;
		std::vector< std::shared_ptr< IComponentRegistration > > implementations_;
        std::deque< std::shared_ptr< IRegistrationSource > > sourcesToQuery_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_SERVICE_REGISTRATION_INFO_H_ */