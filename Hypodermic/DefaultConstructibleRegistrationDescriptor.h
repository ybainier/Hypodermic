#pragma once

#include "Hypodermic2/ConstructorDescriptor.h"
#include "Hypodermic2/Log.h"
#include "Hypodermic2/RegistrationBuilder.h"
#include "Hypodermic2/RegistrationDescriptorBase.h"


namespace Hypodermic2
{

    template <class TDescriptorInfo>
    class DefaultConstructibleRegistrationDescriptor : public RegistrationDescriptorBase
                                                        <
                                                            DefaultConstructibleRegistrationDescriptor< TDescriptorInfo >,
                                                            TDescriptorInfo
                                                        >
    {
    public:
        typedef RegistrationDescriptorBase
        <
            DefaultConstructibleRegistrationDescriptor< TDescriptorInfo >,
            TDescriptorInfo
        >
        BaseType;

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
            HYPODERMIC_LOG_INFO("Describing " << TDescriptorInfo::toString());

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
            auto&& factory = Traits::ConstructorDescriptor< InstanceType() >::describe();

            return [factory](Container& container)
            {
                return std::static_pointer_cast< void >(factory(container));
            };
        }
    };

} // namespace Hypodermic2