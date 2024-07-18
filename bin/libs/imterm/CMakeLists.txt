cmake_minimum_required(VERSION 3.12)
project(ImTerm VERSION 0.1.0 LANGUAGES CXX)

find_package(spdlog REQUIRED)

add_library(${PROJECT_NAME} INTERFACE)
add_library(corona-standalone::terminal ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} INTERFACE spdlog::spdlog)
