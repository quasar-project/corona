cmake_minimum_required(VERSION 3.25)

project(Corona
  VERSION 1.0.0
  DESCRIPTION "Front-end for QuaSAR hardware"
  HOMEPAGE_URL "io.github.quasar-team"
  LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

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

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PRIVATE
    src/c++/main.c++
    src/c++/corona.h
    src/c++/corona.c++

    src/qml/qml.qrc
    src/js/js.qrc
    resources/resources.qrc
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