cmake_minimum_required(VERSION 3.15)

include_guard(GLOBAL)
include(CMakePrintHelpers)
include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

project(corona
  VERSION 2.0.0
  DESCRIPTION "Frontend and interop library for QuaSAR hardware"
  HOMEPAGE_URL "io.quasar-team.github"
  LANGUAGES C CXX
)
set(PROJECT_NAMESPACE quasar::)

option(CORONA_STANDALONE "Build corona as standalone application" OFF)
option(CORONA_TEST "Enable corona unit tests" OFF)
option(CORONA_SELF_SUFFICIENT "Do not require system boost and qt" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

if("${CMAKE_GENERATOR}" MATCHES "^Visual Studio")
  set(CMAKE_GENERATOR_PLATFORM "x64" CACHE STRING "" FORCE)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(floppy REQUIRED)
find_package(reflect-cpp REQUIRED)
find_package(magic_enum REQUIRED)
#find_package(Protobuf REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter)
find_package(Boost REQUIRED COMPONENTS system headers)

add_library(${PROJECT_NAME})
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

target_sources(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/image/metadata.cc
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    floppy::floppy
  PRIVATE
    reflect-cpp::reflect-cpp
    magic_enum::magic_enum
    #boost::boost
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DBOOST_ASIO_HAS_STD_INVOKE_RESULT=1)
endif()

if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 wsock32)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include> PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  message(STATUS "[${PROJECT_NAME}] adding compiler flags: -Wall -Wextra -Wpedantic -Werror")
  target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
  )
endif()

message(STATUS "[${PROJECT_NAME}] setting metadata definitions:")
if(BUILD_SHARED_LIBS)
  message(STATUS "[${PROJECT_NAME}] adding shared export symbols: CORONA_LIBRARY")
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DCORONA_LIBRARY=1)
else()
  message(STATUS "[${PROJECT_NAME}] adding static export symbols: CORONA_STATIC_LIBRARY")
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DCORONA_STATIC_LIBRARY=1)
endif()

target_compile_definitions(${PROJECT_NAME}
  PUBLIC
  -DCORONA_TARGET_NAME="${PROJECT_NAME}"
  -DCORONA_PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
  -DCORONA_PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
  -DCORONA_PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}
  -DCORONA_ORGANIZATION_NAME="QuaSAR Team"
  -DCORONA_ORGANIZATION_URL="${PROJECT_HOMEPAGE_URL}"
)

if(WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS
  )
endif()

if(CORONA_TEST)
  enable_testing()
  add_subdirectory(tests)
endif()

if(CORONA_STANDALONE)
  message(STATUS "[${PROJECT_NAME}] building corona as standalone executable")
  add_subdirectory(bin)
endif()

message(STATUS "[${PROJECT_NAME}] compile integration tests status: ${INTEGRATION_TESTS}")
message(STATUS "[${PROJECT_NAME}] installing ${PROJECT_NAME} in namespace ${PROJECT_NAMESPACE}")
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

install(DIRECTORY $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}> $<INSTALL_INTERFACE:include/${PROJECT_NAME}> ; DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
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
unset(CORONA_STANDALONE CACHE)
unset(CORONA_TEST CACHE)
unset(CORONA_SELF_SUFFICIENT CACHE)