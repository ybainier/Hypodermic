#pragma once

#include "Hypodermic/ConstructorDescriptor.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/ProvidedConstructorRegistrationDescriptor.h"
#include "Hypodermic/RegistrationDescriptorBase.h"
#include "Hypodermic/RegistrationException.h"


namespace Hypodermic
{

    template <class TDescriptorInfo>
    class MissingConstructorRegistrationDescriptor : public RegistrationDescriptorBase
                                                        <
                                                            MissingConstructorRegistrationDescriptor< TDescriptorInfo >,
                                                            TDescriptorInfo
                                                        >
    {
    public:
        typedef RegistrationDescriptorBase
        <
            MissingConstructorRegistrationDescriptor< TDescriptorInfo >,
            TDescriptorInfo
        >
        BaseType;

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
                                                 const std::unordered_map< TypeInfo, std::function< std::shared_ptr< void >(Container&) > >& dependencyFactories)
            : BaseType(instanceType, typeAliases, dependencyFactories)
        {
        }

        template <class TSignature>
        ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TSignature >& usingConstructor()
        {
            auto updatedDescriptor = std::make_shared< ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TSignature > >
            (
                this->instanceType(),
                this->typeAliases(),
                this->dependencyFactories()
            );
            this->registrationDescriptorUpdated()(updatedDescriptor);

            return *updatedDescriptor;
        }

        template <class TNewDescriptorInfo>
        std::shared_ptr< typename UpdateDescriptor< TNewDescriptorInfo >::Type > createUpdate() const
        {
            auto updatedDescriptor = std::make_shared< typename UpdateDescriptor< TNewDescriptorInfo >::Type >
            (
                instanceType(),
                typeAliases(),
                dependencyFactories()
            );

            return updatedDescriptor;
        }

    protected:
        std::shared_ptr< IRegistration > describe() const override
        {
            HYPODERMIC_THROW_REGISTRATION_EXCEPTION("Unable to describe this non default constructible type '" << this->instanceType().fullyQualifiedName() << "' which is " << TDescriptorInfo::toString());
        }
    };

} // namespace Hypodermic