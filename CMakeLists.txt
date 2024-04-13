cmake_minimum_required(VERSION 3.10)

project(ProgressMonitor)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Add all source files in the src directory to the executable
file(GLOB SOURCES "src/*.cpp")

# Add executable target
add_executable(ProgressMonitor ${SOURCES})
