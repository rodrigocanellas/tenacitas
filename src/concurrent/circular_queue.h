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

template <class t_data, size_t t_size> class circular_queue_t {
public:
  typedef t_data data;

public:
  void add(const t_data &p_data) {
    m_array[m_write++] = p_data;
    ++m_lenght;
    if (m_write == (t_size)) {
      m_write = 0;
    }
  }

  void add(t_data &&p_data) {
    m_array[m_write++] = std::move(p_data);
    ++m_lenght;
    if (m_write == (t_size)) {
      m_write = 0;
    }
  }

  t_data get() {
    t_data _data = m_array[m_read++];
    --m_lenght;
    if (m_read == t_size) {
      m_read = 0;
    }
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

  inline bool has_room() const { return m_lenght < t_size; }
  inline bool has_data() const { return m_lenght > 0; }

private:
  typedef std::array<t_data, t_size> array;
  typedef typename array::size_type size;

private:
  array m_array;
  size m_read = 0;
  size m_write = 0;
  size m_lenght = 0;
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_QUEUE_H
