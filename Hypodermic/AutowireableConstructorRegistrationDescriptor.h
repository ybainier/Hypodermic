#pragma once

#include "Hypodermic/As.h"
#include "Hypodermic/AsSelf.h"
#include "Hypodermic/ConstructorDescriptor.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/OnActivated.h"
#include "Hypodermic/RegistrationBuilder.h"
#include "Hypodermic/RegistrationDescriptorBase.h"
#include "Hypodermic/SingleInstance.h"
#include "Hypodermic/With.h"


namespace Hypodermic
{

    template <class TDescriptorInfo>
    class AutowireableConstructorRegistrationDescriptor : public RegistrationDescriptorBase< AutowireableConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                          public RegistrationDescriptorOperations::As< AutowireableConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                          public RegistrationDescriptorOperations::AsSelf< AutowireableConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                          public RegistrationDescriptorOperations::OnActivated< AutowireableConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                          public RegistrationDescriptorOperations::SingleInstance< AutowireableConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                          public RegistrationDescriptorOperations::With< AutowireableConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >
    {
        friend class RegistrationDescriptorOperations::As< AutowireableConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::AsSelf< AutowireableConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::OnActivated< AutowireableConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::SingleInstance< AutowireableConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::With< AutowireableConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;

    public:
        typedef RegistrationDescriptorBase< AutowireableConstructorRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo > BaseType;

        typedef typename TDescriptorInfo::InstanceType InstanceType;

        template <class TNewDescriptorInfo>
        struct UpdateDescriptor
        {
            typedef AutowireableConstructorRegistrationDescriptor< TNewDescriptorInfo > Type;
        };

    public:
        AutowireableConstructorRegistrationDescriptor()
            : BaseType(Utils::getMetaTypeInfo< InstanceType >())
        {
        }

        AutowireableConstructorRegistrationDescriptor(const TypeInfo& instanceType,
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
            auto&& factory = Traits::ConstructorDescriptor< InstanceType >::describe();

            return [factory](Container& container)
            {
                return std::static_pointer_cast< void >(factory(container));
            };
        }
    };

} // namespace Hypodermic