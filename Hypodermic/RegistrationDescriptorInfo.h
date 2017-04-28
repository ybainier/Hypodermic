#pragma once

#include "Hypodermic/DependencyFactoryTag.h"
#include "Hypodermic/InstanceLifetime.h"
#include "Hypodermic/InstanceRegistrationTags.h"
#include "Hypodermic/MetaContains.h"
#include "Hypodermic/MetaIdentity.h"
#include "Hypodermic/MetaInsert.h"
#include "Hypodermic/MetaMap.h"
#include "Hypodermic/MetaPair.h"
#include "Hypodermic/ProvidedDependencyTag.h"
#include "Hypodermic/ProvidedInstanceDependencyTag.h"
#include "Hypodermic/RegistrationDescriptorInfoToString.h"


namespace Hypodermic
{

    template
    <
        class T,
        InstanceLifetimes::InstanceLifetime Lifetime = InstanceLifetimes::Transient,
        class TSelfRegistrationTag = Tags::NotSelfRegistered,
        class TFallbackRegistrationTag = Tags::DefaultRegistration,
        class TRegisteredBases = MetaMap<>,
        class TDependencies = MetaMap<>
    >
    struct RegistrationDescriptorInfo
    {
        typedef T InstanceType;
        typedef std::integral_constant< InstanceLifetimes::InstanceLifetime, Lifetime > InstanceLifetime;
        typedef TSelfRegistrationTag SelfRegistrationTag;
        typedef TFallbackRegistrationTag FallbackRegistrationTag;
        typedef TRegisteredBases RegisteredBases;
        typedef TDependencies Dependencies;

        struct SingleInstance
        {
            typedef RegistrationDescriptorInfo
            <
                InstanceType,
                InstanceLifetimes::Persistent,
                SelfRegistrationTag,
                FallbackRegistrationTag,
                RegisteredBases,
                Dependencies
            >
            Type;
        };

        struct SelfRegistered
        {
            typedef RegistrationDescriptorInfo
            <
                InstanceType,
                InstanceLifetime::value,
                Tags::SelfRegistered,
                FallbackRegistrationTag,
                RegisteredBases,
                Dependencies
            >
            Type;
        };

        struct UseIfNone
        {
            typedef RegistrationDescriptorInfo
            <
                InstanceType,
                InstanceLifetime::value,
                SelfRegistrationTag,
                Tags::FallbackRegistration,
                RegisteredBases,
                Dependencies
            >
            Type;
        };

        template <class TBase>
        struct RegisterBase
        {
            typedef RegistrationDescriptorInfo
            <
                InstanceType,
                InstanceLifetime::value,
                SelfRegistrationTag,
                FallbackRegistrationTag,
                typename MetaInsert< RegisteredBases, MetaPair< TBase, MetaIdentity< TBase > > >::Type,
                Dependencies
            >
            Type;
        };

        template <class TBase>
        struct IsBaseRegistered : MetaContains< RegisteredBases, TBase >
        {
        };

        template <class TDependency>
        struct RegisterDependencyFactory
        {
            typedef RegistrationDescriptorInfo
            <
                InstanceType,
                InstanceLifetime::value,
                SelfRegistrationTag,
                FallbackRegistrationTag,
                RegisteredBases,
                typename MetaInsert< Dependencies, MetaPair< TDependency, Tags::DependencyFactory< TDependency > > >::Type
            >
            Type;
        };

        template <class TDependency, class TProvidedDependency>
        struct RegisterDependencyInstance
        {
            typedef RegistrationDescriptorInfo
            <
                InstanceType,
                InstanceLifetime::value,
                SelfRegistrationTag,
                FallbackRegistrationTag,
                RegisteredBases,
                typename MetaInsert< Dependencies, MetaPair< TDependency, Tags::ProvidedInstanceDependency< TDependency, TProvidedDependency > > >::Type
            >
            Type;
        };

        template <class TDependency, class TProvidedDependency>
        struct RegisterDependency
        {
            typedef RegistrationDescriptorInfo
            <
                InstanceType,
                InstanceLifetime::value,
                SelfRegistrationTag,
                FallbackRegistrationTag,
                RegisteredBases,
                typename MetaInsert< Dependencies, MetaPair< TDependency, Tags::ProvidedDependency< TDependency, TProvidedDependency > > >::Type
            >
            Type;
        };

        template <class TDependency>
        struct IsDependencyRegistered : MetaContains< Dependencies, TDependency >
        {
        };


        static std::string toString()
        {
            return RegistrationDescriptorInfoToString::toString< RegistrationDescriptorInfo< InstanceType, InstanceLifetime::value, SelfRegistrationTag, FallbackRegistrationTag, RegisteredBases, Dependencies > >();
        }
    };

} // namespace Hypodermic