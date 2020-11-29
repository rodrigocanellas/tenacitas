#ifndef TENACITAS_CONCURRENT_QUEUE_H
#define TENACITAS_CONCURRENT_QUEUE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <functional>
#include <memory>
#include <optional>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief Base class for all types of queues used in the tenacitas::concurrent
///
/// \tparam t_log
///
/// \param type of data in the queue
template <typename t_log, typename t_data> struct queue_t {

  /// \brief Alias for the data
  typedef t_data data;

  /// \brief Alias for the log
  typedef t_log log;

  /// \brief Type of pointer to the queue
  typedef std::shared_ptr<queue_t> ptr;

  /// \brief Copy constructor not allowed
  queue_t() = default;

  /// \brief Destructor
  virtual ~queue_t() {}

  /// \brief Copy constructor not allowed
  queue_t(const queue_t &) = delete;

  /// \brief Move constructor not allowed
  queue_t(queue_t &&) = delete;

  /// \brief Copy assignment not allowed
  queue_t &operator=(const queue_t &) = delete;

  /// \brief Move assignment not allowed
  queue_t &operator=(queue_t &&) = delete;

  /// \brief Adds a t_data object to the queue
  virtual void add(const t_data &) = 0;

  /// \brief Adds a t_data object to the queue
  virtual void add(t_data &&) = 0;

  /// \brief Gets the first t_data obect added to the queue, if there is one
  virtual std::optional<t_data> get() = 0;

  /// \brief Informs if the queue is full
  virtual bool full() const = 0;

  /// \brief Informs if the queue is empty
  virtual bool empty() const = 0;

  /// \brief Informs the total capacity of the queue
  virtual size_t capacity() const = 0;

  /// \brief Informs the current number of slots occupied in the queue
  virtual size_t occupied() const = 0;

  /// \brief Traverses the queue
  ///
  /// \param p_function will be called for every data in the queue
  virtual void
  traverse(std ::function<void(const data &)> p_function) const = 0;
};

} // namespace concurrent
} // namespace tenacitas

#endif
