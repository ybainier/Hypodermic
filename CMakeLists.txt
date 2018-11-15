project(Hypodermic)
cmake_minimum_required(VERSION 2.6)

set(HYPODERMIC_VERSION_MAJOR "2")
set(HYPODERMIC_VERSION_MINOR "5")
set(HYPODERMIC_VERSION_PATCH "0")
set(HYPODERMIC_VERSION
    "${HYPODERMIC_VERSION_MAJOR}.${HYPODERMIC_VERSION_MINOR}.${HYPODERMIC_VERSION_PATCH}"
)

enable_testing()

# enable gcc specific stuff
if(CMAKE_COMPILER_IS_GNUCXX)
    if(NOT CXX11_ENABLED)
        message(STATUS "Checking for g++ version >= 4.8")
        execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
        
        if(GCC_VERSION VERSION_LESS 4.8)
            message(FATAL_ERROR "g++ version is ${GCC_VERSION}")
        endif()

        include(CheckCXXCompilerFlag)
        check_cxx_compiler_flag(-std=c++11 FLAG_CXX11)
        if(FLAG_CXX11)
            message(STATUS "C++11 features enabled")
            set(CXX11_ENABLED ON CACHE INTERNAL "C++11 features enabled")
        else()
            message(FATAL_ERROR "g++ does not support the -std=c++11 flag")
        endif()
    endif()

    set(COMPILER_FLAGS "-std=c++11")
    set(WARNING_FLAGS "-W -Wall")
    
    if(CMAKE_BUILD_TYPE STREQUAL "debug")
        message(STATUS "Enabling debugging symbols")
        set(COMPILER_FLAGS "${COMPILER_FLAGS} -g")
    else()
        set(COMPILER_FLAGS "${COMPILER_FLAGS} -O2")
    endif()

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
        add_definitions(-fPIC)
    endif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")

elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(NOT CXX11_ENABLED)
        include(CheckCXXCompilerFlag)
        check_cxx_compiler_flag(-std=c++11 FLAG_CXX11)
        if(FLAG_CXX11)
            message(STATUS "C++11 features enabled")
            set(CXX11_ENABLED ON CACHE INTERNAL "C++11 features enabled")
        else()
            message(FATAL_ERROR "Clang does not support the -std=c++11 flag")
        endif()
    endif()

    set(COMPILER_FLAGS "-std=c++11")
    set(WARNING_FLAGS "-W -Wall")
    
    if(CMAKE_BUILD_TYPE STREQUAL "debug")
        message(STATUS "Enabling debugging symbols")
        set(COMPILER_FLAGS "${COMPILER_FLAGS} -g")
    else()
        set(COMPILER_FLAGS "${COMPILER_FLAGS} -O2")
    endif()

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
        add_definitions(-fPIC)
    endif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS} ${WARNING_FLAGS}")


add_subdirectory(Hypodermic)
add_subdirectory(Hypodermic.Tests)


# uninstall target
##################
#

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)


# Packaging stuff
#################
#

set(CPACK_PACKAGE_VERSION_MAJOR "${HYPODERMIC_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${HYPODERMIC_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${HYPODERMIC_VERSION_PATCH}")

set(CPACK_SOURCE_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-${HYPODERMIC_VERSION}"
)

set(CPACK_SOURCE_GENERATOR "TBZ2")
set(CPACK_SOURCE_IGNORE_FILES
    "/.hg/"
    "/.hgignore$"
    "build/"
    "ipch/"
    "/resources/"
    ".sdf$;.suo$;.tss$"
)

include(CPack)
add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)
