# This project uses C++20 named modules. Update CMake to at least 3.28.
cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
cmake_policy(SET CMP0155 NEW)

project(Corona
  VERSION 1.0.0
  DESCRIPTION "Front-end for QuaSAR hardware"
  HOMEPAGE_URL "io.github.quasar-team"
  LANGUAGES C CXX
)

#set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(spdlog REQUIRED)

find_package(QT NAMES Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS
  Core
  Quick
  Location
  Positioning
  Widgets
  QuickControls2
  Concurrent
)

qt_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PUBLIC
  FILE_SET
  CXX_MODULES
  FILES
    src/c++/launcher/qtlogginghandler.ixx
    src/c++/launcher/qtquickoptions.ixx
    src/c++/launcher/projectinfo.ixx
    src/c++/launcher/logconfig.ixx
    src/c++/launcher/launcher.ixx

    src/c++/map/provider/provider.ixx
)

target_sources(${PROJECT_NAME}
  PRIVATE
    src/c++/main.c++
    src/c++/afx
    src/c++/corona

    src/c++/global/definitions.h
    src/c++/global/type_aliases.h
    src/c++/global/log.h

    src/c++/application/corona.h
    src/c++/application/corona.c++
    src/c++/application/base.h

    src/qml/qml.qrc
    src/js/js.qrc
    resources/resources.qrc
)

qt_add_qml_module(${PROJECT_NAME}
  URI Application
  VERSION 1.0
  QML_FILES
    src/qml/Main.qml
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    Qt6::Core
    Qt6::Quick
    Qt6::Widgets
    Qt6::Location
    Qt6::Positioning
    Qt6::QuickControls2
    Qt6::Concurrent

#    qol::qol
    spdlog::spdlog
    #yaml::yaml_cpp
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    src/c++
)

target_compile_definitions(${PROJECT_NAME}
  PUBLIC
    -DPROJECT_NAME="${PROJECT_NAME}"
    -DPROJECT_VERSION="${PROJECT_VERSION}"
    -DPROJECT_COMPANY="Quasar Team"
    -DPROJECT_DOMAIN="${PROJECT_HOMEPAGE_URL}"
)

qt_generate_deploy_app_script(
  TARGET ${PROJECT_NAME}
  OUTPUT_SCRIPT deploy_script
  NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})