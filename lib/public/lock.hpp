#ifndef __LOCK__HPP
#define __LOCK__HPP

#include "concepts.hpp"
#include <shared_mutex>

namespace cash {

struct NoLock {
  void lockRead() {}
  void unlockRead() {}
  void lockWrite() {}
  void unlockWrite() {}
};

struct SharedLock {
  void lockRead() { _mtx.lock_shared(); }
  void unlockRead() { _mtx.unlock_shared(); }
  void lockWrite() { _mtx.lock(); }
  void unlockWrite() { _mtx.unlock(); }

private:
  std::shared_mutex _mtx;
};

template <typename L>
  requires LockPolicy<L>
class ReadLock {
  L& _lock;

public:
  ReadLock(L& lock) : _lock(lock) { _lock.lockRead(); }
  ~ReadLock() { _lock.unlockRead(); }
};

template <typename L>
  requires LockPolicy<L>
class WriteLock {
  L& _lock;

public:
  WriteLock(L& lock) : _lock(lock) { _lock.lockWrite(); }
  ~WriteLock() { _lock.unlockWrite(); }
};

} // namespace cash

#endif