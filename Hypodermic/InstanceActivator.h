#ifndef		HYPODERMIC_INSTANCE_ACTIVATOR_H_
# define	HYPODERMIC_INSTANCE_ACTIVATOR_H_

# include <typeinfo>

# include <Hypodermic/IInstanceActivator.h>


namespace Hypodermic
{

	class InstanceActivator : public IInstanceActivator
	{
	public:
		InstanceActivator(const std::type_info& typeInfo);
        virtual ~InstanceActivator() = 0;

		const std::type_info& typeInfo();

	private:
		const std::type_info& typeInfo_;
	};

    inline InstanceActivator::~InstanceActivator() {}

} // namespace Hypodermic


#endif /* !HYPODERMIC_INSTANCE_ACTIVATOR_H_ */