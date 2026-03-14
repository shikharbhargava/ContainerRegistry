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
