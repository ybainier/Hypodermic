#pragma once

#include "Hypodermic2/ConstructorDescriptor.h"
#include "Hypodermic2/Log.h"
#include "Hypodermic2/Registration.h"
#include "Hypodermic2/RegistrationDescriptorBase.h"
#include "Hypodermic2/With.h"


namespace Hypodermic2
{

    template
    <
        class TDescriptorInfo,
        class TConstructorSignature
    >
    class ProvidedConstructorRegistrationDescriptor : public RegistrationDescriptorBase
                                                        <
                                                            ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TConstructorSignature >,
                                                            TDescriptorInfo
                                                        >
                                                    , public RegistrationDescriptorOperations::With
                                                        <
                                                            ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TConstructorSignature >,
                                                            TDescriptorInfo
                                                        >
    {
        template <class, class> friend class RegistrationDescriptorOperations::With;

    public:
        typedef RegistrationDescriptorBase
        <
            ProvidedConstructorRegistrationDescriptor< TDescriptorInfo, TConstructorSignature >,
            TDescriptorInfo
        >
        BaseType;

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
                                                  const std::unordered_map< TypeInfo, std::function< std::shared_ptr< void >(Container&) > >& dependencyFactories)
            : BaseType(instanceType, typeAliases, dependencyFactories)
        {
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
            HYPODERMIC_LOG_INFO("Describing " << TDescriptorInfo::toString() << ".UsingConstructor< " << Traits::ConstructorDescriptor< TConstructorSignature >::toString() << " >");

            return RegistrationBuilder< TDescriptorInfo >::build
            (
                instanceType(),
                typeAliases(),
                instanceFactory(),
                dependencyFactories()
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

} // namespace Hypodermic2