#ifndef		HYPODERMIC_ILIFETIME_SCOPE_H_
# define	HYPODERMIC_ILIFETIME_SCOPE_H_

# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IInitializable.h>


namespace Hypodermic
{

	class ILifetimeScope : public IComponentContext, public IInitializable
	{
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_ILIFETIME_SCOPE_H_ */