# Require cmake 3.25
cmake_minimum_required(VERSION 3.22)
project(gl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14) # Require C++14
set(CMAKE_CXX_EXTENSIONS OFF) # Turn off compiler extensions to avoid bugs
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Ensures that we have a defined C++ standard
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Create complie_commands.json so nvim can read the cmake include paths
enable_language(C) # This enables comiling for the glad library written in C

# Add GLFW CMake source location
add_subdirectory(lib/glfw)

# Set the source files to compile
set(SOURCE_FILES
   lib/glad/src/glad.c
   src/app/main.cpp
   src/app/gl.cpp
   src/app/glObject.cpp
   src/utils/random.cpp
   src/utils/data.cpp
   src/utils/matrix.cpp)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Include other include directories for header files
target_include_directories(${PROJECT_NAME} PRIVATE 
   ${CMAKE_SOURCE_DIR}/src
   ${CMAKE_SOURCE_DIR}/src/app
   ${CMAKE_SOURCE_DIR}/lib
   ${CMAKE_SOURCE_DIR}/lib/glad/include)

# Link the GLFW library
target_link_libraries(${PROJECT_NAME} glfw)
