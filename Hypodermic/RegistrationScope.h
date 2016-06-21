#pragma once

#include <mutex>
#include <unordered_map>
#include <vector>

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/IRegistrationScope.h"
#include "Hypodermic/RegistrationContext.h"
#include "Hypodermic/ResolutionContainer.h"
#include "Hypodermic/TypeAliasKey.h"


namespace Hypodermic
{

    class RegistrationScope : public IRegistrationScope
    {
    public:
        void addRegistration(const std::shared_ptr< IRegistration >& registration) override
        {
            if (registration->typeAliases().empty())
            {
                addRegistration(createKeyForType(registration->instanceType()), registration);
                return;
            }

            for (auto&& x : registration->typeAliases())
                addRegistration(x.first, registration);
        }
        
        bool tryGetRegistrations(const TypeAliasKey& typeAliasKey, std::vector< std::shared_ptr< RegistrationContext > >& registrationContexts) const override
        {
            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            auto it = m_registrationContextsByBaseTypes.find(typeAliasKey);
            if (it == std::end(m_registrationContextsByBaseTypes))
                return false;

            auto& contexts = it->second;
            registrationContexts.insert(std::end(registrationContexts), std::begin(contexts), std::end(contexts));
            return true;
        }

    private:
        void addRegistration(const TypeAliasKey& typeAliasKey, const std::shared_ptr< IRegistration >& registration)
        {
            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            auto it = m_registrationContextsByBaseTypes.find(typeAliasKey);
            if (it == std::end(m_registrationContextsByBaseTypes))
                it = m_registrationContextsByBaseTypes.insert(std::make_pair(typeAliasKey, std::vector< std::shared_ptr< RegistrationContext > >())).first;

            it->second.push_back(std::make_shared< RegistrationContext >(m_resolutionContainer, registration));
        }

    private:
        std::unordered_map< TypeAliasKey, std::vector< std::shared_ptr< RegistrationContext > > > m_registrationContextsByBaseTypes;
        mutable std::recursive_mutex m_mutex;
        ResolutionContainer m_resolutionContainer;
    };

} // namespace Hypodermic