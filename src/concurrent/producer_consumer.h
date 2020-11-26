#ifndef TENACITAS_CONCURRENT_PRODUCER_CONSUMER_H
#define TENACITAS_CONCURRENT_PRODUCER_CONSUMER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <array>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <utility>
#include <vector>

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

/// \brief producer_consumer implements the producer/consumer pattern
///
/// \tparam t_container is the type of container used to add produced data, that
/// will be caught to be consumed from.
///
/// It must define:
///   \p data, which is the type of data it stores
///   \code void add(const data &) \endcode, which stores data into the
///   container
///
///   \code void add(data &&) \endcode, which stores data into the container
///
///   \code data get() \endcode, which removes data from the container
///
///   \code bool empty() const \endcode, which informs if the container is empty
///
///   \code bool full() const \endcode, which informs if the container is full
///
///   \code size_t capacity() \endcode, which informs the the capacity
///
///   \code size_t occupied() \endcode, which informs the amount of slots
///   occupied
///
/// \tparam t_container::data must be:
///    - default constructible
///    - move constructible
///
/// t_log(const char *p_id)
/// void debug(int p_line, const t_data&... p_params)
/// void info(int p_line, const t_data&... p_params)
/// void warn(int p_line, const t_data&... p_params)
/// void error(int p_line, const t_data&... p_params)
/// void fatal(int p_line, const t_data&... p_params)
///
/// \tparam t_time is the type of time used for timeout control
///
template <typename t_log, typename t_data> class producer_consumer_t {
public:
  typedef t_data data;

  typedef queue_t<t_log, t_data> queue;

  typedef typename queue::ptr queue_ptr;

  typedef std::function<void(data &&)> worker;

public:
  producer_consumer_t(queue_ptr &&p_queue_ptr) : m_queue(p_queue_ptr) {}

  producer_consumer_t(const producer_consumer_t &) = delete;

  producer_consumer_t(producer_consumer_t &&p_pc) = delete;

  producer_consumer_t &operator=(const producer_consumer_t &) = delete;

  producer_consumer_t &operator=(producer_consumer_t &&p_pc) = delete;

  /// \brief destructor
  ///
  /// If 'stop' was not called, empties the pool, waiting for all the data to
  /// be processed
  ~producer_consumer_t() {
    concurrent_debug(m_log, "destructor");
    //    m_destroying = true;
    if (!all_loops_stopped()) {
      if (!m_stopped) {
        while (!m_queue->empty()) {
          m_data_produced.notify_all();
          //          debug(m_log, "waiting for poping");
          std::unique_lock<std::mutex> _lock(m_mutex_data);
          m_data_consumed.wait(_lock, [this] {
            //            debug(m_log, "data consumed signal
            //            arrived");
            return true;
          });
        }
        stop();
      }
    }
    concurrent_info(m_log, "leaving destructor");
  }

  /// \brief adds data to be consumed
  ///
  /// \p data will only be added if \p start() was called
  ///
  /// \param p_data is the data produced
  ///
  /// \return \p true if it was added, \p false otherwise
  bool add(const data &p_data) {
    if (m_stopped) {
      concurrent_error(m_log, "could not add data because 'producer_consumer' "
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

  /// \brief adds data to be consumed
  ///
  /// \p data will only be added if \p start() was called
  ///
  /// \param p_data is the data produced
  ///
  /// \return \p true if it was added, \p false otherwise
  bool add(data &&p_data) {
    if (m_stopped) {
      concurrent_error(m_log, "could not add data because 'producer_consumer' "
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
  inline uint64_t amount_added() const { return m_queued_data; }

  /// \brief add adds one \p worker function
  ///
  /// \param p_work the \p worker fuction to be added
  ///
  /// \param p_timeout timeout of this \p worker function
  template <typename t_time>
  void add(worker p_work, t_time p_timeout,
           timeout_callback p_timeout_callback) {

    std::unique_lock<std::mutex> _lock_stop(m_mutex_stop);

    auto _breaker = [this]() -> bool { return this->breaker(); };

    auto _provider = [this]() -> std::optional<data> {
      return this->provider();
    };

    async_loop_ptr _loop(std::make_shared<async_loop>(
        p_timeout, p_work, _breaker, _provider, p_timeout_callback));

    add(_loop);
  }

  void add(worker p_work) {

    std::unique_lock<std::mutex> _lock_stop(m_mutex_stop);

    auto _breaker = [this]() -> bool { return this->breaker(); };

    auto _provider = [this]() -> std::optional<data> {
      return this->provider();
    };

    async_loop_ptr _loop(
        std::make_shared<async_loop>(p_work, _breaker, _provider));

    add(_loop);
  }

  /// \brief add_work adds a bunch of \p worker functions
  ///
  /// \param p_num_works the number of \p worker functions to be added
  ///
  /// \param p_work_factory a function that creates \p worker functions
  ///
  /// \param p_worker_timeout timeout for the \p worker functions
  template <typename t_time>
  void add(uint16_t p_num_works, std::function<worker()> p_work_factory,
           t_time p_worker_timeout, timeout_callback p_timeout_callback) {

    std::unique_lock<std::mutex> _lock_stop(m_mutex_stop);

    auto _breaker = [this]() -> bool { return this->breaker(); };

    auto _provider = [this]() -> std::optional<data> {
      return this->provider();
    };

    for (auto _i = 0; _i < p_num_works; ++_i) {
      async_loop_ptr _loop(std::make_shared<async_loop>(
          p_worker_timeout, p_work_factory(), _breaker, _provider,
          p_timeout_callback));

      add(_loop);
    }
  }

  void add(uint16_t p_num_works, std::function<worker()> p_work_factory) {

    std::unique_lock<std::mutex> _lock_stop(m_mutex_stop);

    auto _breaker = [this]() -> bool { return this->breaker(); };

    auto _provider = [this]() -> std::optional<data> {
      return this->provider();
    };

    for (auto _i = 0; _i < p_num_works; ++_i) {
      async_loop_ptr _loop(
          std::make_shared<async_loop>(p_work_factory(), _breaker, _provider));

      add(_loop);
    }
  }

  /// \brief is_stopped
  ///
  /// \return \p true if the loop is not running; \p false othewise
  inline bool is_stopped() const { return m_stopped; }

  /// \brief run starts the producer_consumer
  ///
  /// From this call on, the \p worker functions will to "fight" among each
  /// other, in order to process any instance of \p data that was inserted
  /// into the pool
  ///
  /// \return \p true because no \p worker function was added
  inline bool start() {
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

  /// \brief interrupt stops the \p producer_consumer
  ///
  /// From this call on, the \p worker functions will stop "fighting" among
  /// each other, in order to process any instance of \p data that was
  /// inserted into the pool
  inline void stop() {

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

  /// \brief the capacity if the \p t_container
  inline size_t capacity() const { return m_queue->capacity(); }

  /// \brief the amount of slots occupied in the \p t_container
  inline size_t occupied() const { return m_queue->occupied(); }

private:
  /// \brief async_loop_t is a \p async_loop where a \p worker function will be
  /// running
  typedef async_loop_t<t_log, true, data> async_loop;

  typedef typename std::shared_ptr<async_loop> async_loop_ptr;

  /// \brief async_loops_t is the collection of \p async_loop
  typedef typename std::vector<async_loop_ptr> asyncs_loops;

private:
  /// \brief breaker
  ///
  /// \return \p true if the flag indicating that the \p producer_consumer
  /// should stop is \p true; \p false otherwise
  inline bool breaker() { return (m_stopped ? true : false); }

  /// \brief add_work common function called to add a \p worker function
  ///
  /// \param p_loop the new \p worker function to be added
  void add(async_loop_ptr p_loop) {
    std::lock_guard<std::mutex> _lock(m_add_work);
    m_loops.push_back(p_loop);
  }

  /// \brief provider provides data, if available, to a \p worker function
  ///
  /// \return {true, a filled \p data object}, if there is any instance of
  /// \p data available; of {false, data()} otherwise
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

      // notifying that new data can be added
      //    if (m_destroying && (!m_container.empty())) {
      //      m_data_consumed.notify_all();
      //    }
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
  queue_ptr m_queue;

  /// \brief m_add_work controls access to the \p m_loops while inserting a
  /// new \p t_work function
  std::mutex m_add_work;

  std::mutex m_mutex_data;

  /// \brief m_mutex_stop controls access to attributes while the \p
  /// producer_consumer is stopping
  std::mutex m_mutex_stop;

  std::condition_variable m_data_consumed;

  std::condition_variable m_data_produced;

  asyncs_loops m_loops;

  bool m_stopped{true};

  /// \brief m_destroying indicates that the \p produce_consumer should stop
  bool m_destroying{false};

  uint64_t m_queued_data{0};

  t_log m_log{"concurrent::producer_consumer"};
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_PRODUCER_CONSUMER_H
