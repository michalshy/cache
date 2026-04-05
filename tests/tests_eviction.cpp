#include "cache.hpp"
#include <catch2/catch_test_macros.hpp>
#include <string>

TEST_CASE("Cache eviction tests") {
  cash::Cache<int, std::string> cache{3};
  SECTION("LRU simple eviction") {
    // fill up
    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");
    // check
    REQUIRE(cache.get(1).value() == "one");
    REQUIRE(cache.get(2).value() == "two");
    REQUIRE(cache.get(3).value() == "three");
    REQUIRE(!cache.get(4).has_value());
    // extend size
    cache.put(4, "four");
    // check
    REQUIRE(!cache.get(1).has_value());
    REQUIRE(cache.get(2).value() == "two");
    REQUIRE(cache.get(3).value() == "three");
    REQUIRE(cache.get(4).value() == "four");
  }
}