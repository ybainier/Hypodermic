#include "ExternalRegistrySource.h"

#include <deque>
#include <set>
#include <stdexcept>

#include "ContainerBuilder.h"
#include "IComponentRegistration.h"
#include "NullptrWorkaround.h"
#include "RegistrationBuilderFactory.h"


namespace Hypodermic
{

    ExternalRegistrySource::ExternalRegistrySource(std::shared_ptr< IComponentRegistry > registry)
        : registry_(registry)
    {
        if (registry_ == nullptr)
            throw std::invalid_argument("registry");
    }

    std::vector< std::shared_ptr< IComponentRegistration > >
    ExternalRegistrySource::registrationsFor(std::shared_ptr< Service > service,
                                             std::function< std::vector< std::shared_ptr< IComponentRegistration > >(std::shared_ptr< Service > service) > registrationAccessor)
    {
        std::vector< std::shared_ptr< IComponentRegistration > > result;

        std::set< std::shared_ptr< IComponentRegistration > > seenRegistrations;
        std::set< std::shared_ptr< Service > > seenServices;

        std::deque< std::shared_ptr< Service > > lastRunServices;
        lastRunServices.push_back(service);

        while (lastRunServices.size() > 0)
        {
            auto nextService = lastRunServices.front();
            lastRunServices.pop_front();

            seenServices.insert(nextService);

            auto&& registrationsForService = registry_->registrationsFor(nextService);
            
            auto itEnd = registrationsForService.end();
            for (auto it = registrationsForService.begin(); it != itEnd; ++it)
            {
                auto componentRegistration = *it;

                if (componentRegistration->isAdapting())
                    continue;

                if (seenRegistrations.count(componentRegistration) > 0)
                    continue;

                seenRegistrations.insert(componentRegistration);

                auto&& services = componentRegistration->services();
                auto serviceItEnd = services.end();
                for (auto serviceIt = services.begin(); serviceIt != serviceItEnd; ++serviceIt)
                {
                    if (seenServices.count(*serviceIt) > 0)
                        continue;

                    lastRunServices.push_back(*serviceIt);
                }

                typedef RegistrationBuilderFactory< ContainerBuilder::RegistrationBuilderInterface > BuilderFactory;

                auto rb = BuilderFactory::forDelegate
                    (
                        componentRegistration->activator()->typeInfo(),
                        [componentRegistration](IComponentContext& c)
                        {
                            return c.resolveComponent(componentRegistration);
                        }
                    );

                    //            .Targeting(r)

                for (auto serviceIt = services.begin(); serviceIt != serviceItEnd; ++serviceIt)
                {
                    auto service = *serviceIt;
                    rb->as(service, componentRegistration->getTypeCaster(service->typeInfo()));
                }
                    //            .ExternallyOwned()

                result.push_back(BuilderFactory::createRegistration< void >(rb));
            }
        }

        return result;
    }

} // namespace Hypodermic