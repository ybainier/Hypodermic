#ifndef		HYPODERMIC_FACTORY_DELEGATE_ACTIVATOR_H_
# define	HYPODERMIC_FACTORY_DELEGATE_ACTIVATOR_H_

# include <functional>
# include <memory>
# include <typeinfo>

# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IInstanceActivator.h>
# include <Hypodermic/InstanceActivator.h>


namespace Hypodermic
{

	template <class T>
    class FactoryDelegateActivator : public InstanceActivator
	{
        typedef std::function< std::shared_ptr< T >(IComponentContext&) > ActivationDelegate;

	public:
        FactoryDelegateActivator(const std::type_info& typeInfo, ActivationDelegate activationFunction);

		std::shared_ptr< void > activateInstance(std::shared_ptr< IComponentContext > context);

	private:
		ActivationDelegate activationFunction_;
	};

} // namespace Hypodermic


# include <Hypodermic/FactoryDelegateActivator.hpp>

#endif /* !HYPODERMIC_FACTORY_DELEGATE_ACTIVATOR_H_ */