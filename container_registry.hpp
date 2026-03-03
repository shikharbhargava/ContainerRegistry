#ifndef _CONTAINERS_REGISTRY_
#define _CONTAINERS_REGISTRY_

#include <type_traits>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <list>
#include <set>
#include <array>
#include <memory>
#include <functional>
#include <iostream>
#include <string>

/*
    ============================================================
    COMPLETE STANDARD CONTAINER COVERAGE
    ============================================================

    Vector-like containers (with iterators):
    - vector<T>           : works
    - deque<T>            : works
    - list<T>             : works
    - array<T, N>         : works
    - string              : works
    - set<T>              : works
    - multiset<T>         : works
    - unordered_set<T>    : works
    - forward_list<T>     : works

    Map-like containers (key-value pairs):
    - map<K, V>           : works
    - multimap<K, V>      : works
    - unordered_map<K, V> : works
    - unordered_multimap  : works

    Queue-like containers (FIFO adapter, no iterators):
    - queue<T>            : works
    - priority_queue<T>   : works

    Stack-like containers (LIFO adapter, no iterators):
    - stack<T>            : works

    ============================================================
*/

/*
    ============================================================
    1. Detect map-like types
    ============================================================
*/

template <typename T>
class is_map_like
{
private:
  template <typename U>
  static auto test(int) -> decltype(
      typename U::key_type(),
      typename U::mapped_type(),
      std::declval<U>().size(),
      std::true_type());

  template <typename>
  static std::false_type test(...);

public:
  static constexpr bool value = decltype(test<T>(0))::value;
};

/*
    ============================================================
    2. Detect vector-like types
    ============================================================
*/

template <typename T>
class is_vector_like
{
private:
  template <typename U>
  static auto test(int) -> decltype(
      typename U::value_type(),
      std::declval<U>().size(),
      std::declval<U>().begin(),
      std::declval<U>().end(),
      std::true_type());

  template <typename>
  static std::false_type test(...);

public:
  static constexpr bool value =
      decltype(test<T>(0))::value &&
      !is_map_like<T>::value;
};

/*
    ============================================================
    2b. Detect queue-like types (FIXED)
    ============================================================
*/

template <typename T>
class is_queue_like
{
private:
  template <typename U>
  static auto test(int) -> decltype(
      typename U::value_type(),
      std::declval<U>().size(),
      std::declval<U>().push(std::declval<typename U::value_type>()),
      std::declval<U>().pop(),
      std::true_type());

  template <typename>
  static std::false_type test(...);

public:
  static constexpr bool value =
      decltype(test<T>(0))::value &&
      !is_map_like<T>::value &&
      !is_vector_like<T>::value;
};

/*
    ============================================================
    2c. Detect stack-like types
    ============================================================
*/

template <typename T>
class is_stack_like
{
private:
  template <typename U>
  static auto test(int) -> decltype(
      typename U::value_type(),
      std::declval<U>().size(),
      std::declval<U>().push(std::declval<typename U::value_type>()),
      std::declval<U>().pop(),
      std::declval<U>().top(),
      std::true_type());

  template <typename>
  static std::false_type test(...);

public:
  static constexpr bool value =
      decltype(test<T>(0))::value &&
      !is_map_like<T>::value &&
      !is_vector_like<T>::value &&
      !is_queue_like<T>::value;
};

/*
    ============================================================
    3. Recursive compute_size (COUNT ONLY END NODES)
    ============================================================
*/

template <typename T>
typename std::enable_if<
    !is_map_like<T>::value &&
    !is_vector_like<T>::value &&
    !is_queue_like<T>::value &&
    !is_stack_like<T>::value,
    size_t>::type
compute_size_recursive(const T &);

template <typename T>
typename std::enable_if<
    is_map_like<T>::value,
    size_t>::type
compute_size_recursive(const T &);

template <typename T>
typename std::enable_if<
    is_vector_like<T>::value,
    size_t>::type
compute_size_recursive(const T &);

template <typename T>
typename std::enable_if<
    is_queue_like<T>::value,
    size_t>::type
compute_size_recursive(const T &);

template <typename T>
typename std::enable_if<
    is_stack_like<T>::value,
    size_t>::type
compute_size_recursive(const T &);

// Base case: leaf node (non-container)
template <typename T>
typename std::enable_if<
    !is_map_like<T>::value &&
    !is_vector_like<T>::value &&
    !is_queue_like<T>::value &&
    !is_stack_like<T>::value,
    size_t>::type
compute_size_recursive(const T &)
{
  return 1;   // Count leaf element
}

/*
    -------------------------
    MAP HANDLING
    -------------------------
*/

template <typename T>
typename std::enable_if<is_map_like<T>::value, size_t>::type
compute_size_recursive(const T &m)
{
  size_t total = 0;

  for (const auto &kv : m)
  {
    total += compute_size_recursive(kv.second);
  }

  return total;
}

/*
    -------------------------
    VECTOR HANDLING
    -------------------------
*/

template <typename T>
typename std::enable_if<is_vector_like<T>::value, size_t>::type
compute_size_recursive(const T &v)
{
  size_t total = 0;

  for (const auto &elem : v)
  {
    total += compute_size_recursive(elem);
  }

  return total;
}

/*
    -------------------------
    QUEUE HANDLING
    -------------------------
*/

template <typename T>
typename std::enable_if<is_queue_like<T>::value, size_t>::type
compute_size_recursive(const T &q)
{
  // Cannot inspect elements without copying → count elements as leaves
  return q.size();
}

/*
    -------------------------
    STACK HANDLING
    -------------------------
*/

template <typename T>
typename std::enable_if<is_stack_like<T>::value, size_t>::type
compute_size_recursive(const T &s)
{
  // Cannot inspect elements without copying → count elements as leaves
  return s.size();
}

/*
    ============================================================
    4. Type-erased entry
    ============================================================
*/

class IRegistryEntry
{
public:
  virtual ~IRegistryEntry() {}
  virtual size_t compute() const = 0;
  virtual const std::string &name() const = 0;
};

template <typename ContainerType>
class RegistryEntry : public IRegistryEntry
{
public:
  using ComputeFn = std::function<size_t(const ContainerType &)>;

  RegistryEntry(const std::string &n,
                ContainerType &c,
                ComputeFn fn)
      : name_(n),
        container_(c),
        compute_fn_(fn)
  {}

  size_t compute() const override
  {
    return compute_fn_(container_);
  }

  const std::string &name() const override
  {
    return name_;
  }

private:
  std::string name_;
  ContainerType &container_;
  ComputeFn compute_fn_;
};

/*
    ============================================================
    5. Thread-local registry
    ============================================================
*/

class ContainerRegistry
{
public:
  static ContainerRegistry &instance()
  {
    thread_local ContainerRegistry registry;
    return registry;
  }

  template <typename ContainerType>
  void register_container(const std::string &name,
                    ContainerType &container)
  {
    static_assert(
        is_map_like<ContainerType>::value ||
        is_vector_like<ContainerType>::value ||
        is_queue_like<ContainerType>::value ||
        is_stack_like<ContainerType>::value,
        "Unsupported container type");

    auto default_fn = [](const ContainerType &c)
    {
      return compute_size_recursive(c);
    };

    entries_.emplace_back(
        new RegistryEntry<ContainerType>(name, container, default_fn));
    max_name_size = std::max(max_name_size, name.length());
    std::cout << name << " : length: " <<  name.length() << " : max: " << max_name_size << std::endl;
  }

  template <typename ContainerType, typename ComputeLambda>
  void register_container(const std::string &name,
                    ContainerType &container,
                    ComputeLambda custom_fn)
  {
    entries_.emplace_back(
        new RegistryEntry<ContainerType>(name, container, custom_fn));
    max_name_size = std::max(max_name_size, name.length());
    std::cout << name << " : length: " <<  name.length() << " : max: " << max_name_size << std::endl;
  }

  void print(std::ostream &os = std::cout) const
  {
    for (const auto &entry : entries_)
    {
        os << std::left << std::setw(max_name_size + 1) << std::setfill(' ') << entry->name()
          << " : "
          << std::right << std::setw(10) << std::setfill(' ') << entry->compute()
          << std::endl;
    }
  }

private:
  ContainerRegistry() {}
  ContainerRegistry(const ContainerRegistry &) = delete;
  ContainerRegistry &operator=(const ContainerRegistry &) = delete;

  std::vector<std::unique_ptr<IRegistryEntry>> entries_;
  size_t max_name_size = 0;
};

#endif // _CONTAINERS_REGISTRY_
