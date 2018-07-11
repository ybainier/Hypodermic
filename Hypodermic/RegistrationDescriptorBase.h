#pragma once

#include "Hypodermic/ActivationHandlers.h"
#include "Hypodermic/DependencyFactories.h"
#include "Hypodermic/IRegistration.h"
#include "Hypodermic/IRegistrationDescriptor.h"
#include "Hypodermic/RegistrationDescriptorInfo.h"
#include "Hypodermic/TypeAliases.h"


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
                                   const TypeAliases& typeAliases,
                                   const DependencyFactories& dependencyFactories,
                                   const ActivationHandlers& activationHandlers)
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

        Updated& registrationDescriptorUpdated() const override
        {
            return m_registrationDescriptorUpdated;
        }

        std::function< void(IRegistrationRegistry&) > getDescriptionFactory() const override
        {
            return [&](IRegistrationRegistry& x) { x.addRegistration(describe()); };
        }

    protected:
        const TypeAliases& typeAliases() const
        {
            return m_typeAliases;
        }

        const DependencyFactories& dependencyFactories() const
        {
            return m_dependencyFactories;
        }

        const ActivationHandlers& activationHandlers() const
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
        void addDependencyFactory(const DependencyFactory& factory)
        {
            m_dependencyFactories[Utils::getMetaTypeInfo< T >()] = factory;
        }

        void addActivationHandler(const ActivationHandler& activationHandler)
        {
            if (!activationHandler)
                return;

            m_activationHandlers.push_back(activationHandler);
        }

        virtual std::shared_ptr< IRegistration > describe() const = 0;

    private:
        TypeInfo m_instanceType;
        TypeAliases m_typeAliases;
        DependencyFactories m_dependencyFactories;
        ActivationHandlers m_activationHandlers;
        mutable Updated m_registrationDescriptorUpdated;
    };

} // namespace Hypodermic