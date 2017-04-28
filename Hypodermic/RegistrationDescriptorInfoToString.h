#pragma once

#include <sstream>
#include <string>
#include <type_traits>

#include "Hypodermic/DependencyFactoryTag.h"
#include "Hypodermic/MetaForEach.h"
#include "Hypodermic/MetaPair.h"
#include "Hypodermic/Pragmas.h"
#include "Hypodermic/ProvidedDependencyTag.h"
#include "Hypodermic/ProvidedInstanceDependencyTag.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    namespace Details
    {

        template <class TTag>
        struct TagToString;

        template <class TDependency>
        struct TagToString< Tags::DependencyFactory< TDependency > >
        {
            typedef TDependency Dependency;

            static std::string toString()
            {
                std::stringstream stream;
                stream << ".WithFactoryOf< " <<
                    Utils::getMetaTypeInfo< Dependency >().fullyQualifiedName() << " >";

                return stream.str();
            }
        };

        template <class TDependency, class TProvidedDependency>
        struct TagToString< Tags::ProvidedInstanceDependency< TDependency, TProvidedDependency > >
        {
            typedef TDependency Dependency;
            typedef TProvidedDependency ProvidedDependency;

            static std::string toString()
            {
                std::stringstream stream;
                stream << ".WithInstanceOf< " <<
                    Utils::getMetaTypeInfo< Dependency >().fullyQualifiedName() << ", " <<
                    Utils::getMetaTypeInfo< ProvidedDependency >().fullyQualifiedName() << " >";

                return stream.str();
            }
        };

        template <class TDependency, class TProvidedDependency>
        struct TagToString< Tags::ProvidedDependency< TDependency, TProvidedDependency > >
        {
            typedef TDependency Dependency;
            typedef TProvidedDependency ProvidedDependency;

            static std::string toString()
            {
                std::stringstream stream;
                stream << ".With< " <<
                    Utils::getMetaTypeInfo< Dependency >().fullyQualifiedName() << ", " <<
                    Utils::getMetaTypeInfo< ProvidedDependency >().fullyQualifiedName() << " >";

                return stream.str();
            }
        };

        struct RegisteredBaseToString
        {
            explicit RegisteredBaseToString(std::ostream& stream)
                : m_stream(stream)
            {
            }

            template <class TIdentity, class T>
            void operator()(const MetaPair< TIdentity, T >&)
            {
                m_stream << ".As< " << Utils::getMetaTypeInfo< TIdentity >().fullyQualifiedName() << " >";
            }

        private:
            std::ostream& m_stream;
        };

        struct DependencyToString
        {
            explicit DependencyToString(std::ostream& stream)
                : m_stream(stream)
            {
            }

            template <class TKey, class TDependencyTag>
            void operator()(const MetaPair< TKey, TDependencyTag >&) const
            {
                m_stream << TagToString< TDependencyTag >::toString();
            }

        private:
            std::ostream& m_stream;
        };

    } // namespace Details



    struct RegistrationDescriptorInfoToString
    {
        template <class TDescriptorInfo>
        static std::string toString()
        {
            typedef typename TDescriptorInfo::InstanceType InstanceType;
            typedef typename TDescriptorInfo::SelfRegistrationTag SelfRegistrationTag;
            typedef typename TDescriptorInfo::FallbackRegistrationTag FallbackRegistrationTag;
            typedef typename TDescriptorInfo::InstanceLifetime InstanceLifetime;
            typedef typename TDescriptorInfo::RegisteredBases RegisteredBases;
            typedef typename TDescriptorInfo::Dependencies Dependencies;

            std::stringstream stream;

            stream << "RegistrationOf< " << Utils::getMetaTypeInfo< InstanceType >().fullyQualifiedName() << " >";

            HYPODERMIC_PRAGMA_PUSH
            HYPODERMIC_IGNORE_CONDITIONAL_EXPRESSION_IS_CONSTANT

            if (std::is_same< SelfRegistrationTag, Tags::SelfRegistered >::value)
                stream << ".AsSelf";

            if (std::is_same< InstanceLifetime, PersistentInstance >::value)
                stream << ".SingleInstance";

            if (std::is_same< FallbackRegistrationTag, Tags::FallbackRegistration >::value)
                stream << ".UseIfNone";

            HYPODERMIC_PRAGMA_POP

            metaForEach< RegisteredBases >(Details::RegisteredBaseToString(stream));
            metaForEach< Dependencies >(Details::DependencyToString(stream));

            return stream.str();
        }
    };

} // namespace Hypodermic