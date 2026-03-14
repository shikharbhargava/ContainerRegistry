#include "common.hpp"

// ============================================================
// TEST SUITE: Map-Like Containers
// ============================================================

TEST_F(ContainerRegistryTest, RegisterSimpleMapWithIntValues)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  std::map<std::string, int> map = {
    {"a", 1},
    {"b", 2},
    {"c", 3}
  };
  
  registry.register_container("int_map", map);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["int_map"], 3);
  EXPECT_EQ(compute_size_recursive(map), 3);
}

TEST_F(ContainerRegistryTest, RegisterUnorderedMapWithStringValues)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  std::unordered_map<int, std::string> umap = {
    {1, "one"},
    {2, "two"},
    {3, "three"}
  };
  
  registry.register_container("string_umap", umap);
  
  auto all_sizes = registry.compute_all();
  // Total: "one" (3) + "two" (3) + "three" (5) = 11 characters
  EXPECT_EQ(all_sizes["string_umap"], 11);
  EXPECT_EQ(compute_size_recursive(umap), 11);
}

TEST_F(ContainerRegistryTest, RegisterMapWithVectorValues)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  std::unordered_map<std::string, std::vector<char>> vec_map = {
    {"ubuntu", std::vector<char>{'U', 'b', 'u', 'n', 't', 'u'}},
    {"nginx", std::vector<char>{'N', 'g', 'i', 'n', 'x'}},
    {"redis", std::vector<char>{'R', 'e', 'd', 'i', 's'}}
  };
  
  registry.register_container("vec_map", vec_map);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["vec_map"], 16);
  // Total: 6 + 5 + 5 = 16 characters
  EXPECT_EQ(compute_size_recursive(vec_map), 16);
}
