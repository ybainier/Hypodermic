#ifndef		HYPODERMIC_ICONTAINER_H_
# define	HYPODERMIC_ICONTAINER_H_

# include <Hypodermic/ILifetimeScope.h>


namespace Hypodermic
{

	class IContainer : public ILifetimeScope
	{
    public:
        virtual ~IContainer() {}
	};

} // namespace Hypodermic


#endif /* !HYPODERMIC_ICONTAINER_H_ */