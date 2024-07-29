set(CORONA_STANDALONE_NAME ${PROJECT_NAME}-standalone)

set(CMAKE_AUTOMOC ON)
cmake_policy(SET CMP0100 NEW)

message(STATUS "[${CORONA_STANDALONE_NAME}] cleaning version...")
if(NOT PROJECT_VERSION_PATCH)
  set(PROJECT_VERSION_PATCH 0)
endif()

if(NOT PROJECT_VERSION_TWEAK)
  set(PROJECT_VERSION_TWEAK 0)
endif()

message(STATUS "[${CORONA_STANDALONE_NAME}] configuring os bundle...")
if(APPLE)
  set(OS_BUNDLE MACOSX_BUNDLE)
elseif(WIN32)
  set(OS_BUNDLE WIN32)
endif()

# -- dependencies --
find_package(tomlplusplus REQUIRED)
find_package(qdebugenv REQUIRED)
find_package(Qt6
  COMPONENTS
    Core
    Quick
    QuickControls2
    Positioning
    Location
    ShaderTools
  REQUIRED
)
add_subdirectory(libs/bootstrap-geoservice)

# -- executable --
qt_add_executable(${CORONA_STANDALONE_NAME})
set_target_properties(${CORONA_STANDALONE_NAME} PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  POSITION_INDEPENDENT_CODE ON
)
cmake_print_properties(TARGETS ${CORONA_STANDALONE_NAME} PROPERTIES
  CXX_STANDARD
  CXX_STANDARD_REQUIRED
  CXX_EXTENSIONS
  POSITION_INDEPENDENT_CODE
)

message(STATUS "[${CORONA_STANDALONE_NAME}] generating meta files...")
if(WIN32)
  set(CORONA_ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/icon.ico)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/windows_metafile.rc.in "windows_metafile.rc")
  set(CORONA_RES_FILES "windows_metafile.rc")
  set(CMAKE_RC_COMPILER_INIT windres)
  enable_language(RC)
  set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
  set(CORONA_OUTPUT_SET ${CORONA_RES_FILES})
endif()

qt6_add_shaders(${PROJECT_NAME} "imgui_shaders"
  PREFIX "/"
  BASE ${CMAKE_CURRENT_SOURCE_DIR}
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/imgui.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/imgui.frag
)

qt_add_qml_module(${CORONA_STANDALONE_NAME}
  URI "io.corona.standalone"
  VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
  QML_FILES
    qml/Main.qml
    qml/theme/CircularChanger.qml
    qml/map/MapView.qml
    qml/layouts/ApplicationMenuBar.qml
  RESOURCES
    resources/icon.svg
    resources/icon.png
    resources/icon.ico
    resources/themes/gruvbox.toml
    resources/icons/block.svg
    resources/icons/cog.svg
    resources/icons/dark.svg
    resources/icons/light.svg
    resources/icons/ruler.svg
    resources/icons/grid.svg
  RESOURCE_PREFIX "/"
  NO_RESOURCE_TARGET_PATH
)

target_sources(${CORONA_STANDALONE_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/corona-standalone/main.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/corona-standalone/app/class_logger.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/corona-standalone/app/app.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/corona-standalone/app/class_dirs_wrapper.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/corona-standalone/app/qml/qmlreg.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/corona-standalone/gui/theme/class_reveal.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/corona-standalone/gui/theme/class_theme.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/corona-standalone/gui/theme/values.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/corona-standalone/gui/theme/qml/class_theme_wrapper.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/corona-standalone/gui/immediate/terminal_commands.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/corona-standalone/map/qml/qmlreg.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/corona-standalone/map/class_map_view_manager.cc
    ${CORONA_OUTPUT_SET}
)

target_link_libraries(${CORONA_STANDALONE_NAME}
  PRIVATE
    corona
    corona::geoservice
    reflect-cpp::reflect-cpp
    magic_enum::magic_enum
    tomlplusplus::tomlplusplus
    qdebugenv::qdebugenv
    Qt::Core
    Qt::Gui
    Qt::GuiPrivate
    Qt::Quick
    Qt::QuickPrivate
    Qt::QuickControls2
    Qt::Positioning
    Qt::Location
)

target_compile_definitions(${CORONA_STANDALONE_NAME}
  PUBLIC
  -DCORONA_STANDALONE_TARGET_NAME="${PROJECT_NAME}"
  -DCORONA_STANDALONE_PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
  -DCORONA_STANDALONE_PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
  -DCORONA_STANDALONE_PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}
  -DCORONA_STANDALONE_ORGANIZATION_NAME="QuaSAR Team"
  -DCORONA_STANDALONE_ORGANIZATION_URL="${PROJECT_HOMEPAGE_URL}"
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_definitions(${CORONA_STANDALONE_NAME} PRIVATE -DBOOST_ASIO_HAS_STD_INVOKE_RESULT=1)
endif()

target_include_directories(${CORONA_STANDALONE_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/qrhiimgui
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/qrhiimgui/imgui
)

if(WIN32 AND ${CMAKE_CXX_COMPILER_ID} MATCHES "GNU|Clang|Mingw|MINGW" AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "[${CORONA_STANDALONE_NAME}] adding -mwindows linker opt (${CMAKE_CXX_COMPILER_ID})")
  target_link_options(${CORONA_STANDALONE_NAME} PRIVATE -mwindows)
endif()

# todo: this is not working
#if(WIN32)
#  message(STATUS "[${CORONA_STANDALONE_NAME}] adding -static-libgcc and -static-libstdc++ linker opt (${CMAKE_CXX_COMPILER_ID})")
#  target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
#endif()

if(WIN32)
  add_custom_command(TARGET ${CORONA_STANDALONE_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${CORONA_STANDALONE_NAME}> $<TARGET_FILE_DIR:${CORONA_STANDALONE_NAME}>
    COMMAND_EXPAND_LISTS
  )

  # copy lib${PROJECT_NAME}.dll folder with ${CORONA_STANDALONE_NAME}.exe
  if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" AND BUILD_SHARED_LIBS)
    message(STATUS "copying ${CMAKE_BINARY_DIR}/lib${PROJECT_NAME}.dll to ${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.dll")
    add_custom_command(TARGET ${CORONA_STANDALONE_NAME}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/lib${PROJECT_NAME}.dll" "${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.dll"
      COMMAND_EXPAND_LISTS
    )
  endif()
endif()