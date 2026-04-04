#include "evict.h"

namespace cash {

class LFU {
public:
  EntryBase* getVictim() { return nullptr; }
  void onGet(EntryBase*) {}
  void onPut(EntryBase*) {};
};

} // namespace cash