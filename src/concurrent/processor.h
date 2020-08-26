#ifndef TENACITAS_CONCURRENT_PROCESS_H
#define TENACITAS_CONCURRENT_PROCESS_H

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

///
///
/// \tparam t_log provides log funcionality:
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
/// void fatal(int p_line, const t_params&... p_params)
template <typename t_data, typename t_time, typename t_log> struct processor_t {
  /// \brief worker type
  /// \sa traits_t<t_data>::worker in concurrent/traits.h
  typedef typename traits_t<t_data>::worker worker;

  /// \brief provider type
  /// \sa traits_t<t_data>::provider in concurrent/traits.h
  typedef typename traits_t<t_data>::provider provider;

  processor_t(worker p_worker, provider p_provider, t_time p_timeout)
      : m_worker(p_worker), m_provider(p_provider), m_timeout(p_timeout) {
    concurrent_debug(m_log, "contructor");
  }

  processor_t(const processor_t &) = delete;
  processor_t(processor_t &&p_proc) noexcept {
    concurrent_debug(m_log, "move contructor");
    m_worker = std::move(p_proc.m_worker);
    m_provider = std::move(p_proc.m_provider);
    m_timeout = std::move(p_proc.m_timeout);
    m_stop = false;
    m_result = status::ok;
    if (p_proc.is_running()) {
      p_proc.stop();
      (*this)();
    }
  }

  processor_t &operator=(const processor_t &) = delete;
  processor_t &operator=(processor_t &&p_proc) noexcept {
    concurrent_debug(m_log, "move =");
    if (this != &p_proc) {

      m_worker = std::move(p_proc.m_worker);
      m_provider = std::move(p_proc.m_provider);
      m_timeout = std::move(p_proc.m_timeout);
      m_stop = false;
      m_result = status::ok;
      if (p_proc.is_running()) {
        p_proc.stop();
        (*this)();
      }
    }
    return *this;
  }

  inline bool is_running() const {
    return ((m_thread.get_id() != std::thread::id()) && (!m_stop));
  }

  status::result operator()() {

    if (!is_running()) {
      concurrent_debug(m_log, "starting thread, as it was not running");
      m_thread = concurrent::thread([this]() -> void { loop(); });
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      m_stop = false;
    }

    std::pair<status::result, t_data> _provided = m_provider();

    if (_provided.first != status::ok) {
      m_result = _provided.first;
      return m_result;
    }

    concurrent_debug(m_log, "waiting for data");
    m_data = std::move(_provided.second);
    concurrent_debug(m_log, "data ", m_data, " arrived");

    concurrent_debug(m_log, "sending notification");
    m_cond_exec.notify_one();
    concurrent_debug(m_log, "notification sent");
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
      concurrent_debug(m_log, "waiting for permission to work");
      unique_lock<mutex> _lock(m_mutex_exec);
      m_cond_exec.wait(_lock);
      concurrent_debug(m_log, "permission arrived");

      if (m_stop) {
        concurrent_debug(m_log, "ordered to stop");
        break;
      }

      concurrent_debug(m_log, "provided ", m_data);
      concurrent_debug(m_log, "calling worker");
      m_result = m_worker(std::move(m_data));
      concurrent_debug(m_log, "worker returned ", m_result);
      if (m_stop) {
        concurrent_debug(m_log, "ordered to stop");
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
  bool m_stop = false;
  status::result m_result = status::ok;
  concurrent::thread m_thread;
  std::mutex m_mutex_exec;
  std::mutex m_mutex_time;
  std::condition_variable m_cond_exec;
  std::condition_variable m_cond_time;
  t_data m_data;
  t_log m_log{"concurrent::processor"};
};

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

  processor_t(worker p_worker, provider p_provider, t_time p_timeout)
      : m_worker(p_worker), m_provider(p_provider), m_timeout(p_timeout) {}

  processor_t(processor_t &&p_proc) = delete;

  processor_t &operator=(const processor_t &) = delete;
  processor_t &operator=(processor_t &&p_proc) = delete;

  inline bool is_running() const {
    return ((m_thread.get_id() != std::thread::id()) && (!m_stop));
  }

  status::result operator()() {

    if (!is_running()) {
      concurrent_debug(m_log, "starting thread, as it was not running");
      m_thread = concurrent::thread([this]() -> void { loop(); });
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      m_stop = false;
    }

    concurrent_debug(m_log, "sending notification");
    m_cond_exec.notify_one();
    concurrent_debug(m_log, "notification sent");
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
      concurrent_debug(m_log, "waiting for permission to work");
      unique_lock<mutex> _lock(m_mutex_exec);
      m_cond_exec.wait(_lock);
      concurrent_debug(m_log, "permission arrived");

      if (m_stop) {
        concurrent_debug(m_log, "ordered to stop");
        break;
      }

      m_result = m_worker();
      concurrent_debug(m_log, "worker returned ", m_result);
      if (m_stop) {
        concurrent_debug(m_log, "ordered to stop");
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
  bool m_stop = false;
  status::result m_result = status::ok;
  concurrent::thread m_thread;
  std::mutex m_mutex_exec;
  std::mutex m_mutex_time;
  std::condition_variable m_cond_exec;
  std::condition_variable m_cond_time;
  t_log m_log{"concurrent::processor"};
};
///// \brief process struct executes the core process for a \p loop object which
///// Work function receives data
/////
///// \tparam t_data is the type of data that the Work function receives
/////
///// \tparam t_timeout is the type of time to control the timeout for the Work
///// function execution
/////
// template <typename t_data, typename t_timeout, typename t_log> struct process
// {

//  status::result operator()(typename traits_t<t_data>::worker &p_work,
//                            typename traits_t<t_data>::provider &p_provide,
//                            t_timeout p_timeout) {
//    status::result _result = status::ok;
//    std::pair<status::result, t_data> _provided = p_provide();

//    if (_provided.first != status::ok) {
//      _result = _provided.first;
//    } else {
//      debug(m_log, "_provided.second = ", _provided.second, "
//      ",
//                           &(_provided.second));

//      auto _f = [&p_work](t_data &&p_data) {
//        debug(m_log, "p_data = ", p_data, " ", &p_data);
//        return p_work(std::move(p_data));
//      };

//      std::future<status::result> _future =
//          std::async(std::launch::async, _f, std::move(_provided.second));

//      std::future_status _future_status = _future.wait_for(p_timeout);
//      if (_future_status == std::future_status::timeout) {
//        _result = concurrent::stopped_by_timeout;
//      } else {
//        if (_future_status == std::future_status::ready) {
//          _result = _future.get();
//        }
//      }
//    }

//    return _result;
//  }
//};

///// \brief process struct executes the core process for a \p loop object which
///// Work function does not receive data
/////
///// \tparam t_timeout is the type of time to control the timeout for the Work
///// function execution
/////
// template <typename t_timeout, typename t_log>
// struct process<void, t_timeout, t_log> {
//  status::result operator()(typename traits_t<void>::worker &p_work,
//                            typename traits_t<void>::provider /*p_provide*/,
//                            t_timeout p_timeout) {
//    status::result _result = status::ok;
//    std::future<status::result> _future =
//        std::async(std::launch::async, [&p_work]() { return p_work(); });

//    std::future_status _future_status = _future.wait_for(p_timeout);
//    if (_future_status == std::future_status::timeout) {
//      _result = concurrent::stopped_by_timeout;
//    } else if (_future_status == std::future_status::ready) {
//      _result = _future.get();
//    }

//    return _result;
//  }
//};

} // namespace concurrent
} // namespace tenacitas

#endif // WORK_H
