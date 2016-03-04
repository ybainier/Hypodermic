#pragma once

#include <mutex>
#include <unordered_map>
#include <vector>

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/IRegistrationScope.h"
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
        
        bool tryGetRegistrations(const TypeAliasKey& typeAliasKey, std::vector< std::shared_ptr< IRegistration > >& registrations) const override
        {
            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            auto it = m_registrationsByBaseTypes.find(typeAliasKey);
            if (it == std::end(m_registrationsByBaseTypes))
                return false;

            registrations = it->second;
            return true;
        }

        bool isEmpty() const override
        {
            return m_registrationsByBaseTypes.empty();
        }

    private:
        void addRegistration(const TypeAliasKey& typeAliasKey, const std::shared_ptr< IRegistration >& registration)
        {
            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            auto it = m_registrationsByBaseTypes.find(typeAliasKey);
            if (it == std::end(m_registrationsByBaseTypes))
                it = m_registrationsByBaseTypes.insert(std::make_pair(typeAliasKey, std::vector< std::shared_ptr< IRegistration > >())).first;

            it->second.push_back(registration);
        }

    private:
        std::unordered_map< TypeAliasKey, std::vector< std::shared_ptr< IRegistration > > > m_registrationsByBaseTypes;
        mutable std::recursive_mutex m_mutex;
    };

} // namespace Hypodermic