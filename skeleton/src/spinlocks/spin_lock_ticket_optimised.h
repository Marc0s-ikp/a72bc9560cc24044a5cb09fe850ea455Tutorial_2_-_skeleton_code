#ifndef SPIN_LOCK_TICKET_OPTIMISED_H
#define SPIN_LOCK_TICKET_OPTIMISED_H

#include <emmintrin.h>

#include <atomic>
#include <string>

class SpinLockTicketOptimised {
 public:
  SpinLockTicketOptimised() {}

  void Lock() {
    const auto ticket = next_ticket_.fetch_add(1, std::memory_order_relaxed);
    while (true) {
      const auto ns = now_serving_.load(std::memory_order_acquire);
      if (ns == ticket) {
        break;
      }
      // Pause for a number of iterations proportional to how many threads are ahead of us!
      for (size_t i = 1; i < ticket - ns; ++i) {
        _mm_pause();
      }
    }
  }

  void Unlock() {
    now_serving_.store(now_serving_.load(std::memory_order_relaxed) + 1, std::memory_order_release);
  }

  [[nodiscard]] static std::string GetName() { return "Ticket optimised"; }

 private:
  std::atomic<size_t> next_ticket_;
  std::atomic<size_t> now_serving_{0};
};

#endif  // SPIN_LOCK_TICKET_OPTIMISED_H
