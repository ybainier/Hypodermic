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
    AutowiredConstructor.h
    BoostUuidHashFunctor.h
    CircularDependencyDetector.h
    ComponentRegistration.h
    ComponentRegistry.h
    ContainerActivator.h
    ContainerBuilder.h
    ContainerBuilder.hpp
    Container.h
    CurrentLifetimeScope.h
    DelegateActivator.h
    DelegateActivator.hpp
    DependencyResolutionException.h
    Helpers.h
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
    IRegistrationBuilder.h
    IResolveOperation.h
    ISharingLifetimeScope.h
    ITypeCaster.h
    LifetimeScope.h
    KeyedService.h
    NullptrWorkaround.h
    ProvidedInstanceActivator.h
    ProvidedInstanceActivator.hpp
    RegistrationBuilderFactory.h
    RegistrationBuilderFactory.hpp
    RegistrationBuilder.h
    RegistrationBuilder.hpp
    RegistrationData.h
    ResolveOperation.h
    RootScopeLifetime.h
    Service.h
    ServiceKey.h
    ServiceRegistrationInfo.h
    SingleRegistrationStyle.h
    TypeCaster.h
    TypeCaster.hpp
    TypeIndexWorkaround.h
    TypedService.h
)

set(Hypodermic_sources
    CircularDependencyDetector.cpp
    ComponentRegistration.cpp
    ComponentRegistry.cpp
    ContainerActivator.cpp
    ContainerBuilder.cpp
    Container.cpp
    CurrentLifetimeScope.cpp
    DependencyResolutionException.cpp
    InstanceActivator.cpp
    InstanceLookup.cpp
    KeyedService.cpp
    LifetimeScope.cpp
    RegistrationData.cpp
    ResolveOperation.cpp
    RootScopeLifetime.cpp
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
