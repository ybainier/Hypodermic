#ifndef		HYPODERMIC_CONTAINER_H_
# define	HYPODERMIC_CONTAINER_H_

# include <memory>
# include <Hypodermic/IContainer.h>


namespace Hypodermic
{
    class IComponentRegistration;
    class IComponentRegistry;


	class Container : public std::enable_shared_from_this< Container >, public IContainer
	{
	public:
		std::shared_ptr< IComponentRegistry > componentRegistry();

		std::shared_ptr< void > resolveComponent(std::shared_ptr< IComponentRegistration > registration);

		std::shared_ptr< void > getOrCreateInstance(std::shared_ptr< IComponentRegistration > registration);

        void initialize();

        std::shared_ptr< ILifetimeScope > createLifetimeScope();

	private:
		std::shared_ptr< IComponentRegistry > componentRegistry_;
        std::shared_ptr< ILifetimeScope > rootLifetimeScope_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_CONTAINER_H_ */