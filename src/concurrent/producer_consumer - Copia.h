#ifndef TENACITAS_CONCURRENT_PRODUCER_CONSUMER_H
#define TENACITAS_CONCURRENT_PRODUCER_CONSUMER_H

#include <array>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <utility>
#include <vector>

#include <concurrent/async_loop.h>
#include <concurrent/internal/log.h>
#include <concurrent/result.h>
#include <concurrent/traits.h>
#include <status/result.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

template <class t_data, size_t t_size, typename t_log,
          typename t_time = std::chrono::milliseconds>
class producer_consumer_t {
public:
  typedef t_data data;

  /// \brief log alias for @p t_log
  typedef t_log log;

  typedef t_time time;

  /// \brief worker type
  /// \sa traits_t<t_data>::worker in concurrent/traits.h
  typedef typename traits_t<t_data>::worker worker;

public:
  producer_consumer_t() = default;
  producer_consumer_t(const producer_consumer_t &) = delete;
  producer_consumer_t(producer_consumer_t &&) = delete;
  producer_consumer_t &operator=(const producer_consumer_t &) = delete;
  producer_consumer_t &operator=(producer_consumer_t &&) = delete;

  /// \brief destuctor
  ///
  /// If 'stop ' was not called, empties the pool, waiting for all the data to
  /// be processed
  ~producer_consumer_t() {
    //    m_destroying = true;
    concurrent_log_debug(log, "m_length = ", m_length);

    if (!all_loops_stopped()) {
      if (!m_stopped) {
        while (m_length > 0) {
          concurrent_log_debug(log, "m_length = ", m_length);
          m_data_produced.notify_all();
          concurrent_log_debug(log, "waiting for poping");
          std::unique_lock<std::mutex> _lock(m_mutex_data);
          m_data_consumed.wait(_lock, [this] {
            concurrent_log_debug(log, "data consumed signal arrived");
            return true;
          });
        }
        concurrent_log_debug(log, "m_length = ", m_length);
        stop();
      }
    }
  }

  void add(const t_data &p_data) {
    concurrent_log_debug(t_log, "waiting for room ...");
    std::unique_lock<std::mutex> _lock(m_mutex_data);
    m_data_consumed.wait(_lock, [this]() { return m_length < t_size; });
    concurrent_log_debug(t_log, "there is room");

    m_array[m_write++] = p_data;
    ++m_length;
    if (m_write == (t_size)) {
      m_write = 0;
    }

    concurrent_log_debug(t_log, "adding ", p_data);
    _lock.unlock();
    m_data_produced.notify_all();
  }

  void add(t_data &&p_data) {
    concurrent_log_debug(t_log, "waiting for room ...");
    std::unique_lock<std::mutex> _lock(m_mutex_data);
    m_data_consumed.wait(_lock, [this]() { return m_length < t_size; });
    concurrent_log_debug(t_log, "there is room");

    m_array[m_write++] = std::move(p_data);
    ++m_length;
    if (m_write == (t_size)) {
      m_write = 0;
    }

    concurrent_log_debug(t_log, "adding ", p_data);
    //    _lock.unlock();

    // notifyng that new data is available
    m_data_produced.notify_all();
  }

  /// \brief add_work adds one \p worker function
  /// \param p_work the \p worker fuction to be added
  /// \param p_worker_timeout timeout of this \p worker function
  void add(worker p_work, time p_timeout) {
    async_loop _loop(
        p_work, [this]() -> status::result { return this->breaker(); },
        p_timeout,
        [this]() -> std::pair<status::result, t_data> { return this->get(); });

    add(std::move(_loop));
  }

  /// \brief is_stopped
  /// \return \p true if the loop is not running; \p false othewise
  inline bool is_stopped() const { return m_stopped; }

  /// \brief add_work adds a bunch of \p worker functions
  /// \param p_num_works the number of \p worker functions to be added
  /// \param p_work_factory a function that creates \p worker functions
  /// \param p_worker_timeout timeout for the \p worker functions
  void add(uint16_t p_num_works, std::function<worker()> p_work_factory,
           time p_worker_timeout) {
    for (uint16_t _i = 0; _i < p_num_works; ++_i) {
      async_loop _loop(
          p_work_factory(),
          [this]() -> status::result { return this->breaker(); },
          p_worker_timeout,
          [this]() -> std::pair<status::result, t_data> {
            return this->get();
          });

      add(std::move(_loop));
    }
  }

  /// \brief run starts the producer_consumer
  ///
  /// From this call on, the \p worker functions will to "fight"among each
  /// other, in order to process any instance of \p t_data that was inserted,
  /// using the \p handle method, into the pool
  inline void start() {
    if (!m_stopped) {
      return;
    }

    std::unique_lock<std::mutex> _lock(m_mutex_stop);
    if (m_loops.empty()) {
      concurrent_log_warn(log, "can't run because it is empty");
      return;
    }
    concurrent_log_debug(log, "starting");
    m_stopped = false;
    for (async_loop &_loop : m_loops) {
      _loop.start();
    }
    concurrent_log_debug(log, "started ");
  }

  /// \brief interrupt stops the \p producer_consumer
  ///
  /// From this call on, the \p worker functions will stop "fighting"among
  /// each other, in order to process any instance of \p t_data that was
  /// inserted into the pool
  inline void stop() {

    std::unique_lock<std::mutex> _lock(m_mutex_stop);
    m_stopped = true;
    if (m_loops.empty()) {
      return;
    }

    m_data_produced.notify_all();
    for (async_loop &_loop : m_loops) {
      concurrent_log_debug(log, "stopping loop ", &_loop);
      _loop.stop();
    }
  }

private:
  /// \brief the collection of values
  typedef std::array<t_data, t_size> values;

  typedef typename values::size_type index;

  /// \brief async_loop_t is a \p async_loop where a \p worker function will be
  /// running
  typedef async_loop_t<t_data, t_log, t_time> async_loop;

  /// \brief async_loops_t is the collection of \p async_loop
  typedef typename std::vector<async_loop> asyncs_loops;

private:
  /// \brief breaker
  /// \return \p true if the flag indicating that the \p producer_consumer
  /// should stop is \p true; \p false otherwise
  inline status::result breaker() {
    return (m_stopped ? concurrent::stopped_by_breaker : status::ok);
  }

  /// \brief add_work common function called to add a \p worker function
  /// \param p_loop the new \p worker function to be added
  void add(async_loop &&p_loop) {
    std::lock_guard<std::mutex> _lock(m_add_work);
    m_loops.push_back(std::move(p_loop));
  }

  /// \brief get is the \p provide_t function, which returns data, if
  /// available, to a \p worker function
  ///
  /// \return (true, a filled \p t_data object), if there is any instance of
  /// \p t_data available; of (false, t_data()) otherwise
  std::pair<status::result, t_data> get() {
    using namespace std;

    if (m_stopped) {
      concurrent_log_debug(log, "stopped");
      return {concurrent::stopped_by_provider, t_data()};
    }

    concurrent_log_debug(t_log, "waiting for data...");
    std::unique_lock<std::mutex> _lock(m_mutex_data);
    m_data_produced.wait(
        _lock, [this]() -> bool { return (m_length > 0) || m_stopped; });
    concurrent_log_debug(t_log, "data arrived...");

    if (m_stopped) {
      concurrent_log_debug(log, "stopped");
      return {concurrent::stopped_by_provider, t_data()};
    }

    t_data _data = m_array[m_read++];
    --m_length;
    if (m_read == t_size) {
      m_read = 0;
    }

    concurrent_log_debug(t_log, "getting ", _data);

    // notifying that new data can be added
    //    if (m_destroying && (m_length>0)) {
    //      m_data_consumed.notify_all();
    //    }
    m_data_consumed.notify_all();
    return {status::ok, _data};
  }

  /// \brief informs if all the \p async_loops are stopped
  bool all_loops_stopped() const {
    for (const async_loop &_async_loop : m_loops) {
      if (!_async_loop.is_stopped()) {
        return false;
      }
    }
    return true;
  }

private:
  values m_array;
  index m_read = 0;
  index m_write = 0;
  index m_length = 0;
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
  bool m_stopped = true;
  /// \brief m_destroying indicates that the \p produce_consumer should stop
  //  bool m_destroying = false;
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_PRODUCER_CONSUMER_H
