#ifndef		HYPODERMIC_PROVIDED_INSTANCE_ACTIVATOR_H_
# define	HYPODERMIC_PROVIDED_INSTANCE_ACTIVATOR_H_

# include <stdexcept>

# include <Hypodermic/InstanceActivator.h>


namespace Hypodermic
{

	template <class T>
	class ProvidedInstanceActivator : public InstanceActivator
	{
	public:
		ProvidedInstanceActivator(T instance)
			: InstanceActivator(typeid(T)), instance_(instance)
			, activated_(false), disposeInstance_(false)
		{
		}

		~ProvidedInstanceActivator()
		{
			if (disposeInstance_)
			{
				// TODO: dispose or delete instance?
			}
		}

		void* activateInstance(IComponentContext* context)
		{
			if (activated_)
				throw std::logic_error("Instance already activated");

			activated_ = true;

			return instance_;
		}

		bool disposeInstance() const
		{
			return disposeInstance_;
		}

		void disposeInstance(bool value)
		{
			disposeInstance_ = value;
		}

	private:
		T instance_;
		bool activated_;
		bool disposeInstance_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_PROVIDED_INSTANCE_ACTIVATOR_H_ */