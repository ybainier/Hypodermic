#ifndef     ISERVICE_LOCATOR_H_
# define    ISERVICE_LOCATOR_H_

# include <typeinfo>


namespace Hypodermic
{
    class IService;


    class IServiceLocator
    {
    public:
        virtual IService& getService(const std::type_info& typeInfo) = 0;
    };

} // namespace Hypodermic


#endif /* !ISERVICE_LOCATOR_H_ */