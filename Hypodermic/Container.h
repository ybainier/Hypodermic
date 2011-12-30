#ifndef		HYPODERMIC_CONTAINER_H_
# define	HYPODERMIC_CONTAINER_H_

# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IContainer.h>


namespace Hypodermic
{
    class IComponentRegistration;
    class IComponentRegistry;
    class ILifetimeScope;


	class Container : public IContainer, public IComponentContext
	{
	public:
		Container();

		IComponentRegistry* componentRegistry();

		void* resolveComponent(IComponentRegistration* registration);

		void* getOrCreateInstance(IComponentRegistration* registration);

	private:
		IComponentRegistry* componentRegistry_;
        ILifetimeScope* rootLifetimeScope_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_CONTAINER_H_ */