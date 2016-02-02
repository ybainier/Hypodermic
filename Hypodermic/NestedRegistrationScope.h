#pragma once

#include <unordered_map>
#include <vector>

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/IRegistrationScope.h"
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
            m_scope.addRegistration(registration);
        }
        
        bool tryGetRegistrations(const TypeAliasKey& typeAliasKey, std::vector< std::shared_ptr< IRegistration > >& registrations) const override
        {
            if (m_scope.isEmpty())
                return m_parentScope->tryGetRegistrations(typeAliasKey, registrations);

            if (!m_scope.tryGetRegistrations(typeAliasKey, registrations))
                return m_parentScope->tryGetRegistrations(typeAliasKey, registrations);

            return true;
        }

        bool isEmpty() const override
        {
            return m_scope.isEmpty();
        }

    private:
        RegistrationScope m_scope;
        std::shared_ptr< IRegistrationScope > m_parentScope;
        std::unordered_map< TypeAliasKey, std::vector< std::shared_ptr< IRegistration > > > m_registrationsByBaseTypes;
    };

} // namespace Hypodermic