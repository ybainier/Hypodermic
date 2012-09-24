#ifndef     HYPODERMIC_IACTIVATED_DATA_H_
# define    HYPODERMIC_IACTIVATED_DATA_H_

# include <memory>

# include <Hypodermic/IComponentContext.h>
# include <Hypodermic/IComponentRegistration.h>


namespace Hypodermic
{

    template <class T>
    class IActivatedData
    {
    public:
        virtual ~IActivatedData() {}

        virtual std::shared_ptr< IComponentContext > componentContext() = 0;

        virtual std::shared_ptr< IComponentRegistration > componentRegistration() = 0;

        virtual std::shared_ptr< T > instance() = 0;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_IACTIVATED_DATA_H_ */