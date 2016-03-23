#pragma once

#include <memory>
#include <vector>

#include "Hypodermic/AutowireableConstructor.h"
#include "Hypodermic/ConstructorDescriptor.h"
#include "Hypodermic/IRegistration.h"
#include "Hypodermic/IRegistrationScope.h"
#include "Hypodermic/IRuntimeRegistrationBuilder.h"
#include "Hypodermic/NestedRegistrationScope.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class Container
    {
    public:
        Container(const std::shared_ptr< IRegistrationScope >& registrationScope,
                  const std::shared_ptr< IRuntimeRegistrationBuilder >& runtimeRegistrationBuilder)
            : m_registrationScope(registrationScope)
            , m_runtimeRegistrationBuilder(runtimeRegistrationBuilder)
        {
        }

        /// <summary>
        /// Create a nested scope
        /// </summary>
        /// <returns>A shared pointer to a new IRegistrationScope</returns>
        std::shared_ptr< IRegistrationScope > createNestedScope() const
        {
            return std::make_shared< NestedRegistrationScope >(m_registrationScope);
        }

        /// <summary>
        /// Resolve an instance of type T
        /// </summary>
        /// <param name="T">The type to resolve (i.e. get an instance of T)</param>
        /// <returns>A shared pointer on an instance of type T</returns>
        template <class T>
        std::shared_ptr< T > resolve()
        {
            auto&& instance = resolve< T >(createKeyForType< T >());
            if (instance != nullptr)
                return instance;

            return resolveIfTypeCanBeRegistered< T >();
        }

        /// <summary>
        /// Resolve all instances of type T
        /// </summary>
        /// <param name="T">The type to resolve (i.e. get all instances of T)</param>
        /// <returns>A vector of shared pointers on instances of type T</returns>
        template <class T>
        std::vector< std::shared_ptr< T > > resolveAll()
        {
            return resolveAll< T >(createKeyForType< T >());
        }

    private:
        template <class T> friend struct Traits::ArgumentResolver;

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

        template <class T>
        std::vector< std::shared_ptr< T > > resolveAll(const TypeAliasKey& typeAliasKey)
        {
            std::vector< std::shared_ptr< IRegistration > > registrations;
            if (!tryGetRegistrations(typeAliasKey, registrations))
                return std::vector< std::shared_ptr< T > >();

            return resolveAll< T >(typeAliasKey, registrations);
        }

        template <class T>
        std::vector< std::shared_ptr< T > > resolveAll(const TypeAliasKey& typeAliasKey, const std::vector< std::shared_ptr< IRegistration > >& registrations)
        {
            std::vector< std::shared_ptr< T > > instances;

            for (auto&& registration : registrations)
                instances.push_back(resolve< T >(typeAliasKey, *registration));

            return instances;
        }

        bool tryGetRegistrations(const TypeAliasKey& typeAliasKey, std::vector< std::shared_ptr< IRegistration > >& registrations) const
        {
            return m_registrationScope->tryGetRegistrations(typeAliasKey, registrations);
        }

        template <class TDependency>
        std::function< std::shared_ptr< TDependency >(Container&) > getDependencyFactory(const IRegistration& registration)
        {
            auto&& factory = registration.getDependencyFactory(Utils::getMetaTypeInfo< TDependency >());
            if (!factory)
                return nullptr;

            return [factory](Container& c) { return std::static_pointer_cast< TDependency >(factory(c)); };
        }

        template <class T>
        std::shared_ptr< T > resolveIfTypeCanBeRegistered()
        {
            if (!tryToRegisterType< T >(*m_registrationScope, Traits::HasAutowireableConstructor< T >()))
                return nullptr;

            return resolve< T >(createKeyForType< T >());
        }

        template <class T>
        bool tryToRegisterType(IRegistrationScope& scope, std::true_type /* T has autowireable constructor */)
        {
            auto&& factory = Traits::ConstructorDescriptor< T >::describe();

            scope.addRegistration(m_runtimeRegistrationBuilder->build
            (
                Utils::getMetaTypeInfo< T >(),
                [factory](Container& container) { return std::static_pointer_cast< void >(factory(container)); }
            ));

            return true;
        }

        template <class>
        bool tryToRegisterType(IRegistrationScope&, std::false_type)
        {
            return false;
        }

    private:
        std::shared_ptr< IRegistrationScope > m_registrationScope;
        std::shared_ptr< IRuntimeRegistrationBuilder > m_runtimeRegistrationBuilder;
    };

} // namespace Hypodermic