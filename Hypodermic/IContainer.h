#ifndef     HYPODERMIC_ICONTAINER_H_
# define    HYPODERMIC_ICONTAINER_H_

# include <typeinfo>


namespace Hypodermic
{

    class IContainer
    {
    public:
        template <class T> T* resolve();

    private:
        virtual void* resolve(const std::type_info& typeInfo) = 0;
    };

} // namespace Hypodermic


# include <Hypodermic/IContainer.hpp>

#endif /* !HYPODERMIC_ICONTAINER_H_ */