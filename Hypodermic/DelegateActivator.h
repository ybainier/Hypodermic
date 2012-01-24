#ifndef		HYPODERMIC_DELEGATE_ACTIVATOR_H_
# define	HYPODERMIC_DELEGATE_ACTIVATOR_H_

# include <functional>
# include <memory>
# include <typeinfo>

# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IInstanceActivator.h>
# include <Hypodermic/InstanceActivator.h>


namespace Hypodermic
{

	template <class T>
	class DelegateActivator : public InstanceActivator
	{
        typedef std::function< T*(IComponentContext&) > ActivationDelegate;

	public:
		DelegateActivator(const std::type_info& typeInfo, ActivationDelegate activationFunction)
			: InstanceActivator(typeInfo), activationFunction_(activationFunction)
		{
		}

		std::shared_ptr< void > activateInstance(std::shared_ptr< IComponentContext > context)
		{
			return std::shared_ptr< void >(activationFunction_(*context));
		}

	private:
		ActivationDelegate activationFunction_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_DELEGATE_ACTIVATOR_H_ */