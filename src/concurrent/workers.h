#ifndef TENACITAS_CONCURRENT_DISPATCHER_H
#define TENACITAS_CONCURRENT_DISPATCHER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>
#include <mutex>
#include <set>
#include <thread>
#include <tuple>

#include <calendar/epoch.h>
#include <concurrent/circular_unlimited_size_queue.h>
#include <concurrent/internal/log.h>
#include <concurrent/queue.h>
#include <concurrent/time_unit.h>
#include <concurrent/timeout_callback.h>
#include <concurrent/worker.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief dispatcher implements a publisher/subscriber pattern
///
/// \param t_data is the type of the data to be handled. If it is not \p void,
/// it must be:
///    - default constructible
///    - move constructible
///
/// \tparam t_log provides log funcionality:
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
///
template <typename t_log, typename t_data> class workers_t {
public:
  typedef t_log log;
  typedef t_data data;

  typedef queue_t<t_log, t_data> queue;
  typedef typename queue::ptr queue_ptr;

  typedef std::function<queue_ptr(size_t)> queue_factory;

  typedef worker_t<t_log, t_data> worker;

  typedef typename worker::id worker_id;

  /// \brief type of operation
  typedef std::function<void(data &&)> operation;

  /// \brief dispatcher default constructor not allowed
  workers_t() = delete;

  /// \brief dispatcher copy constructor not allowed
  workers_t(const workers_t &) = delete;

  /// \brief dispatcher move constructor not allowed
  workers_t(workers_t &&) = delete;

  /// \brief dispatcher copy assigment not allowed
  workers_t &operator=(const workers_t &) = delete;

  /// \brief dispatcher move assigment not allowed
  workers_t &operator=(workers_t &&) = delete;

  /// \brief dispatcher destructor
  ///
  /// Stops all the \p worker
  ~workers_t() {
    concurrent_debug(m_log, "leaving");

    for (worker_ptr _worker : m_worker_list) {
      _worker->stop();
    }
  }

  inline static void set_queue_factory(queue_factory p_queue_factory) {
    m_queue_factory = p_queue_factory;
  }

  /// \brief named group, one worker, timeout
  template <typename t_time>
  static void subscribe(
      const worker_id &p_operation_id, operation p_operation, t_time p_timeout,
      timeout_callback p_timeout_callback = [](std::thread::id) -> void {},
      size_t p_queue_size = 20) {

    typename worker_list::iterator _ite = m_worker_list.find(p_operation_id);

    worker_ptr _producer;
    if (_ite != m_worker_list.end()) {
      _producer = _ite->second;
    } else {
      _producer = std::make_shared<worker>(m_queue_factory(p_queue_size));
    }

    _producer->add(p_operation, p_timeout, p_timeout_callback);

    _producer->start();

    std::lock_guard<std::mutex> _lock(m_list_mutex);
    m_worker_list.insert({p_operation_id, _producer});
  }

  /// \brief named group, one worker, no timeout
  static void subscribe(const group &p_operation_id, worker p_operation,
                        size_t p_queue_size = 20) {

    typename worker_list::iterator _ite = m_worker_list.find(p_operation_id);

    worker_ptr _producer;
    if (_ite != m_worker_list.end()) {
      _producer = _ite->second;
    } else {
      _producer = std::make_shared<worker>(m_queue_factory(p_queue_size));
    }

    _producer->add(p_operation);

    _producer->start();

    std::lock_guard<std::mutex> _lock(m_list_mutex);
    m_worker_list.insert({p_operation_id, _producer});
  }

  /// \brief named group, many workers, timeout
  template <typename t_time>
  static void subscribe(
      const group &p_operation_id, uint16_t p_num_workers,
      std::function<worker()> p_factory, t_time p_timeout,
      timeout_callback p_timeout_callback = [](std::thread::id) -> void {},
      size_t p_queue_size = 20) {

    typename worker_list::iterator _ite = m_worker_list.find(p_operation_id);

    worker_ptr _producer;
    if (_ite != m_worker_list.end()) {
      _producer = _ite->second;
    } else {
      _producer = std::make_shared<worker>(m_queue_factory(p_queue_size));
    }

    _producer->add(p_num_workers, p_factory, p_timeout, p_timeout_callback);

    _producer->start();

    std::lock_guard<std::mutex> _lock(m_list_mutex);
    m_worker_list.insert({p_operation_id, _producer});
  }

  /// \brief named group, many workers, no timeout
  static void subscribe(const group &p_operation_id, uint16_t p_num_workers,
                        std::function<worker()> p_factory,
                        size_t p_queue_size = 20) {

    typename worker_list::iterator _ite = m_worker_list.find(p_operation_id);

    worker_ptr _producer;
    if (_ite != m_worker_list.end()) {
      _producer = _ite->second;
    } else {
      _producer = std::make_shared<worker>(m_queue_factory(p_queue_size));
    }

    _producer->add(p_num_workers, p_factory);

    _producer->start();

    std::lock_guard<std::mutex> _lock(m_list_mutex);
    m_worker_list.insert({p_operation_id, _producer});
  }

  // ################

  /// \brief unamed group, one worker, timeout
  template <typename t_time>
  static void subscribe(worker p_operation, t_time p_timeout,
                        timeout_callback p_timeout_callback,
                        size_t p_queue_size = 20) {

    worker_ptr _producer{
        std::make_shared<worker>(m_queue_factory(p_queue_size))};

    _producer->add(p_operation, p_timeout, p_timeout_callback);

    _producer->start();

    const group _group{std::to_string(calendar::epoch::microsecs())};

    std::lock_guard<std::mutex> _lock(m_list_mutex);
    m_worker_list.insert({_group, _producer});
  }

  /// \brief unamed group, one worker, no timeout
  static void subscribe(worker p_operation, size_t p_queue_size = 20) {

    worker_ptr _producer{
        std::make_shared<worker>(m_queue_factory(p_queue_size))};

    _producer->add(p_operation);

    _producer->start();

    const group _group{std::to_string(calendar::epoch::microsecs())};

    std::lock_guard<std::mutex> _lock(m_list_mutex);
    m_worker_list.insert({_group, _producer});
  }

  /// \brief unamed group, many workers, timeout
  template <typename t_time>
  static void subscribe(uint16_t p_num_workers,
                        std::function<worker()> p_factory, t_time p_timeout,
                        timeout_callback p_timeout_callback,
                        size_t p_queue_size = 20) {

    worker_ptr _producer{
        std::make_shared<worker>(m_queue_factory(p_queue_size))};

    _producer->add(p_num_workers, p_factory, p_timeout, p_timeout_callback);

    _producer->start();

    const group _group{std::to_string(calendar::epoch::microsecs())};

    std::lock_guard<std::mutex> _lock(m_list_mutex);
    m_worker_list.insert({_group, _producer});
  }

  /// \brief unamed group, many workers, no timeout
  static void subscribe(uint16_t p_num_workers,
                        std::function<worker()> p_factory,
                        size_t p_queue_size = 20) {

    worker_ptr _producer{
        std::make_shared<worker>(m_queue_factory(p_queue_size))};

    _producer->add(p_num_workers, p_factory);

    _producer->start();

    const group _group{std::to_string(calendar::epoch::microsecs())};

    std::lock_guard<std::mutex> _lock(m_list_mutex);
    m_worker_list.insert({_group, _producer});
  }

  /// \brief handle sends a message to the \p work_t objects to be handled
  ///
  /// \param p_msg an instance of \p t_data
  static void publish(const data &p_msg) {
    typename worker_list::iterator _end = m_worker_list.end();
    for (typename worker_list::iterator _ite = m_worker_list.begin();
         _ite != _end; ++_ite) {
      _ite->second->add(p_msg);
    }
  }

  /// \brief handle sends a message to the \p work_t objects to be handled
  ///
  /// \tparam t_params are the types of parameters to create a \p t_data object
  ///
  /// \param p_params are theparameters values to create a \p t_data object
  template <typename... t_params>
  inline static void publish(t_params... p_params) {
    const data _msg(p_params...);
    publish(_msg);
  }

  /// \brief stops the \p subscriber, which stops \p worker objects to handle
  /// new messages
  static void stop() {
    typename worker_list::iterator _end = m_worker_list.end();
    for (typename worker_list::iterator _ite = m_worker_list.begin();
         _ite != _end; ++_ite) {
      _ite->second->stop();
    }
  }

  static void set_timeout_callback(timeout_callback p_timeout_callback) {
    m_timeout_callback = p_timeout_callback;
  }

private:
  typedef typename std::shared_ptr<worker> worker_ptr;

  struct less {
    constexpr bool()(worker_ptr l, worker_ptr r) { return (*l) < (*r); }
  };

  /// \brief worker_list_t alias for list of \p worker_t
  typedef std::set<worker_ptr, less> worker_list;

private:
  /// \brief m_worker_list the single list of pools object
  static worker_list m_worker_list;

  static timeout_callback m_timeout_callback;
  static t_log m_log;

  static std::mutex m_list_mutex;

  static queue_factory m_queue_factory;
};

/// \brief definition of the single list of pools object
template <typename t_log, typename t_data>
typename workers_t<t_log, t_data>::worker_list
    workers_t<t_log, t_data>::m_worker_list;

template <typename t_log, typename t_data>
timeout_callback workers_t<t_log, t_data>::m_timeout_callback =
    [](std::thread::id) -> void {};

template <typename t_log, typename t_data>
t_log workers_t<t_log, t_data>::m_log{"consumer::dispatcher"};

template <typename t_log, typename t_data>
std::mutex workers_t<t_log, t_data>::m_list_mutex;

template <typename t_log, typename t_data>
typename workers_t<t_log, t_data>::queue_factory
    workers_t<t_log, t_data>::m_queue_factory = [](size_t p_size) ->
    typename workers_t<t_log, t_data>::queue_ptr {
      return typename workers_t<t_log, t_data>::queue_ptr(
          new circular_unlimited_size_queue_t<t_log, t_data>(p_size));
    };

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_DISPATCHER_H
