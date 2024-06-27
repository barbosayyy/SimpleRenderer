cmake_minimum_required(VERSION 3.10)

project(SimpleRenderer VERSION 0.1.0)

message(${CMAKE_PROJECT_NAME})

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

set(PCH_FILE src/pch.h)

add_subdirectory(src)

add_executable(SimpleRenderer WIN32 src/main.cpp)

target_include_directories(SimpleRenderer PRIVATE external/include)

target_sources(SimpleRenderer PRIVATE ${PCH_FILE})
target_precompile_headers(SimpleRenderer PRIVATE ${PCH_FILE})

target_link_libraries(SimpleRenderer PRIVATE SimpleRendererSrc)
