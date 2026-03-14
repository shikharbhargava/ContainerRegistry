#include <container_registry.hpp>
#include <iostream>
#include <map>

int main()
{
  std::map<std::string, std::string> os_maps;
  os_maps["ubuntu"] = "https://hub.docker.com/_/ubuntu";
  os_maps["nginx"] = "https://hub.docker.com/_/nginx";
  os_maps["redis"] = "https://hub.docker.com/_/redis";

  std::unordered_map<std::string, std::vector<char>> os_vec_map;
  for (const auto &pair : os_maps)
  {
    os_vec_map[pair.first] = std::vector<char>(pair.second.begin(), pair.second.end());
  }
  auto &containerRegistry = ContainerRegistry::instance();
  containerRegistry.register_container("OS Maps", os_maps);
  containerRegistry.register_container("OS Vector Maps", os_vec_map);
  containerRegistry.print();
  return 0;
}