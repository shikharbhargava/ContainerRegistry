cmake_minimum_required(VERSION 3.12)

# Compiler flags first
include(cmake/CompilerFlags.cmake)

# Read project version from VERSION file (single source of truth).
file(READ "${CMAKE_SOURCE_DIR}/VERSION" PROJECT_VERSION_STR)
string(STRIP "${PROJECT_VERSION_STR}" PROJECT_VERSION_STR)

project(container VERSION ${PROJECT_VERSION_STR} LANGUAGES CXX)

# Set default installation prefix to build/install/ContainerRegistry
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install/ContainerRegistry" CACHE PATH "Installation prefix" FORCE)
endif()

# Enable testing
enable_testing()

# Add dependencies (including googletest)
add_subdirectory(deps)

add_subdirectory(ContainerRegistry)
add_subdirectory(tests)

add_executable(container main/main.cpp)
target_link_libraries(container PRIVATE ContainerRegistry)

# Install targets
install(TARGETS container
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
