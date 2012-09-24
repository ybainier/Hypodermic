project(Hypodermic)
cmake_minimum_required(VERSION 2.6)

set(HYPODERMIC_VERSION_MAJOR "0")
set(HYPODERMIC_VERSION_MINOR "1")
set(HYPODERMIC_VERSION_PATCH "2")
set(HYPODERMIC_VERSION
    "${HYPODERMIC_VERSION_MAJOR}.${HYPODERMIC_VERSION_MINOR}.${HYPODERMIC_VERSION_PATCH}"
)

enable_testing()

# enable gcc specific stuff
if(CMAKE_COMPILER_IS_GNUCXX)
    if (NOT CXX0x_ENABLED)
        message(STATUS "Checking for g++ version >= 4.5")
        execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
        
        if (GCC_VERSION VERSION_LESS 4.5)
            message(FATAL_ERROR "g++ version is ${GCC_VERSION}")
        endif()

        include(CheckCXXCompilerFlag)
        check_cxx_compiler_flag(-std=c++0x FLAG_CXX0x)
        if(FLAG_CXX0x)
            message(STATUS "C++0x features enabled")
            set(CXX0x_ENABLED ON CACHE INTERNAL "C++0x features enabled")
        else(FLAG_CXX0x)
            message(FATAL_ERROR "g++ does not support the -std=c++0x flag")
        endif(FLAG_CXX0x)
    endif()

    set(COMPILER_FLAGS "-std=c++0x")
    set(WARNING_FLAGS "-W -Wall")
    
    if (CMAKE_BUILD_TYPE STREQUAL "debug")
        message(STATUS "Enabling debugging symbols")
        set(COMPILER_FLAGS "${COMPILER_FLAGS} -g")
    else()
        set(COMPILER_FLAGS "${COMPILER_FLAGS} -O2")
    endif()
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
