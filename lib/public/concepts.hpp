#ifndef __CONCEPTS__HPP
#define __CONCEPTS__HPP

#include "entry.hpp"
#include <concepts>

namespace cash {

template <typename T>
concept EvictStrategy = requires(T t, EntryBase* node) {
  { t.getVictim() } -> std::same_as<EntryBase*>;
  { t.onGet(node) } -> std::same_as<void>;
  { t.onPut(node) } -> std::same_as<void>;
};

template <typename T>
concept LockPolicy = requires(T t) {
  { t.lockRead() } -> std::same_as<void>;
  { t.unlockRead() } -> std::same_as<void>;
  { t.lockWrite() } -> std::same_as<void>;
  { t.unlockWrite() } -> std::same_as<void>;
};

} // namespace cash

#endif