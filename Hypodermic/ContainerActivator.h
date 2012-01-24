#ifndef		HYPODERMIC_CONTAINER_ACTIVATOR_H_
# define	HYPODERMIC_CONTAINER_ACTIVATOR_H_

# include <memory>

# include <Hypodermic/IContainer.h>
# include <Hypodermic/InstanceActivator.h>


namespace Hypodermic
{

	class ContainerActivator : public InstanceActivator
	{
	public:
		ContainerActivator(std::shared_ptr< IContainer > instance);

		std::shared_ptr< void > activateInstance(std::shared_ptr< IComponentContext > context);

	private:
		std::weak_ptr< IContainer > instance_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_CONTAINER_ACTIVATOR_H_ */