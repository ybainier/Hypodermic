#pragma once

#include "Hypodermic/As.h"
#include "Hypodermic/AsSelf.h"
#include "Hypodermic/ConstructorDescriptor.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/OnActivated.h"
#include "Hypodermic/ProvidedConstructorRegistrationDescriptor.h"
#include "Hypodermic/RegistrationDescriptorBase.h"
#include "Hypodermic/RegistrationException.h"
#include "Hypodermic/SingleInstance.h"


namespace Hypodermic
{

    template <class TDescriptorInfo>
    class MissingConstructorRegistrationDescriptor : public RegistrationDescriptorBase< MissingConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                     public RegistrationDescriptorOperations::As< MissingConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                     public RegistrationDescriptorOperations::AsSelf< MissingConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                     public RegistrationDescriptorOperations::OnActivated< MissingConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                     public RegistrationDescriptorOperations::SingleInstance< MissingConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >
    {
        friend class RegistrationDescriptorOperations::As< MissingConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::AsSelf< MissingConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::OnActivated< MissingConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::SingleInstance< MissingConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;

    public:
        typedef RegistrationDescriptorBase< MissingConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo > BaseType;

        typedef typename TDescriptorInfo::InstanceType InstanceType;

        template <class TNewDescriptorInfo>
        struct UpdateDescriptor
        {
            typedef MissingConstructorRegistrationDescriptor< TNewDescriptorInfo > Type;
        };

    public:
        MissingConstructorRegistrationDescriptor()
            : BaseType(Utils::getMetaTypeInfo< InstanceType >())
        {
        }

        MissingConstructorRegistrationDescriptor(const TypeInfo& instanceType,
                                                 const std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > >& typeAliases,
                                                 const std::unordered_map< TypeInfo, std::function< std::shared_ptr< void >(Container&) > >& dependencyFactories,
                                                 const std::vector< std::function< void(Container&, const std::shared_ptr< void >&) > >& activationHandlers)
            : BaseType(instanceType, typeAliases, dependencyFactories, activationHandlers)
        {
        }

        template <class TSignature>
        ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TSignature >& usingConstructor()
        {
            auto updatedDescriptor = std::make_shared< ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TSignature > >
            (
                this->instanceType(),
                this->typeAliases(),
                this->dependencyFactories(),
                this->activationHandlers()
            );
            this->registrationDescriptorUpdated()(updatedDescriptor);

            return *updatedDescriptor;
        }

    protected:
        template <class TNewDescriptorInfo>
        std::shared_ptr< typename UpdateDescriptor< TNewDescriptorInfo >::Type > createUpdate() const
        {
            auto updatedDescriptor = std::make_shared< typename UpdateDescriptor< TNewDescriptorInfo >::Type >
            (
                instanceType(),
                typeAliases(),
                dependencyFactories(),
                activationHandlers()
            );

            return updatedDescriptor;
        }

        std::shared_ptr< IRegistration > describe() const override
        {
            HYPODERMIC_THROW_REGISTRATION_EXCEPTION("Unable to describe this non default constructible type '" << this->instanceType().fullyQualifiedName() << "' which is " << TDescriptorInfo::toString());
        }
    };

} // namespace Hypodermic