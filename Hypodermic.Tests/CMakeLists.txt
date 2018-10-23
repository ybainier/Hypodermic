project(Hypodermic.Tests)
cmake_minimum_required(VERSION 2.6)


find_package(Boost COMPONENTS system unit_test_framework)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif()


set(HypodermicTests_sources

    main.cpp

    CircularDependencyTests.cpp
    ContainerBuilderTests.cpp
    ContainerTests.cpp
    DefaultConstructibleTests.cpp
    FactoryTests.cpp
    IsCompleteTests.cpp
    MemoryTests.cpp
    NamedTests.cpp
    NestedContainerTests.cpp
    PerformanceTests.cpp
    PersistentInstanceRegistrationTests.cpp
    ProvidedDependenciesTests.cpp
    ProvidedInstanceFactoryRegistrationTests.cpp
    ProvidedInstanceRegistrationTests.cpp
    RegistrationTests.cpp
    RuntimeRegistrationTests.cpp
    UseIfNoneTests.cpp
)

include_directories("..")

add_definitions(-DBOOST_TEST_DYN_LINK)

add_executable(Hypodermic.Tests ${HypodermicTests_sources})
target_link_libraries(Hypodermic.Tests ${Boost_LIBRARIES})

add_test(cmake_Hypodermic.Tests ${CMAKE_CURRENT_BINARY_DIR}/Hypodermic.Tests --result_code=no --report_level=no)

add_custom_command(TARGET Hypodermic.Tests POST_BUILD COMMAND ${CMAKE_CURRENT_BINARY_DIR}/Hypodermic.Tests)
