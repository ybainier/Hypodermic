#pragma once

#include <vector>

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/IRegistrationScope.h"
#include "Hypodermic/RegistrationScope.h"
#include "Hypodermic/TypeAliasKey.h"


namespace Hypodermic
{

    class NestedRegistrationScope : public IRegistrationScope
    {
        struct PrivateTag {};

    public:
        explicit NestedRegistrationScope(PrivateTag)
        {
        }

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

        std::shared_ptr< IRegistrationScope > clone() override
        {
            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            auto scopeClone = std::make_shared< NestedRegistrationScope >(PrivateTag());
            scopeClone->m_parentScope = m_parentScope;
            scopeClone->m_scope = m_scope;

            return scopeClone;
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
                m_scope = m_parentScope->clone();

            return *m_scope;
        }

    private:
        std::shared_ptr< IRegistrationScope > m_parentScope;
        std::shared_ptr< IRegistrationScope > m_scope;
        mutable std::recursive_mutex m_mutex;
    };

} // namespace Hypodermic