#ifndef		CONTAINER_H_
# define	CONTAINER_H_

# include <Hypodermic/ComponentRegistry.h>
# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IContainer.h>


namespace Hypodermic
{
	class IComponentRegistry;


	class Container : public IContainer, public IComponentContext
	{
	public:
		Container()
		{
			componentRegistry_ = new ComponentRegistry;
		}

		IComponentRegistry* componentRegistry()
		{
			return componentRegistry_;
		}

		void* resolveComponent(IComponentRegistration* registration)
		{
			return getOrCreateInstance(registration);
		}

		void* getOrCreateInstance(IComponentRegistration* registration)
		{
			auto i = registration->activator()->activateInstance(this);
			return i;
		}

	private:
		IComponentRegistry* componentRegistry_;
	};

} // namespace Hypodermic


#endif /* !CONTAINER_H_ */