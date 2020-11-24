#ifndef TENACITAS_CONCURRENT_QUEUE_H
#define TENACITAS_CONCURRENT_QUEUE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <memory>
#include <optional>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

template <typename t_log, typename t_data> struct queue_t {

  typedef std::shared_ptr<queue_t> ptr;

  virtual ~queue_t() {}

  /// \brief adds a t_data object to the queue
  virtual void add(const t_data &) = 0;

  /// \brief adds a t_data object to the queue
  virtual void add(t_data &&) = 0;

  /// \brief gets the first t_data obect added to the queue, if there is one
  virtual std::optional<t_data> get() = 0;

  virtual bool full() const = 0;

  virtual bool empty() const = 0;

  virtual size_t capacity() const = 0;

  virtual size_t occupied() const = 0;
};

} // namespace concurrent
} // namespace tenacitas

#endif
