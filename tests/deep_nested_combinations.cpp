#include "common.hpp"

// ============================================================
// TEST SUITE: Deep Nested Container Chains
//
// Notation used in test names reflects the nesting chain, e.g.
//   map -> vector -> custom_struct
//   vector -> map -> vector
//   map -> vector -> map  (3 levels of standard containers)
// ============================================================

// ------------------------------------------------------------
// Chain: map -> vector -> custom_struct (DataPacket)
//
// For each map value (vector<DataPacket>):
//   for each DataPacket element → DataPacket.size() (leaf)
// ------------------------------------------------------------
TEST_F(ContainerRegistryTest, ChainMapVectorCustomStruct)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  DataPacket p1; p1.elements = {1, 2, 3};    // size = 3
  DataPacket p2; p2.elements = {4, 5};       // size = 2
  DataPacket p3; p3.elements = {6};          // size = 1

  std::map<std::string, std::vector<DataPacket>> m;
  m["group_a"] = {p1, p2};   // 3 + 2 = 5
  m["group_b"] = {p3};       // 1

  registry.register_container("map_vec_packet", m);

  auto all_sizes = registry.compute_all();
  // Total: 5 + 1 = 6
  EXPECT_EQ(all_sizes["map_vec_packet"], 6);
  EXPECT_EQ(compute_size_recursive(m), 6);
}

// ------------------------------------------------------------
// Chain: vector -> map -> custom_struct (DataPacket)
//
// Each vector element is a map<string, DataPacket>.
// For each map → iterates values (DataPackets) → DataPacket.size()
// ------------------------------------------------------------
TEST_F(ContainerRegistryTest, ChainVectorMapCustomStruct)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  DataPacket p1; p1.elements = {1, 2};       // size = 2
  DataPacket p2; p2.elements = {3, 4, 5};    // size = 3
  DataPacket p3; p3.elements = {6};          // size = 1

  std::map<std::string, DataPacket> inner1;
  inner1["alpha"] = p1;
  inner1["beta"]  = p2;  // 2 + 3 = 5

  std::map<std::string, DataPacket> inner2;
  inner2["gamma"] = p3;  // 1

  std::vector<std::map<std::string, DataPacket>> v = {inner1, inner2};

  registry.register_container("vec_map_packet", v);

  auto all_sizes = registry.compute_all();
  // Total: 5 + 1 = 6
  EXPECT_EQ(all_sizes["vec_map_packet"], 6);
  EXPECT_EQ(compute_size_recursive(v), 6);
}

// ------------------------------------------------------------
// Chain: vector -> map -> vector (3-level standard containers)
//
// Outer vector elements are maps<string, vector<int>>.
// Inner vectors hold int leaf values.
// ------------------------------------------------------------
TEST_F(ContainerRegistryTest, ChainVectorMapVector)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  std::vector<std::map<std::string, std::vector<int>>> outer;

  outer.push_back({{"a", {1, 2}},    {"b", {3}}});         // 2 + 1 = 3
  outer.push_back({{"c", {4, 5, 6}}});                      // 3
  outer.push_back({{"d", {}},        {"e", {7, 8, 9, 10}}});// 0 + 4 = 4

  registry.register_container("vec_map_vec", outer);

  auto all_sizes = registry.compute_all();
  // Total: 3 + 3 + 4 = 10
  EXPECT_EQ(all_sizes["vec_map_vec"], 10);
  EXPECT_EQ(compute_size_recursive(outer), 10);
}

// ------------------------------------------------------------
// Chain: map -> vector -> map (3-level standard containers)
//
// Outer map values are vector<map<int,int>>.
// Innermost map values are int leaves.
// ------------------------------------------------------------
TEST_F(ContainerRegistryTest, ChainMapVectorMap)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  typedef std::map<int, int> InnerMap;
  std::map<std::string, std::vector<InnerMap>> outer;

  outer["outer_a"] = {{{1, 10}, {2, 20}},  // inner map: 2 int leaves
                      {{3, 30}}};           // inner map: 1 int leaf  → 3 total
  outer["outer_b"] = {{{4, 40}, {5, 50}, {6, 60}}};  // inner map: 3 int leaves

  registry.register_container("map_vec_map", outer);

  auto all_sizes = registry.compute_all();
  // Total: 3 + 3 = 6
  EXPECT_EQ(all_sizes["map_vec_map"], 6);
  EXPECT_EQ(compute_size_recursive(outer), 6);
}

// ------------------------------------------------------------
// Chain: map -> map -> vector (3-level, two nested maps)
// ------------------------------------------------------------
TEST_F(ContainerRegistryTest, ChainMapMapVector)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  typedef std::map<std::string, std::vector<int>> InnerMap;
  std::map<int, InnerMap> outer;

  outer[1]["x"] = {1, 2, 3};    // 3
  outer[1]["y"] = {4, 5};       // 2  → key 1 total: 5
  outer[2]["z"] = {6};          // 1  → key 2 total: 1

  registry.register_container("map_map_vec", outer);

  auto all_sizes = registry.compute_all();
  // Total: 5 + 1 = 6
  EXPECT_EQ(all_sizes["map_map_vec"], 6);
  EXPECT_EQ(compute_size_recursive(outer), 6);
}

// ------------------------------------------------------------
// Chain: vector -> vector -> map -> custom_struct (4 levels)
// ------------------------------------------------------------
TEST_F(ContainerRegistryTest, ChainVectorVectorMapCustomStruct)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  DataPacket p1; p1.elements = {1, 2};       // size = 2
  DataPacket p2; p2.elements = {3, 4, 5};    // size = 3
  DataPacket p3; p3.elements = {6, 7};       // size = 2
  DataPacket p4; p4.elements = {8};          // size = 1

  std::map<std::string, DataPacket> m1;
  m1["a"] = p1;
  m1["b"] = p2;  // 2 + 3 = 5

  std::map<std::string, DataPacket> m2;
  m2["c"] = p3;  // 2

  std::map<std::string, DataPacket> m3;
  m3["d"] = p4;  // 1

  // outer: vector< vector< map<string, DataPacket> > >
  std::vector<std::vector<std::map<std::string, DataPacket>>> outer;
  outer.push_back({m1, m2});   // 5 + 2 = 7
  outer.push_back({m3});       // 1

  registry.register_container("vec_vec_map_packet", outer);

  auto all_sizes = registry.compute_all();
  // Total: 7 + 1 = 8
  EXPECT_EQ(all_sizes["vec_vec_map_packet"], 8);
  EXPECT_EQ(compute_size_recursive(outer), 8);
}

// ============================================================
// DeviceProfile / SensorReadings chain tests
//
// Real-world model:
//   SensorReadings → holds vector<int> readings per sensor channel
//                    size() = number of recorded data points
//   DeviceProfile  → holds map<sensor_name, SensorReadings>
//                    size() = total readings across all sensors
//
// Registry behaviour:
//   compute_size_recursive(map<device_id, DeviceProfile>)
//     → iterates map values
//     → compute_size_recursive(DeviceProfile)   [is_sized_container]
//       → calls DeviceProfile::size()
//         → internally sums SensorReadings::size() for each sensor
//           → SensorReadings::size() returns its readings.size()
// ============================================================

// ------------------------------------------------------------
// Chain: map<int /*device_id*/, DeviceProfile>
//          DeviceProfile.sensors: map<sensor_name, SensorReadings>
//            SensorReadings.readings: vector<int>
// ------------------------------------------------------------
TEST_F(ContainerRegistryTest, ChainMapDeviceProfileWithMapOfSensorReadings)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  // Device 1: two sensor channels
  //   "temperature" → SensorReadings{22,23,24}   size=3
  //   "humidity"    → SensorReadings{55,60}       size=2
  //   DeviceProfile::size() = 5
  DeviceProfile dev1;
  dev1.add_sensor("temperature", {22, 23, 24});
  dev1.add_sensor("humidity",    {55, 60});

  // Device 2: two sensor channels
  //   "pressure" → SensorReadings{1013}           size=1
  //   "vibration"→ SensorReadings{0,1,0,2,1,3}    size=6 (but we use 4)
  //   DeviceProfile::size() = 1+4 = 5
  DeviceProfile dev2;
  dev2.add_sensor("pressure",  {1013});
  dev2.add_sensor("vibration", {7, 8, 9, 10});

  // Device 3: offline – no readings
  //   "power" → SensorReadings{}                  size=0
  //   DeviceProfile::size() = 0
  DeviceProfile dev3;
  dev3.add_sensor("power", {});

  std::map<int, DeviceProfile> fleet;
  fleet[1] = dev1;  // 5
  fleet[2] = dev2;  // 5
  fleet[3] = dev3;  // 0

  registry.register_container("device_fleet", fleet);

  auto all_sizes = registry.compute_all();
  // Total: 5 + 5 + 0 = 10
  EXPECT_EQ(all_sizes["device_fleet"], 10);
  EXPECT_EQ(compute_size_recursive(fleet), 10);
}

// ------------------------------------------------------------
// Chain: vector<DeviceProfile>
//   A batch of device profiles collected from a sensor sweep.
//   Each DeviceProfile holds a map<sensor_name, SensorReadings>.
// ------------------------------------------------------------
TEST_F(ContainerRegistryTest, ChainVectorDeviceProfileWithMapOfSensorReadings)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  DeviceProfile dev_a;
  dev_a.add_sensor("voltage",  {120, 121});      // size=2
  dev_a.add_sensor("current",  {5, 5, 6});       // size=3
  // dev_a.size() = 5

  DeviceProfile dev_b;
  dev_b.add_sensor("rpm", {1500, 1520, 1510, 1490}); // size=4
  // dev_b.size() = 4

  std::vector<DeviceProfile> batch = {dev_a, dev_b};

  registry.register_container("device_batch", batch);

  auto all_sizes = registry.compute_all();
  // Total: 5 + 4 = 9
  EXPECT_EQ(all_sizes["device_batch"], 9);
  EXPECT_EQ(compute_size_recursive(batch), 9);
}

// ------------------------------------------------------------
// Chain: map<zone_name, vector<DeviceProfile>>
//   A data centre is split into zones; each zone has a list of
//   devices. Each device has multiple named sensor channels.
//   Outer map → vector → DeviceProfile → map<sensor,SensorReadings> → vector<int>
// ------------------------------------------------------------
TEST_F(ContainerRegistryTest, ChainMapVectorDeviceProfileWithMapOfSensorReadings)
{
  auto &registry = ContainerRegistry::instance();
  registry.clearAll();

  // Zone "rack_A": two devices
  DeviceProfile rack_a_dev1;
  rack_a_dev1.add_sensor("cpu_temp",  {65, 66, 67}); // size=3
  rack_a_dev1.add_sensor("fan_speed", {1200});        // size=1
  // rack_a_dev1.size() = 4

  DeviceProfile rack_a_dev2;
  rack_a_dev2.add_sensor("cpu_temp", {70, 71});       // size=2
  // rack_a_dev2.size() = 2

  // Zone "rack_B": one device
  DeviceProfile rack_b_dev1;
  rack_b_dev1.add_sensor("cpu_temp",   {60, 61, 62}); // size=3
  rack_b_dev1.add_sensor("disk_io",    {100, 200});    // size=2
  // rack_b_dev1.size() = 5

  std::map<std::string, std::vector<DeviceProfile>> datacentre;
  datacentre["rack_A"] = {rack_a_dev1, rack_a_dev2};  // 4 + 2 = 6
  datacentre["rack_B"] = {rack_b_dev1};               // 5

  registry.register_container("datacentre_readings", datacentre);

  auto all_sizes = registry.compute_all();
  // Total: 6 + 5 = 11
  EXPECT_EQ(all_sizes["datacentre_readings"], 11);
  EXPECT_EQ(compute_size_recursive(datacentre), 11);
}

// ------------------------------------------------------------
// Verify DeviceProfile and SensorReadings are detected as
// is_sized_container (opaque leaves) and NOT as vector/map/queue/stack.
// ------------------------------------------------------------
TEST_F(ContainerRegistryTest, DeviceProfileAndSensorReadingsTypeDetection)
{
  EXPECT_TRUE(is_sized_container<DeviceProfile>::value);
  EXPECT_TRUE(is_sized_container<SensorReadings>::value);

  EXPECT_FALSE(is_vector_like<DeviceProfile>::value);
  EXPECT_FALSE(is_vector_like<SensorReadings>::value);
  EXPECT_FALSE(is_map_like<DeviceProfile>::value);
  EXPECT_FALSE(is_map_like<SensorReadings>::value);
}
