cmake_minimum_required(VERSION 3.12)
project(ImTerm VERSION 0.1.0 LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)
add_library(corona-standalone::terminal ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE
    $<INSTALL_INTERFACE:include/>
)
