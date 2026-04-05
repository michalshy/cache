# cash

A header-only C++ cache library written to learn modern C++ patterns.

Supports pluggable eviction strategies and lock policies via C++20 concepts and template parameters. The core idea: `Cache` knows nothing about how eviction works or how synchronization is handled — it delegates both to strategy types passed as template arguments.

## Features

- LRU and LFU eviction strategies out of the box
- Thread-safe and non-thread-safe variants with zero overhead for the latter
- Custom eviction strategies via the `EvictStrategy` concept
- Custom lock policies via the `LockPolicy` concept
- `std::optional` return type on `get` — no sentinel values

## Usage

```cpp
#include "cache.hpp"

// LRU, no synchronization (default)
cash::Cache<int, std::string> cache(100);

// LRU, thread-safe
cash::Cache<int, std::string, cash::LRU, cash::SharedLock> cache(100);

// LFU, thread-safe
cash::Cache<int, std::string, cash::LFU, cash::SharedLock> cache(100);

cache.put(1, "hello");

auto val = cache.get(1);
if (val.has_value()) {
    std::cout << val.value() << "\n";
}
```

## Writing your own eviction strategy

Implement three methods satisfying the `EvictStrategy` concept:

```cpp
class MyStrategy {
public:
    // Called when cache is full — return the entry to evict
    cash::EntryBase* getVictim();

    // Called on every cache hit
    void onGet(cash::EntryBase* node);

    // Called on every cache insert
    void onPut(cash::EntryBase* node);
};
```

`EntryBase` is the base class for all cache entries. Cast to `Entry<K, V>` to access the key and value if needed:

```cpp
cash::EntryBase* victim = getVictim();
auto* entry = static_cast<cash::Entry<int, std::string>*>(victim);
auto key = entry->key();
```

## Writing your own lock policy

Implement four methods satisfying the `LockPolicy` concept:

```cpp
class MyLock {
public:
    void lockRead();
    void unlockRead();
    void lockWrite();
    void unlockWrite();
};
```

## A note on thread safety

`get` is not a read-only operation. Even though it does not modify the cache map, it updates the eviction strategy's internal state (e.g. moving an entry to the front of the LRU list). Using a read lock on `get` causes a data race — both `get` and `put` require a write lock.

## Building

Requires C++20 and CMake 3.20+.

```bash
mkdir build && cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

Tests use [Catch2](https://github.com/catchorg/Catch2) which is fetched automatically by CMake.

## Project structure

```
cash/
├── lib/
│   └── public/
│       ├── evict.hpp    # EntryBase, Entry, LRU, LFU, EvictStrategy concept
│       └── lock.hpp     # NoLock, SharedLock, LockPolicy concept, RAII guards
│   └── cache.hpp        # Cache class
└── tests/
    ├── tests_basic.cpp
    ├── tests_eviction.cpp
    ├── tests_update.cpp
    └── tests_concurrent.cpp
```

## What I learned building this

- Policy-based design with template parameters and C++20 concepts
- Why `get` in a cache with LRU/LFU is not a read-only operation
- Ownership model: `Cache` owns entries via `unique_ptr`, strategies observe via raw pointers
- The difference between `shared_lock` (readers don't block each other) and why it does not apply here
- RAII lock guards to avoid unlock being skipped on early return
- How `std::list::splice` enables O(1) node reordering without memory allocation
- How ThreadSanitizer catches data races that are otherwise nearly impossible to reproduce