#include <stdexcept>
#include <boost/foreach.hpp>

#include "DependencyResolutionException.h"
#include "IComponentRegistration.h"
#include "IInstanceActivator.h"
#include "CircularDependencyDetector.h"


namespace Hypodermic
{

    void CircularDependencyDetector::checkForCircularDependency(IComponentRegistration* registration,
                                                                const std::deque< InstanceLookup* >& activationStack,
                                                                int callDepth)
    {
        if (registration == nullptr)
            throw std::invalid_argument("registration");

        if (callDepth > maxResolveDepth_)
        {
            throw DependencyResolutionException("Max depth exceeded");
        }

        if (isCircularDependency(registration, activationStack))
        {
            throw DependencyResolutionException("Circular dependency: " + createDependencyGraphTo(registration, activationStack));
        }
    }

    std::string CircularDependencyDetector::createDependencyGraphTo(IComponentRegistration* registration,
                                                                    const std::deque< InstanceLookup* >& activationStack)
    {
        if (registration == nullptr)
            throw std::invalid_argument("registration");

        std::string dependencyGraph = display(registration);

        BOOST_FOREACH(auto activation, activationStack) 
        {
            dependencyGraph = display(activation->componentRegistration()) + " -> " + dependencyGraph;
        }

        return dependencyGraph;
    }

    std::string CircularDependencyDetector::display(IComponentRegistration* registration)
    {
        return registration->activator()->typeInfo().name();
    }

    bool CircularDependencyDetector::isCircularDependency(IComponentRegistration* registration,
                                                          const std::deque< InstanceLookup* >& activationStack)
    {
        if (registration == nullptr)
            throw std::invalid_argument("registration");
        int registrationCount = 0;
        BOOST_FOREACH(auto activation, activationStack)
        {
            if (activation->componentRegistration() == registration)
                ++registrationCount;
        }
        return registrationCount > 0;
    }

} // namespace Hypodermic