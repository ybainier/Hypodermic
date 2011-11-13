#ifndef		HYPODERMIC_INSTANCE_ACTIVATOR_H_
# define	HYPODERMIC_INSTANCE_ACTIVATOR_H_

# include <typeinfo>

# include <Hypodermic/IInstanceActivator.h>


namespace Hypodermic
{

	class InstanceActivator : public IInstanceActivator
	{
	public:
		InstanceActivator(const std::type_info& typeInfo)
			: typeInfo_(typeInfo)
		{
		}

		const std::type_info& typeInfo()
		{
			return typeInfo_;
		}

	private:
		const std::type_info& typeInfo_;
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_INSTANCE_ACTIVATOR_H_ */