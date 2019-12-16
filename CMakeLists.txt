cmake_minimum_required(VERSION 3.8)
project(indica VERSION 1.0.0 LANGUAGES CXX)
option(INDICA_BUILD_TESTS OFF)
option(SAMPLES "Build Samples" OFF)
option(DEMO "Build Demo" OFF)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

find_package(Threads REQUIRED)

add_library(indica INTERFACE)
add_library(indica::indica ALIAS indica)

target_compile_features(indica INTERFACE cxx_std_11)
target_include_directories(indica INTERFACE
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)
target_link_libraries(indica INTERFACE Threads::Threads)


if( DEMO )
    add_subdirectory(demo)
endif()

if( SAMPLES )
    add_subdirectory(samples)
endif()

configure_package_config_file(indicaConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/indicaConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/indica)

install(TARGETS indica EXPORT indicaTargets)
install(EXPORT indicaTargets
        FILE indicaTargets.cmake
        NAMESPACE indica::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/indica)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/indicaConfig.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/indica)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/indicators
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        USE_SOURCE_PERMISSIONS
        PATTERN "*.hpp")
