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
#include <concurrent/queue.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief Implements a circular queue which size is increased if it becomes
/// full
///
/// \tparam t_log
///
/// \tparam t_data defines the types of the data contained in the buffer
template <typename t_log, typename t_data>
struct circular_unlimited_size_queue_t : public queue_t<t_log, t_data> {

  /// \brief Alias for the data
  typedef t_data data;

  /// \brief Alias for the log
  typedef t_log log;

  //  /// \brief Type of the super class
  //  typedef queue_t<log, data> queue;

  //  /// \brief Type of the pointer
  //  typedef typename queue::ptr ptr;

  /// \brief Type of pointer to the queue
  typedef std::shared_ptr<circular_unlimited_size_queue_t> ptr;

  /// \brief Creates a queue with a fixed size
  ///
  /// \param p_size the number of slots in the queue
  ///
  /// \return a pointer to the created queue
  static ptr create(size_t p_size) {
    ptr _ptr(new circular_unlimited_size_queue_t(p_size));
    //    ptr _ptr();
    return _ptr;
  }

  /// \brief Traverses the queue
  ///
  /// \param p_function will be called for every data in the queue
  void traverse(std::function<void(const t_data &)> p_visitor) const override {
    node_ptr _p = m_root;
    while (_p && (_p->m_next != m_root)) {
      p_visitor(_p->m_data);
      _p = _p->m_next;
    }
    p_visitor(_p->m_data);
  }

  /// \brief Adds a t_data object to the queue
  void add(data &&p_data) override {
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

  /// \brief Adds a t_data object to the queue
  void add(const data &p_data) override {
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

  /// \brief Gets the first t_data obect added to the queue, if there is one
  std::optional<data> get() override {
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

  /// \brief Informs if the queue is full
  inline bool full() const override { return m_amount == m_size; }

  /// \brief Informs if the queue is empty
  inline bool empty() const override { return m_amount == 0; }

  /// \brief Informs the total capacity of the queue
  inline size_t capacity() const override { return m_size; }

  /// \brief Informs the current number of slots occupied in the queue
  inline size_t occupied() const override { return m_amount; }

private:
  /// \brief Node of the linked list used to implement the queue
  struct node {
    /// \brief Type of pointer
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
  /// \brief Inserts a node in the list after a node
  ///
  /// \param p_node which the new node will be inserted in front of
  ///
  /// \param p_data data inserted in the new node
  ///
  /// \return the new node
  node_ptr insert(node_ptr p_node, t_data &&p_data) {
    node_ptr _new_node = create_node(std::move(p_data));

    _new_node->m_next = p_node->m_next;
    _new_node->m_prev = p_node;

    p_node->m_next->m_prev = _new_node;
    p_node->m_next = _new_node;
    ++m_size;
    return _new_node;
  }

  /// \brief Inserts a node in the list after a node
  ///
  /// \param p_node which the new node will be inserted in front of
  ///
  /// \param p_data data inserted in the new node
  ///
  /// \return the new node
  node_ptr insert(node_ptr p_node, const t_data &p_data) {
    node_ptr _new_node = create_node(p_data);

    _new_node->m_next = p_node->m_next;
    _new_node->m_prev = p_node;

    p_node->m_next->m_prev = _new_node;
    p_node->m_next = _new_node;
    ++m_size;
    return _new_node;
  }

  /// \brief Creates a new node
  node_ptr create_node() {
    node_ptr _p(std::make_shared<node>());
    _p->m_next = _p;
    _p->m_prev = _p;
    return _p;
  }

  /// \brief Creates a new node, defining its data
  ///
  /// \param p_data is the data inside the new node
  ///
  /// \return the new node
  node_ptr create_node(t_data &&p_data) {
    node_ptr _p(std::make_shared<node>(std::move(p_data)));
    _p->m_next = _p;
    _p->m_prev = _p;
    return _p;
  }

  /// \brief Creates a new node, defining its data
  ///
  /// \param p_data is the data inside the new node
  ///
  /// \return the new node
  node_ptr create_node(const t_data &p_data) {
    node_ptr _p(std::make_shared<node>(p_data));
    _p->m_next = _p;
    _p->m_prev = _p;
    return _p;
  }

  /// \brief Constructor
  ///
  /// \param p_size the number of initial slots in the queue
  circular_unlimited_size_queue_t(size_t p_size) {
    m_root = create_node();
    node_ptr _p = m_root;
    for (size_t _i = 1; _i < p_size; ++_i) {
      _p = insert(_p, data());
    }
    m_size = p_size;
    m_write = m_root;
    m_read = m_root;
  }

private:
  /// \brief The first node of the queue
  node_ptr m_root;

  /// \brief The node where the next write, i.e., new data insertion, should be
  /// done
  node_ptr m_write;

  /// \brief The node where the next read, i.e., new data extraction, should be
  /// done
  node_ptr m_read;

  /// \brief Size of the queue
  size_t m_size;

  /// \brief Amount of nodes actually used
  size_t m_amount{0};

  /// \brief Controls insertion
  std::mutex m_mutex;

  /// \brief Log of the class
  t_log m_log{"concurrent::circular_unlimited_size_queue"};
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_CIRCULAR_UNLIMITED_SIZE_QUEUE_H
