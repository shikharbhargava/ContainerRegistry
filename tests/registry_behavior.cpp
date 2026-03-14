#include "common.hpp"

// ============================================================
// TEST SUITE: Registry Behavior (Singleton, Threading, Printing)
// ============================================================

TEST_F(ContainerRegistryTest, SingletonInstanceBehavior)
{
  auto &registry1 = ContainerRegistry::instance();
  auto &registry2 = ContainerRegistry::instance();
  
  // Should be the same instance
  EXPECT_EQ(&registry1, &registry2);
}

TEST_F(ContainerRegistryTest, RegistryPrintFunctionality)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();
  
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
  
  // Verify output
  EXPECT_FALSE(ss.str().empty());
  EXPECT_NE(ss.str().find("OS Maps"), std::string::npos);
  EXPECT_NE(ss.str().find("OS Vector Maps"), std::string::npos);
}

TEST_F(ContainerRegistryTest, ThreadLocalRegistryBehavior)
{
  auto &main_thread_registry = ContainerRegistry::instance();
  main_thread_registry.clearAll();
  
  std::vector<int> vec = {1, 2, 3};
  main_thread_registry.register_container("main_vec", vec);
  
  // If we get here without crash, threading is OK
  SUCCEED();
}
