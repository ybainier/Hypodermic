#pragma once

#include <type_traits>
#include <unordered_map>
#include <unordered_set>

#include "Hypodermic2/As.h"
#include "Hypodermic2/AsSelf.h"
#include "Hypodermic2/IRegistration.h"
#include "Hypodermic2/IRegistrationDescriptor.h"
#include "Hypodermic2/RegistrationDescriptorInfo.h"
#include "Hypodermic2/SingleInstance.h"
#include "Hypodermic2/TypeAliasKey.h"
#include "Hypodermic2/TypeInfo.h"


namespace Hypodermic2
{

    template
    <
        class TDescriptor,
        class TDescriptorInfo
    >
    class RegistrationDescriptorBase : public IRegistrationDescriptor
                                     , public RegistrationDescriptorOperations::As< TDescriptor, TDescriptorInfo >
                                     , public RegistrationDescriptorOperations::AsSelf< TDescriptor, TDescriptorInfo >
                                     , public RegistrationDescriptorOperations::SingleInstance< TDescriptor, TDescriptorInfo >
    {
        friend class RegistrationDescriptorOperations::As< TDescriptor, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::AsSelf< TDescriptor, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::SingleInstance< TDescriptor, TDescriptorInfo >;

    public:
        RegistrationDescriptorBase(const TypeInfo& instanceType,
                                   const std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > >& typeAliases,
                                   const std::unordered_map< TypeInfo, std::function< std::shared_ptr< void >(Container&) > >& dependencyFactories)
            : m_instanceType(instanceType)
            , m_typeAliases(typeAliases)
            , m_dependencyFactories(dependencyFactories)
        {
        }

        explicit RegistrationDescriptorBase(const TypeInfo& instanceType)
            : m_instanceType(instanceType)
        {
        }

        Action& registrationDescriptorUpdated() const override
        {
            return m_registrationDescriptorUpdated;
        }

        std::function< void(IRegistrationScope&) > getDescriptionFactory() const override
        {
            return [&](IRegistrationScope& x) { x.addRegistration(describe()); };
        }
        
        const TypeInfo& instanceType() const override
        {
            return m_instanceType;
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

        virtual std::shared_ptr< IRegistration > describe() const = 0;

    private:
        TypeInfo m_instanceType;
        std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > > m_typeAliases;
        std::unordered_map< TypeInfo, std::function< std::shared_ptr< void >(Container&) > > m_dependencyFactories;
        mutable Action m_registrationDescriptorUpdated;
    };

} // namespace Hypodermic2