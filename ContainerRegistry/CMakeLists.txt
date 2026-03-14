cmake_minimum_required(VERSION 3.12)

add_library(ContainerRegistry INTERFACE)
target_include_directories(ContainerRegistry INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Install header files
install(DIRECTORY include/ 
  DESTINATION include
)

# Install library targets
install(TARGETS ContainerRegistry
  EXPORT ContainerRegistryTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
