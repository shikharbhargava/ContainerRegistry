#include "common.hpp"

// ============================================================
// TEST SUITE: Custom Compute Functions
// ============================================================

TEST_F(ContainerRegistryTest, RegisterWithCustomComputeFunction)
{
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  // Custom function that doubles the size
  auto custom_fn = [](const std::vector<int> &v) {
    return v.size() * 2;
  };
  
  registry.register_container("custom_vec", vec, custom_fn);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["custom_vec"], 10); // 5 * 2 = 10
}

TEST_F(ContainerRegistryTest, RegisterWithCustomSummationFunction)
{
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  // Custom function that sums the elements
  auto sum_fn = [](const std::vector<int> &v) {
    int sum = 0;
    for (int val : v) sum += val;
    return sum;
  };
  
  registry.register_container("sum_vec", vec, sum_fn);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["sum_vec"], 15); // 1+2+3+4+5 = 15
}
