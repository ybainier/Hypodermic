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
            if (m_scope == nullptr)
                m_scope = m_parentScope->clone();

            m_scope->addRegistration(registration);
        }
        
        bool tryGetRegistrations(const TypeAliasKey& typeAliasKey, std::vector< std::shared_ptr< RegistrationContext > >& registrationContexts) const override
        {
            if (m_scope != nullptr)
                return m_scope->tryGetRegistrations(typeAliasKey, registrationContexts);

            return m_parentScope->tryGetRegistrations(typeAliasKey, registrationContexts);
        }

        std::shared_ptr< IRegistrationScope > clone() override
        {
            auto scope = std::make_shared< NestedRegistrationScope >(PrivateTag());
            scope->m_parentScope = m_parentScope;
            scope->m_scope = m_scope;

            return scope;
        }

    private:
        std::shared_ptr< IRegistrationScope > m_parentScope;
        std::shared_ptr< IRegistrationScope > m_scope;
    };

} // namespace Hypodermic