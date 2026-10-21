#ifndef SPIN_LOCK_TICKET_NON_ATOMIC_H
#define SPIN_LOCK_TICKET_NON_ATOMIC_H

#include <emmintrin.h>

#include <atomic>
#include <string>

class SpinLockTicketNonAtomic {
 public:
  SpinLockTicketNonAtomic() {}

  void Lock() {
    const auto ticket = next_ticket_.fetch_add(1);
    while (now_serving_ != ticket) {
      // _mm_pause();
    }
  }

  void Unlock() { 
    now_serving_ = now_serving_ + 1; 
  }

  [[nodiscard]] static std::string GetName() { return "Ticket nonatomic"; }

 private:
  std::atomic<size_t> next_ticket_;
  volatile size_t now_serving_{0};
};

#endif  // SPIN_LOCK_TICKET_NON_ATOMIC_H
