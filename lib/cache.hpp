#ifndef __CACHE__HPP
#define __CACHE__HPP

#include "public/evict.hpp"
#include "public/lock.hpp"
#include <algorithm>
#include <memory>
#include <optional>

namespace cash {

template <typename K, typename V, typename S = LRU, typename L = NoLock>
  requires EvictStrategy<S> && LockPolicy<L>
class Cache {
private:
  size_t _cap;
  std::unordered_map<K, std::unique_ptr<Entry<K, V>>> _cache;
  S _strategy;
  L _lock;

public:
  Cache(size_t cap) : _cap(cap) {}

  std::optional<V> get(K key) {
    _lock.lockRead();
    if (_cache.contains(key)) {
      _strategy.onGet(_cache[key].get());
      return _cache[key]->val();
    }
    _lock.unlockRead();
    return std::nullopt;
  }

  void put(K key, V val) {
    _lock.lockWrite();
    if (_cache.contains(key)) {
      _cache[key]->set_val(val);
      _strategy.onGet(_cache[key].get());
    } else {
      if (_cache.size() >= _cap) {
        evict();
      }
      std::unique_ptr<Entry<K, V>> entry =
          std::make_unique<Entry<K, V>>(key, val);
      _cache[key] = std::move(entry);
      _strategy.onPut(_cache[key].get());
    }
    _lock.unlockWrite();
  }

  ~Cache() = default;

private:
  void evict() {
    EntryBase* victim = _strategy.getVictim();
    Entry<K, V>* entry = static_cast<Entry<K, V>*>(victim);
    K key = entry->key();
    _cache.erase(key);
  }
};

} // namespace cash

#endif