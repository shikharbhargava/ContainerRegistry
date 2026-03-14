#include "common.hpp"

// ============================================================
// TEST SUITE: Edge Cases and Large Data
// ============================================================

TEST_F(ContainerRegistryTest, RegisterLargeVector)
{
  std::vector<int> large_vec;
  for (int i = 0; i < 10000; ++i)
  {
    large_vec.push_back(i);
  }
  
  EXPECT_EQ(compute_size_recursive(large_vec), 10000);
}

TEST_F(ContainerRegistryTest, RegisterNestedVectorsWithMixedSizes)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  std::vector<std::vector<int>> mixed_sizes = {
    {1},
    {2, 3, 4, 5},
    {},
    {6, 7}
  };
  
  registry.register_container("mixed_nested", mixed_sizes);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["mixed_nested"], 7);
  EXPECT_EQ(compute_size_recursive(mixed_sizes), 7);
}
