#include "common.hpp"

// ============================================================
// TEST SUITE: Custom Structures with size() Method
// ============================================================

TEST_F(ContainerRegistryTest, RegisterCustomDataPacket)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  DataPacket packet;
  packet.elements = {10, 20, 30, 40, 50};
  
  registry.register_container("data_packet", packet);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["data_packet"], 5);
  EXPECT_EQ(compute_size_recursive(packet), 5);
}

TEST_F(ContainerRegistryTest, RegisterCustomBufferPool)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  BufferPool pool;
  pool.add_buffer(std::vector<char>{'a', 'b', 'c'});
  pool.add_buffer(std::vector<char>{'d', 'e'});
  pool.add_buffer(std::vector<char>{'f', 'g', 'h', 'i'});
  
  registry.register_container("buffer_pool", pool);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["buffer_pool"], 3);
  EXPECT_EQ(compute_size_recursive(pool), 3);
}

TEST_F(ContainerRegistryTest, RegisterCustomResourceTracker)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  ResourceTracker tracker;
  tracker.resources["cpu"] = 8;
  tracker.resources["memory"] = 16;
  tracker.resources["disk"] = 512;
  
  registry.register_container("resource_tracker", tracker);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["resource_tracker"], 3);
  EXPECT_EQ(compute_size_recursive(tracker), 3);
}

TEST_F(ContainerRegistryTest, RegisterEmptyCustomStructures)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  DataPacket empty_packet;
  BufferPool empty_pool;
  ResourceTracker empty_tracker;
  
  registry.register_container("empty_packet", empty_packet);
  registry.register_container("empty_pool", empty_pool);
  registry.register_container("empty_tracker", empty_tracker);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["empty_packet"], 0);
  EXPECT_EQ(all_sizes["empty_pool"], 0);
  EXPECT_EQ(all_sizes["empty_tracker"], 0);
}

TEST_F(ContainerRegistryTest, CustomStructureWithCustomComputeFunction)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  BufferPool pool;
  pool.add_buffer(std::vector<char>{'a', 'b', 'c'});
  pool.add_buffer(std::vector<char>{'d', 'e'});
  
  // Custom function that returns total bytes instead of buffer count
  auto bytes_fn = [](const BufferPool &p) {
    return p.total_bytes();
  };
  
  registry.register_container("pool_bytes", pool, bytes_fn);
  
  auto all_sizes = registry.compute_all();
  EXPECT_EQ(all_sizes["pool_bytes"], 5); // 3 + 2 bytes
}

TEST_F(ContainerRegistryTest, CompareCustomFunctionWithSizeFunction)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
  BufferPool pool;
  pool.add_buffer(std::vector<char>{'a', 'b', 'c'});
  pool.add_buffer(std::vector<char>{'d', 'e'});
  pool.add_buffer(std::vector<char>{'f', 'g', 'h'});
  
  // Register with default size() function
  registry.register_container("pool_count", pool);
  
  // Register same pool with custom function that computes total bytes
  auto bytes_fn = [](const BufferPool &p) {
    return p.total_bytes();
  };
  registry.register_container("pool_total_bytes", pool, bytes_fn);
  
  auto size_fn = [](const BufferPool &p) {
    return p.size();
  };
  registry.register_container("pool_size_fn", pool, size_fn);

  auto all_sizes = registry.compute_all();
  
  // Default: returns buffer count (via size() method)
  EXPECT_EQ(all_sizes["pool_count"], 3);
  
  // Custom: returns total bytes
  EXPECT_EQ(all_sizes["pool_total_bytes"], 8); // 3 + 2 + 3 = 8 bytes
  
  // Verify they are different
  EXPECT_NE(all_sizes["pool_count"], all_sizes["pool_total_bytes"]);

  // Custom function that returns size() result should match default
  EXPECT_EQ(all_sizes["pool_size_fn"], all_sizes["pool_count"]);
}
