#ifndef		HYPODERMIC_PROVIDED_INSTANCE_ACTIVATOR_H_
# define	HYPODERMIC_PROVIDED_INSTANCE_ACTIVATOR_H_

# include <Hypodermic/InstanceActivator.h>


namespace Hypodermic
{

	template <class T>
	class ProvidedInstanceActivator : public InstanceActivator
	{
	public:
		ProvidedInstanceActivator(T* instance);
		~ProvidedInstanceActivator();

		std::shared_ptr< void > activateInstance(IComponentContext* context);

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