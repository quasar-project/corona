cmake_minimum_required(VERSION 3.22)

include_guard(GLOBAL)

project(qmlbind
  VERSION 0.1.0
  DESCRIPTION "QML bindibg generator submodule for Corona"
  HOMEPAGE_URL "whs31.github.io/qmlbind20"
  LANGUAGES C CXX
)
set(PROJECT_NAMESPACE corona::)
cmake_policy(SET CMP0100 NEW)

include(CMakePrintHelpers)

if("${CMAKE_GENERATOR}" MATCHES "^Visual Studio")
  set(CMAKE_GENERATOR_PLATFORM "x64" CACHE STRING "" FORCE)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# -- dependencies --
find_package(floppy REQUIRED)
find_package(Qt6
  COMPONENTS
    Core
    Quick
  REQUIRED
)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(${PROJECT_NAME} INTERFACE)
add_library(corona::qmlbind ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  POSITION_INDEPENDENT_CODE ON
)

cmake_print_properties(TARGETS ${PROJECT_NAME} PROPERTIES
  CXX_STANDARD
  CXX_STANDARD_REQUIRED
  CXX_EXTENSIONS
  POSITION_INDEPENDENT_CODE
)

target_sources(${PROJECT_NAME}
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/qmlbind/detail/export.h> $<INSTALL_INTERFACE:include/corona/modules/qmlbind/detail/export.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/qmlbind/literals.h> $<INSTALL_INTERFACE:include/corona/modules/qmlbind/literals.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/qmlbind/qmlbind.h> $<INSTALL_INTERFACE:include/corona/modules/qmlbind/qmlbind.h>
)

target_link_libraries(${PROJECT_NAME}
  INTERFACE
    floppy::floppy
    Qt::Core
    Qt::Quick
    Qt::Qml
)

target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>$<INSTALL_INTERFACE:include>)
cmake_print_properties(TARGETS ${PROJECT_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES)

message(STATUS "[${PROJECT_NAME}] installing ${PROJECT_NAME} in namespace ${PROJECT_NAMESPACE}")
include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
message(STATUS "[${PROJECT_NAME}] installing in prefix: ${CMAKE_INSTALL_PREFIX}")

set(PROJECT_CONFIG_FILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in")
set(PROJECT_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake")
set(CONFIG_TARGETS_FILE "${PROJECT_NAME}-config-targets.cmake")
set(CONFIG_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake")
set(INSTALL_EXPORT_DESTINATION_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

install(TARGETS ${PROJECT_NAME} ${QDE_OUTPUT_TARGETS}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona> $<INSTALL_INTERFACE:include/corona> ; DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
export(
  TARGETS ${PROJECT_NAME} ${QDE_OUTPUT_TARGETS}
  NAMESPACE ${PROJECT_NAMESPACE}
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_TARGETS_FILE}"
)
install(EXPORT ${PROJECT_NAME}
  DESTINATION ${INSTALL_EXPORT_DESTINATION_DIR}
  NAMESPACE ${PROJECT_NAMESPACE}
  FILE ${CONFIG_TARGETS_FILE}
)
configure_package_config_file(${PROJECT_CONFIG_FILE_IN} ${PROJECT_CONFIG_FILE} INSTALL_DESTINATION ${INSTALL_EXPORT_DESTINATION_DIR})
write_basic_package_version_file(${CONFIG_VERSION_FILE} COMPATIBILITY AnyNewerVersion)
install(FILES ${PROJECT_CONFIG_FILE} ${CONFIG_VERSION_FILE} DESTINATION ${INSTALL_EXPORT_DESTINATION_DIR})

message(STATUS "[${PROJECT_NAME}] configuring ${PROJECT_NAME} done!")
unset(CBG_DEV CACHE)
