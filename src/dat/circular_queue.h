/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_DAT_CIRCULAR_QUEUE_H
#define TENACITAS_LIB_DAT_CIRCULAR_QUEUE_H

#include <concepts>
#include <cstddef>
#include <functional>
#include <list>
#include <mutex>
#include <optional>

namespace tenacitas::lib::dat {

/// \brief Implements a circular queue which size is increased if it becomes
/// full
///
/// The purpose is to avoid unnecessary memory allocations to create nodes in
/// the queue by reusing nodes which data have been read
///
/// \tparam t_data defines the types of the data contained in the queue
template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
struct circular_queue {
  using data = t_data;

  circular_queue() = default;

  circular_queue(const circular_queue &p_queue);

  circular_queue(circular_queue &&) = default;

  circular_queue &operator=(const circular_queue &p_queue);

  circular_queue &operator=(circular_queue &&p_queue) = default;

  ~circular_queue() = default;

  void traverse(std::function<void(const t_data &)> p_visitor) const;

  void add(t_data &&p_data);

  void add(const t_data &p_data);

  std::optional<t_data> get();

  constexpr bool full() const { return (m_amount == m_list.size()); }

  constexpr bool empty() const { return m_amount == 0; }

  /// \brief amount of nodes in the queue
  constexpr size_t capacity() const { return m_list.size(); }

  /// \brief amount of nodes that that have t_data objects
  constexpr size_t occupied() const { return m_amount; }

  /// \brief returns an identifier used for debug purposes
  size_t get_id() const { return m_id; }

  void clear() {
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_head = m_tail;
    m_amount = 0;
  }

private:
  using list = std::list<t_data>;
  using iterator = typename list::iterator;
  using const_iterator = typename list::const_iterator;

private:
  // Amount of nodes actually used
  size_t m_amount{0};

  list m_list;
  iterator m_head;
  iterator m_tail;

  id_t m_id;

  std::mutex m_mutex;
};

// #############################################################################
// IMPLEMENTATIION

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
circular_queue<t_data>::circular_queue(const circular_queue &p_queue)
    : m_amount(p_queue.m_amount), m_list(p_queue.m_list),
      m_head(p_queue.m_head), m_tail(p_queue.m_tail) {}

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
    circular_queue<t_data>
&circular_queue<t_data>::operator=(const circular_queue &p_queue) {
  if (this != &p_queue) {
    m_amount = p_queue.m_amount;
    m_list = p_queue.m_list;
    m_head = p_queue.m_head;
    m_tail = p_queue.m_tail;
  }
  return *this;
}

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
void circular_queue<t_data>::traverse(
    std::function<void(const t_data &)> p_visitor) const {
  if (m_amount == 0) {
    return;
  }

  if (m_head == m_tail) {
    p_visitor(*m_tail);
    return;
  }

  const_iterator _ite{m_tail};
  while (_ite != m_head) {
    if (_ite == m_list.end()) {
      _ite = m_list.begin();
      p_visitor(*_ite);
    } else {
      p_visitor(*_ite);
      ++_ite;
    }
  }
}

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
void circular_queue<t_data>::add(t_data &&p_data) {
  std::lock_guard<std::mutex> _lock(m_mutex);

  if (m_list.empty()) {
    m_list.emplace_back(std::move(p_data));
    m_tail = m_head = m_list.begin();
    ++m_amount;
    return;
  }

  if (full()) {
    m_head = m_list.emplace(std::next(m_head), std::move(p_data));
  } else {
    ++m_head;
    if (m_head == m_list.end()) {
      m_head = m_list.begin();
    }
  }

  *m_head = std::move(p_data);
  ++m_amount;
}

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
void circular_queue<t_data>::add(const t_data &p_data) {
  std::lock_guard<std::mutex> _lock(m_mutex);

  if (m_list.empty()) {
    m_list.emplace_back(p_data);
    m_tail = m_head = m_list.begin();
    ++m_amount;
    return;
  }

  if (full()) {
    m_head = m_list.emplace(std::next(m_head), p_data);
  } else {
    ++m_head;
    if (m_head == m_list.end()) {
      m_head = m_list.begin();
    }
  }

  *m_head = p_data;
  ++m_amount;
}

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
    std::optional<t_data> circular_queue<t_data>::get() {
  std::lock_guard<std::mutex> _lock(m_mutex);

  if (m_list.empty() || (occupied() == 0)) {
    return {};
  }

  t_data _data = *m_tail;
  ++m_tail;
  --m_amount;
  if (m_tail == m_list.end()) {
    m_tail = m_list.begin();
  }
  return _data;
}

} // namespace tenacitas::lib::dat

namespace tld = tenacitas::lib::dat;

#endif
