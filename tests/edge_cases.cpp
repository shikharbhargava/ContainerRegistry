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

TEST_F(ContainerRegistryTest, ClearAllReturnsCount)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  std::vector<int> v1 = {1, 2, 3};
  std::vector<int> v2 = {4, 5};
  std::map<std::string, int> m = {{"a", 1}};

  registry.register_container("v1", v1);
  registry.register_container("v2", v2);
  registry.register_container("m", m);

  size_t count = registry.clearAll();
  EXPECT_EQ(count, 3);

  // Registry should now be empty
  auto all_sizes = registry.compute_all();
  EXPECT_TRUE(all_sizes.empty());
}

TEST_F(ContainerRegistryTest, RegisterSameContainerWithDifferentNames)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  std::vector<int> vec = {1, 2, 3, 4, 5};
  registry.register_container("alias_a", vec);
  registry.register_container("alias_b", vec);

  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["alias_a"], 5);
  EXPECT_EQ(all_sizes["alias_b"], 5);
}

TEST_F(ContainerRegistryTest, RegisterAfterClear)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  std::vector<int> v1 = {10, 20};
  registry.register_container("before_clear", v1);
  registry.clearAll();

  std::vector<int> v2 = {1, 2, 3};
  registry.register_container("after_clear", v2);

  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes.count("before_clear"), 0);
  EXPECT_EQ(all_sizes["after_clear"], 3);
}
