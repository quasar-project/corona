cmake_minimum_required(VERSION 3.15)

include_guard(GLOBAL)

project(corona-bootstrap-imrenderer
  VERSION 0.1.0
  DESCRIPTION "ImGui Renderer submodule for Corona"
  HOMEPAGE_URL "whs31.github.io/qdebugenv"
  LANGUAGES C CXX
)
set(PROJECT_NAMESPACE corona::)

include(CMakePrintHelpers)

option(CBI_DEV "Enable development mode" OFF)
option(CBI_BUILD_SHARED_LIBS "Build shared libraries" OFF)

if("${CMAKE_GENERATOR}" MATCHES "^Visual Studio")
  set(CMAKE_GENERATOR_PLATFORM "x64" CACHE STRING "" FORCE)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# -- dependencies --
find_package(floppy REQUIRED)
find_package(magic_enum REQUIRED)
find_package(Qt6
  COMPONENTS
    Core
    Quick
    ShaderTools
  REQUIRED
)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(CBI_BUILD_SHARED_LIBS)
  add_library(${PROJECT_NAME} SHARED)
else()
  add_library(${PROJECT_NAME} STATIC)
endif()
add_library(corona::imrenderer ALIAS ${PROJECT_NAME})

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
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/bootstrap/imrenderer/rhi/class_immediate_gui_bridge.h> $<INSTALL_INTERFACE:include/corona/bootstrap/imrenderer/rhi/class_immediate_gui_bridge.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/bootstrap/imrenderer/rhi/class_renderer.h> $<INSTALL_INTERFACE:include/corona/bootstrap/imrenderer/rhi/class_renderer.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/bootstrap/imrenderer/class_generic_renderer.h> $<INSTALL_INTERFACE:include/corona/bootstrap/imrenderer/class_generic_renderer.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/corona/bootstrap/imrenderer/class_extendable_renderer.h> $<INSTALL_INTERFACE:include/corona/bootstrap/imrenderer/class_extendable_renderer.h>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rhi/class_renderer.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rhi/class_immediate_gui_bridge.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/class_generic_renderer.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/class_extendable_renderer.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qml/qdebugenv_qml.qrc
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    corona::corona
    corona::extern_imgui
    corona::imterm
    floppy::floppy
    Qt::Core
    Qt::Gui
    Qt::Quick
  PRIVATE
    Qt::GuiPrivate
    Qt::QuickPrivate
    magic_enum::magic_enum
)

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>$<INSTALL_INTERFACE:include> PRIVATE src)

if(CBI_BUILD_SHARED_LIBS)
  message(STATUS "[${PROJECT_NAME}] adding shared export symbols: CBI_LIBRARY")
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DCBI_LIBRARY=1)
else()
  message(STATUS "[${PROJECT_NAME}] adding static export symbols: CBI_STATIC_LIBRARY")
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DCBI_STATIC_LIBRARY=1)
endif()

target_compile_definitions(${PROJECT_NAME}
  PUBLIC
    -DCBI_TARGET_NAME="${PROJECT_NAME}"
    -DCBI_PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    -DCBI_PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    -DCBI_PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  message(STATUS "[${PROJECT_NAME}] adding compiler flags: -Wall -Wextra -Wpedantic -Werror")
  target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
  )
endif()

qt6_add_shaders(${PROJECT_NAME} "imgui_shaders"
  PREFIX
    "/"
  BASE
    ${CMAKE_CURRENT_SOURCE_DIR}
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/imgui.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/imgui.frag
  OUTPUT_TARGETS CBI_OUTPUT_TARGETS
)
message(STATUS "[${PROJECT_NAME}] shaders: ${QDE_OUTPUT_TARGETS}")

if(CBI_DEV)
  add_subdirectory(bin)
endif()

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

install(TARGETS ${PROJECT_NAME} ${CBI_OUTPUT_TARGETS}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/qdebugenv> $<INSTALL_INTERFACE:include/qdebugenv> ; DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
export(
  TARGETS ${PROJECT_NAME} ${CBI_OUTPUT_TARGETS}
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
unset(TESTS CACHE)
