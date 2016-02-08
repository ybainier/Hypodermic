#pragma once

#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Hypodermic/IRegistration.h"
#include "Hypodermic/IRegistrationDescriptor.h"
#include "Hypodermic/RegistrationDescriptorInfo.h"
#include "Hypodermic/TypeAliasKey.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    template
    <
        class TDescriptor,
        class TDescriptorInfo
    >
    class RegistrationDescriptorBase : public IRegistrationDescriptor
    {
    public:
        RegistrationDescriptorBase(const TypeInfo& instanceType,
                                   const std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > >& typeAliases,
                                   const std::unordered_map< TypeInfo, std::function< std::shared_ptr< void >(Container&) > >& dependencyFactories,
                                   const std::vector< std::function< void(Container&, const std::shared_ptr< void >&) > >& activationHandlers)
            : m_instanceType(instanceType)
            , m_typeAliases(typeAliases)
            , m_dependencyFactories(dependencyFactories)
            , m_activationHandlers(activationHandlers)
        {
        }

        explicit RegistrationDescriptorBase(const TypeInfo& instanceType)
            : m_instanceType(instanceType)
        {
        }

        const TypeInfo& instanceType() const override
        {
            return m_instanceType;
        }

        Action& registrationDescriptorUpdated() const override
        {
            return m_registrationDescriptorUpdated;
        }

        std::function< void(IRegistrationScope&) > getDescriptionFactory() const override
        {
            return [&](IRegistrationScope& x) { x.addRegistration(describe()); };
        }

    protected:
        const std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > >& typeAliases() const
        {
            return m_typeAliases;
        }

        const std::unordered_map< TypeInfo, std::function< std::shared_ptr< void >(Container&) > >& dependencyFactories() const
        {
            return m_dependencyFactories;
        }

        const std::vector< std::function< void(Container&, const std::shared_ptr< void >&) > >& activationHandlers() const
        {
            return m_activationHandlers;
        }

        void addTypeIfMissing(const TypeAliasKey& typeAliasKey)
        {
            addTypeIfMissing(typeAliasKey, nullptr);
        }

        void addTypeIfMissing(const TypeAliasKey& typeAliasKey, const std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) >& alignPointersFunc)
        {
            m_typeAliases.insert(std::make_pair(typeAliasKey, alignPointersFunc));
        }

        template <class T>
        void addDependencyFactory(const std::function< std::shared_ptr< void >(Container&) >& factory)
        {
            m_dependencyFactories[Utils::getMetaTypeInfo< T >()] = factory;
        }

        void addActivationHandler(const std::function< void(Container&, const std::shared_ptr< void >&) >& activationHandler)
        {
            if (!activationHandler)
                return;

            m_activationHandlers.push_back(activationHandler);
        }

        virtual std::shared_ptr< IRegistration > describe() const = 0;

    private:
        TypeInfo m_instanceType;
        std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > > m_typeAliases;
        std::unordered_map< TypeInfo, std::function< std::shared_ptr< void >(Container&) > > m_dependencyFactories;
        std::vector< std::function< void(Container&, const std::shared_ptr< void >&) > > m_activationHandlers;
        mutable Action m_registrationDescriptorUpdated;
    };

} // namespace Hypodermic