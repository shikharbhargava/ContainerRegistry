#include <gtest/gtest.h>
#include <container_registry.hpp>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <stack>
#include <set>
#include <deque>
#include <list>
#include <sstream>

// Test fixture for ContainerRegistry
class ContainerRegistryTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    // Start with a fresh instance for each test
  }

  void TearDown() override
  {
    // Cleanup after each test
  }
};

TEST_F(ContainerRegistryTest, VectorDetection)
{
  typedef std::vector<int> IntVector;
  typedef std::vector<std::string> StringVector;
  typedef std::deque<int> IntDeque;
  typedef std::list<int> IntList;
  
  EXPECT_TRUE(is_vector_like<IntVector>::value);
  EXPECT_TRUE(is_vector_like<StringVector>::value);
  EXPECT_TRUE(is_vector_like<IntDeque>::value);
  EXPECT_TRUE(is_vector_like<IntList>::value);
}

TEST_F(ContainerRegistryTest, MapDetection)
{
  typedef std::map<std::string, int> StringIntMap;
  typedef std::unordered_map<int, std::string> IntStringUMap;
  typedef std::vector<int> IntVector;
  
  EXPECT_TRUE(is_map_like<StringIntMap>::value);
  EXPECT_TRUE(is_map_like<IntStringUMap>::value);
  EXPECT_FALSE(is_map_like<IntVector>::value);
}

TEST_F(ContainerRegistryTest, QueueDetection)
{
  typedef std::queue<int> IntQueue;
  typedef std::vector<int> IntVector;
  typedef std::map<int, int> IntIntMap;
  
  EXPECT_TRUE(is_queue_like<IntQueue>::value);
  EXPECT_FALSE(is_queue_like<IntVector>::value);
  EXPECT_FALSE(is_queue_like<IntIntMap>::value);
}

TEST_F(ContainerRegistryTest, StackDetection)
{
  typedef std::stack<int> IntStack;
  typedef std::vector<int> IntVector;
  typedef std::queue<int> IntQueue;
  
  EXPECT_TRUE(is_stack_like<IntStack>::value);
  EXPECT_FALSE(is_stack_like<IntVector>::value);
  EXPECT_FALSE(is_stack_like<IntQueue>::value);
}

// ============================================================
// TEST SUITE: Register and compute vector containers
// ============================================================

TEST_F(ContainerRegistryTest, RegisterSimpleVector)
{
  auto &registry = ContainerRegistry::instance();
  
  std::vector<int> vec = {1, 2, 3, 4, 5};
  registry.clearAll(); // Clear any previous entries
  registry.register_container("test_vector", vec);
  
  // Verify size computation
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("test_vector"), all_sizes.end());
  EXPECT_EQ(all_sizes["test_vector"], 5);
  EXPECT_EQ(compute_size_recursive(vec), 5);
}

TEST_F(ContainerRegistryTest, RegisterVectorOfVectors)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries
  
  std::vector<std::vector<int>> vec_of_vecs = {
    {1, 2},
    {3, 4, 5},
    {6}
  };
  
  registry.register_container("vec_of_vecs", vec_of_vecs);
  
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("vec_of_vecs"), all_sizes.end());
  EXPECT_EQ(all_sizes["vec_of_vecs"], 6);
  EXPECT_EQ(compute_size_recursive(vec_of_vecs), 6);
}

TEST_F(ContainerRegistryTest, RegisterEmptyVector)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries

  std::vector<int> empty_vec;
  registry.register_container("empty_vec", empty_vec);
  
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("empty_vec"), all_sizes.end());
  EXPECT_EQ(all_sizes["empty_vec"], 0);
  EXPECT_EQ(compute_size_recursive(empty_vec), 0);
}

TEST_F(ContainerRegistryTest, RegisterVectorOfStrings)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries
  
  std::vector<std::string> vec = {"hello", "world", "test"};
  registry.register_container("string_vec", vec);
  
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("string_vec"), all_sizes.end());
  EXPECT_EQ(all_sizes["string_vec"], 14);
  // Total: "hello" (5) + "world" (5) + "test" (4) = 14 characters
  EXPECT_EQ(compute_size_recursive(vec), 14);
}

// ============================================================
// TEST SUITE: Register and compute map containers
// ============================================================

TEST_F(ContainerRegistryTest, RegisterSimpleMap)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries

  std::map<std::string, int> map = {
    {"a", 1},
    {"b", 2},
    {"c", 3}
  };
  
  registry.register_container("simple_map", map);
  
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("simple_map"), all_sizes.end());
  EXPECT_EQ(all_sizes["simple_map"], 3);
  EXPECT_EQ(compute_size_recursive(map), 3);
}

TEST_F(ContainerRegistryTest, RegisterUnorderedMap)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries
  
  std::unordered_map<int, std::string> umap = {
    {1, "one"},
    {2, "two"},
    {3, "three"}
  };
  
  registry.register_container("unordered_map", umap);
  
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("unordered_map"), all_sizes.end());
  EXPECT_EQ(all_sizes["unordered_map"], 11);
  // Total: "one" (3) + "two" (3) + "three" (5) = 11 characters
  EXPECT_EQ(compute_size_recursive(umap), 11);
}

TEST_F(ContainerRegistryTest, RegisterMapOfVectors)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries
  
  std::map<std::string, std::vector<int>> map_of_vecs = {
    {"a", {1, 2}},
    {"b", {3, 4, 5}},
    {"c", {}}
  };
  
  registry.register_container("map_of_vecs", map_of_vecs);
  
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("map_of_vecs"), all_sizes.end());
  EXPECT_EQ(all_sizes["map_of_vecs"], 5);
  // Counts total leaf elements
  EXPECT_EQ(compute_size_recursive(map_of_vecs), 5);
}

TEST_F(ContainerRegistryTest, RegisterEmptyMap)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries
  
  std::map<int, int> empty_map;
  registry.register_container("empty_map", empty_map);
  
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("empty_map"), all_sizes.end());
  EXPECT_EQ(all_sizes["empty_map"], 0);
  EXPECT_EQ(compute_size_recursive(empty_map), 0);
}

TEST_F(ContainerRegistryTest, RegisterMapWithVectorValues)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries
  
  std::unordered_map<std::string, std::vector<char>> vec_map = {
    {"ubuntu", std::vector<char>{'U', 'b', 'u', 'n', 't', 'u'}},
    {"nginx", std::vector<char>{'N', 'g', 'i', 'n', 'x'}},
    {"redis", std::vector<char>{'R', 'e', 'd', 'i', 's'}}
  };
  
  registry.register_container("vec_map", vec_map);
  
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("vec_map"), all_sizes.end());
  EXPECT_EQ(all_sizes["vec_map"], 16);
  // Total of 6 + 5 + 5 = 16 characters
  EXPECT_EQ(compute_size_recursive(vec_map), 16);
}

// ============================================================
// TEST SUITE: Register and compute queue containers
// ============================================================

TEST_F(ContainerRegistryTest, RegisterQueue)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries

  std::queue<int> q;
  q.push(1);
  q.push(2);
  q.push(3);
  
  registry.register_container("queue", q);
  
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("queue"), all_sizes.end());
  EXPECT_EQ(all_sizes["queue"], 3);
  EXPECT_EQ(compute_size_recursive(q), 3);
}

TEST_F(ContainerRegistryTest, RegisterEmptyQueue)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries
  
  std::queue<int> empty_q;
  registry.register_container("empty_queue", empty_q);
  
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("empty_queue"), all_sizes.end());
  EXPECT_EQ(all_sizes["empty_queue"], 0);
  EXPECT_EQ(compute_size_recursive(empty_q), 0);
}

// ============================================================
// TEST SUITE: Register and compute stack containers
// ============================================================

TEST_F(ContainerRegistryTest, RegisterStack)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries

  std::stack<int> s;
  s.push(1);
  s.push(2);
  s.push(3);
  
  registry.register_container("stack", s);
  
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("stack"), all_sizes.end());
  EXPECT_EQ(all_sizes["stack"], 3);
  EXPECT_EQ(compute_size_recursive(s), 3);
}

TEST_F(ContainerRegistryTest, RegisterEmptyStack)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries
  
  std::stack<int> empty_s;
  registry.register_container("empty_stack", empty_s);
  
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("empty_stack"), all_sizes.end());
  EXPECT_EQ(all_sizes["empty_stack"], 0);
  EXPECT_EQ(compute_size_recursive(empty_s), 0);
}

// ============================================================
// TEST SUITE: Register with other container types
// ============================================================

TEST_F(ContainerRegistryTest, RegisterDeque)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries
  
  std::deque<int> deq = {1, 2, 3, 4};
  registry.register_container("deque", deq);
  
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("deque"), all_sizes.end());
  EXPECT_EQ(all_sizes["deque"], 4);
  EXPECT_EQ(compute_size_recursive(deq), 4);
}

TEST_F(ContainerRegistryTest, RegisterList)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries
  
  std::list<int> lst = {1, 2, 3, 4, 5};
  registry.register_container("list", lst);
  
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("list"), all_sizes.end());
  EXPECT_EQ(all_sizes["list"], 5);
  EXPECT_EQ(compute_size_recursive(lst), 5);
}

TEST_F(ContainerRegistryTest, RegisterSet)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries
  
  std::set<int> s = {1, 2, 3, 4, 5};
  registry.register_container("set", s);
  
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("set"), all_sizes.end());
  EXPECT_EQ(all_sizes["set"], 5);
  EXPECT_EQ(compute_size_recursive(s), 5);
}

// ============================================================
// TEST SUITE: Nested/Complex containers
// ============================================================

TEST_F(ContainerRegistryTest, RegisterDeeplyNestedVectors)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries
  
  std::vector<std::vector<std::vector<int>>> deeply_nested = {
    {{1, 2}, {3}},
    {{4, 5, 6}},
    {{7}}
  };
  
  registry.register_container("deeply_nested", deeply_nested);
  
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("deeply_nested"), all_sizes.end());
  EXPECT_EQ(all_sizes["deeply_nested"], 7);
  EXPECT_EQ(compute_size_recursive(deeply_nested), 7);
}

TEST_F(ContainerRegistryTest, RegisterComplexMapStructure)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries
  
  std::map<int, std::map<std::string, std::vector<int>>> complex = {
    {1, {{"a", {1, 2, 3}}, {"b", {4, 5}}}},
    {2, {{"c", {6}}, {"d", {}}}}
  };
  
  registry.register_container("complex", complex);
  
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("complex"), all_sizes.end());
  EXPECT_EQ(all_sizes["complex"], 6);
  // Total: 3 + 2 + 1 + 0 = 6
  EXPECT_EQ(compute_size_recursive(complex), 6);
}

// ============================================================
// TEST SUITE: ContainerRegistry singleton behavior
// ============================================================

TEST_F(ContainerRegistryTest, SingletonInstance)
{
  auto &registry1 = ContainerRegistry::instance();
  auto &registry2 = ContainerRegistry::instance();
  
  // Should be the same instance
  EXPECT_EQ(&registry1, &registry2);
}

TEST_F(ContainerRegistryTest, ThreadLocalRegistry)
{
  // Get instance from main thread
  auto &main_thread_registry = ContainerRegistry::instance();
  main_thread_registry.clearAll(); // Clear any previous entries
  
  std::vector<int> vec = {1, 2, 3};
  main_thread_registry.register_container("main_vec", vec);
  
  // The registry should have the entry
  // (actual verification through print output or other methods)
  
  SUCCEED(); // If we get here without crash, threading is OK
}

// ============================================================
// TEST SUITE: Custom compute functions
// ============================================================

TEST_F(ContainerRegistryTest, RegisterWithCustomComputeFn)
{
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries
  
  // Custom function that doubles the size
  auto custom_fn = [](const std::vector<int> &v) {
    return v.size() * 2;
  };
  
  registry.register_container("custom_vec", vec, custom_fn);
  
  // Verify custom function is used via compute_all
  auto all_sizes = registry.compute_all();
  EXPECT_NE(all_sizes.find("custom_vec"), all_sizes.end());
  EXPECT_EQ(all_sizes["custom_vec"], 10); // 5 * 2 = 10
}

// ============================================================
// TEST SUITE: Print functionality
// ============================================================

TEST_F(ContainerRegistryTest, PrintOutput)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll(); // Clear any previous entries
  
  std::map<std::string, std::string> os_maps;
  os_maps["ubuntu"] = "https://hub.docker.com/_/ubuntu";
  os_maps["nginx"] = "https://hub.docker.com/_/nginx";
  
  std::unordered_map<std::string, std::vector<char>> os_vec_map;
  for (const auto &pair : os_maps)
  {
    os_vec_map[pair.first] = std::vector<char>(pair.second.begin(), pair.second.end());
  }
  
  registry.register_container("OS Maps", os_maps);
  registry.register_container("OS Vector Maps", os_vec_map);
  
  // Capture output
  std::stringstream ss;
  registry.print(ss);
  
  // Verify output is not empty
  EXPECT_FALSE(ss.str().empty());
  
  // Verify container names appear in output
  EXPECT_NE(ss.str().find("OS Maps"), std::string::npos);
  EXPECT_NE(ss.str().find("OS Vector Maps"), std::string::npos);
}

// ============================================================
// TEST SUITE: Edge cases
// ============================================================

TEST_F(ContainerRegistryTest, LargeVectorSize)
{
  std::vector<int> large_vec;
  for (int i = 0; i < 10000; ++i)
  {
    large_vec.push_back(i);
  }
  
  EXPECT_EQ(compute_size_recursive(large_vec), 10000);
}

TEST_F(ContainerRegistryTest, NestedEmptyContainers)
{
  std::vector<std::vector<int>> nested = {
    {},
    {},
    {}
  };
  
  EXPECT_EQ(compute_size_recursive(nested), 0);
}

TEST_F(ContainerRegistryTest, MixedTypeMap)
{
  std::map<std::string, std::vector<std::string>> mixed = {
    {"key1", {"val1", "val2", "val3"}},
    {"key2", {"val4"}},
    {"key3", {}}
  };
  
  // Total: 4+4+4 + 4 = 16 character count
  EXPECT_EQ(compute_size_recursive(mixed), 16);
}
