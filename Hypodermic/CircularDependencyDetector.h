#ifndef     HYPODERMIC_CIRCULAR_DEPENDENCY_DETECTOR_H_
# define    HYPODERMIC_CIRCULAR_DEPENDENCY_DETECTOR_H_

# include <deque>
# include <stdexcept>
# include <string>

# include <Hypodermic/DependencyResolutionException.h>
# include <Hypodermic/IComponentRegistration.h>
# include <Hypodermic/InstanceLookup.h>


namespace Hypodermic
{

    class CircularDependencyDetector
    {
    public:
        void checkForCircularDependency(IComponentRegistration* registration,
                                        const std::deque< InstanceLookup* >& activationStack,
                                        int callDepth)
        {
            if (registration == nullptr)
                throw std::invalid_argument("registration");

            if (callDepth > maxResolveDepth_)
            {
                throw DependencyResolutionException("Max depth exceeded");
                //throw new DependencyResolutionException(string.Format(CultureInfo.CurrentCulture,
                //CircularDependencyDetectorResources.MaxDepthExceeded, registration));
            }

            if (isCircularDependency(registration, activationStack))
            {
                throw DependencyResolutionException("Circular dependency: " + createDependencyGraphTo(registration, activationStack));
                //throw new DependencyResolutionException(string.Format(CultureInfo.CurrentCulture,
                //CircularDependencyDetectorResources.CircularDependency, CreateDependencyGraphTo(registration, activationStack)));
            }
        }

    private:
        static std::string createDependencyGraphTo(IComponentRegistration* registration,
                                                   const std::deque< InstanceLookup* >& activationStack)
        {
            if (registration == nullptr)
                throw std::invalid_argument("registration");

            std::string dependencyGraph;
            //var dependencyGraph = Display(registration);

            //foreach (var requestor in activationStack.Select(a => a.ComponentRegistration))
            //    dependencyGraph = Display(requestor) + " -> " + dependencyGraph;

            return dependencyGraph;
        }

        static std::string display(IComponentRegistration* registration)
        {
            //return registration.Activator.LimitType.FullName ?? string.Empty;
            return "";
        }

        static bool isCircularDependency(IComponentRegistration* registration,
                                         const std::deque< InstanceLookup* >& activationStack)
        {
            //if (registration == null) throw new ArgumentNullException("registration");
            //if (activationStack == null) throw new ArgumentNullException("activationStack");
            //return activationStack.Count(a => a.ComponentRegistration == registration) >= 1;
            return false;
        }

        static const int maxResolveDepth_ = 50;
    };

} // namespace Hypodermic


#endif /* !HYPODERMIC_CIRCULAR_DEPENDENCY_DETECTOR_H_ */