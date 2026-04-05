#ifndef CASH_EVICT_HPP
#define CASH_EVICT_HPP

#include "entry.hpp"
#include <list>
#include <unordered_map>

namespace cash {

class LFU {
  std::unordered_map<int, std::list<EntryBase*>> _frequencies;
  std::unordered_map<EntryBase*, int> _nodesFrequencies;
  std::unordered_map<EntryBase*, std::list<EntryBase*>::iterator> _iterators;
  int _min{0};

public:
  EntryBase* getVictim() {
    EntryBase* victim = _frequencies[_min].back();
    _frequencies[_min].erase(_iterators[victim]);
    _nodesFrequencies.erase(victim);
    _iterators.erase(victim);
    return victim;
  }

  void onGet(EntryBase* node) {
    int currentFreq = _nodesFrequencies[node]++;
    _frequencies[currentFreq].erase(_iterators[node]);
    // potentially update min if empty
    if (_frequencies[currentFreq].empty() && currentFreq == _min)
      _min++;

    _frequencies[_nodesFrequencies[node]].emplace_front(node);
    _iterators[node] = _frequencies[_nodesFrequencies[node]].begin();
  }

  void onPut(EntryBase* node) {
    _nodesFrequencies.insert({node, 1});
    _frequencies[1].emplace_front(node);
    _iterators[node] = _frequencies[1].begin();
    _min = 1;
  };
};

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

#endif