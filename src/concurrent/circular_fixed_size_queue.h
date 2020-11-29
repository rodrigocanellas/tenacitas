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

/// \brief Implements a circular queue with fixed size
///
/// Unlike the usual implementation, if the buffer is full, the insertion of an
/// element *does not* override the first element
///
/// \tparam t_log
///
/// \tparam t_data defines the types of the data contained in the buffer
template <typename t_log, typename t_data>
struct circular_fixed_size_queue_t : public queue_t<t_log, t_data> {

  /// \brief Alias for the data
  typedef t_data data;

  /// \brief Alias for the log
  typedef t_log log;

  /// \brief Type of pointer to the queue
  typedef std::shared_ptr<circular_fixed_size_queue_t> ptr;

  /// \brief Creates a queue with a fixed size
  ///
  /// \param p_size the number of slots in the queue
  ///
  /// \return a pointer to the created queue
  static ptr create(size_t p_size) {
    ptr _ptr(new circular_fixed_size_queue_t(p_size));
    return _ptr;
  }

  /// \brief Adds a t_data object to the queue
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

  /// \brief Adds a t_data object to the queue
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

  /// \brief Gets the first t_data obect added to the queue, if there is one
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

  /// \brief Traverses the queue
  ///
  /// \param p_function will be called for every data in the queue
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

  /// \brief Informs if the queue is full
  inline bool full() const override { return m_amount == m_size; }

  /// \brief Informs if the queue is empty
  inline bool empty() const override { return m_amount == 0; }

  /// \brief Informs the total capacity of the queue
  inline size_t capacity() const override { return m_size; }

  /// \brief Informs the current number of slots occupied in the queue
  inline size_t occupied() const override { return m_amount; }

private:
  /// \brief Constructor
  ///
  /// \param p_size the number of slots in the queue
  circular_fixed_size_queue_t(size_t p_size)
      : m_size(p_size), m_values(p_size) {}

private:
  /// \brief Queue implemented as a vector
  typedef std::vector<t_data> values;

  /// \brief Type of size of the queue
  typedef typename values::size_type size;

  // private:
  //  /// \brief Helper
  //  void report(const char *p_str, const t_data &p_data) {
  //    concurrent_debug(m_log, p_str, ": data = ", p_data, ", read = ", m_read,
  //                     ", write = ", m_write, ", length = ", m_amount);
  //  }

private:
  /// \brief Size of the buffer
  size m_size = 0;

  /// \brief Vector with the values
  values m_values;

  /// \brief Position from where to read
  size m_read = 0;

  /// \brief Position where to write
  size m_write = 0;

  /// \brief Amount of elements
  size m_amount = 0;

  /// \brief Controls access to positions
  std::mutex m_mutex;

  /// \brief Llogs messages
  t_log m_log{"concurrent::circular_fixed_size_queue"};
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_CIRCULAR_FIXED_SIZE_QUEUE_H
