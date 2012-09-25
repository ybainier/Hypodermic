#ifndef     HYPODERMIC_ACTIVATING_DATA_H_
# define    HYPODERMIC_ACTIVATING_DATA_H_

# include <memory>
# include <stdexcept>

# include <Hypodermic/IActivatingData.h>
# include <Hypodermic/NullptrWorkaround.h>


namespace Hypodermic
{

    template <class T>
    class ActivatingData : public IActivatingData< T >
    {
    public:
        ActivatingData(std::shared_ptr< IComponentContext > componentContext, std::shared_ptr< IComponentRegistration > componentRegistration,
                       std::shared_ptr< T > instance)
            : componentContext_(componentContext)
            , componentRegistration_(componentRegistration)
            , instance_(instance)
        {
            if (componentContext == nullptr)
                throw std::invalid_argument("componentContext");
            if (componentRegistration == nullptr)
                throw std::invalid_argument("componentRegistration");
            if (instance == nullptr)
                throw std::invalid_argument("instance");
        }

        std::shared_ptr< IComponentContext > componentContext() { return componentContext_; }

        std::shared_ptr< IComponentRegistration > componentRegistration() { return componentRegistration_; }

        std::shared_ptr< T > instance() { return instance_; }

        void instance(std::shared_ptr< T > value)
        {
            if (value == nullptr)
                throw std::invalid_argument("value");
            instance_ = value;
        }

    private:
        std::shared_ptr< IComponentContext > componentContext_;
        std::shared_ptr< IComponentRegistration > componentRegistration_;
        std::shared_ptr< T > instance_;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_ACTIVATING_DATA_H_ */