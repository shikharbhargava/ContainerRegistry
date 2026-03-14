# ContainerRegistry

A modern C++17 header-only library for registering and managing STL containers with type-aware operations. Provides recursive size computation, type detection, and a thread-safe registry pattern for container introspection and management.

## Features

- **Multiple Container Support**: Vector-like, Map-like, Queue-like, Stack-like, and custom structures
- **Recursive Size Computation**: Automatically computes sizes of nested containers
- **Type-Safe Registration**: Thread-local singleton registry with type-erased entries
- **String Support**: Strings are treated as vector-like containers, counting character content
- **Custom Compute Functions**: Override default size computation with custom logic
- **Custom Structures**: Support for any type with a `size()` method
- **Header-Only**: No compilation needed, just include and use

## Supported Container Types

### Vector-Like Containers
- `std::vector<T>`
- `std::deque<T>`
- `std::list<T>`
- `std::set<T>`
- `std::string` (counts characters)

### Map-Like Containers
- `std::map<K, V>`
- `std::unordered_map<K, V>`

### Adapter Containers
- `std::queue<T>`
- `std::stack<T>`

### Custom Structures
Any user-defined class with a `size()` method and optionally a custom size computation function.

## Building

### Prerequisites
- C++17 or later
- CMake 3.12+
- GTest (included in deps for testing)
- Windows, Linux, or macOS

### Build Steps

```bash
cd ContainerRegistry
mkdir ../build
cd ../build

# Configure
cmake ../ContainerRegistry

# Build
cmake --build . --config Release

# (Optional) Run tests
cmake --build . --target RUN_TESTS --config Release
```

## Installation

### Default Installation
By default, files are installed to `build/install/ContainerRegistry`:

```bash
cmake --install . --config Release
```

### Custom Installation Path
```bash
cmake --install . --config Release --prefix "/custom/install/path"
```

### Installed Files
- **bin/** - Executables (container.exe, container_tests.exe)
- **include/** - Header files (container_registry.hpp)

## Usage Examples

### Basic Registration

```cpp
#include "container_registry.hpp"
#include <vector>
#include <map>

int main() {
    // Get the thread-local registry singleton
    auto& registry = ContainerRegistry::get_instance();
    
    // Register a vector
    std::vector<int> vec = {1, 2, 3, 4, 5};
    registry.register_container<std::vector<int>>("MyVector");
    
    // Register a map
    std::map<std::string, int> map = {{"a", 1}, {"b", 2}};
    registry.register_container<std::map<std::string, int>>("MyMap");
    
    // Print all registered containers
    registry.print();
    
    return 0;
}
```

### Custom Compute Functions

Override the default size computation:

```cpp
// Doubling compute function
auto double_compute = [](const std::vector<int>& v) {
    return v.size() * 2;
};

registry.register_container<std::vector<int>>(
    "DoubledVector", 
    double_compute
);
```

### Nested Containers

```cpp
// Vector of vectors
std::vector<std::vector<int>> nested = {
    {1, 2, 3},
    {4, 5},
    {6, 7, 8, 9}
};
registry.register_container<std::vector<std::vector<int>>>("NestedVectors");
```

### Custom Structures

Create a custom struct with size computation:

```cpp
struct DataPacket {
    std::vector<int> data;
    
    size_t size() const {
        return data.size();
    }
};

// Register custom structure
DataPacket packet;
packet.data = {1, 2, 3};
registry.register_container<DataPacket>("CustomPacket");
```

## API Reference

### Core Types

#### `is_vector_like<T>`
Type trait that detects vector-like containers (vector, deque, list, set, string).

#### `is_map_like<T>`
Type trait that detects map-like containers (map, unordered_map).

#### `is_queue_like<T>`
Type trait that detects queue containers.

#### `is_stack_like<T>`
Type trait that detects stack containers.

#### `is_sized_container<T>`
Type trait that detects custom structures with `size()` method (checked last after standard types).

### Registry Interface

#### `ContainerRegistry::get_instance()`
Returns the thread-local singleton registry instance.

#### `register_container<T>(name)`
Register a container by type with a given name. Uses default size computation.

#### `register_container<T>(name, compute_fn)`
Register a container with a custom size computation function.
- **Template Parameter**: `T` - Container type
- **Parameters**: 
  - `name` - Registry key (std::string)
  - `compute_fn` - Function returning size_t from `const T&`

#### `print()`
Print all registered containers and their computed sizes to stdout.

## Type Detection Hierarchy

The library uses a hierarchical type detection system:

1. **Map-like containers** - Checked first (map, unordered_map)
2. **Vector-like containers** - Checked second (vector, deque, list, set, string)
3. **Queue-like containers** - Checked third (queue)
4. **Stack-like containers** - Checked fourth (stack)
5. **Custom sized containers** - Checked last (user-defined with size())

This ensures correct categorization and prevents misclassification of custom types.

## Custom Compute Functions

Create specialized size computations for specific use cases:

```cpp
// Summation function
auto sum_elements = [](const std::vector<int>& v) {
    size_t sum = 0;
    for (int x : v) sum += x;
    return sum;
};

registry.register_container<std::vector<int>>(
    "SummedVector", 
    sum_elements
);
```

## Testing

Run the comprehensive test suite:

```bash
cd build

# Run all tests
.\bin\Release\container_tests.exe

# Run with CTest
ctest --config Release
```

### Test Coverage

- **34 test cases** organized across 10 categories:
  - Type detection traits
  - Vector-like containers
  - Map-like containers
  - Adapter containers (queue, stack)
  - Nested structures
  - Empty containers
  - Edge cases (large vectors, mixed sizes)
  - Custom compute functions
  - Registry behavior and thread-safety
  - Custom structures

## Architecture

### Thread Safety
The registry uses thread-local storage (`thread_local`) to ensure each thread has its own independent registry instance, preventing race conditions in multi-threaded applications.

### Type Erasure
Container entries are stored using type-erased function pointers, allowing different container types to coexist in a single registry.

### Recursive Size Computation
The `compute_size_recursive` function template recursively computes sizes for nested containers by:
1. Checking if the element type is a container
2. If yes, summing the recursive sizes of all elements
3. If no, counting leaf nodes directly

## Examples

See [main/main.cpp](main/main.cpp) for a complete working example.

## License

See [LICENSE](LICENSE) for details.

## Version

Current version: **0.1.0**
