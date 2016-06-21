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
    public:
        explicit NestedRegistrationScope(const std::shared_ptr< IRegistrationScope >& parentScope)
            : m_parentScope(parentScope)
        {
        }

        void addRegistration(const std::shared_ptr< IRegistration >& registration) override
        {
            m_scope.addRegistration(registration);
        }
        
        bool tryGetRegistrations(const TypeAliasKey& typeAliasKey, std::vector< std::shared_ptr< RegistrationContext > >& registrationContexts) const override
        {
            auto result = m_parentScope->tryGetRegistrations(typeAliasKey, registrationContexts);
            return m_scope.tryGetRegistrations(typeAliasKey, registrationContexts) || result;
        }

    private:
        RegistrationScope m_scope;
        std::shared_ptr< IRegistrationScope > m_parentScope;
    };

} // namespace Hypodermic