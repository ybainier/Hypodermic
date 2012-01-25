#ifndef		HYPODERMIC_ICOMPONENT_CONTEXT_H_
# define	HYPODERMIC_ICOMPONENT_CONTEXT_H_

# include <memory>
# include <typeinfo>
# include <vector>

# include <boost/foreach.hpp>

# include <Hypodermic/IComponentRegistration.h>
# include <Hypodermic/IComponentRegistry.h>
# include <Hypodermic/NullptrWorkaround.h>
# include <Hypodermic/TypedService.h>


namespace Hypodermic
{
	class IComponentContext
	{
	public:
		virtual std::shared_ptr< IComponentRegistry > componentRegistry() = 0;

		virtual std::shared_ptr< void > resolveComponent(std::shared_ptr< IComponentRegistration > registration) = 0;

		template <class TService>
		std::shared_ptr< TService > resolve()
		{
			return resolve< TService >(typeid(TService));
		}

		template <class TService>
		std::shared_ptr< TService > resolve(const std::type_info& serviceTypeInfo)
		{
            auto service = std::make_shared< TypedService >(serviceTypeInfo);
			return resolveService< TService >(service);
		}

        template <class TService>
        std::vector< std::shared_ptr< TService > > resolveAll()
        {
            return resolveAll< TService >(typeid(TService));
        }

		template <class TService>
		std::vector< std::shared_ptr< TService > > resolveAll(const std::type_info& serviceTypeInfo)
		{
            auto service = std::make_shared< TypedService >(serviceTypeInfo);
			return resolveAllForService< TService >(service);
		}

    private:
        template <class TService>
        std::shared_ptr< TService > resolveService(std::shared_ptr< Service > service)
        {
            std::shared_ptr< IComponentRegistration > registration = componentRegistry()->getRegistration(service);

            if (registration == nullptr)
                return std::shared_ptr< TService >();

            std::shared_ptr< void > result = registration->castOrForward(service->typeInfo(), resolveComponent(registration));

            return std::static_pointer_cast< TService >(result);
        }

		template <class TService>
		std::vector< std::shared_ptr< TService > > resolveAllForService(std::shared_ptr< Service > service)
		{
			auto registrations = componentRegistry()->registrationsFor(service);
			
            std::vector< std::shared_ptr< TService > > allResults;

            BOOST_FOREACH(auto registration, registrations)
            {
                std::shared_ptr< void > result = registration->castOrForward(service->typeInfo(),
                                                                             resolveComponent(registration));
                
                allResults.push_back(std::static_pointer_cast< TService >(result));
            }

            return allResults;
		}
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_ICOMPONENT_CONTEXT_H_ */