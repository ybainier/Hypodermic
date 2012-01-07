#ifndef     HYPODERMIC_CONTAINER_H_
# define    HYPODERMIC_CONTAINER_H_

# include <Hypodermic/IContainer.h>


namespace Hypodermic
{

    class Container : public IContainer
    {
    private:
        void* resolve(const std::type_info& typeInfo);
    };

} // namespace Hypodermic


# include <Hypodermic/Container.hpp>

#endif /* !HYPODERMIC_CONTAINER_H_ */