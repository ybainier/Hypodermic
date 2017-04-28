#pragma once

#include "Hypodermic/As.h"
#include "Hypodermic/AsSelf.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/Named.h"
#include "Hypodermic/RegistrationBuilder.h"
#include "Hypodermic/RegistrationDescriptorBase.h"
#include "Hypodermic/UseIfNone.h"


namespace Hypodermic
{

    template <class TDescriptorInfo>
    class ProvidedInstanceRegistrationDescriptor : public RegistrationDescriptorBase< ProvidedInstanceRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                   public RegistrationDescriptorOperations::As< ProvidedInstanceRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                   public RegistrationDescriptorOperations::AsSelf< ProvidedInstanceRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                   public RegistrationDescriptorOperations::Named< ProvidedInstanceRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                   public RegistrationDescriptorOperations::UseIfNone< ProvidedInstanceRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >
    {
        friend class RegistrationDescriptorOperations::As< ProvidedInstanceRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::AsSelf< ProvidedInstanceRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::Named< ProvidedInstanceRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::UseIfNone< ProvidedInstanceRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;

    public:
        typedef RegistrationDescriptorBase< ProvidedInstanceRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo > BaseType;

        typedef typename TDescriptorInfo::InstanceType InstanceType;

        template <class TNewDescriptorInfo>
        struct UpdateDescriptor
        {
            typedef ProvidedInstanceRegistrationDescriptor< TNewDescriptorInfo > Type;
        };

    public:
        explicit ProvidedInstanceRegistrationDescriptor(const std::shared_ptr< InstanceType >& instance)
            : BaseType(Utils::getMetaTypeInfo< InstanceType >())
            , m_instance(instance)
        {
        }

        ProvidedInstanceRegistrationDescriptor(const TypeInfo& instanceType,
                                               const TypeAliases& typeAliases,
                                               const DependencyFactories& dependencyFactories,
                                               const ActivationHandlers& activationHandlers,
                                               const std::shared_ptr< InstanceType >& instance)
            : BaseType(instanceType, typeAliases, dependencyFactories, activationHandlers)
            , m_instance(instance)
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
                m_instance
            );

            return updatedDescriptor;
        }

        std::shared_ptr< IRegistration > describe() const override
        {
            HYPODERMIC_LOG_INFO("Describing " << TDescriptorInfo::toString());

            return RegistrationBuilder< TDescriptorInfo >::buildForProvidedInstance(m_instance, this->typeAliases());
        }

    private:
        std::shared_ptr< InstanceType > m_instance;
    };

} // namespace Hypodermic