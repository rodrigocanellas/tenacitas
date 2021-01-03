#ifndef TENACITAS_CALENDAR_BUSINESS_EPOCH_H
#define TENACITAS_CALENDAR_BUSINESS_EPOCH_H

#include <chrono>
#include <cstdint>

namespace tenacitas {
namespace calendar {

struct epoch {
  inline static uint64_t microsecs() {
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count());
  }

  inline static uint64_t millisecs() {
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count());
  }

  inline static uint64_t secs() {
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count());
  }

  inline static uint64_t minutes() {
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::minutes>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count());
  }

  inline static uint64_t hours() {
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::hours>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count());
  }
};

} // namespace calendar
} // namespace tenacitas

#endif // TENACITAS_CALENDAR_EPOCH_H
