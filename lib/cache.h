#include "public/lru.h"
#include <memory>

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

  std::optional<V> get(K);
  void put(K key, V val);

  ~Cache() = default;
};

} // namespace cash