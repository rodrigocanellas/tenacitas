#ifndef TENACITAS_LIB_CONTAINER_TYP_CIRCULAR_QUEUE_H
#define TENACITAS_LIB_CONTAINER_TYP_CIRCULAR_QUEUE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <condition_variable>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <thread>
#include <utility>

#include <tenacitas.lib/src/log/alg/logger.h>
#include <tenacitas.lib/src/number/typ/id.h>

/// \brief master namespace
namespace tenacitas::lib::container::typ {

// \brief Implements a circular queue which size is increased if it becomes
// full
//
// \tparam t_data defines the types of the data contained in the
// buffer. It must implement default constructor
template <typename t_data> struct circular_queue_t {

  using data = t_data;

  circular_queue_t() = default;

  //  circular_unlimited_size_queue_t() = default;

  ~circular_queue_t() = default;

  // \brief Copy constructor not allowed
  circular_queue_t(const circular_queue_t &) = delete;

  // \brief Move constructor
  circular_queue_t(circular_queue_t &&p_queue)
      : m_list(std::move(p_queue.m_list)), m_write(std::move(p_queue.m_write)),
        m_read(std::move(p_queue.m_read)),
        m_amount(std::move(p_queue.m_amount)) {}

  // \brief Copy assignment not allowed
  circular_queue_t &operator=(const circular_queue_t &) = delete;

  // \brief Move assignment
  circular_queue_t &operator=(circular_queue_t &&p_queue) {
    if (this != &p_queue) {
      m_amount = p_queue.m_amount;

      m_list = std::move(p_queue.m_list);
      m_write = std::move(p_queue.m_write);
      m_read = std::move(p_queue.m_read);
    }
    return *this;
  }

  // \brief Traverses the queue
  //
  // \param p_function will be called for every data in the queue
  void traverse(std::function<void(const t_data &)> p_visitor) {

    if (m_amount == 0) {
      return;
    }
    auto _end{m_list.end()};
    auto _begin{m_list.begin()};
#ifdef TENACITAS_LOG
    TNCT_LOG_DEB("read = ", std::distance(_begin, m_read),
                 ", write = ", std::distance(_begin, m_write));
#endif

    auto _dist{std::distance(m_read, m_write)};

    if (_dist < 0) {
      for (const_iterator _ite = m_read; _ite != _end; ++_ite) {
        p_visitor(*_ite);
      }

      for (const_iterator _ite = _begin; _ite != m_write; ++_ite) {
        p_visitor(*_ite);
      }

    } else {
      for (const_iterator _ite = m_read; _ite != m_write; ++_ite) {
        p_visitor(*_ite);
      }
    }

    //    auto _end{m_list.end()};
    //    auto _begin{m_list.begin()};
    //#ifdef TENACITAS_LOG
    //    TNCT_LOG_DEB("read = ", std::distance(_begin, m_read),
    //                 ", write = ", std::distance(_begin, m_write));
    //#endif

    //    for (const_iterator _ite = _begin; _ite != _end; ++_ite) {
    //      p_visitor(*_ite);
    //    }
  }

  // \brief Adds a t_data object to the queue
  //
  // \param p_data is a t_data to be added
  void add(t_data &&p_data) {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("queue ", m_id, " - adding data");
#endif

    std::lock_guard<std::recursive_mutex> _lock(m_mutex);

    if (m_list.empty()) {
      m_list.push_back(std::forward<t_data>(p_data));
      m_read = m_list.begin();
      m_write = m_list.end();
      ++m_amount;
      return;
    }

    auto _begin = m_list.begin();
    auto _end = m_list.end();

    if (m_write == _end) {
      if (m_read == _begin) {
        m_list.push_back(std::forward<t_data>(p_data));
        m_write = m_list.end();
        ++m_amount;
      } else {
        m_write = _begin;
        add(std::forward<t_data>(p_data));
      }
      return;
    }

    if (m_write == m_read) {
      m_write = m_list.insert(std::next(m_write), std::forward<t_data>(p_data));
      ++m_write;
      ++m_amount;
      return;
    }

    *m_write = std::forward<t_data>(p_data);
    ++m_write;
    ++m_amount;

#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("queue ", m_id, " - capacity = ", capacity(),
                 ", occupied = ", occupied());
#endif
  }

  void add(const t_data &p_data) {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("queue ", m_id, " - adding data");
#endif

    std::lock_guard<std::recursive_mutex> _lock(m_mutex);

    if (m_list.empty()) {
      m_list.push_back(p_data);
      m_read = m_list.begin();
      m_write = m_list.end();
      ++m_amount;
      return;
    }

    auto _begin = m_list.begin();
    auto _end = m_list.end();

    if (m_write == _end) {
      if (m_read == _begin) {
        m_list.push_back(p_data);
        m_write = m_list.end();
        ++m_amount;
      } else {
        m_write = _begin;
        add(p_data);
      }
      return;
    }

    if (m_write == m_read) {
      m_write = m_list.insert(std::next(m_write), p_data);
      ++m_write;
      ++m_amount;
      return;
    }

    *m_write = p_data;
    ++m_write;
    ++m_amount;

#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("queue ", m_id, " - capacity = ", capacity(),
                 ", occupied = ", occupied());
#endif
  }

  // \brief Tries to get a t_data object from the queue
  std::optional<t_data> get() {
    std::lock_guard<std::recursive_mutex> _lock(m_mutex);

    if (m_amount == 0) {
      return {};
    }

    auto _begin = m_list.begin();

    if ((m_read == m_list.end()) && (m_write != _begin)) {
      m_read = _begin;
      return get();
    }

    t_data _data = *m_read;
    ++m_read;
    --m_amount;
    return {_data};
  }

  // \brief Informs if the queue is full
  inline constexpr bool full() const { return (m_amount == m_list.size()); }

  // \brief Informs if the queue is empty
  inline constexpr bool empty() const { return m_amount == 0; }

  // \brief Informs the total capacity of the queue
  inline constexpr size_t capacity() const { return m_list.size(); }

  // \brief Informs the current number of slots occupied in the
  // queue
  inline constexpr size_t occupied() const { return m_amount; }

  inline size_t get_id() const { return m_id; }

  void clear() { m_list.clear(); }

private:
  using list = std::list<t_data>;
  using iterator = typename list::iterator;
  using const_iterator = typename list::const_iterator;

private:
  // \brief Amount of nodes actually used
  size_t m_amount{0};

  // \brief Controls insertion
  std::recursive_mutex m_mutex;

  // \brief for debug purposes
  number::typ::id m_id;

  list m_list;
  iterator m_write;
  iterator m_read;
};

} // namespace tenacitas::lib::container::typ

#endif
