/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_DAT_CIRCULAR_QUEUE_H
#define TNCT_CONTAINER_DAT_CIRCULAR_QUEUE_H

#include <cstddef>
#include <mutex>
#include <optional>
#include <string_view>
#include <vector>

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
template <log::cpt::logger t_logger, typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
         && ostream::cpt::has_output_operator<t_data>
class circular_queue final
{
public:
  using data   = t_data;
  using logger = t_logger;

public:
  circular_queue() = delete;

  static constexpr std::size_t default_initial_size{100};
  static constexpr std::size_t default_incremental_size{default_initial_size
                                                         / 2};

  static std::optional<circular_queue>
  create(t_logger &p_logger, std::size_t p_initial_size = default_initial_size,
         std::size_t      p_incremental_size = default_incremental_size,
         std::string_view p_desc             = "NO DESC")
  {
    try
    {
      if (p_initial_size == 0)
      {
        return std::nullopt;
      }

      const auto _incremental_size{(p_incremental_size == 0
                                        ? (p_initial_size / 2) + 1
                                        : p_incremental_size)};
      return circular_queue(p_logger, p_desc, p_initial_size,
                            _incremental_size);
    }
    catch (...)
    {
      TNCT_LOG_ERR(
          p_logger,
          format::bus::fmt("Error creating 'circular_queue' named '", p_desc,
                           "', with initial size =  ", p_initial_size,
                           ", and incremental size = ", p_initial_size / 2));
    }
    return std::nullopt;
  }

  ~circular_queue()
  {
  }

  circular_queue(const circular_queue &p_queue)
      : m_logger(p_queue.m_logger), m_desc(p_queue.m_desc),
        m_initial_size(p_queue.m_initial_size),
        m_incremental_size(p_queue.m_incremental_size),
        m_vector(p_queue.m_vector), m_head(p_queue.m_head),
        m_tail(p_queue.m_tail), m_occupied(p_queue.m_occupied)
  {
  }

  circular_queue(circular_queue &&p_queue)
      : m_logger(p_queue.m_logger), m_desc(std::move(p_queue.m_desc)),
        m_initial_size(p_queue.m_initial_size),
        m_incremental_size(p_queue.m_incremental_size),
        m_vector(std::move(p_queue.m_vector)),
        m_head(std::move(p_queue.m_head)), m_tail(std::move(p_queue.m_tail)),
        m_occupied(p_queue.m_occupied)
  {
  }

  circular_queue &operator=(const circular_queue &p_queue)
  {
    if (this != &p_queue)
    {
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_initial_size     = p_queue.m_initial_size;
      m_incremental_size = p_queue.m_incremental_size;
      m_vector           = p_queue.m_vector;
      m_head             = p_queue.m_head;
      m_tail             = p_queue.m_tail;
      m_occupied         = p_queue.m_occupied;
      m_desc             = p_queue.m_desc;
    }
    return *this;
  }

  circular_queue &operator=(circular_queue &&p_queue)
  {
    if (this != &p_queue)
    {
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_initial_size     = p_queue.m_initial_size;
      m_incremental_size = p_queue.m_incremental_size;
      m_vector           = std::move(p_queue.m_vector);
      m_head             = p_queue.m_head;
      m_tail             = p_queue.m_tail;
      m_occupied         = p_queue.m_occupied;
      m_desc             = std::move(p_queue.m_desc);
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

      auto _last(m_vector.size() - 1);

      for (decltype(_last) _idx = 0; _idx < (_last - 1); ++_idx)
      {
        _out << "queue[" << _idx << "] = " << *m_vector[_idx] << '\n';
      }
      _out << "queue[" << _last << "] = " << *m_vector[_last];
    }
    return _out.str();
  }

  std::string brief_report()
  {
    std::stringstream _out;
    _out << "desc = '" << m_desc << "', head = " << m_head
         << ", tail = " << m_tail << ", occupied = " << occupied()
         << ", capacity = " << capacity()
         << ", increment = " << m_incremental_size;
    return _out.str();
  }

  void push(t_data &&p_data)
  {
    std::lock_guard<std::mutex> _lock(m_mutex);

    TNCT_LOG_TRA(this->m_logger,
                 format::bus::fmt("push - entering ", brief_report()));

    if (full())
    {
      enlarge();
    }

    m_vector[m_head] = std::optional<t_data>(p_data);

    if (m_head == (m_vector.size() - 1))
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

    if (full())
    {
      enlarge();
    }

    m_vector[m_head] = std::optional<t_data>(p_data);

    if (m_head == (m_vector.size() - 1))
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

    std::optional<t_data> _data(m_vector[m_tail]);
    ++m_tail;

    if (m_tail == m_vector.size())
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
    return m_occupied == m_vector.size();
  }

  constexpr bool empty() const
  {
    return m_occupied == 0;
  }

  constexpr size_t capacity() const
  {
    return m_vector.size();
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
    return m_vector[p_index];
  }

private:
  using vector = std::vector<std::optional<t_data>>;

private:
  circular_queue(t_logger &p_logger, std::string_view p_desc,
                 std::size_t p_initial_size, std::size_t p_incremental_size)
      : m_logger(p_logger), m_desc(p_desc), m_initial_size(p_initial_size),
        m_incremental_size(p_incremental_size),
        m_vector(m_initial_size, std::optional<t_data>()), m_head(0), m_tail(0)
  {

    TNCT_LOG_TRA(this->m_logger,
                 format::bus::fmt("creating - ", brief_report()));
  }

  void enlarge()
  {
    TNCT_LOG_TRA(this->m_logger,
                 format::bus::fmt("enlarging - entering ", full_report()));

    vector _aux(m_vector.size() + m_incremental_size, std::optional<t_data>());

    auto _head(m_head);
    if (m_head == 0)
    {
      _head = m_vector.size();
    }

    std::move(&m_vector[_head], &m_vector[m_vector.size()],
              &_aux[_head + m_incremental_size]);

    std::move(&m_vector[0], &m_vector[_head], &_aux[0]);

    // m_head = _head + 1;

    // m_tail += m_incremental_size;

    if (m_head == 0)
    {
      m_head = m_vector.size();
    }

    if (m_tail != 0)
    {
      m_tail += m_incremental_size;
    }

    m_vector = std::move(_aux);
    TNCT_LOG_TRA(this->m_logger,
                 format::bus::fmt("enlarging - leaving ", full_report()));

    TNCT_LOG_TRA(this->m_logger,
                 format::bus::fmt("enlarged - ", brief_report()));
  }

private:
  logger     &m_logger;
  std::string m_desc;

  size_t m_initial_size{0};
  size_t m_incremental_size{0};
  vector m_vector;
  size_t m_head{0};
  size_t m_tail{0};
  size_t m_occupied{0};

  std::mutex m_mutex;
};

} // namespace tnct::container::dat

#endif
