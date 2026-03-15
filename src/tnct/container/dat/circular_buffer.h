/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_DAT_CIRCULAR_BUFFER_H
#define TNCT_CONTAINER_DAT_CIRCULAR_BUFFER_H

#include <array>
#include <cstddef>
#include <functional>
#include <mutex>
#include <optional>

#include "tnct/format/bus/fmt.h"
#include "tnct/log/cpt/logger.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/ostream/cpt/has_output_operator.h"

namespace tnct::container::dat
{

/// \brief Implements a circular queue which size is increased if it becomes
/// full
///
/// The purpose is to avoid unnecessary memory allocations to create nodes in
/// the queue by reusing nodes which data have been read
///
/// \tparam t_data defines the types of the data contained in the queue
template <log::cpt::logger t_logger, typename t_data, std::size_t t_size>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
         && ostream::cpt::has_output_operator<t_data>
class circular_buffer final
{
public:
  using data   = t_data;
  using logger = t_logger;

public:
  circular_buffer() = default;

  ~circular_buffer() = default;

  circular_buffer(const circular_buffer &p_queue)
      : m_logger(p_queue.m_logger), m_values(p_queue.m_values),
        m_head(p_queue.m_head), m_tail(p_queue.m_tail),
        m_occupied(p_queue.m_occupied)
  {
  }

  circular_buffer(circular_buffer &&p_queue)
      : m_logger(p_queue.m_logger), m_values(std::move(p_queue.m_values)),
        m_head(std::move(p_queue.m_head)), m_tail(std::move(p_queue.m_tail)),
        m_occupied(p_queue.m_occupied)
  {
  }

  circular_buffer &operator=(const circular_buffer &p_queue)
  {
    if (this != &p_queue)
    {
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_values   = p_queue.m_values;
      m_head     = p_queue.m_head;
      m_tail     = p_queue.m_tail;
      m_occupied = p_queue.m_occupied;
    }
    return *this;
  }

  circular_buffer &operator=(circular_buffer &&p_queue)
  {
    if (this != &p_queue)
    {
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_values   = std::move(p_queue.m_values);
      m_head     = p_queue.m_head;
      m_tail     = p_queue.m_tail;
      m_occupied = p_queue.m_occupied;
    }
    return *this;
  }

  std::string full_report()
  {
    std::stringstream _out;

    if (occupied() != 0)
    {
      _out << '\n';
    }
    _out << brief_report();
    if (occupied() != 0)
    {
      _out << '\n';

      auto _last(m_values.size() - 1);

      for (decltype(_last) _idx = 0; _idx < (_last - 1); ++_idx)
      {
        _out << "queue[" << _idx << "] = " << *m_values[_idx] << '\n';
      }
      _out << "queue[" << _last << "] = " << *m_values[_last];
    }
    return _out.str();
  }

  std::string brief_report()
  {
    std::stringstream _out;
    _out << "head = " << m_head << ", tail = " << m_tail
         << ", occupied = " << occupied() << ", capacity = " << capacity();
    return _out.str();
  }

  void push(t_data &&p_data)
  {
    std::lock_guard<std::mutex> _lock(m_mutex);

    TNCT_LOG_TRA(this->m_logger,
                 format::bus::fmt("push - entering ", brief_report()));

    m_values[m_head] = std::optional<t_data>(p_data);

    if (m_head == (m_values.size() - 1))
    {
      m_head = 0;
    }
    else
    {
      ++m_head;
    }
    ++m_occupied;

    TNCT_LOG_TRA(this->m_logger,
                 format::bus::fmt("push - leaving: ", brief_report()));
  }

  void push(const t_data &p_data)
  {
    std::lock_guard<std::mutex> _lock(m_mutex);

    TNCT_LOG_TRA(this->m_logger,
                 format::bus::fmt("push - entering with data = ", p_data, ": ",
                                  brief_report()));

    m_values[m_head] = std::optional<t_data>(p_data);

    if (m_head == (m_values.size() - 1))
    {
      m_head = 0;
    }
    else
    {
      ++m_head;
    }
    ++m_occupied;

    TNCT_LOG_TRA(this->m_logger,
                 format::bus::fmt("push - leaving: ", brief_report()));
  }

  std::optional<t_data> pop()
  {
    std::lock_guard<std::mutex> _lock(m_mutex);

    TNCT_LOG_TRA(this->m_logger,
                 format::bus::fmt("pop - entering: ", brief_report()));

    if (empty())
    {
      return std::nullopt;
    }

    std::optional<t_data> _data(m_values[m_tail]);
    ++m_tail;

    if (m_tail == m_values.size())
    {
      m_tail = 0;
    }

    --m_occupied;

    TNCT_LOG_TRA(this->m_logger,
                 format::bus::fmt("pop - leaving: ", brief_report()));

    return _data;
  }

  constexpr bool full() const
  {
    return m_occupied == m_values.size();
  }

  constexpr bool empty() const
  {
    return m_occupied == 0;
  }

  constexpr size_t capacity() const
  {
    return m_values.size();
  }

  constexpr size_t occupied() const
  {
    return m_occupied;
  }

  void clear()
  {
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_head = m_tail = 0;
    m_occupied      = 0;
  }

  constexpr size_t head() const
  {
    return m_head;
  }
  constexpr size_t tail() const
  {
    return m_tail;
  }

  constexpr t_data operator[](size_t p_index) const
  {
    return m_values[p_index];
  }

  void traverse(std::function<void(const data &)> &p_function)
  {
    if (empty())
    {
      TNCT_LOG_DEB(m_logger, "not traversing because buffer is empty");
      return;
    }
    traverse(m_head, p_function);
  }

  static constexpr decltype(t_size) size = t_size;

private:
  using values = std::array<std::optional<data>, size>;

private:
  void traverse(std::size_t                        p_start,
                std::function<void(const data &)> &p_function)
  {
    std::size_t _current{p_start};
    while ((_current != m_tail) || (_current != size))
    {
      p_function(*m_values[_current]);
    }
    if (_current == size)
    {
      traverse(0, p_function);
    }
  }

private:
  logger &m_logger;

  values      m_values;
  std::size_t m_head{0};
  std::size_t m_tail{0};
  std::size_t m_occupied{0};

  std::mutex m_mutex;
};

} // namespace tnct::container::dat

#endif
