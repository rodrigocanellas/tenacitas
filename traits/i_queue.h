/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_I_QUEUE_H
#define TENACITAS_LIB_TRAITS_I_QUEUE_H

#include <cstddef>
#include <optional>
#include <string>

#include "tenacitas.lib/traits/has_output_operator.h"
#include "tenacitas.lib/traits/logger.h"

namespace tenacitas::lib::traits {

template <traits::logger t_logger, typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data> &&
    has_output_operator<t_data>
class i_queue {
public:
  using data = t_data;
  using logger = t_logger;

  // i_queue() = default;

  // queue name, logger and initial size
  i_queue(std::string_view p_queue_id, logger &p_logger)
      : m_id(p_queue_id), m_logger(p_logger) {}

  i_queue(const i_queue &p_queue)
      : m_id(p_queue.m_id), m_logger(p_queue.m_logger) {}

  i_queue(i_queue &&p_queue)
      : m_id(std::move(p_queue.m_id)), m_logger(p_queue.m_logger) {}

  i_queue &operator=(const i_queue &p_queue) {
    if (this != &p_queue) {
      m_id = p_queue.m_id;
      m_logger = p_queue.m_logger;
    }
    return *this;
  }

  i_queue &operator=(i_queue &&p_queue) {
    if (this != &p_queue) {
      m_id = std::move(p_queue.m_id);
      m_logger = p_queue.m_logger;
    }
    return *this;
  }

  virtual ~i_queue() {}

  virtual void push(data &&) = 0;

  virtual void push(const data &) = 0;

  virtual std::optional<data> pop() = 0;

  virtual bool full() const = 0;

  virtual bool empty() const = 0;

  virtual size_t capacity() const = 0;

  virtual size_t occupied() const = 0;

  virtual void clear() = 0;

  virtual const std::string &id() const = 0;

protected:
  std::string m_id;
  logger &m_logger;
};

} // namespace tenacitas::lib::traits

#endif
