#ifndef SENSE_REVERSING_BARRIER_NO_TLS_H
#define SENSE_REVERSING_BARRIER_NO_TLS_H

#include <atomic>

class SenseReversingBarrierNoTls {
 public:
  SenseReversingBarrierNoTls(size_t num_participants)
      : num_participants_(num_participants) {
    count_.store(num_participants);
    sense_.store(false);
  }

  void Await() {
    bool my_sense = !sense_.load();
    if (count_.fetch_sub(1) == 1) {
      count_.store(num_participants_);
      sense_.store(my_sense);
    } else {
      while (sense_.load() != my_sense) {
        _mm_pause();
      }
    }
    my_sense_ = !my_sense;
  }
  private:
  std::atomic<size_t> count_;
  std::atomic<bool> sense_;
  const size_t num_participants_;
  bool my_sense_{true};
};

#endif  // SENSE_REVERSING_BARRIER_NO_TLS_H
