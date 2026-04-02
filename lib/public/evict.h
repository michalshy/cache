#include "entry.h"
#include <concepts>

namespace cash {

template <typename T>
concept EvictStrategy = requires(T t, EntryBase* node) {
  { t.getVictim() } -> std::same_as<EntryBase*>;
  { t.onGet(node) } -> std::same_as<void>;
  { t.onPut(node) } -> std::same_as<void>;
};

} // namespace cash