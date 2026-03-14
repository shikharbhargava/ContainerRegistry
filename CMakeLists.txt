cmake_minimum_required(VERSION 3.12)

# Compiler flags first
include(cmake/CompilerFlags.cmake)

project(container VERSION 0.1.0 LANGUAGES CXX)

# Enable testing
enable_testing()

# Add dependencies (including googletest)
add_subdirectory(deps)

add_subdirectory(ContainerRegistry)
add_subdirectory(tests)

add_executable(container main/main.cpp)
target_link_libraries(container PRIVATE ContainerRegistry)
