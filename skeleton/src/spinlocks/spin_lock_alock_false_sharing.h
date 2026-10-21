#ifndef SPIN_LOCK_ALOCK_FALSE_SHARING_H
#define SPIN_LOCK_ALOCK_FALSE_SHARING_H

#include <array>
#include <atomic>
#include <string>
#include <emmintrin.h>

class SpinLockALockFalseSharing {
 public:
  SpinLockALockFalseSharing() {
    flag_[0].store(true);
    for (size_t i = 1; i < kCapacity; ++i) {
      flag_[i].store(false);
    }
  }

  void Lock() {
    my_slot_ = next_slot_.fetch_add(1) % kCapacity;
    while (!flag_[my_slot_].load()) {
      _mm_pause();
    } 
  }

  void Unlock() {
    flag_[my_slot_].store(false);
    flag_[(my_slot_ + 1) % kCapacity].store(true);
  }

  [[nodiscard]] static std::string GetName() { return "ALock false sharing"; }

 private:
  static constexpr size_t kCapacity = 64;
  std::array<std::atomic<bool>, kCapacity> flag_;
  std::atomic<size_t> next_slot_{0};
  inline static thread_local size_t my_slot_{0};
};

#endif  // SPIN_LOCK_ALOCK_FALSE_SHARING_H
