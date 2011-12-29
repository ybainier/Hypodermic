#ifndef     HYPODERMIC_DEPENDENCY_RESOLUTION_EXCEPTION_H_
# define    HYPODERMIC_DEPENDENCY_RESOLUTION_EXCEPTION_H_

# include <stdexcept>
# include <string>


namespace Hypodermic
{

    class DependencyResolutionException : public std::logic_error
    {
    public:
        DependencyResolutionException(std::string message)
            : std::logic_error(message)
        {
        }
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_DEPENDENCY_RESOLUTION_EXCEPTION_H_ */