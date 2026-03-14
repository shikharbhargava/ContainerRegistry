#include "common.hpp"

// ============================================================
// TEST SUITE: Empty Containers
// ============================================================

TEST_F(ContainerRegistryTest, RegisterEmptyContainers)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  std::vector<int> empty_vec;
  std::map<int, int> empty_map;
  std::queue<int> empty_queue;
  std::stack<int> empty_stack;
  
  registry.register_container("empty_vector", empty_vec);
  registry.register_container("empty_map", empty_map);
  registry.register_container("empty_queue", empty_queue);
  registry.register_container("empty_stack", empty_stack);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["empty_vector"], 0);
  EXPECT_EQ(all_sizes["empty_map"], 0);
  EXPECT_EQ(all_sizes["empty_queue"], 0);
  EXPECT_EQ(all_sizes["empty_stack"], 0);
}

TEST_F(ContainerRegistryTest, RegisterNestedEmptyContainers)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  std::vector<std::vector<int>> nested_empty = {
    {},
    {},
    {}
  };
  
  registry.register_container("nested_empty", nested_empty);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["nested_empty"], 0);
  EXPECT_EQ(compute_size_recursive(nested_empty), 0);
}
