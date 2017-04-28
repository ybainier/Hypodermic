#pragma once

#include <unordered_map>

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class PersistentInstanceRegistration : public IRegistration
    {
    public:
        explicit PersistentInstanceRegistration(const std::shared_ptr< IRegistration >& registration)
            : m_registration(registration)
        {
        }

        const TypeInfo& instanceType() const override
        {
            return m_registration->instanceType();
        }

        const TypeAliases& typeAliases() const override
        {
            return m_registration->typeAliases();
        }

        DependencyFactory getDependencyFactory(const TypeInfo& dependencyType) const override
        {
            return m_registration->getDependencyFactory(dependencyType);
        }

        IRegistrationActivator& activator() const override
        {
            return m_registration->activator();
        }

        InstanceLifetimes::InstanceLifetime instanceLifetime() const override
        {
            return InstanceLifetimes::Persistent;
        }

        bool isFallback() const override
        {
            return m_registration->isFallback();
        }

    private:
        std::shared_ptr< IRegistration > m_registration;
    };

} // namespace Hypodermic