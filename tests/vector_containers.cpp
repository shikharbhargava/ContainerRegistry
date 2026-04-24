#include "common.hpp"

// ============================================================
// TEST SUITE: Vector-Like Containers (vector, deque, list, set)
// ============================================================

TEST_F(ContainerRegistryTest, RegisterSimpleIntVector)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  std::vector<int> vec = {1, 2, 3, 4, 5};
  registry.register_container("int_vector", vec);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["int_vector"], 5);
  EXPECT_EQ(compute_size_recursive(vec), 5);
}

TEST_F(ContainerRegistryTest, RegisterVectorOfStrings)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  std::vector<std::string> vec = {"hello", "world", "test"};
  registry.register_container("string_vector", vec);
  
  auto all_sizes = registry.compute_all();
  // Total: "hello" (5) + "world" (5) + "test" (4) = 14 characters
  EXPECT_EQ(all_sizes["string_vector"], 14);
  EXPECT_EQ(compute_size_recursive(vec), 14);
}

TEST_F(ContainerRegistryTest, RegisterDeque)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  std::deque<int> deq = {1, 2, 3, 4};
  registry.register_container("deque", deq);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["deque"], 4);
  EXPECT_EQ(compute_size_recursive(deq), 4);
}

TEST_F(ContainerRegistryTest, RegisterList)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  std::list<int> lst = {1, 2, 3, 4, 5};
  registry.register_container("list", lst);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["list"], 5);
  EXPECT_EQ(compute_size_recursive(lst), 5);
}

TEST_F(ContainerRegistryTest, RegisterSetOfInts)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  std::set<int> s = {1, 2, 3, 4, 5};
  registry.register_container("int_set", s);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["int_set"], 5);
  EXPECT_EQ(compute_size_recursive(s), 5);
}

TEST_F(ContainerRegistryTest, RegisterArray)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  std::array<int, 4> arr = {10, 20, 30, 40};
  registry.register_container("int_array", arr);

  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["int_array"], 4);
  EXPECT_EQ(compute_size_recursive(arr), 4);
}

TEST_F(ContainerRegistryTest, RegisterMultiset)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  std::multiset<int> ms = {1, 2, 2, 3, 3, 3};
  registry.register_container("int_multiset", ms);

  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["int_multiset"], 6);
  EXPECT_EQ(compute_size_recursive(ms), 6);
}

TEST_F(ContainerRegistryTest, RegisterUnorderedSet)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  std::unordered_set<int> us = {1, 2, 3, 4};
  registry.register_container("int_uset", us);

  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["int_uset"], 4);
  EXPECT_EQ(compute_size_recursive(us), 4);
}
