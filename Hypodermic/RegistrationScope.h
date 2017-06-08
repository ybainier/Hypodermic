#pragma once

#include <algorithm>
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

            auto hasRegistrations = false;
            {
                auto it = m_fallbackRegistrationContextsByBaseTypes.find(typeAliasKey);
                if (it != std::end(m_fallbackRegistrationContextsByBaseTypes))
                {
                    auto& contexts = it->second;
                    std::copy(contexts.rbegin(), contexts.rend(), std::back_inserter(registrationContexts));

                    hasRegistrations = true;
                }
            }

            auto it = m_registrationContextsByBaseTypes.find(typeAliasKey);
            if (it == std::end(m_registrationContextsByBaseTypes))
                return hasRegistrations;

            auto& contexts = it->second;
            std::copy(std::begin(contexts), std::end(contexts), std::back_inserter(registrationContexts));
            return true;
        }

        void copyTo(IRegistrationScope& other) const override
        {
            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            copyRegistrationContextsTo(other);
            copyFallbackRegistrationContextsTo(other);
        }

        void addRegistrationContext(const std::shared_ptr< RegistrationContext >& registrationContext) override
        {
            if (registrationContext->registration()->typeAliases().empty())
            {
                addRegistrationContext(createKeyForType(registrationContext->registration()->instanceType()), registrationContext);
                return;
            }

            for (auto&& x : registrationContext->registration()->typeAliases())
                addRegistrationContext(x.first, registrationContext);
        }

    private:
        void addRegistration(const TypeAliasKey& typeAliasKey, const std::shared_ptr< IRegistration >& registration)
        {
            addRegistrationContext(typeAliasKey, std::make_shared< RegistrationContext >(m_resolutionContainer, registration));
        }

        void addRegistrationContext(const TypeAliasKey& typeAliasKey, const std::shared_ptr< RegistrationContext >& registrationContext)
        {
            if (registrationContext->registration()->isFallback())
            {
                addRegistrationContext(m_fallbackRegistrationContextsByBaseTypes, typeAliasKey, registrationContext);
            }
            else
            {
                addRegistrationContext(m_registrationContextsByBaseTypes, typeAliasKey, registrationContext);
            }
        }

        void copyRegistrationContextsTo(IRegistrationScope& other) const
        {
            copyRegistrationContextsToRegistrationScope(m_registrationContextsByBaseTypes, other);
        }

        void copyFallbackRegistrationContextsTo(IRegistrationScope& other) const
        {
            copyRegistrationContextsToRegistrationScope(m_fallbackRegistrationContextsByBaseTypes, other);
        }

        template <class TRegistrationContextsByBaseTypes>
        void addRegistrationContext(TRegistrationContextsByBaseTypes& contextsByBaseTypes, const TypeAliasKey& typeAliasKey, const std::shared_ptr< RegistrationContext >& registrationContext)
        {
            std::lock_guard< decltype(m_mutex) > lock(m_mutex);

            auto it = contextsByBaseTypes.find(typeAliasKey);
            if (it == std::end(contextsByBaseTypes))
                it = contextsByBaseTypes.insert(std::make_pair(typeAliasKey, std::vector< std::shared_ptr< RegistrationContext > >())).first;

            it->second.push_back(registrationContext);
        }

        template <class TRegistrationContextsByBaseTypes>
        void copyRegistrationContextsToRegistrationScope(const TRegistrationContextsByBaseTypes& contextsByBaseTypes, IRegistrationScope& other) const
        {
            for (auto& x : contextsByBaseTypes)
            {
                for (auto& registrationContext : x.second)
                {
                    other.addRegistrationContext(registrationContext);
                }
            }
        }

    private:
        std::unordered_map< TypeAliasKey, std::vector< std::shared_ptr< RegistrationContext > > > m_registrationContextsByBaseTypes;
        std::unordered_map< TypeAliasKey, std::vector< std::shared_ptr< RegistrationContext > > > m_fallbackRegistrationContextsByBaseTypes;
        mutable std::recursive_mutex m_mutex;
        ResolutionContainer m_resolutionContainer;
    };

} // namespace Hypodermic