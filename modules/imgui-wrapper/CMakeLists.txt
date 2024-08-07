cmake_minimum_required(VERSION 3.15)

include_guard(GLOBAL)

project(imgui_wrapper
  VERSION 0.1.0
  LANGUAGES C CXX
)
set(PROJECT_NAMESPACE corona::)

include(CMakePrintHelpers)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(floppy REQUIRED)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAMESPACE}${PROJECT_NAME} ALIAS ${PROJECT_NAME})

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

target_sources(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/detail/export.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/detail/export.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/detail/raii.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/detail/raii.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/drawable.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/drawable.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/palette.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/palette.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/text.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/text.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/window.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/window.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/collapsing_header.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/collapsing_header.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/combo.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/combo.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/group.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/group.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/list_box.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/list_box.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/menu.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/menu.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/modifiers.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/modifiers.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/palette.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/palette.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/popup.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/popup.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/tab.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/tab.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/table.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/table.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/tooltip.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/tooltip.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/modules/imgui_wrapper/tree.h> $<INSTALL_INTERFACE:include/corona/modules/imgui_wrapper/tree.h>
)

target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
  INTERFACE
    floppy::floppy
    corona::extern_imgui
)

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

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona> $<INSTALL_INTERFACE:include/corona> ; DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
export(TARGETS ${PROJECT_NAME} NAMESPACE ${PROJECT_NAMESPACE} FILE "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_TARGETS_FILE}")
install(EXPORT ${PROJECT_NAME}
  DESTINATION ${INSTALL_EXPORT_DESTINATION_DIR}
  NAMESPACE ${PROJECT_NAMESPACE}
  FILE ${CONFIG_TARGETS_FILE}
)
configure_package_config_file(${PROJECT_CONFIG_FILE_IN} ${PROJECT_CONFIG_FILE} INSTALL_DESTINATION ${INSTALL_EXPORT_DESTINATION_DIR})
write_basic_package_version_file(${CONFIG_VERSION_FILE} COMPATIBILITY AnyNewerVersion)
install(FILES ${PROJECT_CONFIG_FILE} ${CONFIG_VERSION_FILE} DESTINATION ${INSTALL_EXPORT_DESTINATION_DIR})

message(STATUS "[${PROJECT_NAME}] configuring ${PROJECT_NAME} done!")
unset(TESTS CACHE)
