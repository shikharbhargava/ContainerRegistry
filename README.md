# ContainerRegistry

ContainerRegistry is a C++17 header-only library for registering STL containers (and custom sized types), computing logical sizes recursively, and querying all registered results through a thread-local registry.

## Highlights

- Type-aware container detection (map-like, vector-like, queue-like, stack-like, custom sized).
- Recursive size computation for nested standard containers.
- Custom compute function support per registered container.
- Thread-local singleton registry for per-thread isolation.
- Type-erased storage so heterogeneous container types can coexist in one registry.
- Generated version header from [VERSION](VERSION) during CMake configure.

## Supported Types

### Vector-like
- std::vector<T>
- std::deque<T>
- std::list<T>
- std::array<T, N>
- std::set<T>
- std::multiset<T>
- std::unordered_set<T>
- std::string

### Map-like
- std::map<K, V>
- std::multimap<K, V>
- std::unordered_map<K, V>
- std::unordered_multimap<K, V>

### Adapters
- std::queue<T>
- std::stack<T>
- std::priority_queue<T>

### Custom sized types
Any user-defined type with a size() method that is not classified as map/vector/queue/stack-like.

## How Size Is Computed

- Leaf non-container values count as 1.
- Map-like containers sum sizes of mapped values.
- Vector-like containers sum sizes of each element.
- Queue/stack/priority_queue return adapter size() directly.
- Custom sized types return obj.size().

## Versioning

This project follows Semantic Versioning and Keep a Changelog conventions.

- Current version source: [VERSION](VERSION)
- Change history: [CHANGELOG.md](CHANGELOG.md)

Version info is also available in code through:

- CONTAINER_REGISTRY_VERSION
- CONTAINER_REGISTRY_VERSION_MAJOR
- CONTAINER_REGISTRY_VERSION_MINOR
- CONTAINER_REGISTRY_VERSION_PATCH
- container_registry_version()

## Build

### Prerequisites

- C++17 compiler
- CMake 3.12+

### Configure and build

```bash
cd ContainerRegistry
mkdir -p ../build
cd ../build

cmake ../ContainerRegistry -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

### Run tests

```bash
cd ../build
./bin/Release/container_tests
```

Or with CTest:

```bash
cd ../build
ctest --output-on-failure
```

## Install

Default install prefix is inside the build directory:

```bash
cmake --install . --config Release
```

Custom prefix:

```bash
cmake --install . --config Release --prefix /custom/install/path
```

## Usage

### Basic registration and compute

```cpp
#include <container_registry.hpp>
#include <vector>
#include <map>

int main()
{
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::map<std::string, int> m = {{"a", 1}, {"b", 2}};

    auto &registry = ContainerRegistry::instance();
    registry.clearAll();

    registry.register_container("int_vector", vec);
    registry.register_container("int_map", m);

    auto all = registry.compute_all();
    registry.print();

    return 0;
}
```

### Custom compute function

```cpp
std::vector<int> vec = {1, 2, 3, 4, 5};

auto doubled_size = [](const std::vector<int> &v) {
    return v.size() * 2;
};

auto &registry = ContainerRegistry::instance();
registry.register_container("custom_vec", vec, doubled_size);
```

### Nested containers

```cpp
std::map<std::string, std::vector<int>> nested = {
    {"a", {1, 2, 3}},
    {"b", {4, 5}}
};

size_t total = compute_size_recursive(nested); // 5
```

## API

### Type traits

- is_map_like<T>
- is_vector_like<T>
- is_queue_like<T>
- is_stack_like<T>
- is_sized_container<T>

### Free function

- compute_size_recursive(const T &value)

### Registry

- ContainerRegistry::instance()
- register_container(const std::string &name, ContainerType &container)
- register_container(const std::string &name, ContainerType &container, ComputeLambda custom_fn)
- compute_all() -> std::unordered_map<std::string, size_t>
- print(std::ostream &os = std::cout)
- clearAll() -> size_t

## Testing

The suite currently includes 60 tests across:

- Type detection
- Vector-like containers
- Map-like containers
- Adapter containers
- Nested and deep-nested combinations
- Empty and edge cases
- Custom compute functions
- Registry behavior and thread-local isolation
- Custom structures and real-world nested modeling

## Notes

- Registry entries store references to registered containers; ensure container lifetime exceeds registry usage.
- For adapter containers (queue/stack/priority_queue), size computation uses adapter size() without iterating elements.

## Example Program

See [main/main.cpp](main/main.cpp).

## License

See [LICENSE](LICENSE).
