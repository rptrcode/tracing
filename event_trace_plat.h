/*
        This file defines platform specific items that are required by
   event_trace
*/
#ifndef EVENT_TRACE_PLAT_H
#define EVENT_TRACE_PLAT_H

#include <time.h>

#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

namespace event_trace {

////////////////////////////////
// TYPEDEFS
////////////////////////////////
typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;

typedef long long int64;
typedef unsigned long long uint64;
#define _tlsVal __thread

////////////////////////////////
// TIMERS
////////////////////////////////
typedef unsigned long long timer_ticks;
typedef timer_ticks TimeTicks;

#include <sys/time.h>
inline void query_counter(timer_ticks* pTicks) {
  struct timeval cur_time;
  gettimeofday(&cur_time, NULL);
  *pTicks = static_cast<unsigned long long>(cur_time.tv_sec) * 1000000ULL +
            static_cast<unsigned long long>(cur_time.tv_usec);
}
inline void query_counter_frequency(timer_ticks* pTicks) {
  *pTicks = 1000000;
}

////////////////////////////////
// LOCKING
////////////////////////////////
class Lock {
 public:
  Lock() { pthread_mutex_init(&mCrit, 0); };
  ~Lock() { pthread_mutex_destroy(&mCrit); };
  void Aquire() { pthread_mutex_lock(&mCrit); };
  void Release() { pthread_mutex_unlock(&mCrit); };

 private:
  pthread_mutex_t mCrit;
};

class ScopedLock : public Lock {
 public:
  ScopedLock(Lock& lock) : pLock_(&lock) { lock.Aquire(); };
  ~ScopedLock() {
    if (pLock_)
      pLock_->Release();
    pLock_ = 0;
  };

 private:
  Lock* pLock_;
};

// threading
inline uint32 getCurrentProcId() {
  return getpid();
}

inline uint32 getCurrentThreadId() {
  return pthread_self();
}

// When enough events are collected, they are handed (in bulk) to
// the output callback. If no callback is set, the output will be
// silently dropped. The callback must be thread safe.
// typedef RefCountedData<std::string> std::string*;
// typedef base::Callback<void(std::string*)> OutputCallback;
void OutputCallback(const char*);

// The trace buffer does not flush dynamically, so when it fills up,
// subsequent trace events will be dropped. This callback is generated when
// the trace buffer is full. The callback must be thread safe.
// typedef base::Callback<void(void)> BufferFullCallback;
void BufferFullCallback();
}  // namespace event_trace

#endif