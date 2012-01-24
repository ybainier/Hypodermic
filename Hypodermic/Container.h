#ifndef		HYPODERMIC_CONTAINER_H_
# define	HYPODERMIC_CONTAINER_H_

# include <memory>
# include <Hypodermic/IContainer.h>


namespace Hypodermic
{
    class IComponentRegistration;
    class IComponentRegistry;


	class Container : public IContainer
	{
	public:
		Container();

		IComponentRegistry* componentRegistry();

		std::shared_ptr< void > resolveComponent(IComponentRegistration* registration);

		std::shared_ptr< void > getOrCreateInstance(IComponentRegistration* registration);

	private:
		IComponentRegistry* componentRegistry_;
        ILifetimeScope* rootLifetimeScope_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_CONTAINER_H_ */