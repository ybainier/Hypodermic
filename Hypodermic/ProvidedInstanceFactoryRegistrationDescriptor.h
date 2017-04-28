#pragma once

#include "Hypodermic/As.h"
#include "Hypodermic/AsSelf.h"
#include "Hypodermic/InstanceFactory.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/Named.h"
#include "Hypodermic/OnActivated.h"
#include "Hypodermic/RegistrationBuilder.h"
#include "Hypodermic/RegistrationDescriptorBase.h"
#include "Hypodermic/SingleInstance.h"
#include "Hypodermic/UseIfNone.h"


namespace Hypodermic
{

    template <class TDescriptorInfo>
    class ProvidedInstanceFactoryRegistrationDescriptor : public RegistrationDescriptorBase< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                          public RegistrationDescriptorOperations::As< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                          public RegistrationDescriptorOperations::AsSelf< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                          public RegistrationDescriptorOperations::Named< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                          public RegistrationDescriptorOperations::OnActivated< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                          public RegistrationDescriptorOperations::SingleInstance< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                          public RegistrationDescriptorOperations::UseIfNone< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >
    {
        friend class RegistrationDescriptorOperations::As< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::AsSelf< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::Named< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::OnActivated< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::SingleInstance< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::UseIfNone< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;

    public:
        typedef RegistrationDescriptorBase< ProvidedInstanceFactoryRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo > BaseType;

        typedef typename TDescriptorInfo::InstanceType InstanceType;

        template <class TNewDescriptorInfo>
        struct UpdateDescriptor
        {
            typedef ProvidedInstanceFactoryRegistrationDescriptor< TNewDescriptorInfo > Type;
        };

    public:
        explicit ProvidedInstanceFactoryRegistrationDescriptor(const std::function< std::shared_ptr< InstanceType >(ComponentContext&) >& instanceFactory)
            : BaseType(Utils::getMetaTypeInfo< InstanceType >())
            , m_instanceFactory(instanceFactory)
        {
        }

        ProvidedInstanceFactoryRegistrationDescriptor(const TypeInfo& instanceType,
                                                      const TypeAliases& typeAliases,
                                                      const DependencyFactories& dependencyFactories,
                                                      const ActivationHandlers& activationHandlers,
                                                      const std::function< std::shared_ptr< InstanceType >(ComponentContext&) >& instanceFactory)
            : BaseType(instanceType, typeAliases, dependencyFactories, activationHandlers)
            , m_instanceFactory(instanceFactory)
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
                this->activationHandlers(),
                m_instanceFactory
            );

            return updatedDescriptor;
        }

        std::shared_ptr< IRegistration > describe() const override
        {
            HYPODERMIC_LOG_INFO("Describing " << TDescriptorInfo::toString());

            return RegistrationBuilder< TDescriptorInfo >::build
            (
                this->instanceType(),
                this->typeAliases(),
                typeErasedInstanceFactory(),
                this->dependencyFactories(),
                this->activationHandlers()
            );
        }

    private:
        InstanceFactory typeErasedInstanceFactory() const
        {
            auto&& factory = m_instanceFactory;

            return [factory](const IRegistration&, IResolutionContext& resolutionContext)
            {
                return std::static_pointer_cast< void >(factory(resolutionContext.componentContext()));
            };
        }

        std::function< std::shared_ptr< InstanceType >(ComponentContext&) > m_instanceFactory;
    };

} // namespace Hypodermic