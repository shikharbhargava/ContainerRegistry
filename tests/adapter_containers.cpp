#include "common.hpp"

// ============================================================
// TEST SUITE: Adapter Containers (queue, stack)
// ============================================================

TEST_F(ContainerRegistryTest, RegisterQueueOfInts)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  std::queue<int> q;
  q.push(1);
  q.push(2);
  q.push(3);
  
  registry.register_container("int_queue", q);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["int_queue"], 3);
  EXPECT_EQ(compute_size_recursive(q), 3);
}

TEST_F(ContainerRegistryTest, RegisterStackOfInts)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  std::stack<int> s;
  s.push(1);
  s.push(2);
  s.push(3);
  
  registry.register_container("int_stack", s);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["int_stack"], 3);
  EXPECT_EQ(compute_size_recursive(s), 3);
}
