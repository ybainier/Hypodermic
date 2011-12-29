#ifndef		HYPODERMIC_INSTANCE_OWNERSHIP_H_
# define	HYPODERMIC_INSTANCE_OWNERSHIP_H_


namespace Hypodermic
{
namespace InstanceOwnership
{

    enum Mode
    {
        ExternallyOwned = 0,
        OwnedByLifetimeScope
    };

} // namespace InstanceOwnership
} // namespace Hypodermic


#endif /* !HYPODERMIC_INSTANCE_OWNERSHIP_H_ */