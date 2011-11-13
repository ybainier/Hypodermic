#ifndef     HYPODERMIC_RESOLVE_OPERATION_H_
# define    HYPODERMIC_RESOLVE_OPERATION_H_

# include <stack>

# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IResolveOperation.h>


namespace Hypodermic
{

    class ResolveOperation : public IComponentContext, public IResolveOperation
    {
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_RESOLVE_OPERATION_H_ */