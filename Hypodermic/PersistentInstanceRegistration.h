#pragma once

#include <mutex>
#include <unordered_map>

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/PersistentInstanceRegistrationActivator.h"
#include "Hypodermic/TypeAliasKey.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class PersistentInstanceRegistration : public IRegistration
    {
    public:
        explicit PersistentInstanceRegistration(const std::shared_ptr< IRegistration >& registration)
            : m_activator(*registration)
            , m_registration(registration)
        {
        }

        const TypeInfo& instanceType() const override
        {
            return m_registration->instanceType();
        }

        const std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > >& typeAliases() const override
        {
            return m_registration->typeAliases();
        }

        std::function< std::shared_ptr< void >(Container&) > getDependencyFactory(const TypeInfo& dependencyType) const override
        {
            return m_registration->getDependencyFactory(dependencyType);
        }

        IRegistrationActivator& activator() const override
        {
            return m_activator;
        }

    private:
        mutable PersistentInstanceRegistrationActivator m_activator;
        std::shared_ptr< IRegistration > m_registration;
    };

} // namespace Hypodermic