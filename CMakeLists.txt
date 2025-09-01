cmake_minimum_required(VERSION 4.0)
project(raytracing2d)

set(CMAKE_CXX_STANDARD 23)

add_executable(raytracing2d main.cpp)

# Dependencies
include(FetchContent)
set(RAYLIB_VERSION 5.0)
FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
        FIND_PACKAGE_ARGS ${RAYLIB_VERSION} EXACT
)

# Removes examples
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")


#Fetches content of raylib and adds to the project files
FetchContent_MakeAvailable(raylib)

#Links libraries to the project
target_link_libraries(${PROJECT_NAME} raylib)

# All shader files in project/shaders
file(GLOB SHADER_FILES "${CMAKE_SOURCE_DIR}/shaders/*")

# Copy them one by one to build/shaders
foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(FILE_NAME ${SHADER_FILE} NAME)
    configure_file(${SHADER_FILE} ${CMAKE_BINARY_DIR}/shaders/${FILE_NAME} COPYONLY)
endforeach()
