#ifndef TEST_COMMON_HPP
#define TEST_COMMON_HPP

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

// ============================================================
// CUSTOM STRUCTURES WITH SIZE METHOD
// ============================================================

/**
 * Custom container class: DataPacket
 * A simple structure that holds a collection of data elements
 * and provides a size() method to retrieve element count.
 */
struct DataPacket
{
  std::vector<int> elements;
  
  size_t size() const
  {
    return elements.size();
  }
};

/**
 * Custom container class: BufferPool
 * Manages a pool of memory buffers where size() returns
 * the total number of buffers in the pool.
 */
class BufferPool
{
private:
  std::vector<std::vector<char>> buffers;

public:
  BufferPool() = default;

  void add_buffer(const std::vector<char> &buf)
  {
    buffers.push_back(buf);
  }

  size_t size() const
  {
    return buffers.size();
  }

  size_t total_bytes() const
  {
    size_t total = 0;
    for (const auto &buf : buffers)
    {
      total += buf.size();
    }
    return total;
  }
};

/**
 * Custom container class: ResourceTracker
 * Tracks resources where size() returns the count of resources.
 */
struct ResourceTracker
{
  std::map<std::string, int> resources;
  
  size_t size() const
  {
    return resources.size();
  }
};

// ============================================================
// TEST FIXTURE
// ============================================================

/**
 * Base test fixture for all ContainerRegistry tests
 */
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

#endif // TEST_COMMON_HPP
