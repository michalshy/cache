#include "cache.hpp"
#include <catch2/catch_test_macros.hpp>
#include <optional>
#include <thread>

TEST_CASE("SharedLock thread safety") {
  cash::Cache<int, int, cash::LRU, cash::SharedLock> cache{10};
  std::atomic<int> maxInserted{0};

  SECTION("concurrent reads and writes with value correctness") {
    std::thread writer([&]() {
      for (int i = 0; i < 1000; i++) {
        cache.put(i, i * 2);
        maxInserted.store(i, std::memory_order_release);
      }
    });

    std::vector<std::thread> readers;
    for (int i = 0; i < 5; i++) {
      readers.emplace_back([&]() {
        for (int j = 0; j < 1000; j++) {
          int max = maxInserted.load(std::memory_order_acquire);
          if (max <= 0)
            continue;
          int key = j % max;
          auto result = cache.get(key);
          if (result.has_value()) {
            REQUIRE(result.value() == key * 2);
          }
        }
      });
    }

    writer.join();
    for (auto& t : readers)
      t.join();
  }
}