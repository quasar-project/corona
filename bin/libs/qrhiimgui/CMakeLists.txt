cmake_minimum_required(VERSION 3.16)

project(qrhiimgui LANGUAGES CXX)

find_package(Qt6
  COMPONENTS
    Core
    Quick
    ShaderTools
  REQUIRED
)

add_library(${PROJECT_NAME} STATIC)
add_library(corona-standalone::qrhiimgui ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/qrhiimgui/qrhiimgui.h
  ${CMAKE_CURRENT_SOURCE_DIR}/qrhiimgui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/qrhiimgui/qrhiimguiitem.h
  ${CMAKE_CURRENT_SOURCE_DIR}/qrhiimguiitem.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::GuiPrivate
    Qt6::Quick
    Qt6::QuickPrivate
    Qt6::ShaderTools
)

qt6_add_shaders(${PROJECT_NAME} "imgui_shaders"
  PREFIX
    "/"
  BASE
    ${CMAKE_CURRENT_SOURCE_DIR}
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui.frag
)

