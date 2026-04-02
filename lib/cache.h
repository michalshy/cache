#include <concepts>
#include <list>
#include <memory>
#include <optional>
#include <unordered_map>

namespace cash {

class EntryBase {
public:
  virtual ~EntryBase() = default;
};

template <typename KEY, typename VAL>
class Entry : public EntryBase {
  KEY _key;
  VAL _val;

public:
  Entry(KEY k, VAL v) : _key(k), _val(v) {}
  const KEY& key() const { return _key; }
  const VAL& val() const { return _val; }
};

template <typename T>
concept EvictStrategy = requires(T t, EntryBase* node) {
  { t.getVictim() } -> std::same_as<EntryBase*>;
  { t.onGet(node) } -> std::same_as<void>;
  { t.onPut(node) } -> std::same_as<void>;
};

template <typename K, typename V>
class LRU {
  std::list<Entry<K, V>*> nodes;
  std::unordered_map<EntryBase*, typename std::list<Entry<K, V>*>::iterator>
      iterators;

public:
  EntryBase* getVictim();
  void onGet(EntryBase* node);
  void onPut(EntryBase* node);
};

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

  ~Cache();
};

} // namespace cash