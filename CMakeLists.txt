cmake_minimum_required(VERSION 3.8)

if(DEFINED PROJECT_NAME)
  set(INDICATORS_SUBPROJECT ON)
endif()

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.12")
  project(indicators VERSION 2.2.0 LANGUAGES CXX
    HOMEPAGE_URL "https://github.com/p-ranav/indicators"
    DESCRIPTION "Activity Indicators for Modern C++")
elseif(CMAKE_VERSION VERSION_GREATER_EQUAL "3.9")
  project(indicators VERSION 2.2.0 LANGUAGES CXX
    DESCRIPTION "Activity Indicators for Modern C++")
else()
  project(indicators VERSION 2.2.0 LANGUAGES CXX)
endif()

if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_basic_setup()
endif()

option(INDICATORS_BUILD_TESTS "Build indicators tests + enable CTest")
option(INDICATORS_SAMPLES "Build indicators samples")
option(INDICATORS_DEMO "Build indicators demo" OFF)

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

if(INDICATORS_DEMO)
  add_subdirectory(demo)
endif()

if(INDICATORS_SAMPLES)
  add_subdirectory(samples)
endif()

if(NOT INDICATORS_SUBPROJECT)
  configure_package_config_file(indicatorsConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/indicatorsConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/indicators)
  write_basic_package_version_file(indicatorsConfigVersion.cmake
                                   COMPATIBILITY AnyNewerVersion)

  configure_file(indicators.pc.in indicators.pc @ONLY)

  install(TARGETS indicators EXPORT indicatorsTargets)
  install(EXPORT indicatorsTargets
          FILE indicatorsTargets.cmake
          NAMESPACE indicators::
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/indicators)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/indicatorsConfig.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/indicatorsConfigVersion.cmake
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/indicators)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/indicators.pc
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
  install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/indicators
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
          USE_SOURCE_PERMISSIONS
          PATTERN "*.hpp")
  install(FILES LICENSE LICENSE.termcolor
          DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/licenses/indicators)

  if(EXISTS "${PROJECT_SOURCE_DIR}/.gitignore")
    # Simple glob to regex conversion (.gitignore => CPACK_SOURCE_IGNORE_FILES)
    file(READ ".gitignore" DOT_GITIGNORE)
    string(REPLACE ";" "RANDOMSEQUENCE" DOT_GITIGNORE "${DOT_GITIGNORE}")
    string(REPLACE "\n" ";" DOT_GITIGNORE "${DOT_GITIGNORE}")
    string(REPLACE "RANDOMSEQUENCE" "\\;" DOT_GITIGNORE "${DOT_GITIGNORE}")
    foreach(IGNORE_LINE ${DOT_GITIGNORE})
      if(NOT IGNORE_LINE OR IGNORE_LINE MATCHES "^#")
        continue()
      endif()
      string(REPLACE "\\" "\\\\" IGNORE_LINE "${IGNORE_LINE}")
      string(REPLACE "." "\\\\." IGNORE_LINE "${IGNORE_LINE}")
      string(REPLACE "*" ".*" IGNORE_LINE "${IGNORE_LINE}")
      string(REPLACE "+" "\\\\+" IGNORE_LINE "${IGNORE_LINE}")
      list(APPEND CPACK_SOURCE_IGNORE_FILES "${IGNORE_LINE}")
    endforeach()
  endif()

  # extra ignored files
  list(APPEND CPACK_SOURCE_IGNORE_FILES
    .editorconfig
    .git
    .gitignore
    .travis.yml
    .appveyor.yml
  )
  set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
  set(CPACK_GENERATOR "TGZ;TXZ")
  set(CPACK_SOURCE_GENERATOR "TGZ;TXZ")
  include(CPack)
endif()
