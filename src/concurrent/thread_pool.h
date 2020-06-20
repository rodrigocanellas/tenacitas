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
/// static void debug(const std::string & p_file, int p_line, const
/// t_params&... p_params)
/// static void info(const std::string & p_file, int p_line, const t_params&...
/// p_params)
/// static void warn(const std::string & p_file, int p_line, const t_params&...
/// p_params)
/// static void error(const std::string & p_file, int p_line, const
/// t_params&... p_params)
/// static void fatal(const std::string & p_file, int p_line, const
/// t_params&... p_params)
template <typename t_data, typename t_log> class thread_pool_t {
public:
  /// \brief work_t is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  /// \param t_data is an instance of the data to be handled
  ///
  /// \return result::stop if the loop where this function is being
  /// called should stop, or status::dont_stop if it should continue
  typedef typename traits_t<t_data>::worker worker;

  /// \brief log alias for @p t_log
  typedef t_log log;

  /// \brief thread_pool constructor
  thread_pool_t()
      : m_loops(), m_values(), m_stopped(true), m_destroying(false) {}

  /// \brief move constructor
  thread_pool_t(thread_pool_t &&p_pool) noexcept {
    // save if the right side was stopped
    bool _stopped = p_pool.is_stopped();

    // stop the right side
    p_pool.stop();

    // move the \p t_data collection
    m_values = std::move(p_pool.m_values);

    // build the work function collection, moving the work function from the
    // right side, and reseting the provide and break function
    for (async_loop &_loop_right : p_pool.m_loops) {
      async_loop _loop(
          std::move(_loop_right.get_work()), _loop_right.get_timeout(),
          [this]() -> status { return this->stop_condition(); },
          [this]() -> std::pair<bool, t_data> { return this->data(); });

      add_work(std::move(_loop));
    }

    // seting the flags
    m_destroying = false;

    m_stopped = true;

    // if the right side was not stopped
    if (!_stopped) {
      // run this thread pool
      run();
    }
  }

  /// \brief copy constructor not allowed
  thread_pool_t(const thread_pool_t &) = delete;

  /// \brief if 'stop ' was not called, empties the pool, waiting for all the
  /// data to be processed
  ~thread_pool_t() {
    m_destroying = true;
    concurrent_log_debug(log, this, " m_values.size() = ", m_values.size());

    if (!m_stopped) {
      while (!m_values.empty()) {
        concurrent_log_debug(log, this, " m_values.size() = ", m_values.size());
        m_cv_data.notify_all();
        concurrent_log_debug(log, this, " waiting for poping, ", this);
        std::unique_lock<std::mutex> _lock(m_mutex_poped);
        m_cv_poped.wait(_lock, [this] {
          concurrent_log_debug(log, this, " poping signal arrived, ", this);
          return true;
        });
        concurrent_log_debug(log, this, " lock free");
      }
      concurrent_log_debug(log, "this, m_values.size() = ", m_values.size());
      stop();
    }
  }

  /// \brief move assignment
  thread_pool_t &operator=(thread_pool_t &&p_pool) noexcept {
    if (this != &p_pool) {
      // save if the right side was stopped
      bool _stopped = p_pool.is_stopped();

      // stop the right side
      p_pool.stop();

      // move the \p t_data collection
      m_values = std::move(p_pool.m_values);

      // build the work function collection, moving the work function from
      // the right side, and reseting the provide and break function
      for (async_loop &_loop_right : p_pool.m_loops) {
        async_loop _loop(
            std::move(_loop_right.get_work()), _loop_right.get_timeout(),
            [this]() -> bool { return this->stop_condition(); },
            [this]() -> std::pair<bool, t_data> { return this->data(); });

        add_work(std::move(_loop));
      }

      // seting the flags
      m_destroying = false;

      m_stopped = true;

      // if the right side was not stopped
      if (!_stopped) {
        // run this thread pool
        run();
      }
    }
    return *this;
  }

  /// \brief copy assignment not allowed
  thread_pool_t &operator=(const thread_pool_t &) = delete;

  /// \brief is_stopped
  /// \return \p true if the loop is not running; \p false othewise
  bool is_stopped() const { return m_stopped; }

  /// \brief add_work adds a bunch of \p work_t functions
  /// \param p_num_works the number of \p work_t functions to be added
  /// \param p_work_factory a function that creates \p work_t functions
  /// \param p_work_timeout timeout for the \p work_t functions
  void add_work(uint16_t p_num_works, std::function<worker()> p_work_factory,
                std::chrono::milliseconds p_work_timeout) {
    for (uint16_t _i = 0; _i < p_num_works; ++_i) {
      async_loop _loop(
          p_work_factory(), p_work_timeout,
          [this]() -> status { return this->stop_condition(); },
          [this]() -> std::pair<bool, t_data> { return this->data(); });

      add_work(std::move(_loop));
    }
  }

  /// \brief add_work adds one \p work_t function
  /// \param p_work the \p work_t fuction to be added
  /// \param p_work_timeout timeout of this \p work_t function
  void add_work(worker p_work, std::chrono::milliseconds p_work_timeout) {
    async_loop _loop(
        p_work, p_work_timeout,
        [this]() -> status { return this->stop_condition(); },
        [this]() -> std::pair<bool, t_data> { return this->data(); });

    add_work(std::move(_loop));
  }

  /// \brief handle sends an instance of \p t_data to be handled
  /// \param p_value the value to be handled
  inline void handle(const t_data &p_value) { add_data(p_value); }

  /// \brief run starts the thread_pool
  ///
  /// From this call on, the \p work_t functions will to "fight" among each
  /// other, in order to process any instance of \p t_data that was inserted,
  /// using the \p handle method, into the pool
  inline void run() {
    if (m_stopped) {
      run_common();
    }
  }

  /// \brief interrupt stops the \p thread_pool
  ///
  /// From this call on, the \p work_t functions will stop "fighting" among
  /// each other, in order to process any instance of \p t_data that was
  /// inserted, using the \p handle method, into the pool
  inline void stop() {

    std::unique_lock<std::mutex> _lock(m_mutex_stop);
    m_stopped = true;
    if (m_loops.empty()) {
      return;
    }

    m_cv_data.notify_all();
    for (async_loop &_loop : m_loops) {
      concurrent_log_debug(log, "stopping loop ", &_loop);
      _loop.stop();
    }
  }

private:
  /// \brief provide_t is the type of function that provides data to the work
  /// function during the loop execution
  ///
  /// \return a pair, where if \p first is \p true, the \p second has a
  /// meaningful data; if \p first is \p false, then \p second has a default
  /// value of \p t_data
  typedef typename traits_t<t_data>::provider provider;

  /// \brief the collection of values
  typedef std::list<t_data> values_t;

  /// \brief async_loop_t is an simples for the \p async_loop where a \p
  /// work_t function will be running
  typedef async_loop_t<t_data, t_log> async_loop;

  /// \brief async_loops_t is the collection of \p async_loop
  typedef std::vector<async_loop> async_loops_t;

private:
  /// \brief run_common is called from other functions, in order to start the
  /// \p thread_pool
  void run_common() {

    concurrent_log_debug(log, this, " running ", this);
    std::unique_lock<std::mutex> _lock(m_mutex_stop);
    if (m_loops.empty()) {
      concurrent_log_debug(log, this, " cant run because it is empty");
      return;
    }
    m_stopped = false;
    concurrent_log_debug(log, this, " m_stopped = ", m_stopped);

    for (async_loop &_loop : m_loops) {
      concurrent_log_debug(log, this, " starting loop ", &_loop);
      _loop.run();
    }
    concurrent_log_debug(log, this, " started ");
  }

  /// \brief add_work common function called to add a \p work_t function
  /// \param p_loop the new \p work_t function to be added
  void add_work(async_loop &&p_loop) {
    std::lock_guard<std::mutex> _lock(m_add_work);
    m_loops.push_back(std::move(p_loop));
  }

  /// \brief stop_condition
  /// \return \p true if the flag indicating that the \p thread_pool should
  /// stop is \p true; \p false otherwise
  status stop_condition() {

    concurrent_log_debug(log, this, " stopped = ", m_stopped);
    return (m_stopped ? status::stop : status::dont_stop);
  }

  /// \brief data is the \p provide_t function, which returns data, if
  /// available, to a \p work_t function \return
  ///
  /// \return (true, a filled \p t_data object), if there is any instance of
  /// \p data available; of (false, t_data()) otherwise
  std::pair<bool, t_data> data() {

    using namespace std;

    if (m_stopped) {
      concurrent_log_debug(log, this,
                           " not waiting for more data because it is stopped ");

      return std::make_pair(false, t_data());
    }

    unique_lock<std::mutex> _lock(m_mutex_data);
    m_cv_data.wait(_lock, [this] {
      concurrent_log_debug(log, this, " waiting");
      return (!m_values.empty() || m_stopped);
    });

    if (m_stopped) {
      concurrent_log_debug(log, this, " m_stopped, returning 'false'");
      return std::make_pair(false, t_data());
    }

    std::pair<bool, t_data> _return =
        std::make_pair(true, std::move(m_values.front()));
    concurrent_log_debug(log, this, " data to be handled ", _return.second);
    m_values.pop_front();

    if (m_destroying && !m_values.empty()) {
      concurrent_log_debug(log, this, " signaling poped");
      m_cv_poped.notify_all();
    }

    return _return;
  }

  /// \brief add_data adds a instance of \p t_data to the pool
  /// \param p_value is an instance of \p t_data to be added
  void add_data(const t_data &p_value) {
    concurrent_log_debug(log, this, " adding ", p_value);
    {
      std::lock_guard<std::mutex> _lock(m_mutex_data);
      m_values.push_back(p_value);
    }
    concurrent_log_debug(log, this, " added and notifying ");
    m_cv_data.notify_all();
  }

private:
  /// \brief m_loops the pool of async_loops
  async_loops_t m_loops;

  /// \brief m_values is the collection of instances of \p t_data
  values_t m_values;

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
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_THREAD_POOL_H
