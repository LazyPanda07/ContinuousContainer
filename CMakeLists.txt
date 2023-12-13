cmake_minimum_required(VERSION 3.10.0)

project(continuous_container CXX)

add_library(${PROJECT_NAME} STATIC src/ContinuousContainer.cpp)

set(CMAKE_CXX_STANDARD_REQUIRED 17)

if (MSVC)
    add_compile_options(/Zc:auto)
else()
    
endif()

target_include_directories(${PROJECT_NAME} PUBLIC include)
