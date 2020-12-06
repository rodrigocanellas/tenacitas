#ifndef TENACITAS_CONCURRENT_EXECUTER_H
#define TENACITAS_CONCURRENT_EXECUTER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>

#include <concurrent/internal/constants.h>
#include <concurrent/internal/log.h>
#include <concurrent/internal/worker_wrapper.h>
#include <concurrent/thread.h>
#include <concurrent/time_unit.h>
#include <concurrent/timeout_callback.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

enum class timeout_control : char { yes = 'y', no = 'n' };

/// \brief Executes a worker function asynchronously with creating a new thread
/// for each execution.
///
/// Sub classes will define if the worker with will have a timeout or not
///
/// \tparam t_log
///
/// \tparam t_result
///
/// \tparam t_params
template <typename t_log, typename t_result, typename... t_params>
struct executer_base_t {

  /// \brief type of worker to be executed asynchronously
  typedef std::function<t_result(t_params...)> worker;

  /// \brief destructor
  ///
  /// Stops the execution of the single created thread for the execution of the
  /// worker asynchronously
  virtual ~executer_base_t() {
    concurrent_debug(m_log, "entering destructor");
    stop();
    concurrent_debug(m_log, "leaving destructor");
  }

  /// \brief Stars the single thread that will execute the worker
  /// asynchronously
  void start() {
    if (!m_worker_wrapper) {
      throw std::runtime_error("worker not defined");
    }

    if (m_stopped) {
      concurrent_debug(m_log, "starting");
      m_stopped = false;
      m_thread = concurrent::thread([this]() -> void { loop(); });
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  }

  /// \brief Retrieves the worker
  inline worker get_worker() const { return m_worker_wrapper.get_worker(); }

  /// \brief Stops the execution of the single created thread for the execution
  /// of the worker asynchronously
  void stop() {
    if (!m_stopped) {
      concurrent_debug(m_log, "stopping");
      m_stopped = true;
      concurrent_debug(m_log, "m_stopped = true");
      m_cond_exec.notify_one();
      concurrent_debug(m_log, "m_cond_exec.notify_one()");
      m_cond_wait.notify_one();
      concurrent_debug(m_log, "m_cond_wait.notify_one()");
      m_thread.join();
      concurrent_debug(m_log, "m_thread.join()");
    } else {
      concurrent_debug(m_log, "not stopping because it was already stopped");
    }
  }

protected:
  virtual void loop() = 0;

protected:
  /// \brief Type to adapt the different possible return types and parameters of
  /// the \p worker
  typedef worker_wrapper_t<t_result, t_params...> worker_wrapper;

protected:
  /// \brief Wrapper that adapts to the different possible return types and
  /// parameters of the \p worker
  worker_wrapper m_worker_wrapper;

  /// \brief Logger object
  t_log m_log{"concurrent::executer"};

  /// \brief The loop initiates stopped
  bool m_stopped{true};

  /// \brief Controls the execution of the execution loop
  std::mutex m_mutex_worker;

  /// \brief Controls the execution of the execution loop
  std::condition_variable m_cond_exec;

  /// \brief Controls the waiting of the execution loop
  std::mutex m_mutex_wait;

  /// \brief Controls the waiting of the execution loop
  std::condition_variable m_cond_wait;

  /// \brief Single thread used to execute the \p worker over and over
  /// asynchronously
  concurrent::thread m_thread;

  /// \brief Controls the execution of the thread
  std::mutex m_mutex_exec;
};

template <typename t_log, timeout_control, typename t_result,
          typename... t_params>
struct executer_t;

/// \brief Executes a worker function asynchronously with creating a new thread
/// for each execution, with timeout control for the worker
///
/// \tparam t_log
///
/// \tparam t_result
///
/// \tparam t_params
///
template <typename t_log, typename t_result, typename... t_params>
struct executer_t<t_log, timeout_control::yes, t_result, t_params...>
    : public executer_base_t<t_log, t_result, t_params...> {

  /// \brief type of worker to be executed asynchronously
  typedef executer_base_t<t_log, t_result, t_params...> executer_base;
  typedef typename executer_base::worker worker;

  /// \brief constructor
  ///
  /// \tparam t_time is the type of time used to define the timeout for the
  /// worker to execute, before the calling code loses interest in the
  /// result
  ///
  /// \param p_timeout is the amount of time defined as timeout for the
  /// worker
  ///
  /// \param p_worker is the function to be executed asynchronously
  ///
  /// \param p_timeout_callback is the function to be executed if \p p_worker
  /// times out. \p p_timeout_callback is executed asynchronously with a timeout
  /// of 300 ms
  template <typename t_time>
  executer_t(
      t_time p_timeout, worker p_worker,
      timeout_callback p_timeout_callback = [](std::thread::id) -> void {})
      : m_timeout(to_timeout(p_timeout)), executer_base::m_worker_wrapper(
                                              p_worker),
        m_timeout_callback(p_timeout_callback) {
    this->start();
  }

  /// \brief Retrieves the amount of time defined for timeout
  inline constexpr timeout get_timeout() const { return m_timeout; }

  /// \brief Retrieves the function executed when the worker exceeds
  /// the defined timeout
  inline timeout_callback get_timeout_callback() { return m_timeout_callback; }

  /// \brief Executes the defined worker
  ///
  /// \param p_params are the defined parameters in the \p worker typedef
  /// signature
  ///
  /// \returns whatever was defined in the \p worker typedef signature
  auto operator()(t_params... p_params) {

    std::lock_guard<std::mutex> _lock_worker(this->m_mutex_worker);
    concurrent_debug(this->m_log, "operator()()");

    if (this->m_stopped) {
      concurrent_warn(this->m_log, "executer is stopped; call 'start()' first");
      return this->m_worker_wrapper.get_result_not_ok();
    }

    this->m_worker_wrapper.set_params(p_params...);

    m_is_timeout = false;

    concurrent_debug(this->m_log, "notifying there is work to do");
    this->m_cond_exec.notify_one();

    concurrent_debug(this->m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(this->m_mutex_wait);
    if (!this->m_cond_wait.wait_for(
            _lock, m_timeout, [this]() -> bool { return this->m_stopped; })) {
      concurrent_warn(this->m_log, "timeout: ", this->m_thread.get_id(),
                      " did not finish in time");

      timeout_callback_thread();

      m_is_timeout = true;
      return this->m_worker_wrapper.get_result_not_ok();
    }
    if (this->m_stopped) {
      concurrent_debug(this->m_log, "stopped");
      return this->m_worker_wrapper.get_result_not_ok();
    }

    concurrent_debug(this->m_log, "worker finished on time");
    return this->m_worker_wrapper.get_result_ok();
  }

  /// \brief Thread to execute the function that is called if the \p worker
  /// times out
  void timeout_callback_thread() {
    std::thread::id _id{std::this_thread::get_id()};
    m_timeout_thread =
        std::thread([this, _id]() -> void { m_timeout_callback(_id); });
    m_timeout_thread.detach();
  }

  /// \brief Loop executed asynchronously that waits for notification to execute
  /// the \p worker
  void loop() override {
    while (true) {

      concurrent_debug(this->m_log, "waiting for work");

      std::unique_lock<std::mutex> _lock(this->m_mutex_exec);
      this->m_cond_exec.wait(_lock);

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        this->m_cond_wait.notify_one();
        concurrent_debug(this->m_log, "about to break the loop");
        break;
      }

      concurrent_debug(this->m_log, "worker to execute!");

      this->m_worker_wrapper();

      if (!m_is_timeout) {

        if (this->m_stopped) {
          concurrent_debug(this->m_log, "stopped while in worker");
          this->m_cond_wait.notify_one();
          break;
        }

        concurrent_debug(this->m_log, "notifying that work is done");
        this->m_cond_wait.notify_one();
      } else {
        concurrent_debug(this->m_log, "timeout");
        break;
      }
    }
    concurrent_debug(this->m_log, "leaving the loop");
  }

private:
  /// \brief Timeout for the worker to complete its work
  timeout m_timeout;

  /// \brief Function that should be executed in case of timeout
  timeout_callback m_timeout_callback;

  /// \brief Nothing is in execution, so there is no timeout yet
  bool m_is_timeout{false};

  std::thread m_timeout_thread;
};

/// \brief Executes a worker function asynchronously with creating a new thread
/// for each execution, with no timeout control for the worker
///
/// \tparam t_log
///
/// \tparam t_result
///
/// \tparam t_params
///
template <typename t_log, typename t_result, typename... t_params>
struct executer_t<t_log, timeout_control::no, t_result, t_params...>
    : executer_base_t<t_log, t_result, t_params...> {

  /// \brief type of worker to be executed asynchronously
  typedef executer_base_t<t_log, t_result, t_params...> executer_base;
  typedef typename executer_base::worker worker;

  /// \brief constructor
  ///
  /// \param p_worker is the function to be executed asynchronously
  executer_t(worker p_worker) : executer_base::m_worker_wrapper(p_worker) {
    this->start();
  }

  /// \brief Executes the defined worker
  ///
  /// \param p_params are the defined parameters in the \p worker typedef
  /// signature
  ///
  /// \returns whatever was defined in the \p worker typedef signature
  auto operator()(t_params... p_params) {

    std::lock_guard<std::mutex> _lock_worker(this->m_mutex_worker);
    concurrent_debug(this->m_log, "operator()()");

    if (this->m_stopped) {
      concurrent_warn(this->m_log, "executer is stopped; call 'start()' first");
      return this->m_worker_wrapper.get_result_not_ok();
    }

    this->m_worker_wrapper.set_params(p_params...);

    concurrent_debug(this->m_log, "notifying there is work to be done");
    this->m_cond_exec.notify_one();

    concurrent_debug(this->m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(this->m_mutex_wait);
    this->m_cond_wait.wait(_lock, [this]() -> bool { return this->m_stopped; });
    concurrent_debug(this->m_log, "work was done or 'executer' was stopped");
    if (this->m_stopped) {
      concurrent_debug(this->m_log, "stopped");
      return this->m_worker_wrapper.get_result_not_ok();
    }

    concurrent_debug(this->m_log, "worker done");
    return this->m_worker_wrapper.get_result_ok();
  }

private:
  /// \brief Type to adapt the different possible return types and parameters of
  /// the \p worker
  typedef worker_wrapper_t<t_result, t_params...> worker_wrapper;

private:
  /// \brief Loop executed asynchronously that waits for notification to execute
  /// the \p worker
  void loop() override {
    while (true) {

      concurrent_debug(this->m_log, "waiting for work");

      std::unique_lock<std::mutex> _lock(this->m_mutex_exec);
      this->m_cond_exec.wait(_lock);

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        this->m_cond_wait.notify_one();
        concurrent_debug(this->m_log, "about to break the loop");
        break;
      }

      concurrent_debug(this->m_log, "worker to execute!");

      this->m_worker_wrapper();

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped while in worker");
        this->m_cond_wait.notify_one();
        break;
      }

      concurrent_debug(this->m_log, "notifying that work is done");
      this->m_cond_wait.notify_one();
    }

    concurrent_debug(this->m_log, "leaving the loop");
  }
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_EXECUTER_H
