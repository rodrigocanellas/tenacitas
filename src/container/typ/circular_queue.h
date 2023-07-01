#ifndef TENACITAS_LIB_CONTAINER_TYP_CIRCULAR_QUEUE_H
#define TENACITAS_LIB_CONTAINER_TYP_CIRCULAR_QUEUE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <condition_variable>
#include <functional>
#include <future>
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
  // \brief Constructor
  //
  // \param p_size the number of initial slots in the queue
  circular_queue_t(size_t p_size = 1) {
    m_root = create_node(t_data{});
    if (m_root == nullptr) {
      throw std::runtime_error("error creating circular_queue root node");
    }
    node_ptr _p = m_root;
    for (size_t _i = 0; _i < p_size; ++_i) {
      _p = insert(_p);
      if (_p == nullptr) {
        throw std::runtime_error("error inserting node #" + std::to_string(_i));
      }
    }
    //    this->m_size = p_size;
    m_write = m_root;
    m_read = m_root;
  }

  //  circular_unlimited_size_queue_t() = default;

  ~circular_queue_t() = default;

  // \brief Copy constructor not allowed
  circular_queue_t(const circular_queue_t &) = delete;

  // \brief Move constructor
  circular_queue_t(circular_queue_t &&p_queue) {
    m_root = p_queue.m_root;
    m_write = p_queue.m_write;
    m_read = p_queue.m_read;
    m_amount = p_queue.m_amount;
  }

  // \brief Copy assignment not allowed
  circular_queue_t &operator=(const circular_queue_t &) = delete;

  // \brief Move assignment
  circular_queue_t &operator=(circular_queue_t &&p_queue) {
    if (this != p_queue) {
      m_root = p_queue.m_root;
      m_write = p_queue.m_write;
      m_read = p_queue.m_read;
      m_amount = p_queue.m_amount;
    }
    return *this;
  }

  // \brief Traverses the queue
  //
  // \param p_function will be called for every data in the queue
  void traverse(std::function<void(const t_data &)> p_visitor) const {
    node_ptr _p = m_root;
    while (_p && (_p->m_next != m_root)) {
      if (_p->m_data) {
        p_visitor(_p->m_data);
      }
      _p = _p->m_next;
    }
    if (_p->m_data) {
      p_visitor(_p->m_data);
    }
  }

  // \brief Adds a t_data object to the queue
  //
  // \param p_data is a t_data to be added
  void add(t_data &&p_data) {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("queue ", m_id, " - adding data");
#endif
    {
      std::lock_guard<std::mutex> _lock(m_mutex);

      if (!full()) {
        m_write->m_data = std::forward<t_data>(p_data);
        m_write = m_write->m_next;
      } else {
        if (!insert(m_write->m_prev, std::forward<t_data>(p_data))) {
          throw("error inserting node");
        }
      }
      ++m_amount;
    }
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("queue ", m_id, " - capacity = ", capacity(),
                 ", occupied = ", occupied());
#endif
  }

  // \brief Adds a t_data object to the queue
  //
  // \param p_data is a t_data to be added
  void add(const t_data &p_data) {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("queue ", m_id, " - adding data");
#endif
    {
      std::lock_guard<std::mutex> _lock(m_mutex);

      if (!full()) {
        m_write->m_data = p_data;
        m_write = m_write->m_next;
      } else {
        if (!insert(m_write->m_prev, p_data)) {
          throw("error inserting node");
        }
      }
      ++m_amount;
    }
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("queue ", m_id, " - capacity = ", capacity(),
                 ", occupied = ", occupied());
#endif
  }

  // \brief Tries to get a t_data object from the queue
  //
  // \return valid t_data pointer, if it was possible to get; a nullptr if it
  // was not possible
  std::optional<t_data> get() {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (empty()) {
      return {};
    }

    t_data _data{m_read->m_data};

    m_read = m_read->m_next;
    --m_amount;
    return {_data};
  }

  // \brief Informs if the queue is full
  inline constexpr bool full() const {
    if (m_size == 0) {
      return false;
    }

    return (m_amount == m_size);
  }

  // \brief Informs if the queue is empty
  inline constexpr bool empty() const { return m_amount == 0; }

  // \brief Informs the total capacity of the queue
  inline constexpr size_t capacity() const { return m_size; }

  // \brief Informs the current number of slots occupied in the
  // queue
  inline constexpr size_t occupied() const { return m_amount; }

  inline number::typ::id get_id() const { return m_id; }

private:
  // \brief Node of the linked list used to implement the queue
  struct node {
    // \brief Type of pointer
    typedef std::shared_ptr<node> ptr;

    node() = default;

    // \brief Copy constructor not allowed
    node(const node &) = delete;

    node(node &&) = default;

    // \brief Copy assignment not allowed
    node &operator=(const node &) = delete;

    node &operator=(node &&) = default;

    // \brief Constructor
    //
    // \param p_data is a t_data to be stored in the node
    node(t_data &&p_data) : m_data(std::forward<t_data>(p_data)) {}

    // \brief Constructor
    //
    // \param p_data is a t_data to be stored in the node
    node(const t_data &p_data) : m_data(p_data) {}

    // \brief data in the node
    t_data m_data;

    // \brief next node
    ptr m_next;

    // \brief previous node
    ptr m_prev;
  };

  // \brief Alias for the pointer to a node
  typedef typename node::ptr node_ptr;

private:
  // \brief Creates a new node, defining its data
  //
  // \param p_data is the data inside the new node
  //
  // \return the new node
  node_ptr create_node(t_data &&p_data) {
    node_ptr _p = nullptr;
    try {
      _p = std::make_shared<node>(std::forward<t_data>(p_data));
      _p->m_next = _p;
      _p->m_prev = _p;
    } catch (std::exception &_ex) {
      _p = nullptr;
#ifdef TENACITAS_LOG
      TNCT_LOG_ERR("error alocating new node: '", _ex.what(), '\'');
#endif
    }
    return _p;
  }

  // \brief Creates a new node, defining its data
  //
  // \param p_data is the data inside the new node
  //
  // \return the new node
  node_ptr create_node(const t_data &p_data) {
    node_ptr _p = nullptr;
    try {
      _p = std::make_shared<node>(p_data);
      _p->m_next = _p;
      _p->m_prev = _p;
    } catch (std::exception &_ex) {
      _p = nullptr;
#ifdef TENACITAS_LOG
      TNCT_LOG_ERR("error alocating new node: '", _ex.what(), '\'');
#endif
    }
    return _p;
  }

  node_ptr create_node() {
    node_ptr _p = nullptr;
    try {
      _p = std::make_shared<node>();
      _p->m_next = _p;
      _p->m_prev = _p;
    } catch (std::exception &_ex) {
      _p = nullptr;
#ifdef TENACITAS_LOG
      TNCT_LOG_ERR("error alocating new node: '", _ex.what(), '\'');
#endif
    }
    return _p;
  }

  // \brief Inserts a node in the list after a node
  //
  // \param p_node which the new node will be inserted in front of
  //
  // \param p_data data inserted in the new node
  //
  // \return the new node
  node_ptr insert(node_ptr p_node, t_data &&p_data) {
    node_ptr _new_node = create_node(std::forward<t_data>(p_data));

    if (_new_node == nullptr) {
      return nullptr;
    }

    _new_node->m_next = p_node->m_next;
    _new_node->m_prev = p_node;

    p_node->m_next->m_prev = _new_node;
    p_node->m_next = _new_node;
    ++this->m_size;
    return _new_node;
  }

  // \brief Inserts a node in the list after a node
  //
  // \param p_node which the new node will be inserted in front of
  //
  // \param p_data data inserted in the new node
  //
  // \return the new node
  node_ptr insert(node_ptr p_node, const t_data &p_data) {
    node_ptr _new_node = create_node(p_data);

    if (_new_node == nullptr) {
      return nullptr;
    }

    _new_node->m_next = p_node->m_next;
    _new_node->m_prev = p_node;

    p_node->m_next->m_prev = _new_node;
    p_node->m_next = _new_node;
    ++this->m_size;
    return _new_node;
  }

  node_ptr insert(node_ptr p_node) {
    node_ptr _new_node = create_node();

    if (_new_node == nullptr) {
      return nullptr;
    }

    _new_node->m_next = p_node->m_next;
    _new_node->m_prev = p_node;

    p_node->m_next->m_prev = _new_node;
    p_node->m_next = _new_node;
    ++this->m_size;
    return _new_node;
  }

private:
  // \brief The first node of the queue
  node_ptr m_root;

  // \brief The node where the next write, i.e., new data
  // insertion, should be done
  node_ptr m_write;

  // \brief The node where the next read, i.e., new data
  // extraction, should be done
  node_ptr m_read;

  // \brief Amount of nodes actually used
  size_t m_amount{0};

  // \brief Amount of nodes in the queue
  size_t m_size{0};

  // \brief Controls insertion
  std::mutex m_mutex;

  // \brief for debug purposes
  number::typ::id m_id;
};

} // namespace tenacitas::lib::container::typ

#endif
