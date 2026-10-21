#ifndef SPIN_LOCK_ACTIVE_BACKOFF_WEAKER_ORDERINGS_H
#define SPIN_LOCK_ACTIVE_BACKOFF_WEAKER_ORDERINGS_H

#include <atomic>
#include <string>

class SpinLockActiveBackoffWeakerOrderings {
 public:
  SpinLockActiveBackoffWeakerOrderings() {}

void Lock() {
    while (lock_bit_.exchange(true, std::memory_order_acquire)) {
      do {
        for (volatile size_t i = 0; i < 100; i = i + 1) {
          // Do nothing
        }
      } while (lock_bit_.load(std::memory_order_relaxed));
    }
  }

  void Unlock() {
    lock_bit_.store(false, std::memory_order_release);
  }

  [[nodiscard]] static std::string GetName() {
    return "Active backoff weaker orderings";
  }

 private:
  std::atomic<bool> lock_bit_{false};
};

#endif  // SPIN_LOCK_ACTIVE_BACKOFF_WEAKER_ORDERINGS_H
