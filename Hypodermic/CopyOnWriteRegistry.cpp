#include "CopyOnWriteRegistry.h"

#include <stdexcept>

#include "IComponentRegistration.h"
#include "IRegistrationSource.h"
#include "NullptrWorkaround.h"


namespace Hypodermic
{

    CopyOnWriteRegistry::CopyOnWriteRegistry(std::shared_ptr< IComponentRegistry > readRegistry, std::function< std::shared_ptr< IComponentRegistry >() > createWriteRegistry)
        : readRegistry_(readRegistry)
        , createWriteRegistry_(createWriteRegistry)
        , writeRegistry_()
    {
        if (readRegistry_ == nullptr)
            throw std::invalid_argument("readRegistry");
    }

    std::shared_ptr< IComponentRegistry > CopyOnWriteRegistry::registry()
    {
        return writeRegistry_ != nullptr ? writeRegistry_ : readRegistry_;
    }

    std::shared_ptr< IComponentRegistry > CopyOnWriteRegistry::writeRegistry()
    {
        if (writeRegistry_ == nullptr)
            writeRegistry_ = createWriteRegistry_();

        return writeRegistry_;
    }

    std::shared_ptr< IComponentRegistration > CopyOnWriteRegistry::getRegistration(std::shared_ptr< Service > service)
    {
        return registry()->getRegistration(service);
    }

    bool CopyOnWriteRegistry::isRegistered(std::shared_ptr< Service > service)
    {
        return registry()->isRegistered(service);
    }

    void CopyOnWriteRegistry::addRegistration(std::shared_ptr< IComponentRegistration > registration)
    {
        writeRegistry()->addRegistration(registration);
    }

    void CopyOnWriteRegistry::addRegistration(std::shared_ptr< IComponentRegistration > registration, bool preserveDefaults)
    {
        writeRegistry()->addRegistration(registration, preserveDefaults);
    }

    std::vector< std::shared_ptr< IComponentRegistration > > CopyOnWriteRegistry::registrations()
    {
        return registry()->registrations();
    }

    std::vector< std::shared_ptr< IComponentRegistration > > CopyOnWriteRegistry::registrationsFor(std::shared_ptr< Service > service)
    {
        return registry()->registrationsFor(service);
    }

    bool CopyOnWriteRegistry::hasLocalComponents() const
    {
        return writeRegistry_ != nullptr;
    }

    void CopyOnWriteRegistry::addRegistrationSource(std::shared_ptr< IRegistrationSource > source)
    {
        writeRegistry()->addRegistrationSource(source);
    }

} // namespace Hypodermic