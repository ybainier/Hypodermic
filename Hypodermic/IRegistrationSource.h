#ifndef     HYPODERMIC_IREGISTRATION_SOURCE_H_
# define    HYPODERMIC_IREGISTRATION_SOURCE_H_

# include <functional>
# include <memory>
# include <vector>

# include <Hypodermic/Service.h>


namespace Hypodermic
{
    class IComponentRegistration;

    class IRegistrationSource
    {
    public:
        virtual ~IRegistrationSource() {}

        virtual std::vector< std::shared_ptr< IComponentRegistration > >
        registrationsFor(std::shared_ptr< Service > service,
                         std::function< std::vector< std::shared_ptr< IComponentRegistration > >(std::shared_ptr< Service > service) > registrationAccessor) = 0;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_IREGISTRATION_SOURCE_H_ */