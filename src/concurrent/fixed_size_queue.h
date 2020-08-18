#ifndef TENACITAS_CONCURRENT_QUEUE_H
#define TENACITAS_CONCURRENT_QUEUE_H

#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <vector>

#include <concurrent/internal/log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

template <class t_data> class fixed_size_queue_t {
public:
  typedef t_data data;

public:
  fixed_size_queue_t() = delete;
  fixed_size_queue_t(fixed_size_queue_t &&) = default;
  fixed_size_queue_t(const fixed_size_queue_t &) = default;
  fixed_size_queue_t &operator=(fixed_size_queue_t &&) = default;
  fixed_size_queue_t &operator=(const fixed_size_queue_t &) = default;

  template <typename t_int>
  fixed_size_queue_t(t_int p_size)
      : m_size(static_cast<size>(p_size)), m_values(p_size, t_data()) {}

  void add(const t_data &p_data) {
    m_values[m_write++] = p_data;
    ++m_lenght;
    if (m_write == (m_size)) {
      m_write = 0;
    }
  }

  void add(t_data &&p_data) {
    m_values[m_write++] = std::move(p_data);
    ++m_lenght;
    if (m_write == (m_size)) {
      m_write = 0;
    }
  }

  t_data get() {
    t_data _data = m_values[m_read++];
    --m_lenght;
    if (m_read == m_size) {
      m_read = 0;
    }
    return _data;
  }

  void traverse(std ::function<void(const t_data &)> p_function) {
    size _i = m_read;
    size _counter = 0;
    while (_counter < m_lenght) {
      if (_i == m_size) {
        _i = 0;
      }
      p_function(m_values[_i++]);
      ++_counter;
    }
  }

  inline bool full() const { return m_lenght < m_size; }
  inline bool empty() const { return m_lenght > 0; }

private:
  typedef std::vector<t_data> values;
  typedef typename values::size_type size;

private:
  size m_size = 0;
  values m_values;
  size m_read = 0;
  size m_write = 0;
  size m_lenght = 0;
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_QUEUE_H
