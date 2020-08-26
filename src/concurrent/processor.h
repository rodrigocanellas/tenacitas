#ifndef TENACITAS_CONCURRENT_PROCESSOR_H
#define TENACITAS_CONCURRENT_PROCESSOR_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <condition_variable>
#include <mutex>
#include <thread>
#include <utility>

#include <concurrent/internal/log.h>
#include <concurrent/result.h>
#include <concurrent/thread.h>
#include <concurrent/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief processor_t allows timeout control of a work function without
/// starting a new thread for each execution
///
/// \tparam t_data, if not \p void, is the type of the data
/// manipulated during the loop; it must be:
///    - default constructible
///    - move constructible/
///
/// \tparam t_time is the type of time used for timeout control
///
/// \tparam t_log must implement:
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
/// void fatal(int p_line, const t_params&... p_params)
///
template <typename t_data, typename t_time, typename t_log> struct processor_t {

  /// \brief worker type
  /// \sa traits_t<t_data>::worker in concurrent/traits.h
  typedef typename traits_t<t_data>::worker worker;

  /// \brief provider type
  /// \sa traits_t<t_data>::provider in concurrent/traits.h
  typedef typename traits_t<t_data>::provider provider;

  /// \brief constructor
  ///
  /// \param p_worker is the work function to be executed
  ///
  /// \param p_provider is the function that will provide a \p t_data object for
  /// \p p_work
  ///
  /// \param p_timeout is the amount of time that \p p_work has to execute.
  processor_t(worker p_worker, provider p_provider, t_time p_timeout)
      : m_worker(p_worker), m_provider(p_provider), m_timeout(p_timeout) {
    concurrent_debug(m_log, "contructor");
    m_thread = concurrent::thread([this]() -> void { loop(); });
    m_stop = false;
  }

  processor_t(const processor_t &) = delete;
  processor_t(processor_t &&p_proc) = delete;

  processor_t &operator=(const processor_t &) = delete;
  processor_t &operator=(processor_t &&p_proc) = delete;

  /// \brief informs if the processor is actually controlling the execution of
  /// the work function
  inline bool is_running() const {
    return ((m_thread.get_id() != std::thread::id()) && (!m_stop));
  }

  /// \brief starts the execution of the processor
  ///
  /// \return status::ok if all went well
  ///         concurrent::stopped_by_user if the method \p stop has was called
  ///         concurrent::stopped_by_timeout if the work function did not
  ///         execute in the defined amount of time
  ///         concurrent::stopped_by_worker if the work function indicates that
  ///         the processor must stop
  status::result operator()() {

    if (m_stop) {
      concurrent_info(m_log, "stopped by user code");
      m_cond_exec.notify_one();
      return concurrent::stopped_by_user;
    }

    concurrent_debug(m_log, "waiting for data");
    std::pair<status::result, t_data> _provided = m_provider();

    if (m_stop) {
      concurrent_info(m_log, "stopped by user code");
      m_cond_exec.notify_one();
      return concurrent::stopped_by_user;
    }

    if (_provided.first != status::ok) {
      concurrent_info(m_log, "provider says there is no more data");
      m_result = _provided.first;
      m_cond_exec.notify_one();
      return m_result;
    }

    if (m_stop) {
      concurrent_info(m_log, "stopped by user code");
      m_cond_exec.notify_one();
      return concurrent::stopped_by_user;
    }

    m_data = std::move(_provided.second);
    concurrent_debug(m_log, "data ", m_data, " arrived");

    concurrent_debug(m_log, "sending notification that excecution can start");
    m_cond_exec.notify_one();
    concurrent_debug(m_log, "notification sent");

    if (m_stop) {
      concurrent_info(m_log, "stopped by user code");
      m_cond_exec.notify_one();
      return concurrent::stopped_by_user;
    }

    std::unique_lock<std::mutex> _lock(m_mutex_time);
    concurrent_debug(m_log, "waiting for worker to finish for data ", m_data);
    if (m_cond_time.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "worker timeout for data ", m_data);
      return concurrent::stopped_by_timeout;
    }
    concurrent_debug(m_log, "worker finished on time");
    return m_result;
  }

  /// \brief stops the execution of the processor
  void stop() {
    if (is_running()) {
      concurrent_debug(m_log, "marking to stop, and sending notification");
      m_stop = true;
      m_cond_exec.notify_one();
      m_thread.join();
    } else {
      concurrent_debug(m_log, "thread not running");
    }
  }

  /// \brief destructor, which stops the processor
  ///
  ~processor_t() {
    concurrent_debug(m_log, "destructor");
    stop();
  }

private:
  /// \brief loop of execution that controls the execution of the work function
  ///
  /// It cooperates with operator()() to determine when the work function must
  /// be started, and if a timeout occurred
  void loop() {
    using namespace std;
    while (true) {
      concurrent_debug(m_log, "waiting for the signal to work");
      unique_lock<mutex> _lock(m_mutex_exec);
      m_cond_exec.wait(_lock);
      concurrent_debug(m_log, "signal to work received");

      if (m_stop) {
        m_result = concurrent::stopped_by_user;
        concurrent_debug(m_log, "ordered to stop");
        m_cond_time.notify_one();
        break;
      }

      concurrent_debug(m_log, "provided ", m_data, " - calling worker");
      m_result = m_worker(std::move(m_data));
      concurrent_debug(m_log, "worker returned ", m_result);

      if (m_stop) {
        concurrent_debug(m_log, "ordered to stop");
        m_result = concurrent::stopped_by_user;
        m_cond_time.notify_one();
        break;
      }

      concurrent_debug(m_log, "sending notification of work done");
      m_cond_time.notify_one();
      concurrent_debug(m_log, "notification of work done is sent");
    }
  }

private:
  worker m_worker;
  provider m_provider;
  t_time m_timeout;
  bool m_stop{true};
  status::result m_result = status::ok;
  concurrent::thread m_thread;
  std::mutex m_mutex_exec;
  std::mutex m_mutex_time;
  std::condition_variable m_cond_exec;
  std::condition_variable m_cond_time;
  t_data m_data;
  t_log m_log{"concurrent::processor"};
};

/// \brief processor_t allows timeout control without starting a new thread for
/// each execution
///
/// Specialization for the case where \p t_data is \p void
///
/// \tparam t_time is the type of time used for timeout control
///
/// \tparam t_log must implement:
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
/// void fatal(int p_line, const t_params&... p_params)
///
template <typename t_time, typename t_log>
struct processor_t<void, t_time, t_log> {
  /// \brief worker type
  /// \sa traits_t<t_data>::worker in concurrent/traits.h
  typedef typename traits_t<void>::worker worker;

  /// \brief provider type
  /// \sa traits_t<t_data>::provider in concurrent/traits.h
  typedef typename traits_t<void>::provider provider;

  /// \brief log alias for @p t_log
  typedef t_log log;

  /// \brief constructor
  ///
  /// \param p_worker is the work function to be executed
  ///
  /// \param p_provider is the function that will provide a \p t_data object for
  /// \p p_work. In this specialization, this parameter exists to keep the same
  /// signature of the constructor of 'processor<t_data, t_time, t_log>'
  ///
  /// \param p_timeout is the amount of time that \p p_work has to execute.
  processor_t(worker p_worker, provider p_provider, t_time p_timeout)
      : m_worker(p_worker), m_provider(p_provider), m_timeout(p_timeout) {
    m_thread = concurrent::thread([this]() -> void { loop(); });
    m_stop = false;
  }

  processor_t(processor_t &&p_proc) = delete;

  processor_t &operator=(const processor_t &) = delete;
  processor_t &operator=(processor_t &&p_proc) = delete;

  /// \brief informs if the processor is actually controlling the execution of
  /// the work function
  inline bool is_running() const {
    return ((m_thread.get_id() != std::thread::id()) && (!m_stop));
  }

  /// \brief starts the execution of the processor
  ///
  /// \return status::ok if all went well
  ///         concurrent::stopped_by_user if the method \p stop has was called
  ///         concurrent::stopped_by_timeout if the work function did not
  ///         execute in the defined amount of time
  ///         concurrent::stopped_by_worker if the work function indicates that
  ///         the processor must stop
  status::result operator()() {

    if (m_stop) {
      concurrent_info(m_log, "stopped by user code");
      m_cond_exec.notify_one();
      return concurrent::stopped_by_user;
    }

    concurrent_debug(m_log, "sending notification that excecution can start");
    m_cond_exec.notify_one();
    concurrent_debug(m_log, "notification sent");

    if (m_stop) {
      concurrent_info(m_log, "stopped by user code");
      m_cond_exec.notify_one();
      return concurrent::stopped_by_user;
    }

    std::unique_lock<std::mutex> _lock(m_mutex_time);
    concurrent_debug(m_log, "waiting for worker to finish");
    if (m_cond_time.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "worker timeout");
      return concurrent::stopped_by_timeout;
    }
    concurrent_debug(m_log, "worker finished on time");
    return m_result;
  }

  void stop() {
    if (is_running()) {
      concurrent_debug(m_log, "marking to stop, and sending notification");
      m_cond_exec.notify_one();
      m_stop = true;
      m_thread.join();
    } else {
      concurrent_debug(m_log, "thread not running");
    }
  }

  ~processor_t() {
    concurrent_debug(m_log, "destructor");
    stop();
  }

private:
  void loop() {
    using namespace std;
    while (true) {
      concurrent_debug(m_log, "waiting for the signal to work");
      unique_lock<mutex> _lock(m_mutex_exec);
      m_cond_exec.wait(_lock);
      concurrent_debug(m_log, "signal to work received");

      if (m_stop) {
        concurrent_debug(m_log, "ordered to stop");
        m_result = concurrent::stopped_by_user;
        m_cond_time.notify_one();
        break;
      }

      m_result = m_worker();
      concurrent_debug(m_log, "worker returned ", m_result);

      if (m_stop) {
        concurrent_debug(m_log, "ordered to stop");
        m_result = concurrent::stopped_by_user;
        m_cond_time.notify_one();
        break;
      }

      concurrent_debug(m_log, "sending notification of work done");
      m_cond_time.notify_one();
      concurrent_debug(m_log, "notification of work done is sent");
    }
  }

private:
  worker m_worker;
  provider m_provider;
  t_time m_timeout;
  bool m_stop{true};
  status::result m_result = status::ok;
  concurrent::thread m_thread;
  std::mutex m_mutex_exec;
  std::mutex m_mutex_time;
  std::condition_variable m_cond_exec;
  std::condition_variable m_cond_time;
  t_log m_log{"concurrent::processor"};
};

} // namespace concurrent
} // namespace tenacitas

#endif // WORK_H
