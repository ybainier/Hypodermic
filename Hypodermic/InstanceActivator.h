#ifndef		INSTANCE_ACTIVATOR_H_
# define	INSTANCE_ACTIVATOR_H_

# include <typeinfo>
# include "IInstanceActivator.h"


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


#endif /* !INSTANCE_ACTIVATOR_H_ */