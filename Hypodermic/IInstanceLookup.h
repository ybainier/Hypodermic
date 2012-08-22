#ifndef     HYPODERMIC_IINSTANCE_LOOKUP_H_
# define    HYPODERMIC_IINSTANCE_LOOKUP_H_

# include <memory>


namespace Hypodermic
{
    class IComponentRegistration;
    class ILifetimeScope;


    class IInstanceLookup
    {
    public:
        virtual ~IInstanceLookup() = 0;

        virtual std::shared_ptr< IComponentRegistration > componentRegistration() = 0;

        virtual std::shared_ptr< ILifetimeScope > activationScope() = 0;
    };

    inline IInstanceLookup::~IInstanceLookup() {}

} // namespace Hypodermic


#endif /* !HYPODERMIC_IINSTANCE_LOOKUP_H_ */