#pragma once

#include "Hypodermic/Log.h"
#include "Hypodermic/RegistrationBuilder.h"
#include "Hypodermic/RegistrationDescriptorBase.h"


namespace Hypodermic
{

    template <class TDescriptorInfo>
    class ProvidedInstanceRegistrationDescriptor : public RegistrationDescriptorBase
                                                    <
                                                        ProvidedInstanceRegistrationDescriptor< TDescriptorInfo >,
                                                        TDescriptorInfo
                                                    >
    {
        template <class> friend class ProvidedInstanceRegistrationDescriptor;

    public:
        typedef RegistrationDescriptorBase
        <
            ProvidedInstanceRegistrationDescriptor< TDescriptorInfo >,
            TDescriptorInfo
        >
        BaseType;

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
            updatedDescriptor->m_instance = m_instance;

            return updatedDescriptor;
        }

    protected:
        std::shared_ptr< IRegistration > describe() const override
        {
            HYPODERMIC_LOG_INFO("Describing " << TDescriptorInfo::toString());

            return RegistrationBuilder< TDescriptorInfo >::buildForProvidedInstance(m_instance, typeAliases());
        }

    private:
        std::shared_ptr< InstanceType > m_instance;
    };

} // namespace Hypodermic