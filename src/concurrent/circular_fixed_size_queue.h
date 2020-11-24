#ifndef TENACITAS_CONCURRENT_CIRCULAR_FIXED_SIZE_QUEUE_H
#define TENACITAS_CONCURRENT_CIRCULAR_FIXED_SIZE_QUEUE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstdint>
#include <functional>
#include <mutex>
#include <optional>
#include <vector>

#include <concurrent/internal/log.h>
#include <concurrent/queue.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief fixed_size_queue_t implments a circular queue with fixed size
///
/// Unlike the usual implementation, if the buffer is full, the insertion of an
/// element *does not* override the first element
///
/// This specialization defines a single type contained in the queue
///
/// \tparam t_log provides log funcionality:
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
/// void fatal(int p_line, const t_params&... p_params)
///
/// \tparam t_data... defines the types of the data contained in the buffer
///
template <typename t_log, typename t_data>
struct circular_fixed_size_queue_t : public queue_t<t_log, t_data> {
public:
  typedef t_data data;

  typedef t_log log;

  typedef queue_t<log, data> queue;

  typedef typename queue::ptr ptr;

  static ptr create(size_t p_queue_size) {
    return ptr(new circular_fixed_size_queue_t(p_queue_size));
  }

public:
  circular_fixed_size_queue_t() = delete;

  circular_fixed_size_queue_t(int16_t p_size)
      : m_size(p_size), m_values(p_size) {}

  circular_fixed_size_queue_t(circular_fixed_size_queue_t &&p_queue) noexcept {
    m_size = std::move(p_queue.m_size);
    m_values = std::move(p_queue.m_values);
    m_read = std::move(p_queue.m_read);
    m_write = std::move(p_queue.m_write);
    m_amount = std::move(p_queue.m_amount);
  }

  circular_fixed_size_queue_t(const circular_fixed_size_queue_t &) = default;

  circular_fixed_size_queue_t &
  operator=(circular_fixed_size_queue_t &&p_queue) noexcept {
    if (this != &p_queue) {
      m_size = std::move(p_queue.m_size);
      m_values = std::move(p_queue.m_values);
      m_read = std::move(p_queue.m_read);
      m_write = std::move(p_queue.m_write);
      m_amount = std::move(p_queue.m_amount);
    }
    return *this;
  }

  circular_fixed_size_queue_t &
  operator=(const circular_fixed_size_queue_t &) = default;

  void add(const t_data &p_data) override {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (!full()) {
      m_values[m_write++] = p_data;
      ++m_amount;
      if (m_write == m_size) {
        m_write = 0;
      }
    } else {
      concurrent_debug(m_log, "could not add ", p_data, ", because it is full");
    }
  }

  void add(data &&p_data) override {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (!full()) {
      m_values[m_write++] = std::move(p_data);
      ++m_amount;
      if (m_write == m_size) {
        m_write = 0;
      }
    } else {
      concurrent_debug(m_log, "could not add ", p_data, ", because it is full");
    }
  }

  std::optional<data> get() override {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (empty()) {
      concurrent_debug(m_log, "could not get because it is empty");
      return {};
    }
    data _data(m_values[m_read++]);
    --m_amount;
    if (m_read == m_size) {
      m_read = 0;
    }
    return {_data};
  }

  void traverse(std ::function<void(const data &)> p_function) const {
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

  inline bool full() const override { return m_amount == m_size; }
  inline bool empty() const override { return m_amount == 0; }
  inline size_t capacity() const override { return m_size; }
  inline size_t occupied() const override { return m_amount; }

private:
  typedef std::vector<t_data> values;
  typedef typename values::size_type size;

private:
  void report(const char *p_str, const t_data &p_data) {
    concurrent_debug(m_log, p_str, ": data = ", p_data, ", read = ", m_read,
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
