#include "common.hpp"

// ============================================================
// TEST SUITE: Type Detection
// ============================================================

TEST_F(ContainerRegistryTest, VectorLikeTypeDetection)
{
  typedef std::vector<int> IntVector;
  typedef std::vector<std::string> StringVector;
  typedef std::deque<int> IntDeque;
  typedef std::list<int> IntList;
  typedef std::set<int> IntSet;
  
  EXPECT_TRUE(is_vector_like<IntVector>::value);
  EXPECT_TRUE(is_vector_like<StringVector>::value);
  EXPECT_TRUE(is_vector_like<IntDeque>::value);
  EXPECT_TRUE(is_vector_like<IntList>::value);
  EXPECT_TRUE(is_vector_like<IntSet>::value);
}

TEST_F(ContainerRegistryTest, MapLikeTypeDetection)
{
  typedef std::map<std::string, int> StringIntMap;
  typedef std::unordered_map<int, std::string> IntStringUMap;
  typedef std::vector<int> IntVector;
  
  EXPECT_TRUE(is_map_like<StringIntMap>::value);
  EXPECT_TRUE(is_map_like<IntStringUMap>::value);
  EXPECT_FALSE(is_map_like<IntVector>::value);
}

TEST_F(ContainerRegistryTest, QueueLikeTypeDetection)
{
  typedef std::queue<int> IntQueue;
  typedef std::vector<int> IntVector;
  typedef std::map<int, int> IntIntMap;
  
  EXPECT_TRUE(is_queue_like<IntQueue>::value);
  EXPECT_FALSE(is_queue_like<IntVector>::value);
  EXPECT_FALSE(is_queue_like<IntIntMap>::value);
}

TEST_F(ContainerRegistryTest, StackLikeTypeDetection)
{
  typedef std::stack<int> IntStack;
  typedef std::vector<int> IntVector;
  typedef std::queue<int> IntQueue;
  
  EXPECT_TRUE(is_stack_like<IntStack>::value);
  EXPECT_FALSE(is_stack_like<IntVector>::value);
  EXPECT_FALSE(is_stack_like<IntQueue>::value);
}

TEST_F(ContainerRegistryTest, PriorityQueueIsStackLike)
{
  typedef std::priority_queue<int> IntPQ;
  typedef std::queue<int> IntQueue;

  // priority_queue has top() not front(), so it is stack_like, not queue_like
  EXPECT_TRUE(is_stack_like<IntPQ>::value);
  EXPECT_FALSE(is_queue_like<IntPQ>::value);
  EXPECT_FALSE(is_vector_like<IntPQ>::value);
  EXPECT_FALSE(is_map_like<IntPQ>::value);
}

TEST_F(ContainerRegistryTest, AdditionalVectorLikeTypeDetection)
{
  typedef std::multiset<int> IntMultiset;
  typedef std::unordered_set<int> IntUnorderedSet;
  typedef std::array<int, 4> IntArray;

  EXPECT_TRUE(is_vector_like<IntMultiset>::value);
  EXPECT_TRUE(is_vector_like<IntUnorderedSet>::value);
  EXPECT_TRUE(is_vector_like<IntArray>::value);
}

TEST_F(ContainerRegistryTest, AdditionalMapLikeTypeDetection)
{
  typedef std::multimap<std::string, int> StringIntMultimap;
  typedef std::unordered_multimap<std::string, int> StringIntUnorderedMultimap;

  EXPECT_TRUE(is_map_like<StringIntMultimap>::value);
  EXPECT_TRUE(is_map_like<StringIntUnorderedMultimap>::value);
}

TEST_F(ContainerRegistryTest, SizedContainerTypeDetection)
{
  EXPECT_TRUE(is_sized_container<DataPacket>::value);
  EXPECT_TRUE(is_sized_container<BufferPool>::value);
  EXPECT_TRUE(is_sized_container<ResourceTracker>::value);

  // Standard containers should NOT be sized containers
  typedef std::map<int, int> IntIntMap;
  EXPECT_FALSE(is_sized_container<std::vector<int>>::value);
  EXPECT_FALSE(is_sized_container<IntIntMap>::value);
  EXPECT_FALSE(is_sized_container<std::queue<int>>::value);
  EXPECT_FALSE(is_sized_container<std::stack<int>>::value);
}
