#ifndef     HYPODERMIC_IINSTANCE_LOOKUP_H_
# define    HYPODERMIC_IINSTANCE_LOOKUP_H_


namespace Hypodermic
{
    class IComponentRegistration;
    class ILifetimeScope;


    class IInstanceLookup
    {
        virtual IComponentRegistration* componentRegistration() = 0;

        virtual ILifetimeScope* activationScope() = 0;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_IINSTANCE_LOOKUP_H_ */