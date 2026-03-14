#include "common.hpp"

// ============================================================
// TEST SUITE: Nested and Complex Data Structures
// ============================================================

TEST_F(ContainerRegistryTest, RegisterVectorOfVectors)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  std::vector<std::vector<int>> vec_of_vecs = {
    {1, 2},
    {3, 4, 5},
    {6}
  };
  
  registry.register_container("vec_of_vecs", vec_of_vecs);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["vec_of_vecs"], 6);
  EXPECT_EQ(compute_size_recursive(vec_of_vecs), 6);
}

TEST_F(ContainerRegistryTest, RegisterDeeplyNestedVectors)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  std::vector<std::vector<std::vector<int>>> deeply_nested = {
    {{1, 2}, {3}},
    {{4, 5, 6}},
    {{7}}
  };
  
  registry.register_container("deeply_nested", deeply_nested);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["deeply_nested"], 7);
  EXPECT_EQ(compute_size_recursive(deeply_nested), 7);
}

TEST_F(ContainerRegistryTest, RegisterMapOfVectors)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  std::map<std::string, std::vector<int>> map_of_vecs = {
    {"a", {1, 2}},
    {"b", {3, 4, 5}},
    {"c", {}}
  };
  
  registry.register_container("map_of_vecs", map_of_vecs);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["map_of_vecs"], 5);
  EXPECT_EQ(compute_size_recursive(map_of_vecs), 5);
}

TEST_F(ContainerRegistryTest, RegisterMapOfMapsWithVectorValues)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  std::map<int, std::map<std::string, std::vector<int>>> complex = {
    {1, {{"a", {1, 2, 3}}, {"b", {4, 5}}}},
    {2, {{"c", {6}}, {"d", {}}}}
  };
  
  registry.register_container("nested_map", complex);
  
  auto all_sizes = registry.compute_all();
  // Total: 3 + 2 + 1 + 0 = 6
  EXPECT_EQ(all_sizes["nested_map"], 6);
  EXPECT_EQ(compute_size_recursive(complex), 6);
}

TEST_F(ContainerRegistryTest, RegisterMapOfVectorsOfStrings)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  std::map<std::string, std::vector<std::string>> mixed = {
    {"key1", {"val1", "val2", "val3"}},
    {"key2", {"val4"}},
    {"key3", {}}
  };
  
  registry.register_container("map_string_vecs", mixed);
  
  auto all_sizes = registry.compute_all();
  // Total characters: 4+4+4 + 4 = 16
  EXPECT_EQ(all_sizes["map_string_vecs"], 16);
  EXPECT_EQ(compute_size_recursive(mixed), 16);
}
