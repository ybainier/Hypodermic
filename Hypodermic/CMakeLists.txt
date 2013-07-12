project(Hypodermic)
cmake_minimum_required(VERSION 2.6)


find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else(Boost_FOUND)
    message(FATAL_ERROR "Boost not found")
endif()


include_directories("..")

set(Hypodermic_headers
    ActivatedData.h
    ActivatingData.h
    AutowiredConstructor.h
    BoostUuidHashFunctor.h
    CircularDependencyDetector.h
    ComponentRegistration.h
    ComponentRegistrationLifetimeDecorator.h
    ComponentRegistry.h
    ContainerActivator.h
    ContainerBuilder.h
    ContainerBuilder.hpp
    Container.h
    CopyOnWriteRegistry.h
    CurrentLifetimeScope.h
    DelegateActivator.h
    DelegateActivator.hpp
    DependencyResolutionException.h
    ExternalRegistrySource.h
    ForwardTypeCaster.h
    Helpers.h
    IActivatedData.h
    IActivatingData.h
    IComponentContext.h
    IComponentContext.hpp
    IComponentLifetime.h
    IComponentRegistration.h
    IComponentRegistry.h
    IContainer.h
    IInitializable.h
    IInstanceActivator.h
    IInstanceLookup.h
    ILifetimeScope.h
    InstanceActivator.h
    InstanceLookup.h
    InstanceOwnership.h
    InstanceSharing.h
    IPreparingData.h
    IRegistrationBuilder.h
    IRegistrationSource.h
    IResolveOperation.h
    ISharingLifetimeScope.h
    ITypeCaster.h
    KeyedService.h
    libsel.h
    LifetimeScope.h
    MatchingScopeLifetime.h
    NullptrWorkaround.h
    PreparingData.h
    ProvidedInstanceActivator.h
    ProvidedInstanceActivator.hpp
    RegistrationBuilderFactory.h
    RegistrationBuilderFactory.hpp
    RegistrationBuilder.h
    RegistrationBuilder.hpp
    RegistrationData.h
    ResolveOperation.h
    RootScopeLifetime.h
    ScopeRestrictedRegistry.h
    Service.h
    ServiceKey.h
    ServiceRegistrationInfo.h
    SingleRegistrationStyle.h
    TypeCaster.h
    TypeCaster.hpp
    TypedService.h
    TypeIndexWorkaround.h
)

set(Hypodermic_sources
    CircularDependencyDetector.cpp
    ComponentRegistration.cpp
    ComponentRegistrationLifetimeDecorator.cpp
    ComponentRegistry.cpp
    ContainerActivator.cpp
    ContainerBuilder.cpp
    Container.cpp
    CopyOnWriteRegistry.cpp
    CurrentLifetimeScope.cpp
    DependencyResolutionException.cpp
    ExternalRegistrySource.cpp
    ForwardTypeCaster.cpp
    InstanceActivator.cpp
    InstanceLookup.cpp
    KeyedService.cpp
    LifetimeScope.cpp
    MatchingScopeLifetime.cpp
    RegistrationData.cpp
    ResolveOperation.cpp
    RootScopeLifetime.cpp
    ScopeRestrictedRegistry.cpp
    ServiceKey.cpp
    ServiceRegistrationInfo.cpp
    SingleRegistrationStyle.cpp
    TypedService.cpp
)

add_library(HypodermicShared SHARED ${Hypodermic_sources})
set_target_properties(HypodermicShared PROPERTIES OUTPUT_NAME hypodermic)
set_target_properties(HypodermicShared PROPERTIES VERSION ${HYPODERMIC_VERSION})
set_target_properties(HypodermicShared PROPERTIES SOVERSION ${HYPODERMIC_VERSION_MAJOR})

add_library(HypodermicStatic STATIC ${Hypodermic_sources})
set_target_properties(HypodermicStatic PROPERTIES OUTPUT_NAME hypodermic)

install(FILES ${Hypodermic_headers} DESTINATION include/Hypodermic)
install(TARGETS HypodermicShared HypodermicStatic
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
