#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "Hypodermic/Container.h"
#include "Hypodermic/ContainerInstanceRegistration.h"
#include "Hypodermic/IRegistrationScope.h"
#include "Hypodermic/Log.h"
#include "Hypodermic/RegistrationDescriptorBuilder.h"
#include "Hypodermic/RuntimeRegistrationBuilder.h"


namespace Hypodermic
{

    class ContainerBuilder
    {
        class Validator : public IRegistrationRegistry
        {
        public:
            void addRegistration(const std::shared_ptr< IRegistration >& registration) override
            {
                if (registration->typeAliases().empty())
                {
                    registeredTypeAliasKeys.insert(createKeyForType(registration->instanceType()));
                    return;
                }

                for (auto&& x : registration->typeAliases())
                    registeredTypeAliasKeys.insert(x.first);
            }

            std::unordered_set< TypeAliasKey > registeredTypeAliasKeys;
        };

    public:
        /// <summary>
        /// Register type T
        /// </summary>
        /// <param name="T">The type to register inside ContainerBuilder</param>
        /// <returns>A reference on a new IRegistrationDescriptor</returns>
        template <class T>
        typename RegistrationDescriptorBuilder::ForTypeConstruction< T >::Type& registerType()
        {
            typedef typename RegistrationDescriptorBuilder::ForTypeConstruction< T >::Type RegistrationDescriptorType;

            return finalizeRegistration(std::make_shared< RegistrationDescriptorType >());
        }

        /// <summary>
        /// Register a shared instance of type T
        /// </summary>
        /// <param name="instance">The shared instance to register inside ContainerBuilder</param>
        /// <returns>A reference on a new ProvidedInstanceRegistrationDescriptor</returns>
        template <class T>
        typename RegistrationDescriptorBuilder::ForProvidedInstance< T >::Type& registerInstance(const std::shared_ptr< T >& instance)
        {
            typedef typename RegistrationDescriptorBuilder::ForProvidedInstance< T >::Type RegistrationDescriptorType;

            return finalizeRegistration(std::make_shared< RegistrationDescriptorType >(instance));
        }

        /// <summary>
        /// Register a functor producing an instance of T
        /// </summary>
        /// <param name="instanceFactory">The functor to register inside ContainerBuilder</param>
        /// <returns>A reference on a new IRegistrationDescriptor</returns>
        template <class TCallable>
        typename RegistrationDescriptorBuilder::ForProvidedInstanceFactory< TCallable >::Type& registerInstanceFactory(const TCallable& instanceFactory)
        {
            typedef typename RegistrationDescriptorBuilder::ForProvidedInstanceFactory< TCallable >::Type RegistrationDescriptorType;

            return finalizeRegistration(std::make_shared< RegistrationDescriptorType >(instanceFactory));
        }

        /// <summary>
        /// Build a new container
        /// </summary>
        /// <returns>A shared pointer to a new Container</returns>
        std::shared_ptr< Container > build() const
        {
            HYPODERMIC_LOG_INFO("Building container");

            auto scope = std::make_shared< RegistrationScope >();
            build(*scope);

            return createAndRegisterContainerInstance(scope);
        }

        /// <summary>
        /// Update a container
        /// </summary>
        /// <param name="container">The container to update</param>
        void updateContainer(Container& container)
        {
            HYPODERMIC_LOG_INFO("Updating container");

            for (auto&& x : m_registrationDescriptors)
                container.updateRegistrationScope(m_buildActions[x]);
        }

        /// <summary>
        /// Build a new and nested container from a passed Container
        /// </summary>
        /// <param name="container">The Container from which to create a nested Container</param>
        /// <returns>A shared pointer to a new Container</returns>
        std::shared_ptr< Container > buildNestedContainerFrom(const Container& container) const
        {
            HYPODERMIC_LOG_INFO("Building nested container");

            auto scope = container.createNestedScope();
            build(*scope);

            return createAndRegisterContainerInstance(scope);
        }

        /// <summary>
        /// Add all registrations of a ContainerBuilder
        /// </summary>
        /// <param name="builder">The ContainerBuilder from which registrations are copied</param>
        void addRegistrations(const ContainerBuilder& builder)
        {
            m_registrationDescriptors.insert(std::end(m_registrationDescriptors), std::begin(builder.m_registrationDescriptors), std::end(builder.m_registrationDescriptors));
            m_buildActions.insert(std::begin(builder.m_buildActions), std::end(builder.m_buildActions));
        }

        void validate() const
        {
            HYPODERMIC_LOG_INFO("Validating container");

            Validator validator;
            build(validator);

            for (auto&& x : validator.registeredTypeAliasKeys)
            {
                HYPODERMIC_LOG_DEBUG("Validating resolution of " << x.typeAlias().toString());

                auto container = build();
                container->resolveErasedType(x);
            }
        }

    private:
        void registerContainerInstance(const std::shared_ptr< Container >& container, const std::shared_ptr< IRegistrationRegistry >& scope) const
        {
            scope->addRegistration(std::make_shared< ContainerInstanceRegistration >(container));
        }

        void build(IRegistrationRegistry& registrationRegistry) const
        {
            for (auto&& x : m_registrationDescriptors)
            {
                auto&& action = m_buildActions.find(x)->second;
                action(registrationRegistry);
            }
        }

        std::shared_ptr< Container > createAndRegisterContainerInstance(const std::shared_ptr< IRegistrationScope >& scope) const
        {
            auto&& container = Container::create(scope, std::make_shared< RuntimeRegistrationBuilder >());
            registerContainerInstance(container, scope);

            return container;
        }

        template <class TRegistrationDescriptor>
        TRegistrationDescriptor& finalizeRegistration(const std::shared_ptr< TRegistrationDescriptor >& registrationDescriptor)
        {
            m_buildActions[registrationDescriptor] = registrationDescriptor->getDescriptionFactory();
            m_registrationDescriptors.push_back(registrationDescriptor);

            listenToRegistrationDescriptorUpdates(registrationDescriptor);

            return *registrationDescriptor;
        }

        void listenToRegistrationDescriptorUpdates(const std::shared_ptr< IRegistrationDescriptor >& registrationDescriptor)
        {
            std::weak_ptr< IRegistrationDescriptor > weakDescriptor = registrationDescriptor;

            registrationDescriptor->registrationDescriptorUpdated().connect([this, weakDescriptor](const std::shared_ptr< IRegistrationDescriptor >& x)
            {
                auto descriptor = weakDescriptor.lock();
                descriptor->registrationDescriptorUpdated().disconnect_all_slots();

                m_buildActions.erase(descriptor);
                m_registrationDescriptors.insert
                (
                    m_registrationDescriptors.erase(std::find
                    (
                        std::begin(m_registrationDescriptors),
                        std::end(m_registrationDescriptors),
                        descriptor
                    )),
                    x
                );

                m_buildActions[x] = x->getDescriptionFactory();

                listenToRegistrationDescriptorUpdates(x);
            });
        }

    private:
        std::vector< std::shared_ptr< IRegistrationDescriptor > > m_registrationDescriptors;
        std::unordered_map< std::shared_ptr< IRegistrationDescriptor >, std::function< void(IRegistrationRegistry&) > > m_buildActions;
    };

} // namespace Hypodermic
