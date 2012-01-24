#ifndef     HYPODERMIC_CIRCULAR_DEPENDENCY_DETECTOR_H_
# define    HYPODERMIC_CIRCULAR_DEPENDENCY_DETECTOR_H_

# include <deque>
# include <memory>
# include <string>

# include <Hypodermic/InstanceLookup.h>


namespace Hypodermic
{
    class IComponentRegistration;


    class CircularDependencyDetector
    {
    public:
        void checkForCircularDependency(std::shared_ptr< IComponentRegistration > registration,
                                        const std::deque< std::shared_ptr< InstanceLookup > >& activationStack,
                                        int callDepth);

    private:
        static std::string createDependencyGraphTo(std::shared_ptr< IComponentRegistration > registration,
                                                   const std::deque< std::shared_ptr< InstanceLookup > >& activationStack);

        static std::string display(std::shared_ptr< IComponentRegistration > registration);

        static bool isCircularDependency(std::shared_ptr< IComponentRegistration > registration,
                                         const std::deque< std::shared_ptr< InstanceLookup > >& activationStack);

        static const int maxResolveDepth_ = 50;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_CIRCULAR_DEPENDENCY_DETECTOR_H_ */