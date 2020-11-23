#ifndef TENACITAS_CONCURRENT_CIRCULAR_UNLIMITED_SIZE_QUEUE_H
#define TENACITAS_CONCURRENT_CIRCULAR_UNLIMITED_SIZE_QUEUE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <functional>
#include <iostream>
#include <memory>
#include <mutex>

#include <concurrent/internal/log.h>
//#include <concurrent/queue.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

template <typename t_log, typename t_data>
struct circular_unlimited_size_queue_t /*: public queue_t<t_log, t_data>*/ {

  typedef t_data data;

  circular_unlimited_size_queue_t(size_t p_size) {
    m_root = create();
    node_ptr _p = m_root;
    for (size_t _i = 1; _i < p_size; ++_i) {
      _p = insert(_p, data());
    }
    m_size = p_size;
    m_write = m_root;
    m_read = m_root;
  }

  void traverse(std::function<void(const t_data &)> p_visitor) {
    node_ptr _p = m_root;
    while (_p && (_p->m_next != m_root)) {
      p_visitor(_p->m_data);
      _p = _p->m_next;
    }
    p_visitor(_p->m_data);
  }

  void add(data &&p_data) /*override*/ {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (!full()) {
      concurrent_warn(m_log, "not adding more slots");
      m_write->m_data = std::move(p_data);
      m_write = m_write->m_next;
    } else {
      concurrent_warn(m_log, "adding more slots");
      insert(m_write->m_prev, std::move(p_data));
    }
    ++m_amount;
  }

  void add(const data &p_data) /*override*/ {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (!full()) {
      concurrent_warn(m_log, "not adding more slots");
      m_write->m_data = p_data;
      m_write = m_write->m_next;
    } else {
      concurrent_warn(m_log, "adding more slots");
      insert(m_write->m_prev, p_data);
    }
    ++m_amount;
  }

  std::optional<data> get() /*override*/ {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (empty()) {
      concurrent_debug(m_log, "empty");
      return {};
    }
    concurrent_debug(m_log, "not empty");
    data _data(m_read->m_data);
    m_read = m_read->m_next;
    --m_amount;
    return {_data};
  }

  inline bool full() const /*override*/ { return m_amount == m_size; }

  inline bool empty() const /*override*/ { return m_amount == 0; }

  inline size_t capacity() const /*override*/ { return m_size; }

  inline size_t occupied() const /*override*/ { return m_amount; }

private:
  struct node {
    typedef std::shared_ptr<node> ptr;

    node() = default;
    node(const node &) = delete;
    node(node &&) = delete;
    node &operator=(const node &) = delete;
    node &operator=(node &&) = delete;

    node(t_data &&p_data) : m_data(std::move(p_data)) {}

    node(const t_data &p_data) : m_data(p_data) {}

    t_data m_data;
    ptr m_next;
    ptr m_prev;
  };

  typedef typename node::ptr node_ptr;

private:
  node_ptr insert(node_ptr p_node, t_data &&p_data) {
    node_ptr _new_node = create(std::move(p_data));

    _new_node->m_next = p_node->m_next;
    _new_node->m_prev = p_node;

    p_node->m_next->m_prev = _new_node;
    p_node->m_next = _new_node;
    ++m_size;
    return _new_node;
  }

  node_ptr insert(node_ptr p_node, const t_data &p_data) {
    node_ptr _new_node = create(p_data);

    _new_node->m_next = p_node->m_next;
    _new_node->m_prev = p_node;

    p_node->m_next->m_prev = _new_node;
    p_node->m_next = _new_node;
    ++m_size;
    return _new_node;
  }

  node_ptr create() {
    node_ptr _p(std::make_shared<node>());
    _p->m_next = _p;
    _p->m_prev = _p;
    return _p;
  }

  node_ptr create(t_data &&p_data) {
    node_ptr _p(std::make_shared<node>(std::move(p_data)));
    _p->m_next = _p;
    _p->m_prev = _p;
    return _p;
  }

  node_ptr create(const t_data &p_data) {
    node_ptr _p(std::make_shared<node>(p_data));
    _p->m_next = _p;
    _p->m_prev = _p;
    return _p;
  }

private:
  node_ptr m_root;
  node_ptr m_write;
  node_ptr m_read;
  size_t m_size;
  size_t m_amount{0};
  std::mutex m_mutex;
  t_log m_log{"concurrent::circular_unlimited_size_queue"};
};

} // namespace concurrent
} // namespace tenacitas

#endif

// struct data {
//  inline data() : m_value(m_counter++) {}

//  inline operator int16_t() const { return m_value; }

//  inline void set(int16_t p_i) { m_value = p_i; }

//  friend std::ostream &operator<<(std::ostream &p_out, const data &p_data) {
//    p_out << p_data.m_value;
//    return p_out;
//  }

// private:
//  static int16_t m_counter;
//  int16_t m_value;
//};

// int16_t data::m_counter{0};

// void test000() {
//  typedef circular_list_t<data> circular_list;

//  circular_list _list(5);

//  _list.traverse([](const data &p_data) -> void {
//    std::cout << "(" << p_data << ")";
//    std::cout.flush();
//  });
//}

// int main() {
//  test000();

//  return 0;
//}
