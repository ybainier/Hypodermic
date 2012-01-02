#ifndef		HYPODERMIC_ICOMPONENT_CONTEXT_H_
# define	HYPODERMIC_ICOMPONENT_CONTEXT_H_

# include <typeinfo>

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
		TService resolve()
		{
			return resolve< TService >(typeid(TService));
		}

		template <class TService>
		TService resolve(const std::type_info& serviceTypeInfo)
		{
			return resolveService< TService >(new TypedService(serviceTypeInfo));
		}

		template <class TService>
		TService resolveService(Service* service)
		{
			IComponentRegistration* registration = componentRegistry()->getRegistration(service);
			
			if (registration == nullptr)
				return 0;
			
            void* result = resolveComponent(registration);
            
            BOOST_FOREACH(auto& typeCaster, registration->typeCasters())
            {
                if (typeCaster->typeInfo() == service->serviceTypeInfo())
                    result = typeCaster->cast(result);
            }

			return static_cast< TService >(result);
		}
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_ICOMPONENT_CONTEXT_H_ */