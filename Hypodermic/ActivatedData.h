#ifndef     HYPODERMIC_ACTIVATED_DATA_H_
# define    HYPODERMIC_ACTIVATED_DATA_H_

# include <memory>
# include <stdexcept>

# include <Hypodermic/IActivatedData.h>
# include <Hypodermic/NullptrWorkaround.h>


namespace Hypodermic
{

    template <class T>
    class ActivatedData : public IActivatedData< T >
    {
    public:
        ActivatedData(std::shared_ptr< IComponentContext > componentContext, std::shared_ptr< IComponentRegistration > componentRegistration,
                      std::shared_ptr< T > instance)
            : componentContext_(componentContext)
            , componentRegistration_(componentRegistration)
            , instance_(instance)
        {
            if (componentContext == nullptr)
                std::invalid_argument("componentContext");
            if (componentRegistration == nullptr)
                std::invalid_argument("componentRegistration");
            if (instance == nullptr)
                std::invalid_argument("instance");
        }

        std::shared_ptr< IComponentContext > componentContext() { return componentContext_; }

        std::shared_ptr< IComponentRegistration > componentRegistration() { return componentRegistration_; }

        std::shared_ptr< T > instance() { return instance_; }

    private:
        std::shared_ptr< IComponentContext > componentContext_;
        std::shared_ptr< IComponentRegistration > componentRegistration_;
        std::shared_ptr< T > instance_;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_ACTIVATED_DATA_H_ */