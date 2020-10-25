#ifndef TENACITAS_CONCURRENT_THREAD_POOL_H
#define TENACITAS_CONCURRENT_THREAD_POOL_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cassert>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include <concurrent/async_loop.h>
#include <concurrent/internal/log.h>
#include <concurrent/traits.h>
#include <status/result.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief thread_pool implements a thread pool, which allows to use the
/// producer/consumer pattern
///
/// \tparam t_data is the type of data inserted into the pool, so that
/// registered work functions can "fight" among each other to get the data to
/// process it.
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
/// void fatal(int p_line, const t_params&... p_params)
///
/// \tparam t_time is the type of time used for timeout control
///
template <typename t_data, typename t_log,
          typename t_time = std::chrono::milliseconds>
class thread_pool_t {
public:
  /// \brief worker type
  /// \sa traits_t<t_data>::worker in concurrent/traits.h
  typedef typename traits_t<t_data>::worker worker;

  /// \brief thread_pool constructor
  thread_pool_t()
      : m_loops(), m_values(), m_stopped(true), m_destroying(false) {}

  /// \brief move constructor
  thread_pool_t(thread_pool_t &&p_pool) noexcept;

  /// \brief copy constructor not allowed
  thread_pool_t(const thread_pool_t &) = delete;

  /// \brief destuctor
  ///
  /// If 'stop ' was not called, empties the pool, waiting for all the data to
  /// be processed
  ~thread_pool_t() {
    m_destroying = true;
    concurrent_debug(m_log, "m_values.size() = ", m_values.size());

    if (!all_loops_stopped()) {
      if (!m_stopped) {
        while (!m_values.empty()) {
          concurrent_debug(m_log, "m_values.size() = ", m_values.size());
          m_cv_data.notify_all();
          concurrent_debug(m_log, "waiting for poping");
          std::unique_lock<std::mutex> _lock(m_mutex_poped);
          m_cv_poped.wait(_lock, [this] {
            concurrent_debug(m_log, "poping signal arrived");
            return true;
          });
          concurrent_debug(m_log, "lock free");
        }
        concurrent_debug(m_log, "m_values.size() = ", m_values.size());
        stop();
      }
    }
  }

  /// \brief move assignment
  thread_pool_t &operator=(thread_pool_t &&p_pool) = delete;

  /// \brief copy assignment not allowed
  thread_pool_t &operator=(const thread_pool_t &) = delete;

  /// \brief is_stopped
  /// \return \p true if the loop is not running; \p false othewise
  inline bool is_stopped() const { return m_stopped; }

  /// \brief add_work adds a bunch of \p worker functions
  /// \param p_num_works the number of \p worker functions to be added
  /// \param p_work_factory a function that creates \p worker functions
  /// \param p_worker_timeout timeout for the \p worker functions
  void add_work(uint16_t p_num_works, std::function<worker()> p_work_factory,
                t_time p_worker_timeout) {
    auto _breaker = [this]() -> bool { return this->stop_condition(); };

    auto _provider = [this]() -> std::pair<bool, t_data> {
      return this->data();
    };

    for (uint16_t _i = 0; _i < p_num_works; ++_i) {
      async_loop_ptr _loop(std::make_shared<async_loop>(
          p_work_factory(), _breaker, p_worker_timeout, _provider));

      add_work(_loop);
    }
  }

  /// \brief add_work adds one \p worker function
  /// \param p_work the \p worker fuction to be added
  /// \param p_worker_timeout timeout of this \p worker function
  void add_work(worker p_work, t_time p_worker_timeout) {
    auto _breaker = [this]() -> bool { return this->stop_condition(); };

    auto _provider = [this]() -> std::pair<bool, t_data> {
      return this->data();
    };

    async_loop_ptr _loop(std::make_shared<async_loop>(
        p_work, _breaker, p_worker_timeout, _provider));

    add_work(_loop);
  }

  /// \brief handle sends an instance of \p t_data to be handled
  /// \param p_value the value to be handled
  inline void handle(const t_data &p_value) { add_data(p_value); }

  /// \brief handle sends an instance of \p t_data to be handled
  /// \param p_value the value to be handled
  inline void handle(t_data &&p_value) { add_data(p_value); }

  /// \brief run starts the thread_pool
  ///
  /// From this call on, the \p worker functions will to "fight"among each
  /// other, in order to process any instance of \p t_data that was inserted,
  /// using the \p handle method, into the pool
  inline void start() {
    if (m_stopped) {
      run_common();
    }
  }

  /// \brief interrupt stops the \p thread_pool
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

    m_cv_data.notify_all();
    for (async_loop_ptr _loop : m_loops) {
      concurrent_debug(m_log, "stopping loop ", &_loop);
      _loop->stop();
    }
  }

private:
  /// \brief provider type
  /// \sa traits_t<t_data>::provider in concurrent/traits.h
  typedef typename traits_t<t_data>::provider provider;

  /// \brief the collection of values
  typedef std::list<t_data> values;

  /// \brief async_loop_t is a \p async_loop where a \p worker function will be
  /// running
  typedef async_loop_t<t_data, t_log, t_time> async_loop;

  typedef typename std::shared_ptr<async_loop> async_loop_ptr;

  //  typedef typename std::shared_ptr<const async_loop> async_loop_const_ptr;

  /// \brief async_loops_t is the collection of \p async_loop
  typedef std::vector<async_loop_ptr> async_loops;

private:
  /// \brief run_common is called from other functions, in order to start the
  /// \p thread_pool
  void run_common() {

    concurrent_debug(m_log, "running ", this);
    std::unique_lock<std::mutex> _lock(m_mutex_stop);
    if (m_loops.empty()) {
      concurrent_debug(m_log, "can't run because it is empty");
      return;
    }
    m_stopped = false;
    concurrent_debug(m_log, "m_stopped = ", m_stopped);

    for (async_loop_ptr _loop : m_loops) {
      concurrent_debug(m_log, "starting loop ", &_loop);
      _loop->start();
    }
    concurrent_debug(m_log, "started ");
  }

  /// \brief add_work common function called to add a \p worker function
  /// \param p_loop the new \p worker function to be added
  void add_work(async_loop_ptr p_loop) {
    std::lock_guard<std::mutex> _lock(m_add_work);
    m_loops.push_back(p_loop);
  }

  /// \brief stop_condition
  /// \return \p true if the flag indicating that the \p thread_pool should
  /// stop is \p true; \p false otherwise
  bool stop_condition() {
    concurrent_debug(m_log, "stopped = ", m_stopped);
    return (m_stopped ? true : false);
  }

  /// \brief data is the \p provide_t function, which returns data, if
  /// available, to a \p worker function \return
  ///
  /// \return (true, a filled \p t_data object), if there is any instance of
  /// \p data available; of (false, t_data()) otherwise
  std::pair<bool, t_data> data() {

    using namespace std;

    if (m_stopped) {
      concurrent_debug(m_log,
                       "not waiting for more data because it is stopped ");

      return std::make_pair(false, t_data());
    }

    unique_lock<std::mutex> _lock(m_mutex_data);
    m_cv_data.wait(_lock, [this] {
      concurrent_debug(m_log, "waiting");
      return (!m_values.empty() || m_stopped);
    });

    if (m_stopped) {
      concurrent_debug(m_log, "m_stopped, returning 'false'");
      return std::make_pair(false, t_data());
    }

    std::pair<bool, t_data> _return = {true, std::move(m_values.front())};
    concurrent_debug(m_log, "data to be handled ", _return.second);
    m_values.pop_front();

    if (m_destroying && !m_values.empty()) {
      concurrent_debug(m_log, "signaling poped");
      m_cv_poped.notify_all();
    }

    return _return;
  }

  /// \brief add_data adds a instance of \p t_data to the pool
  /// \param p_value is an instance of \p t_data to be added
  void add_data(const t_data &p_value) {
    concurrent_debug(m_log, "adding ", p_value);
    {
      std::lock_guard<std::mutex> _lock(m_mutex_data);
      m_values.push_back(p_value);
    }
    concurrent_debug(m_log, "added and notifying ");
    m_cv_data.notify_all();
  }

  /// \brief add_data adds a instance of \p t_data to the pool
  /// \param p_value is an instance of \p t_data to be added
  void add_data(t_data &&p_value) {
    concurrent_debug(m_log, "adding ", p_value);
    {
      std::lock_guard<std::mutex> _lock(m_mutex_data);
      m_values.push_back(std::move(p_value));
    }
    concurrent_debug(m_log, "added and notifying ");
    m_cv_data.notify_all();
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
  /// \brief m_loops the pool of async_loops
  async_loops m_loops;

  /// \brief m_values is the collection of instances of \p t_data
  values m_values;

  /// \brief m_stopped indicates if the \p thread_pool should stop
  bool m_stopped = true;

  /// \brief m_mutex_data controls access to \p m_values
  std::mutex m_mutex_data;

  /// \brief m_cv_data signalizes if there is \p t_data available
  std::condition_variable m_cv_data;

  /// \brief m_mutex_stop controls access to attributes while the \p
  /// thread_pool is stopping
  std::mutex m_mutex_stop;

  /// \brief m_mutex_poped controls acesss to \p m_values while the \p t_data
  /// instances are being consumed during the destruction of the \p
  /// thread_pool
  std::mutex m_mutex_poped;

  /// \brief m_cv_poped signalizes if there is \p t_data available the
  /// destruction of the \p thread_pool
  std::condition_variable m_cv_poped;

  /// \brief m_add_work controls access to the \p m_loops while inserting a
  /// new \p t_work function
  std::mutex m_add_work;

  /// \brief m_destroying indicates that the \p thread_pool should stop
  bool m_destroying = false;

  t_log m_log{"concurrent::thread_pool"};
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_THREAD_POOL_H
