#ifndef TENACITAS_CONCURRENT_QUEUE_H
#define TENACITAS_CONCURRENT_QUEUE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstdint>
#include <functional>
#include <mutex>
#include <optional>
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
template <typename t_log, typename... t_data> class fixed_size_queue_t {
public:
  typedef std::vector<std::tuple<t_data...>> values;
  typedef int16_t size;

public:
  fixed_size_queue_t() = delete;

  fixed_size_queue_t(int16_t p_size) : m_size(p_size), m_values(p_size) {}

  fixed_size_queue_t(fixed_size_queue_t &&p_queue) noexcept {
    m_size = std::move(p_queue.m_size);
    m_values = std::move(p_queue.m_values);
    m_read = std::move(p_queue.m_read);
    m_write = std::move(p_queue.m_write);
    m_amount = std::move(p_queue.m_amount);
  }

  fixed_size_queue_t(const fixed_size_queue_t &) = default;

  fixed_size_queue_t &operator=(fixed_size_queue_t &&p_queue) noexcept {
    if (this != &p_queue) {
      m_size = std::move(p_queue.m_size);
      m_values = std::move(p_queue.m_values);
      m_read = std::move(p_queue.m_read);
      m_write = std::move(p_queue.m_write);
      m_amount = std::move(p_queue.m_amount);
    }
    return *this;
  }

  fixed_size_queue_t &operator=(const fixed_size_queue_t &) = default;

  void add(const t_data &... p_data) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (!full()) {
      m_values[m_write++] = std::move(std::make_tuple(p_data...));
      ++m_amount;
      if (m_write == m_size) {
        m_write = 0;
      }
    } else {
      concurrent_debug(m_log, "could not add ", std::make_tuple(p_data...),
                       ", because it is full");
    }
  }

  void add(t_data &&... p_data) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (!full()) {
      m_values[m_write++] = std::move(std::make_tuple(p_data...));
      ++m_amount;
      if (m_write == (m_size)) {
        m_write = 0;
      }
    } else {
      concurrent_debug(m_log, "could not add ", std::make_tuple(p_data...),
                       ", because it is full");
    }
  }

  std::optional<std::tuple<t_data...>> get() {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (empty()) {
      concurrent_debug(m_log, "could not get because it is empty");
      return {};
    }
    std::tuple<t_data...> _data(m_values[m_read++]);
    --m_amount;
    if (m_read == m_size) {
      m_read = 0;
    }
    return {_data};
  }

  void traverse(
      std ::function<void(const std::tuple<t_data...> &)> p_function) const {
    size _i = m_read;
    size _counter = 0;
    while (_counter < m_amount) {
      if (_i == m_size) {
        _i = 0;
      }
      p_function(m_values[_i++]);
      ++_counter;
    }
  }

  inline bool full() const { return m_amount == m_size; }
  inline bool empty() const { return m_amount == 0; }
  inline size capacity() const { return m_size; }
  inline size occupied() const { return m_amount; }

private:
  void report(const char *p_str, const t_data &... p_data) {
    concurrent_debug(m_log, p_str, ": data = ", p_data..., ", read = ", m_read,
                     ", write = ", m_write, ", length = ", m_amount);
  }

private:
  /// \brief size of the buffer
  size m_size = 0;
  /// \brief buffer
  values m_values;
  /// \brief position from where to read
  size m_read = 0;
  /// \brief position where to write
  size m_write = 0;
  /// \brief amount of elements
  size m_amount = 0;
  /// \brief controls access to positions
  std::mutex m_mutex;
  /// \brief logs messages
  t_log m_log{"concurrent::fixed_size_queue.h"};
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_QUEUE_H
