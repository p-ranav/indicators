cmake_minimum_required(VERSION 3.8)
project(indica VERSION 1.0.0 LANGUAGES CXX)
option(INDICA_BUILD_TESTS OFF)

include(GNUInstallDirs)

add_library(indica INTERFACE)
add_library(indica::indica ALIAS indica)

target_compile_features(indica INTERFACE cxx_std_11)
target_include_directories(indica INTERFACE
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

install(TARGETS indica EXPORT indicaConfig)
install(EXPORT indicaConfig
        NAMESPACE indica::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/indica)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/include/indica.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/indica)
