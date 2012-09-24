#ifndef     HYPODERMIC_IPREPARING_DATA_H_
# define    HYPODERMIC_IPREPARING_DATA_H_

# include <memory>

# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IComponentRegistration.h>


namespace Hypodermic
{

    class IPreparingData
    {
    public:
        virtual ~IPreparingData() {}

        virtual std::shared_ptr< IComponentContext > componentContext() = 0;

        virtual std::shared_ptr< IComponentRegistration > componentRegistration() = 0;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_IPREPARING_DATA_H_ */