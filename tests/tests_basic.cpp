#include "cache.hpp"
#include <catch2/catch_test_macros.hpp>
#include <optional>

TEST_CASE("Cache basic") {
  cash::Cache<int, int> cache(3);
  cash::Cache<int, int> small(1);

  SECTION("get returns nullopt for missing key") {
    REQUIRE(!cache.get(1).has_value());
  }

  SECTION("put and get") {
    cache.put(1, 10);
    REQUIRE(cache.get(1).value() == 10);
  }

  SECTION("double put") {
    cache.put(1, 1);
    cache.put(1, 2);
    REQUIRE(cache.get(1) == 2);
  }

  SECTION("empty") {
    for (int i = 0; i < 1'000'000; i++) {
      REQUIRE(cache.get(i) == std::nullopt);
    }
  }

  SECTION("small capacity") {
    for (int i = 0; i < 1'000'000; i++) {
      small.put(i, i);
      REQUIRE(small.get(i) == i);
      REQUIRE(small.get(i - 1) == std::nullopt);
    }
  }
}