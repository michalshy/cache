#include <concepts>

namespace cash {

class NodeBase {
public:
  virtual ~NodeBase() = default;
};

template <typename KEY, typename VAL>
class Node : public NodeBase {
  KEY key;
  VAL val;

public:
  Node(KEY k, VAL v) : key(k), val(v) {}
  const KEY& Key() { return key; }
  const VAL& Val() { return val; }
};

template <typename T>
concept EvictStrategy = requires(T t, NodeBase* node) {
  { t.getVictim() } -> std::same_as<NodeBase*>;
  { t.onGet(node) } -> std::same_as<void>;
  { t.onPut(node) } -> std::same_as<void>;
};

template <typename T>
class Cache {
private:
public:
  Cache(/* args */);
  ~Cache();
};

} // namespace cash