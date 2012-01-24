#ifndef		HYPODERMIC_PROVIDED_INSTANCE_ACTIVATOR_H_
# define	HYPODERMIC_PROVIDED_INSTANCE_ACTIVATOR_H_

# include <memory>
# include <Hypodermic/InstanceActivator.h>


namespace Hypodermic
{

	template <class T>
	class ProvidedInstanceActivator : public InstanceActivator
	{
	public:
		ProvidedInstanceActivator(std::shared_ptr< T > instance);
		~ProvidedInstanceActivator();

		std::shared_ptr< void > activateInstance(std::shared_ptr< IComponentContext > context);

		bool disposeInstance() const;

		void disposeInstance(bool value);

	private:
		std::shared_ptr< T > instance_;
		bool activated_;
		bool disposeInstance_;
	};

} // namespace Hypodermic


# include <Hypodermic/ProvidedInstanceActivator.hpp>

#endif /* !HYPODERMIC_PROVIDED_INSTANCE_ACTIVATOR_H_ */