#ifndef		HYPODERMIC_ICOMPONENT_CONTEXT_H_
# define	HYPODERMIC_ICOMPONENT_CONTEXT_H_

# include <typeinfo>
# include <vector>

# include <boost/foreach.hpp>

# include <Hypodermic/IComponentRegistration.h>
# include <Hypodermic/TypedService.h>


namespace Hypodermic
{
	class IComponentRegistry;


	class IComponentContext
	{
	public:
		virtual IComponentRegistry* componentRegistry() = 0;

		virtual void* resolveComponent(IComponentRegistration* registration) = 0;

		template <class TService>
		TService* resolve()
		{
			return resolve< TService >(typeid(TService));
		}

		template <class TService>
		TService* resolve(const std::type_info& serviceTypeInfo)
		{
			return resolveService< TService >(new TypedService(serviceTypeInfo));
		}

		template <class TService>
		TService* resolveService(Service* service)
		{
			IComponentRegistration* registration = componentRegistry()->getRegistration(service);
			
			if (registration == nullptr)
				return 0;
			
            void* result = registration->castOrForward(service->typeInfo(), resolveComponent(registration));

			return static_cast< TService* >(result);
		}

        template <class TService>
        std::vector< TService* > resolveAll()
        {
            return resolveAll< TService >(typeid(TService));
        }

		template <class TService>
		std::vector< TService* > resolveAll(const std::type_info& serviceTypeInfo)
		{
			return resolveAllForService< TService >(new TypedService(serviceTypeInfo));
		}

		template <class TService>
		std::vector< TService* > resolveAllForService(Service* service)
		{
			auto registrations = componentRegistry()->registrationsFor(service);
			
            std::vector< TService* > allResults;

            BOOST_FOREACH(auto registration, registrations)
            {
                void* result = registration->castOrForward(service->typeInfo(), resolveComponent(registration));
                allResults.push_back(static_cast< TService* >(result));
            }

            return allResults;
		}
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_ICOMPONENT_CONTEXT_H_ */