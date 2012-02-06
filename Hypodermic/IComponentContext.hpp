#ifdef      HYPODERMIC_ICOMPONENT_CONTEXT_H_
# ifndef    HYPODERMIC_ICOMPONENT_CONTEXT_HPP_
#  define   HYPODERMIC_ICOMPONENT_CONTEXT_HPP_

# include <boost/foreach.hpp>

# include <Hypodermic/KeyedService.h>
# include <Hypodermic/NullptrWorkaround.h>
# include <Hypodermic/TypedService.h>


namespace Hypodermic
{
	template <class ServiceT>
	std::shared_ptr< ServiceT > IComponentContext::resolve()
	{
		return resolve< ServiceT >(typeid(ServiceT));
	}

	template <class ServiceT>
	std::shared_ptr< ServiceT > IComponentContext::resolve(const std::type_info& serviceTypeInfo)
	{
        auto service = std::make_shared< TypedService >(serviceTypeInfo);
		return resolveService< ServiceT >(service);
	}

	template <class ServiceT>
	std::shared_ptr< ServiceT > IComponentContext::resolveNamed(const std::string& serviceName)
	{
        auto service = std::make_shared< KeyedService >(serviceName, typeid(ServiceT));
		return resolveService< ServiceT >(service);
	}

    template <class ServiceT>
    std::vector< std::shared_ptr< ServiceT > > IComponentContext::resolveAll()
    {
        return resolveAll< ServiceT >(typeid(ServiceT));
    }

	template <class ServiceT>
	std::vector< std::shared_ptr< ServiceT > > IComponentContext::resolveAll(const std::type_info& serviceTypeInfo)
	{
        auto service = std::make_shared< TypedService >(serviceTypeInfo);
		return resolveAllForService< ServiceT >(service);
	}

    template <class ServiceT>
    std::shared_ptr< ServiceT > IComponentContext::resolveService(std::shared_ptr< Service > service)
    {
        std::shared_ptr< IComponentRegistration > registration = componentRegistry()->getRegistration(service);

        if (registration == nullptr)
            return std::shared_ptr< ServiceT >();

        std::shared_ptr< void > result = registration->castOrForward(service->typeInfo(), resolveComponent(registration));

        return std::static_pointer_cast< ServiceT >(result);
    }

	template <class ServiceT>
	std::vector< std::shared_ptr< ServiceT > > IComponentContext::resolveAllForService(std::shared_ptr< Service > service)
	{
		auto registrations = componentRegistry()->registrationsFor(service);
			
        std::vector< std::shared_ptr< ServiceT > > allResults;

        BOOST_FOREACH(auto registration, registrations)
        {
            std::shared_ptr< void > result = registration->castOrForward(service->typeInfo(),
                                                                         resolveComponent(registration));
                
            allResults.push_back(std::static_pointer_cast< ServiceT >(result));
        }

        return allResults;
	}

} // namespace Hypodermic


# endif /* !HYPODERMIC_ICOMPONENT_CONTEXT_HPP_ */
#endif  /* HYPODERMIC_ICOMPONENT_CONTEXT_H_ */