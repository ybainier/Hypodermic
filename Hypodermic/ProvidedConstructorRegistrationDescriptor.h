#pragma once

#include "Hypodermic/As.h"
#include "Hypodermic/AsSelf.h"
#include "Hypodermic/ConstructorDescriptor.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/OnActivated.h"
#include "Hypodermic/Registration.h"
#include "Hypodermic/RegistrationDescriptorBase.h"
#include "Hypodermic/SingleInstance.h"
#include "Hypodermic/With.h"


namespace Hypodermic
{

    template
    <
        class TDescriptorInfo,
        class TConstructorSignature
    >
    class ProvidedConstructorRegistrationDescriptor : public RegistrationDescriptorBase< ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TConstructorSignature >, TDescriptorInfo >,
                                                      public RegistrationDescriptorOperations::As< ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TConstructorSignature >, TDescriptorInfo >,
                                                      public RegistrationDescriptorOperations::AsSelf< ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TConstructorSignature >, TDescriptorInfo >,
                                                      public RegistrationDescriptorOperations::OnActivated< ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TConstructorSignature >, TDescriptorInfo >,
                                                      public RegistrationDescriptorOperations::SingleInstance< ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TConstructorSignature >, TDescriptorInfo >,
                                                      public RegistrationDescriptorOperations::With< ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TConstructorSignature >, TDescriptorInfo >
    {
        friend class RegistrationDescriptorOperations::As< ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TConstructorSignature >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::AsSelf< ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TConstructorSignature >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::OnActivated< ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TConstructorSignature >, TDescriptorInfo >;
        friend class RegistrationDescriptorOperations::SingleInstance< ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TConstructorSignature >, TDescriptorInfo >;
        template <class, class> friend class RegistrationDescriptorOperations::With;

    public:
        typedef RegistrationDescriptorBase< ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TConstructorSignature >, TDescriptorInfo > BaseType;

        typedef typename TDescriptorInfo::InstanceType InstanceType;

        template <class TNewDescriptorInfo>
        struct UpdateDescriptor
        {
            typedef ProvidedConstructorRegistrationDescriptor< TNewDescriptorInfo, TConstructorSignature > Type;
        };

    public:
        ProvidedConstructorRegistrationDescriptor()
            : BaseType(Utils::getMetaTypeInfo< InstanceType >())
        {
        }

        ProvidedConstructorRegistrationDescriptor(const TypeInfo& instanceType,
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
            HYPODERMIC_LOG_INFO("Describing " << TDescriptorInfo::toString() << ".UsingConstructor< " << Traits::ConstructorDescriptor< TConstructorSignature >::toString() << " >");

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
            auto&& factory = Traits::ConstructorDescriptor< TConstructorSignature >::describe();

            return [factory](Container& container)
            {
                return std::static_pointer_cast< void >(factory(container));
            };
        }
    };

} // namespace Hypodermic