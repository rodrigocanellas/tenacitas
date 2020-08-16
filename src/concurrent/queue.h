#ifndef TENACITAS_CONCURRENT_QUEUE_H
#define TENACITAS_CONCURRENT_QUEUE_H

#include <array>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>

#include <concurrent/internal/log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

template <typename t_data, uint16_t t_size, typename t_log> struct queue {

  void add(const t_data &p_data) {

    concurrent_log_debug(t_log, "trying to add, m_end = ", m_end);
    std::unique_lock<std::mutex> _lock(m_mutex);
    m_cond_add.wait(_lock, [this]() { return m_end < t_size; });

    concurrent_log_debug(t_log, "adding ", m_end);
    m_array[m_end++] = p_data;
    _lock.unlock();
    m_cond_get.notify_one();
  }

  void add(t_data &&p_data) {

    concurrent_log_debug(t_log, "trying to add, m_end = ", m_end);
    std::unique_lock<std::mutex> _lock(m_mutex);
    m_cond_add.wait(_lock, [this]() -> bool { return m_end < t_size; });

    concurrent_log_debug(t_log, "adding ", p_data);
    m_array[m_end++] = std::move(p_data);
    //    _lock.unlock();
    m_cond_get.notify_one();
  }

  t_data get() {
    concurrent_log_debug(t_log, "trying to get, m_end = ", m_end);
    std::unique_lock<std::mutex> _lock(m_mutex);
    m_cond_get.wait(_lock, [this]() -> bool { return m_end > 0; });

    t_data _data = m_array[0];
    concurrent_log_debug(t_log, "got ", _data);
    m_end--;
    //    _lock.unlock();
    m_cond_add.notify_one();
    return _data;
  }

  bool empty() {
    std::lock_guard<std::mutex> _lock(m_mutex);
    return m_end == 0;
  }

  void traverse(std::function<void(const t_data &)> p_function) {
    for (decltype(m_end) _i = 0; _i < m_end; ++_i) {
      p_function(m_array[_i]);
    }
  }

private:
  typedef std::array<t_data, t_size> array;
  typedef typename array::size_type size;

private:
  array m_array;
  std::mutex m_mutex;
  std::condition_variable m_cond_add;
  std::condition_variable m_cond_get;
  int16_t m_end = 0;
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_QUEUE_H
