#ifndef TENACITAS_COMMUNICATION_TIMEOUT_H
#define TENACITAS_COMMUNICATION_TIMEOUT_H

#include <chrono>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace communication {

template <typename t_time> struct timeout {
  timeout(t_time p_target)
      : m_target(std::chrono::duration<double>(p_target).count()),
        m_start(timeout::now()) {}

  inline bool operator()() { return (timeout::now() - m_start) > m_target; }

private:
  static double now() {
    return std::chrono::duration<double>(
               std::chrono::high_resolution_clock::now().time_since_epoch())
        .count();
  }

private:
  double m_target;
  double m_start;
  ;
};

template <> struct timeout<bool> {
  inline explicit timeout(bool) {}
  inline bool operator()() { return false; }
};

} // namespace communication
} // namespace tenacitas

#endif // TIMEOUT_H
