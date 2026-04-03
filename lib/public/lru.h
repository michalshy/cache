#include "evict.h"
#include <list>
#include <unordered_map>

namespace cash {

class LRU {
  std::list<EntryBase*> _nodes;
  std::unordered_map<EntryBase*, typename std::list<EntryBase*>::iterator>
      _iterators;

public:
  EntryBase* getVictim() {
    EntryBase* node = _nodes.back();
    _iterators.erase(node);
    _nodes.pop_back();
    return node;
  }
  void onGet(EntryBase* node) {
    _nodes.splice(_nodes.begin(), _nodes, _iterators[node]);
  }
  void onPut(EntryBase* node) {
    _nodes.emplace_front(node);
    _iterators[node] = _nodes.begin();
  };
};

} // namespace cash