#ifndef TENACITAS_CONCURRENT_WORKER_H
#define TENACITAS_CONCURRENT_WORKER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <array>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include <calendar/epoch.h>
#include <concurrent/async_loop.h>
#include <concurrent/breaker.h>
#include <concurrent/internal/log.h>
#include <concurrent/queue.h>
#include <concurrent/result.h>
#include <concurrent/timeout_callback.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief worker implements the producer/consumer pattern
///
/// A worker uses some type of queue to cache data in order a bunch of
/// operation compete for the first instance of data inserted into the queue
///
/// \tparam t_log
///
/// \param t_data type of data to be processed
template <typename t_log, typename t_data> class worker_t {
public:
  /// \brief Alias for the data
  typedef t_data data;

  /// \brief Alias for a generic queue type, used to order data to be processed
  typedef queue_t<t_log, t_data> queue;

  /// \brief Type of pointer to the generic queur type
  typedef typename queue::ptr queue_ptr;

  /// \brief Type of operation that a worker will execute
  typedef std::function<void(data &&)> operation;

  /// \brief Type for the identifier of the worker
  typedef std::string id;

  /// \brief Type fot the priority of a worker
  typedef int16_t priority;

public:
  /// \brief Constructor
  ///
  /// \param p_id identifier of the worker
  ///
  /// \param p_queue_ptr pointer to the queue used to cache data to be processed
  worker_t(const id &p_id, queue_ptr &&p_queue_ptr)
      : m_id(p_id), m_queue(p_queue_ptr) {}

  /// \brief Constructor
  ///
  /// In this constructor, which is epoch in microseconds is set as the
  /// identifier
  ///
  /// \param p_queue_ptr pointer to the queue used to cache data to be processed
  worker_t(queue_ptr &&p_queue_ptr)
      : m_id(std::to_string(calendar::epoch::microsecs())),
        m_queue(p_queue_ptr) {}

  /// \brief copy constructor not allowed
  worker_t(const worker_t &) = delete;

  /// \brief move constructor not allowed
  worker_t(worker_t &&) = delete;

  /// \brief copy assignemnt not allowed
  worker_t &operator=(const worker_t &) = delete;

  /// \brief move assignemnt not allowed
  worker_t &operator=(worker_t &&p_pc) = delete;

  /// \brief destructor
  ///
  /// If 'stop' was not called, empties the queue, waiting for all the data to
  /// be processed
  ~worker_t() {
    concurrent_debug(m_log, "destructor");
    if (!all_loops_stopped()) {
      if (!m_stopped) {
        while (!m_queue->empty()) {
          m_data_produced.notify_all();
          std::unique_lock<std::mutex> _lock(m_mutex_data);
          m_data_consumed.wait(_lock, [this] { return true; });
        }
        stop();
      }
    }
    concurrent_info(m_log, "leaving destructor");
  }

  /// \brief Equal-to operator
  ///
  /// a worker is equal to another if theirs ids are equals
  bool operator==(const worker_t &p_operation) const {
    return m_id == p_operation.m_id;
  }

  /// \brief Not-equal-to operator
  ///
  /// a worker is equal to another if theirs ids are not equals
  bool operator!=(const worker_t &p_operation) const {
    return m_id != p_operation.m_id;
  }

  /// \brief Less-than operator
  ///
  /// a worker is less than another if its priority is *higher* than the other;
  /// this makes worker with higher priority numbers to become before workers
  /// with lower priority, i.e., a worker with priority 5 comes before a worker
  /// with priority 2
  bool operator<(const worker_t &p_operation) const {
    return m_priority > p_operation.m_priority;
  }

  /// \brief Greater-than operator
  ///
  /// a worker is less than another if its priority is *lower* than the other;
  /// this makes worker with higher priority numbers to become before workers
  /// with lower priority, i.e., a worker with priority 5 comes before a worker
  /// with priority 2
  bool operator>(const worker_t &p_operation) const {
    return m_priority < p_operation.m_priority;
  }

  /// \brief Defines the priority for this worker
  void set_priority(priority p_priority) { m_priority = p_priority; }

  /// \brief Defines the priority for this worker
  priority get_priority() { return m_priority; }

  /// \brief adds data to be passed to an operation
  ///
  /// \p data will only be added if \p start() was called
  ///
  /// \param p_data is the data to be passed to an operation
  ///
  /// \return \p true if it was added, \p false otherwise
  bool add(const data &p_data) {
    if (m_stopped) {
      concurrent_error(m_log, "could not add data because 'worker' "
                              "is not running; call 'start()' first");
      return false;
    }

    std::unique_lock<std::mutex> _lock_stop(m_mutex_stop);

    concurrent_debug(m_log, "waiting for room ...");
    std::unique_lock<std::mutex> _lock(m_mutex_data);
    m_data_consumed.wait(_lock,
                         [this]() { return (!m_queue->full() || m_stopped); });

    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
    } else {
      m_queue->add(p_data);
      ++m_queued_data;
    }

    // notifyng that new data is available
    concurrent_debug(m_log, "notifying");
    m_data_produced.notify_all();
    return true;
  }

  /// \brief adds data to be passed to an operation
  ///
  /// \p data will only be added if \p start() was called
  ///
  /// \param p_data is the data to be passed to an operation
  ///
  /// \return \p true if it was added, \p false otherwise
  bool add(data &&p_data) {
    if (m_stopped) {
      concurrent_error(m_log, "could not add data because 'worker' "
                              "is not running; call 'start()' first");
      return false;
    }

    std::unique_lock<std::mutex> _lock_stop(m_mutex_stop);

    concurrent_debug(m_log, "waiting for room ...");
    std::unique_lock<std::mutex> _lock(m_mutex_data);
    m_data_consumed.wait(_lock,
                         [this]() { return (!m_queue->full() || m_stopped); });

    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
    } else {
      m_queue->add(std::move(p_data));
      ++m_queued_data;
    }

    // notifyng that new data is available
    concurrent_debug(m_log, "notifying");
    m_data_produced.notify_all();
    return true;
  }

  /// \brief informs the amount of data added
  inline size_t amount_added() const { return m_queued_data; }

  /// \brief add adds one \p operation
  ///
  /// \tparam t_time type of time used to define timeout for the operation
  ///
  /// \param p_operation the \p operation to be added
  ///
  /// \param p_timeout timeout of \p operation
  template <typename t_time>
  void add(operation p_operation, t_time p_timeout,
           timeout_callback p_timeout_callback) {

    std::unique_lock<std::mutex> _lock_stop(m_mutex_stop);

    auto _breaker = [this]() -> bool { return this->breaker(); };

    auto _provider = [this]() -> std::optional<data> {
      return this->provider();
    };

    async_loop_ptr _loop(std::make_shared<async_loop>(
        p_timeout, p_operation, _breaker, _provider, p_timeout_callback));

    add(_loop);
  }

  /// \brief add adds one \p operation
  ///
  /// \param p_operation the \p operation to be added
  void add(operation p_operation) {

    std::unique_lock<std::mutex> _lock_stop(m_mutex_stop);

    auto _breaker = [this]() -> bool { return this->breaker(); };

    auto _provider = [this]() -> std::optional<data> {
      return this->provider();
    };

    async_loop_ptr _loop(
        std::make_shared<async_loop>(p_operation, _breaker, _provider));

    add(_loop);
  }

  /// \brief adds a bunch of \p operation
  ///
  /// \tparam t_time type of time used to define timeout for the operation
  ///
  /// \param p_num_works the number of \p operations to be added
  ///
  /// \param p_operation_factory a function that creates \p operations
  ///
  /// \param p_timeout timeout for the operations
  ///
  /// \param p_timeout_callback function to be called when the operation times
  /// out
  template <typename t_time>
  void add(uint16_t p_num_works, std::function<operation()> p_operation_factory,
           t_time p_timeout, timeout_callback p_timeout_callback) {

    std::unique_lock<std::mutex> _lock_stop(m_mutex_stop);

    auto _breaker = [this]() -> bool { return this->breaker(); };

    auto _provider = [this]() -> std::optional<data> {
      return this->provider();
    };

    for (auto _i = 0; _i < p_num_works; ++_i) {
      async_loop_ptr _loop(std::make_shared<async_loop>(
          p_timeout, p_operation_factory(), _breaker, _provider,
          p_timeout_callback));

      add(_loop);
    }
  }

  /// \brief adds a bunch of \p operation
  ///
  /// \param p_num_works the number of \p operations to be added
  ///
  /// \param p_operation_factory a function that creates \p operations
  void add(uint16_t p_num_works,
           std::function<operation()> p_operation_factory) {

    std::unique_lock<std::mutex> _lock_stop(m_mutex_stop);

    auto _breaker = [this]() -> bool { return this->breaker(); };

    auto _provider = [this]() -> std::optional<data> {
      return this->provider();
    };

    for (auto _i = 0; _i < p_num_works; ++_i) {
      async_loop_ptr _loop(std::make_shared<async_loop>(p_operation_factory(),
                                                        _breaker, _provider));

      add(_loop);
    }
  }

  /// \brief informs if the worker is stopped
  inline bool is_stopped() const { return m_stopped; }

  /// \brief starts the worker
  ///
  /// From this call on, the operations will compete among each other, in order
  /// to process any instance of \p data that was inserted into the queue
  ///
  /// \return \p false if it not start because no \p operation was added
  ///         \p true if it was already started, or if it started successfully
  bool start() {
    if (!m_stopped) {
      concurrent_error(m_log, "not starting because it is already running");
      return true;
    }

    std::unique_lock<std::mutex> _lock(m_mutex_stop);
    if (m_loops.empty()) {
      concurrent_warn(m_log, "can't run because there are no workers");
      return false;
    }

    concurrent_debug(m_log, "starting");
    m_stopped = false;
    for (async_loop_ptr _loop : m_loops) {
      _loop->start();
    }

    concurrent_debug(m_log, "started");
    return true;
  }

  /// \brief stops the \p worker
  ///
  /// From this call on, the \p operations will stop competing among each other,
  /// in order to process any instance of \p data that was inserted into the
  /// queue
  void stop() {

    std::unique_lock<std::mutex> _lock(m_mutex_stop);
    m_stopped = true;
    if (m_loops.empty()) {
      return;
    }

    m_data_produced.notify_all();
    for (async_loop_ptr _loop : m_loops) {
      concurrent_debug(m_log, "stopping loop");
      _loop->stop();
    }
  }

  /// \brief retrieves the capacity if the queue
  inline size_t capacity() const { return m_queue->capacity(); }

  /// \brief the amount of slots occupied in the \p queue
  inline size_t occupied() const { return m_queue->occupied(); }

private:
  /// \brief async_loop_t is a \p async_loop where a \p operation will be
  /// running
  typedef async_loop_t<t_log, true, data> async_loop;

  /// \brief pointer to the async_loop
  typedef typename std::shared_ptr<async_loop> async_loop_ptr;

  /// \brief async_loops_t is the collection of \p async_loop pointers
  typedef typename std::vector<async_loop_ptr> asyncs_loops;

private:
  /// \brief breaker
  ///
  /// \return \p true if the flag indicating that the \p operation should stop
  /// is \p true; \p false otherwise
  inline bool breaker() { return (m_stopped ? true : false); }

  /// \brief common function called to add a \p operation
  ///
  /// \param p_loop the new \p operation to be added
  void add(async_loop_ptr p_loop) {
    std::lock_guard<std::mutex> _lock(m_add_work);
    m_loops.push_back(p_loop);
  }

  /// \brief provider provides data, if available, to a \p operation
  std::optional<data> provider() {
    using namespace std;

    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return {};
    }

    concurrent_debug(m_log, "waiting for data...");
    std::unique_lock<std::mutex> _lock(m_mutex_data);
    m_data_produced.wait(_lock, [this]() -> bool {
      //      return (!m_container.empty() || m_stopped);
      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        return true;
      }
      if (!m_queue->empty()) {
        concurrent_debug(m_log, "not empty");
        return true;
      }
      concurrent_debug(m_log, "still waiting for data");

      return false;
    });

    concurrent_debug(m_log, "either there is data, or it was stopped");

    if (m_stopped) {
      concurrent_debug(m_log, "stopped and notifying");
      m_data_consumed.notify_all();
      return {};
    }

    concurrent_debug(m_log, "there is data");

    std::optional<data> _maybe = m_queue->get();
    if (_maybe) {
      data _data(_maybe.value());

      concurrent_debug(m_log, "getting ", _data, " and notifying");

      m_data_consumed.notify_all();
      return {_data};
    }
    concurrent_debug(m_log, "it was not possible to get data");
    return {};
  }

  /// \brief informs if all the \p async_loops are stopped
  bool all_loops_stopped() const {
    for (async_loop_ptr _async_loop : m_loops) {
      if (!_async_loop->is_stopped()) {
        return false;
      }
    }
    return true;
  }

private:
  /// \brief identifier of the worker
  id m_id;

  /// \brief priority of this worker
  priority m_priority{0};

  /// \brief queue where data will be inserted for the operation to compete for
  queue_ptr m_queue;

  /// \brief controls access to the \p m_loops while inserting a new \p
  /// operation
  std::mutex m_add_work;

  /// \brief controls access to inserting data
  std::mutex m_mutex_data;

  /// \brief controls access to attributes while the operation is stopping
  std::mutex m_mutex_stop;

  /// \brief controls access to the data consumed
  std::condition_variable m_data_consumed;

  /// \brief controls access to the data produced
  std::condition_variable m_data_produced;

  /// \brief asynchronous loops, where the operations are running
  asyncs_loops m_loops;

  /// \brief indicates if the worker is running
  bool m_stopped{true};

  /// \brief m_destroying indicates that the \p produce_consumer should stop
  bool m_destroying{false};

  /// \brief amount of queued data
  size_t m_queued_data{0};

  /// \brief log for the class
  t_log m_log{"concurrent::worker"};
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_WORKER_H
