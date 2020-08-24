#ifndef TENACITAS_CONCURRENT_QUEUE_H
#define TENACITAS_CONCURRENT_QUEUE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstdint>
#include <functional>
#include <mutex>
#include <vector>

#include <concurrent/internal/log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

///
///
/// \tparam t_log provides log funcionality:
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
/// void fatal(int p_line, const t_params&... p_params)
///
template <class t_data, typename t_log> class fixed_size_queue_t {
public:
  typedef t_data data;
  typedef std::vector<t_data> values;
  typedef typename values::size_type size;

public:
  fixed_size_queue_t() = delete;

  template <typename t_int>
  fixed_size_queue_t(t_int p_size)
      : m_size(static_cast<size>(p_size)), m_read(0), m_write(0), m_length(0),
        m_values(p_size, t_data()) {}

  fixed_size_queue_t(fixed_size_queue_t &&p_queue) noexcept {
    m_size = std::move(p_queue.m_size);
    m_values = std::move(p_queue.m_values);
    m_read = std::move(p_queue.m_read);
    m_write = std::move(p_queue.m_write);
    m_length = std::move(p_queue.m_length);
  }

  fixed_size_queue_t(const fixed_size_queue_t &) = default;

  fixed_size_queue_t &operator=(fixed_size_queue_t &&p_queue) noexcept {
    if (this != &p_queue) {
      m_size = std::move(p_queue.m_size);
      m_values = std::move(p_queue.m_values);
      m_read = std::move(p_queue.m_read);
      m_write = std::move(p_queue.m_write);
      m_length = std::move(p_queue.m_length);
    }
    return *this;
  }

  fixed_size_queue_t &operator=(const fixed_size_queue_t &) = default;

  void add(const t_data &p_data) {
    //    report("starting add", p_data);
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_values[m_write++] = p_data;
    ++m_length;
    if (m_write == (m_size)) {
      m_write = 0;
    }
    //    report("finish add", p_data);
  }

  void add(t_data &&p_data) {
    //    report("starting add", p_data);
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_values[m_write++] = std::move(p_data);
    ++m_length;
    if (m_write == (m_size)) {
      m_write = 0;
    }
    //    report("finishing add", p_data);
  }

  t_data get() {
    report("starting get", m_values[m_read]);
    std::lock_guard<std::mutex> _lock(m_mutex);
    t_data _data = m_values[m_read++];
    --m_length;
    if (m_read == m_size) {
      m_read = 0;
    }
    report("after get", _data);
    return _data;
  }

  void traverse(std ::function<void(const t_data &)> p_function) const {
    size _i = m_read;
    size _counter = 0;
    while (_counter < m_length) {
      if (_i == m_size) {
        _i = 0;
      }
      p_function(m_values[_i++]);
      ++_counter;
    }
  }

  inline bool full() const { return m_length == m_size; }
  inline bool empty() const {
    //    debug(t_log, "m_length = ", m_length);
    return m_length == 0;
  }
  inline size capacity() const {
    //    debug(t_log, "m_length = ", m_length);
    return m_size;
  }
  inline size occupied() const { return m_length; }

private:
  void report(const char *p_str, const data &p_data) {
    concurrent_debug(m_log, p_str, ": data = ", p_data, ", read = ", m_read,
          ", write = ", m_write, ", length = ", m_length);
  }

private:
  size m_size = 0;
  size m_read = 0;
  size m_write = 0;
  size m_length = 0;
  values m_values;
  std::mutex m_mutex;
  t_log m_log{"fixed_size_queue.h"};
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_QUEUE_H
