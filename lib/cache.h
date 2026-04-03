#include "public/lru.h"
#include <algorithm>
#include <memory>
#include <optional>

namespace cash {

template <typename K, typename V,
          template <typename, typename> typename S = LRU>
  requires EvictStrategy<S<K, V>>
class Cache {
private:
  size_t _cap;
  std::unordered_map<K, std::unique_ptr<Entry<K, V>>> cache;
  S<K, V> _strategy;

public:
  Cache(size_t cap) : _cap(cap) {}

  std::optional<V> get(K key) {
    if (cache.contains(key)) {
      _strategy.onGet(cache[key].get());
      return cache[key]->val();
    }
    return std::nullopt;
  }

  void put(K key, V val) {
    if (cache.contains(key)) {
      cache[key]->set_val(val);
      _strategy.onGet(cache[key].get());
    } else {
      if (cache.size() >= _cap) {
        evict();
      }
      std::unique_ptr<Entry<K, V>> entry =
          std::make_unique<Entry<K, V>>(key, val);
      cache[key] = std::move(entry);
      _strategy.onPut(cache[key].get());
    }
  }

  ~Cache() = default;

private:
  void evict() {
    EntryBase* victim = _strategy.getVictim();
    Entry<K, V>* entry = static_cast<Entry<K, V>*>(victim);
    K key = entry->key();
    cache.erase(key);
  }
};

} // namespace cash