#ifndef TENACITAS_LIB_CONTAINER_TYP_CIRCULAR_QUEUE_H
#define TENACITAS_LIB_CONTAINER_TYP_CIRCULAR_QUEUE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <concepts>
#include <condition_variable>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <thread>
#include <utility>

#include <tenacitas.lib/src/number/typ/id.h>

/// \brief master namespace
namespace tenacitas::lib::container::typ {

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

  void traverse(std::function<void(const t_data &)> p_visitor);

  void add(t_data &&p_data);

  void add(const t_data &p_data);

  std::optional<t_data> get();

  constexpr bool full() const { return (m_amount == m_list.size()); }

  constexpr bool empty() const { return m_amount == 0; }

  constexpr size_t capacity() const { return m_list.size(); }

  constexpr size_t occupied() const { return m_amount; }

  size_t get_id() const { return m_id; }

  void clear() {
    m_list.clear();
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
  iterator m_write;
  iterator m_read;

  // \brief for debug purposes
  number::typ::id m_id;

  // \brief Controls insertion
  std::recursive_mutex m_mutex;
};

// #############################################################################
// IMPLEMENTATIION

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
circular_queue<t_data>::circular_queue(const circular_queue &p_queue)
    : m_amount(p_queue.m_amount), m_list(p_queue.m_list),
      m_write(p_queue.m_write), m_read(p_queue.m_read) {}

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
    circular_queue<t_data>
&circular_queue<t_data>::operator=(const circular_queue &p_queue) {
  if (this != &p_queue) {
    m_amount = p_queue.m_amount;
    m_list = p_queue.m_list;
    m_write = p_queue.m_write;
    m_read = p_queue.m_read;
  }
  return *this;
}

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
void circular_queue<t_data>::traverse(
    std::function<void(const t_data &)> p_visitor) {

  if (m_amount == 0) {
    return;
  }
  auto _end{m_list.end()};
  auto _begin{m_list.begin()};

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
}

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
void circular_queue<t_data>::add(t_data &&p_data) {

  std::lock_guard<std::recursive_mutex> _lock(m_mutex);

  if (m_list.empty()) {
    m_list.push_back(std::forward<t_data>(p_data));
    m_read = m_list.begin();
    m_write = m_list.end();
    ++m_amount;
    return;
  }

  if (m_write == m_list.end()) {
    m_write = m_list.begin();
    add(std::forward<t_data>(p_data));
    return;
  }

  if (m_write == m_read) {
    m_list.insert(m_read, std::forward<t_data>(p_data));
    ++m_amount;
    return;
  }

  *m_write = std::forward<t_data>(p_data);
  ++m_write;
  ++m_amount;
}

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
void circular_queue<t_data>::add(const t_data &p_data) {

  std::lock_guard<std::recursive_mutex> _lock(m_mutex);

  if (m_list.empty()) {
    m_list.push_back(p_data);
    m_read = m_list.begin();
    m_write = m_list.end();
    ++m_amount;
    return;
  }

  if (m_write == m_list.end()) {
    m_write = m_list.begin();
    add(p_data);
    return;
  }

  if (m_write == m_read) {
    m_list.insert(m_read, p_data);
    ++m_amount;
    return;
  }

  *m_write = p_data;
  ++m_write;
  ++m_amount;
}

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
    std::optional<t_data> circular_queue<t_data>::get() {
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

} // namespace tenacitas::lib::container::typ

#endif
