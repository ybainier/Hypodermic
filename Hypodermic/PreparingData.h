#ifndef     HYPODERMIC_PREPARING_DATA_H_
# define    HYPODERMIC_PREPARING_DATA_H_

# include <memory>
# include <stdexcept>

# include <Hypodermic/IPreparingData.h>
# include <Hypodermic/NullptrWorkaround.h>


namespace Hypodermic
{

    class PreparingData : public IPreparingData
    {
    public:
        PreparingData(std::shared_ptr< IComponentContext > componentContext, std::shared_ptr< IComponentRegistration > componentRegistration)
            : componentContext_(componentContext)
            , componentRegistration_(componentRegistration)
        {
            if (componentContext == nullptr)
                std::invalid_argument("componentContext");
            if (componentRegistration == nullptr)
                std::invalid_argument("componentRegistration");
        }

        std::shared_ptr< IComponentContext > componentContext() { return componentContext_; }

        std::shared_ptr< IComponentRegistration > componentRegistration() { return componentRegistration_; }

    private:
        std::shared_ptr< IComponentContext > componentContext_;
        std::shared_ptr< IComponentRegistration > componentRegistration_;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_PREPARING_DATA_H_ */