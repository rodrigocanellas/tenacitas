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

template <class t_data, size_t t_size, typename t_log> class queue_t {
public:
  void add(const t_data &p_data) {
    concurrent_log_debug(t_log, "waiting for room ...");
    std::unique_lock<std::mutex> _lock(m_mutex);
    m_cond_add.wait(_lock, [this]() { return m_lenght < t_size; });
    concurrent_log_debug(t_log, "there is room");

    m_array[m_write++] = p_data;
    ++m_lenght;
    if (m_write == (t_size)) {
      m_write = 0;
    }

    concurrent_log_debug(t_log, "adding ", p_data);
    _lock.unlock();
    m_cond_get.notify_all();
  }

  void add(t_data &&p_data) {
    concurrent_log_debug(t_log, "waiting for room ...");
    std::unique_lock<std::mutex> _lock(m_mutex);
    m_cond_add.wait(_lock, [this]() { return m_lenght < t_size; });
    concurrent_log_debug(t_log, "there is room");

    m_array[m_write++] = std::move(p_data);
    ++m_lenght;
    if (m_write == (t_size)) {
      m_write = 0;
    }

    concurrent_log_debug(t_log, "adding ", p_data);
    _lock.unlock();
    m_cond_get.notify_all();
  }

  t_data get() {
    concurrent_log_debug(t_log, "waiting for data...");
    std::unique_lock<std::mutex> _lock(m_mutex);
    m_cond_get.wait(_lock, [this]() -> bool { return m_lenght > 0; });
    concurrent_log_debug(t_log, "data arrived...");

    t_data _data = m_array[m_read++];
    --m_lenght;
    if (m_read == t_size) {
      m_read = 0;
    }

    concurrent_log_debug(t_log, "getting ", _data);
    _lock.unlock();
    m_cond_add.notify_all();
    return _data;
  }

  void traverse(std ::function<void(const t_data &)> p_function) {
    size _i = m_read;
    size _counter = 0;
    while (_counter < m_lenght) {
      if (_i == t_size) {
        _i = 0;
      }
      p_function(m_array[_i++]);
      ++_counter;
    }
  }

  bool empty() {
    std::lock_guard<std::mutex> _lock(m_mutex);
    return m_lenght == 0;
  }

private:
  typedef std::array<t_data, t_size> array;
  typedef typename array::size_type size;

private:
  array m_array;
  size m_read = 0;
  size m_write = 0;
  size m_lenght = 0;
  std::mutex m_mutex;
  std::condition_variable m_cond_add;
  std::condition_variable m_cond_get;
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_QUEUE_H
