#include "cache.hpp"
#include "public/evict.hpp"
#include <catch2/catch_test_macros.hpp>
#include <string>

TEST_CASE("LRU Cache eviction tests") {
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

TEST_CASE("LFU cache eviction test") {
  cash::Cache<int, int, cash::LFU> cache{3};
  SECTION("LFU simple eviciton") {
    // populate
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(3, 3);
    // pop up frequency
    cache.get(1);
    cache.get(1);
    cache.get(2);
    cache.get(2);
    // exceed
    cache.put(4, 4);
    // check
    REQUIRE(cache.get(1).value() == 1);
    REQUIRE(cache.get(2).value() == 2);
    REQUIRE(!cache.get(3).has_value());
    REQUIRE(cache.get(4).value() == 4);
    cache.put(5, 5);
    REQUIRE(cache.get(1).value() == 1);
    REQUIRE(cache.get(2).value() == 2);
    REQUIRE(!cache.get(3).has_value());
    REQUIRE(!cache.get(4).has_value());
    REQUIRE(cache.get(5).value() == 5);
  }
}