#include <container_registry.hpp>
#include <iostream>
#include <map>

class DataPacket
{
public:
  std::vector<int> elements;

  size_t size() const
  {
    return elements.size();
  }
};

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

  std::queue<DataPacket> packet_queue;
  DataPacket p1; p1.elements = {1, 2, 3};
  packet_queue.push(p1);
  DataPacket p2; p2.elements = {4, 5, 6};
  packet_queue.push(p2);  
  // Custom lambda to compute total size of all DataPackets in the queue
  // We need to copy the queue because std::queue doesn't provide iterators
  // This could be inefficient for large queues, but it's just for demonstration
  containerRegistry.register_container("Packet Queue", packet_queue,
    [](const std::queue<DataPacket> &q) {
      std::queue<DataPacket> copy = q;
      size_t total = 0;
      while (!copy.empty())
      {
        total += copy.front().size();
        copy.pop();
      }
      return total;
    });

  containerRegistry.print();
  return 0;
}