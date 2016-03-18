#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Hypodermic/InstanceLifetime.h"
#include "Hypodermic/Registration.h"
#include "Hypodermic/PersistentInstanceRegistration.h"
#include "Hypodermic/ProvidedInstanceRegistration.h"
#include "Hypodermic/TypeAliasKey.h"


namespace Hypodermic
{

    template
    <
        class TRegistrationDescriptorInfo,
        class TInstanceLifetime = typename TRegistrationDescriptorInfo::InstanceLifetime
    >
    class RegistrationBuilder;


    template <class TRegistrationDescriptorInfo>
    class RegistrationBuilder< TRegistrationDescriptorInfo, TransientInstance >
    {
    private:
        typedef std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > > TypeAliases;
        typedef std::unordered_map< TypeInfo, std::function< std::shared_ptr< void >(Container&) > > DependencyFactories;

    public:
        static std::shared_ptr< IRegistration > build(const TypeInfo& instanceType,
                                                      const TypeAliases& typeAliases,
                                                      const std::function< std::shared_ptr< void >(Container&) >& instanceFactory,
                                                      const DependencyFactories& dependencyFactories,
                                                      const std::vector< std::function< void(Container&, const std::shared_ptr< void >&) > >& activationHandlers)
        {
            return std::make_shared< Registration >(instanceType, typeAliases, instanceFactory, dependencyFactories, activationHandlers);
        }
        
        template <class T>
        static std::shared_ptr< IRegistration > buildForProvidedInstance(const std::shared_ptr< T >& instance, const TypeAliases& typeAliases)
        {
            return std::make_shared< ProvidedInstanceRegistration< T > >(instance, typeAliases);
        }
    };


    template <class TRegistrationDescriptorInfo>
    class RegistrationBuilder< TRegistrationDescriptorInfo, PersistentInstance >
    {
    private:
        typedef std::unordered_map< TypeAliasKey, std::function< std::shared_ptr< void >(const std::shared_ptr< void >&) > > TypeAliases;
        typedef std::unordered_map< TypeInfo, std::function< std::shared_ptr< void >(Container&) > > DependencyFactories;

    public:
        static std::shared_ptr< IRegistration > build(const TypeInfo& instanceType,
                                                      const TypeAliases& typeAliases,
                                                      const std::function< std::shared_ptr< void >(Container&) >& instanceFactory,
                                                      const DependencyFactories& dependencyFactories,
                                                      const std::vector< std::function< void(Container&, const std::shared_ptr< void >&) > >& activationHandlers)
        {
            return std::make_shared< PersistentInstanceRegistration >
            (
                RegistrationBuilder< TRegistrationDescriptorInfo, TransientInstance >::build
                (
                    instanceType,
                    typeAliases,
                    instanceFactory,
                    dependencyFactories,
                    activationHandlers
                )
            );
        }

        template <class T>
        static std::shared_ptr< IRegistration > buildForProvidedInstance(const std::shared_ptr< T >& instance, const TypeAliases& typeAliases)
        {
            return std::make_shared< PersistentInstanceRegistration >
            (
                RegistrationBuilder< TRegistrationDescriptorInfo, TransientInstance >::template buildForProvidedInstance< T >
                (
                    instance,
                    typeAliases
                )
            );
        }
    };

} // namespace Hypodermic