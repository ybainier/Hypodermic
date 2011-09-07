#ifndef		DELEGATE_ACTIVATOR_H_
# define	DELEGATE_ACTIVATOR_H_

# include <typeinfo>

# include <Hypodermic/Func.h>
# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IInstanceActivator.h>
# include <Hypodermic/InstanceActivator.h>


namespace Hypodermic
{

	template <class T>
	class DelegateActivator : public InstanceActivator
	{
	public:
		DelegateActivator(const std::type_info& typeInfo, Func< IComponentContext*, T > activationFunction)
			: InstanceActivator(typeInfo), activationFunction_(activationFunction)
		{
		}

		void* activateInstance(IComponentContext* context)
		{
			return activationFunction_(context);
		}

	private:
		Func< IComponentContext*, T > activationFunction_;
	};

} // namespace Hypodermic



#endif /* !DELEGATE_ACTIVATOR_H_ */