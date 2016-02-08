#pragma once

#include "Hypodermic/ConstructorDescriptor.h"
#include "Hypodermic/Log.h"

#include "Hypodermic/As.h"
#include "Hypodermic/AsSelf.h"
#include "Hypodermic/OnActivated.h"
#include "Hypodermic/RegistrationBuilder.h"
#include "Hypodermic/RegistrationDescriptorBase.h"
#include "Hypodermic/SingleInstance.h"


namespace Hypodermic
{

    template <class TDescriptorInfo>
    class DefaultConstructibleRegistrationDescriptor : public RegistrationDescriptorBase< DefaultConstructibleRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                       public RegistrationDescriptorOperations::As< DefaultConstructibleRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                       public RegistrationDescriptorOperations::AsSelf< DefaultConstructibleRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                       public RegistrationDescriptorOperations::OnActivated< DefaultConstructibleRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >,
                                                       public RegistrationDescriptorOperations::SingleInstance< DefaultConstructibleRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >
    {
        friend class RegistrationDescriptorOperations::As< DefaultConstructibleRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::AsSelf< DefaultConstructibleRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::OnActivated< DefaultConstructibleRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::SingleInstance< DefaultConstructibleRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo >;

    public:
        typedef RegistrationDescriptorBase< DefaultConstructibleRegistrationDescriptor< TDescriptorInfo >, TDescriptorInfo > BaseType;

        typedef typename TDescriptorInfo::InstanceType InstanceType;

        template <class TNewDescriptorInfo>
        struct UpdateDescriptor
        {
            typedef DefaultConstructibleRegistrationDescriptor< TNewDescriptorInfo > Type;
        };

    public:
        DefaultConstructibleRegistrationDescriptor()
            : BaseType(Utils::getMetaTypeInfo< InstanceType >())
        {
        }

        DefaultConstructibleRegistrationDescriptor(const TypeInfo& instanceType,
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
                instanceType(),
                typeAliases(),
                dependencyFactories(),
                activationHandlers()
            );

            return updatedDescriptor;
        }

        std::shared_ptr< IRegistration > describe() const override
        {
            HYPODERMIC_LOG_INFO("Describing " << TDescriptorInfo::toString());

            return RegistrationBuilder< TDescriptorInfo >::build
            (
                instanceType(),
                typeAliases(),
                instanceFactory(),
                dependencyFactories(),
                activationHandlers()
            );
        }

    private:
        std::function< std::shared_ptr< void >(Container&) > instanceFactory() const
        {
            auto&& factory = Traits::ConstructorDescriptor< InstanceType() >::describe();

            return [factory](Container& container)
            {
                return std::static_pointer_cast< void >(factory(container));
            };
        }
    };

} // namespace Hypodermic