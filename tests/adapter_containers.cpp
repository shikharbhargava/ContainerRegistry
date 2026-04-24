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

TEST_F(ContainerRegistryTest, RegisterPriorityQueue)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  std::priority_queue<int> pq;
  pq.push(5);
  pq.push(1);
  pq.push(3);
  pq.push(7);

  registry.register_container("int_pqueue", pq);

  auto all_sizes = registry.compute_all();
  // priority_queue has top() not front(), so it is detected as stack_like
  // compute_size_recursive returns pq.size()
  EXPECT_EQ(all_sizes["int_pqueue"], 4);
  EXPECT_EQ(compute_size_recursive(pq), 4);
}
