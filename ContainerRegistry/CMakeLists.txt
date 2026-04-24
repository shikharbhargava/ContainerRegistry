cmake_minimum_required(VERSION 3.12)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/container_registry_version.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/generated/container_registry_version.hpp
  @ONLY
)

add_library(ContainerRegistry INTERFACE)
target_include_directories(ContainerRegistry INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>
    $<INSTALL_INTERFACE:include>
)

# Install header files
install(DIRECTORY include/ 
  DESTINATION include
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/generated/container_registry_version.hpp
  DESTINATION include
)

# Install library targets
install(TARGETS ContainerRegistry
  EXPORT ContainerRegistryTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
