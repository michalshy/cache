#include "evict.h"
#include <list>
#include <unordered_map>

namespace cash {

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

}