#pragma once

#include "Hypodermic/As.h"
#include "Hypodermic/AsSelf.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/OnActivated.h"
#include "Hypodermic/RegistrationBuilder.h"
#include "Hypodermic/RegistrationDescriptorBase.h"
#include "Hypodermic/SingleInstance.h"


namespace Hypodermic
{

    template <class TDescriptorInfo>
    class ProvidedInstanceFactoryRegistrationDescriptor : public RegistrationDescriptorBase< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                          public RegistrationDescriptorOperations::As< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                          public RegistrationDescriptorOperations::AsSelf< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                          public RegistrationDescriptorOperations::OnActivated< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                          public RegistrationDescriptorOperations::SingleInstance< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >

    {
        friend class RegistrationDescriptorOperations::As< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::AsSelf< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::OnActivated< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::SingleInstance< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;

    public:
        typedef RegistrationDescriptorBase< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo > BaseType;

        typedef typename TDescriptorInfo::InstanceType InstanceType;

        template <class TNewDescriptorInfo>
        struct UpdateDescriptor
        {
            typedef ProvidedInstanceFactoryRegistrationDescriptor< TNewDescriptorInfo > Type;
        };

    public:
        explicit ProvidedInstanceFactoryRegistrationDescriptor(const std::function< std::shared_ptr< InstanceType >(Container&) >& instanceFactory)
            : BaseType(Utils::getMetaTypeInfo< InstanceType >())
            , m_instanceFactory(instanceFactory)
        {
        }

        ProvidedInstanceFactoryRegistrationDescriptor(const TypeInfo& instanceType,
                                                      const std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > >& typeAliases,
                                                      const std::unordered_map< TypeInfo, std::function< std::shared_ptr< void >(Container&) > >& dependencyFactories,
                                                      const std::vector< std::function< void(Container&, const std::shared_ptr< void >&) > >& activationHandlers)
            : BaseType(instanceType, typeAliases, dependencyFactories, activationHandlers)
        {
        }

    protected:
        template <class TNewDescriptorInfo>
        std::shared_ptr< typename UpdateDescriptor< TNewDescriptorInfo >::Type > createUpdate() const
        {
            auto updatedDescriptor = std::make_shared< typename UpdateDescriptor< TNewDescriptorInfo >::Type >
            (
                this->instanceType(),
                this->typeAliases(),
                this->dependencyFactories(),
                this->activationHandlers()
            );
            updatedDescriptor->m_instanceFactory = m_instanceFactory;

            return updatedDescriptor;
        }

        std::shared_ptr< IRegistration > describe() const override
        {
            HYPODERMIC_LOG_INFO("Describing " << TDescriptorInfo::toString());

            return RegistrationBuilder< TDescriptorInfo >::build
            (
                this->instanceType(),
                this->typeAliases(),
                instanceFactory(),
                this->dependencyFactories(),
                this->activationHandlers()
            );
        }

    private:
        std::function< std::shared_ptr< void >(Container&) > instanceFactory() const
        {
            auto&& factory = m_instanceFactory;

            return [factory](Container& container)
            {
                return std::static_pointer_cast< void >(factory(container));
            };
        }

        std::function< std::shared_ptr< InstanceType >(Container&) > m_instanceFactory;
    };

} // namespace Hypodermic