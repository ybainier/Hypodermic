#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/IRegistrationScope.h"
#include "Hypodermic/NestedRegistrationScope.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class Container
    {
    public:
        explicit Container(const std::shared_ptr< IRegistrationScope >& registrationScope)
            : m_registrationScope(registrationScope)
        {
        }

        std::shared_ptr< IRegistrationScope > createNestedScope() const
        {
            return std::make_shared< NestedRegistrationScope >(m_registrationScope);
        }

        template <class T>
        std::shared_ptr< T > resolve()
        {
            return resolve< T >(createKeyForType< T >());
        }

        template <class T>
        std::vector< std::shared_ptr< T > > resolveAll()
        {
            auto typeAliasKey = createKeyForType< T >();

            std::vector< std::shared_ptr< T > > instances;

            std::vector< std::shared_ptr< IRegistration > > registrations;
            if (!tryGetRegistrations(typeAliasKey, registrations))
                return instances;

            if (registrations.empty())
                return instances;

            for (auto&& registration : registrations)
                instances.push_back(resolve< T >(typeAliasKey, *registration));

            return instances;
        }

        template <class T, class TDependency>
        std::function< std::shared_ptr< TDependency >(Container&) > getDependencyFactory()
        {
            std::vector< std::shared_ptr< IRegistration > > registrations;
            if (!tryGetRegistrations(createKeyForType< T >(), registrations))
                return nullptr;

            if (registrations.empty())
                return nullptr;

            return getDependencyFactory< TDependency >(*registrations.back());
        }

    private:
        bool tryGetRegistrations(const TypeAliasKey& typeAliasKey, std::vector< std::shared_ptr< IRegistration > >& registrations) const
        {
            return m_registrationScope->tryGetRegistrations(typeAliasKey, registrations);
        }

        template <class T>
        std::shared_ptr< T > resolve(const TypeAliasKey& typeAliasKey)
        {
            std::vector< std::shared_ptr< IRegistration > > registrations;
            if (!tryGetRegistrations(typeAliasKey, registrations))
                return nullptr;

            if (registrations.empty())
                return nullptr;

            return resolve< T >(typeAliasKey, *registrations.back());
        }

        template <class T>
        std::shared_ptr< T > resolve(const TypeAliasKey& typeAliasKey, IRegistration& registration)
        {
            return std::static_pointer_cast< T >(registration.activate(*this, typeAliasKey));
        }

        template <class TDependency>
        std::function< std::shared_ptr< TDependency >(Container&) > getDependencyFactory(const IRegistration& registration)
        {
            auto&& factory = registration.getDependencyFactory(Utils::getMetaTypeInfo< TDependency >());
            if (!factory)
                return nullptr;

            return [factory](Container& c) { return std::static_pointer_cast< TDependency >(factory(c)); };
        }

    private:
        std::shared_ptr< IRegistrationScope > m_registrationScope;
    };

} // namespace Hypodermic