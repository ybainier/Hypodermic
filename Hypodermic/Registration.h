#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/RegistrationActivator.h"
#include "Hypodermic/TypeAliasKey.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class Container;


    class Registration : public IRegistration
    {
    public:
        Registration(const TypeInfo& instanceType,
                     const std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > >& typeAliases,
                     const std::function< std::shared_ptr< void >(Container&) >& instanceFactory,
                     const std::unordered_map< TypeInfo, std::function< std::shared_ptr< void >(Container&) > >& dependencyFactories,
                     const std::vector< std::function< void(Container&, const std::shared_ptr< void >&) > >& activationHandlers)
            : m_activator(*this, instanceFactory, activationHandlers)
            , m_instanceType(instanceType)
            , m_typeAliases(typeAliases)
            , m_dependencyFactories(dependencyFactories)
        {
        }

        const TypeInfo& instanceType() const override
        {
            return m_instanceType;
        }

        const std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > >& typeAliases() const override
        {
            return m_typeAliases;
        }

        std::function< std::shared_ptr< void >(Container&) > getDependencyFactory(const TypeInfo& dependencyType) const override
        {
            auto factoryIt = m_dependencyFactories.find(dependencyType);
            if (factoryIt == std::end(m_dependencyFactories))
                return nullptr;

            return factoryIt->second;
        }

        IRegistrationActivator& activator() const override
        {
            return m_activator;
        }

    private:
        mutable RegistrationActivator m_activator;
        TypeInfo m_instanceType;
        std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > > m_typeAliases;
        std::unordered_map< TypeInfo, std::function< std::shared_ptr< void >(Container&) > > m_dependencyFactories;
    };

} // namespace Hypodermic