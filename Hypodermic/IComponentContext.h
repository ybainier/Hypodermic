#ifndef		HYPODERMIC_ICOMPONENT_CONTEXT_H_
# define	HYPODERMIC_ICOMPONENT_CONTEXT_H_

# include <memory>
# include <typeinfo>
# include <vector>

# include <Hypodermic/IComponentRegistration.h>
# include <Hypodermic/IComponentRegistry.h>
# include <Hypodermic/Service.h>


namespace Hypodermic
{

    class IComponentContext
    {
    public:
        virtual ~IComponentContext() {}

        virtual std::shared_ptr< IComponentRegistry > componentRegistry() = 0;

        virtual std::shared_ptr< void > resolveComponent(std::shared_ptr< IComponentRegistration > registration) = 0;

        template <class ServiceT>
        std::shared_ptr< ServiceT > resolve();

        template <class ServiceT>
        std::shared_ptr< ServiceT > resolve(const std::type_info& serviceTypeInfo);

        template <class ServiceT>
        std::shared_ptr< ServiceT > resolveNamed(const std::string& serviceName);

        template <class ServiceT>
        std::vector< std::shared_ptr< ServiceT > > resolveAll();

        template <class ServiceT>
        std::vector< std::shared_ptr< ServiceT > > resolveAll(const std::type_info& serviceTypeInfo);

    private:
        template <class ServiceT>
        std::shared_ptr< ServiceT > resolveService(std::shared_ptr< Service > service);

        template <class ServiceT>
        std::vector< std::shared_ptr< ServiceT > > resolveAllForService(std::shared_ptr< Service > service);
    };

} // namespace Hypodermic


# include <Hypodermic/IComponentContext.hpp>

#endif /* !HYPODERMIC_ICOMPONENT_CONTEXT_H_ */