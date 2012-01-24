#ifndef     HYPODERMIC_CIRCULAR_DEPENDENCY_DETECTOR_H_
# define    HYPODERMIC_CIRCULAR_DEPENDENCY_DETECTOR_H_

# include <deque>
# include <string>

# include <Hypodermic/InstanceLookup.h>


namespace Hypodermic
{
    class IComponentRegistration;


    class CircularDependencyDetector
    {
    public:
        void checkForCircularDependency(IComponentRegistration* registration,
                                        const std::deque< InstanceLookup* >& activationStack,
                                        int callDepth);

    private:
        static std::string createDependencyGraphTo(IComponentRegistration* registration,
                                                   const std::deque< InstanceLookup* >& activationStack);

        static std::string display(IComponentRegistration* registration);

        static bool isCircularDependency(IComponentRegistration* registration,
                                         const std::deque< InstanceLookup* >& activationStack);

        static const int maxResolveDepth_ = 50;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_CIRCULAR_DEPENDENCY_DETECTOR_H_ */