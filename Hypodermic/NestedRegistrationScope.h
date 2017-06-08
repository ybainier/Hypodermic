#pragma once

#include <memory>
#include <mutex>
#include <vector>

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/RegistrationScope.h"
#include "Hypodermic/TypeAliasKey.h"


namespace Hypodermic
{

    class NestedRegistrationScope : public IRegistrationScope
    {
    public:
        explicit NestedRegistrationScope(const std::shared_ptr< IRegistrationScope >& parentScope)
            : m_parentScope(parentScope)
        {
        }

        void addRegistration(const std::shared_ptr< IRegistration >& registration) override
        {
            writeScope().addRegistration(registration);
        }
        
        bool tryGetRegistrations(const TypeAliasKey& typeAliasKey, std::vector< std::shared_ptr< RegistrationContext > >& registrationContexts) const override
        {
            return scope().tryGetRegistrations(typeAliasKey, registrationContexts);
        }

        void copyTo(IRegistrationScope& other) const override
        {
            scope().copyTo(other);
        }

        void addRegistrationContext(const std::shared_ptr< RegistrationContext >& registrationContext) override
        {
            writeScope().addRegistrationContext(registrationContext);
        }

    private:
        IRegistrationScope& scope() const
        {
            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            if (m_scope != nullptr)
                return *m_scope;

            return *m_parentScope;
        }

        IRegistrationScope& writeScope()
        {
            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            if (m_scope == nullptr)
            {
                m_scope = std::make_shared< RegistrationScope >();
                m_parentScope->copyTo(*m_scope);
            }

            return *m_scope;
        }

    private:
        std::shared_ptr< IRegistrationScope > m_parentScope;
        std::shared_ptr< RegistrationScope > m_scope;
        mutable std::recursive_mutex m_mutex;
    };

} // namespace Hypodermic