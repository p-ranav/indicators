cmake_minimum_required(VERSION 3.8)
project(indicators VERSION 1.5.0 LANGUAGES CXX DESCRIPTION "Activity Indicators for Modern C++")
option(INDICATORS_BUILD_TESTS OFF)
option(SAMPLES "Build Samples" OFF)
option(DEMO "Build Demo" OFF)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

find_package(Threads REQUIRED)

add_library(indicators INTERFACE)
add_library(indicators::indicators ALIAS indicators)

target_compile_features(indicators INTERFACE cxx_std_11)
target_include_directories(indicators INTERFACE
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)
target_link_libraries(indicators INTERFACE Threads::Threads)


if( DEMO )
    add_subdirectory(demo)
endif()

if( SAMPLES )
    add_subdirectory(samples)
endif()

configure_package_config_file(indicatorsConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/indicatorsConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/indicators)

configure_file(indicators.pc.in
  "${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/indicators.pc"
  @ONLY)

install(TARGETS indicators EXPORT indicatorsTargets)
install(EXPORT indicatorsTargets
        FILE indicatorsTargets.cmake
        NAMESPACE indicators::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/indicators)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/indicatorsConfig.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/indicators)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/indicators.pc
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig/)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/indicators
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        USE_SOURCE_PERMISSIONS
        PATTERN "*.hpp")
