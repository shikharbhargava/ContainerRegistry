cmake_minimum_required(VERSION 3.12)

# Enable testing
enable_testing()

# Create the test executable
add_executable(container_tests
  container_registry_test.cpp
)

# Link against ContainerRegistry and Google Test (from deps)
target_link_libraries(container_tests
  PRIVATE
    ContainerRegistry
    GTest::gtest_main
)

# Register the test with CTest
add_test(NAME ContainerRegistryTests COMMAND container_tests)
