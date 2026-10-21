#ifndef SPIN_LOCK_ALOCK_PADDED_H
#define SPIN_LOCK_ALOCK_PADDED_H

#include <array>
#include <atomic>
#include <string>

struct __attribute__((aligned(64))) PaddedAtomicBool {
  std::atomic<bool> value;
};

class SpinLockALockPadded {
 public:
  SpinLockALockPadded() {
    flag_[0].value.store(true);
    for (size_t i = 1; i < kCapacity; ++i) {
      flag_[i].value.store(false);
    }
  }

  void Lock() {
    my_slot_ = next_slot_.fetch_add(1) % kCapacity;
    while (!flag_[my_slot_].value.load()) {
      _mm_pause();
    } 
  }

  void Unlock() {
    flag_[my_slot_].value.store(false);
    flag_[(my_slot_ + 1) % kCapacity].value.store(true);
  }

  [[nodiscard]] static std::string GetName() { return "ALock padded"; }

 private:
  static constexpr size_t kCapacity = 64;
  std::array<PaddedAtomicBool, kCapacity> flag_;
  std::atomic<size_t> next_slot_{0};
  inline static thread_local size_t my_slot_{0};
};

#endif  // SPIN_LOCK_ALOCK_PADDED_H
