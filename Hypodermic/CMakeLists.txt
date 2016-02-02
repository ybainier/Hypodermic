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
)


install(FILES ${Hypodermic_headers} DESTINATION include/Hypodermic)

#########################################
# pkgconfig
#########################################
set(VERSION "${HYPODERMIC_VERSION}")
set(prefix "${CMAKE_INSTALL_PREFIX}")
set(exec_prefix "\${prefix}")
set(includedir "\${prefix}/include")
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/libhypodermic.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/libhypodermic.pc
@ONLY)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/libhypodermic.pc
    DESTINATION lib/pkgconfig
)


