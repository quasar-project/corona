cmake_minimum_required(VERSION 3.25)

project(Corona
  VERSION 1.0.0
  DESCRIPTION "Front-end for QuaSAR hardware"
  HOMEPAGE_URL "io.github.quasar-team"
  LANGUAGES C CXX
)

include(cmake/all.cmake)

set_language_standard(23)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(leaf 0.3.0 CONFIG REQUIRED)
find_package(leafconf 0.1.0 CONFIG REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(QT NAMES Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS
  Core
  Quick
  Location
  Positioning
  Widgets
  QuickControls2
  Concurrent
  StateMachine
)

qt_add_executable(${PROJECT_NAME})
generate_meta_files(META_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/resources/icon.ico"
  "${PROJECT_SOURCE_DIR}/resources/windows_metafile.rc.in"
)

target_sources(${PROJECT_NAME}
  PRIVATE
    src/c++/main.c++
    src/c++/corona

    src/c++/config/config.h
    src/c++/config/config.c++
    src/c++/config/configqmlwrapper.h
    src/c++/config/configqmlwrapper.c++

    src/c++/application/corona.h
    src/c++/application/corona.c++
    src/c++/application/base.h

    src/c++/map/provider/providerqml.h
    src/c++/map/provider/providerqml.c++
    src/c++/map/provider/openstreetmapprovider.h
    src/c++/map/provider/openstreetmapprovider.c++
    src/c++/map/utils/geomath.h
    src/c++/map/utils/geomath.c++
    src/c++/map/utils/orthodrom.h
    src/c++/map/utils/orthodrom.c++

    src/c++/gui/theme/circular_reveal.h
    src/c++/gui/theme/circular_reveal.c++
    src/c++/gui/theme/themeprovider.h
    src/c++/gui/theme/themeprovider.c++
    src/c++/gui/theme/themeqmlwrapper.h
    src/c++/gui/theme/themeqmlwrapper.c++

    src/c++/launcher/qtlogginghandler.h
    src/c++/launcher/qtlogginghandler.c++
    src/c++/launcher/qtquickoptions.h
    src/c++/launcher/projectinfo.h
    src/c++/launcher/logconfig.h
    src/c++/launcher/logconfig.c++
    src/c++/launcher/launcher.h

    src/c++/network/localaddr.h
    src/c++/network/localaddr.c++
    src/c++/network/modules/powerswitch.h
    src/c++/network/modules/powerswitch.c++

    src/qml/qml.qrc
    src/js/js.qrc
    resources/resources.qrc
    ${META_FILES}
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
    Qt6::StateMachine

    leaf::leaf
    leaf::leafconf
    yaml-cpp::yaml-cpp
    tomlplusplus::tomlplusplus
)

target_include_directories(${PROJECT_NAME} PRIVATE src/c++)

target_compile_definitions(${PROJECT_NAME}
  PUBLIC
    -DPROJECT_NAME="${PROJECT_NAME}"
    -DPROJECT_VERSION="${PROJECT_VERSION}"
    -DPROJECT_COMPANY="Quasar Team"
    -DPROJECT_DOMAIN="${PROJECT_HOMEPAGE_URL}"
)

#enable_static_crt()

qt_generate_deploy_app_script(
  TARGET ${PROJECT_NAME}
  OUTPUT_SCRIPT deploy_script
  NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})