#ifndef TENACITAS_CONCURRENT_ASYNC_LOOP_H
#define TENACITAS_CONCURRENT_ASYNC_LOOP_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>

#include <concurrent/breaker.h>
#include <concurrent/internal/constants.h>
#include <concurrent/internal/log.h>
#include <concurrent/thread.h>
#include <concurrent/time_unit.h>
#include <concurrent/timeout_callback.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

template <typename t_log> struct async_loop_base_t {

  async_loop_base_t() = delete;
  async_loop_base_t(const async_loop_base_t &) = delete;
  async_loop_base_t(async_loop_base_t &&) = delete;
  async_loop_base_t &operator=(const async_loop_base_t &) = delete;
  async_loop_base_t &operator=(async_loop_base_t &&) = delete;

  virtual ~async_loop_base_t() { stop(); }

  void start() {
    if (!m_stopped) {
      return;
    }
    m_thread = {[this]() { loop(); }};
  }

  void stop() {
    if (m_stopped) {
      return;
    }
    m_stopped = true;
    m_thread.join();
  }

protected:
  template <typename t_time>
  async_loop_base_t(t_time p_timeout, timeout_callback p_timeout_callback,
                    breaker p_breaker)
      : m_timeout(to_timeout(p_timeout)),
        m_timeout_callback(p_timeout_callback), m_breaker(p_breaker) {}

  virtual void loop() = 0;

  void launch_timeout_callback(std::thread::id p_id) {
    m_thread_timeout_callback = {[this, p_id]() { m_timeout_callback(p_id); }};
    m_thread_timeout_callback.detach();
  }

protected:
  timeout m_timeout;

  timeout_callback m_timeout_callback;

  breaker m_breaker;

  bool m_stopped{true};

  concurrent::thread m_thread;

  concurrent::thread m_thread_timeout_callback;

  t_log m_log{"async_loop"};
};

/// #### 1 ####
template <typename t_log, typename... t_params>
struct async_loop_t : public async_loop_base_t<t_log> {

  typedef std::function<void(t_params...)> worker;
  typedef std::function<std::tuple<t_params...>()> provider;

  template <typename t_time>
  async_loop_t(t_time p_timeout, timeout_callback p_timeout_callback,
               breaker p_breaker, worker p_worker, provider p_provider)
      : async_loop_base_t<t_log>(p_timeout, p_timeout_callback, p_breaker),
        m_worker(p_worker), m_provider(p_provider) {}

protected:
  void loop() override {
    while (true) {

      std::tuple<t_params...> _params = m_provider();

      if (this->m_stopped) {
        break;
      }

      if (this->m_breaker()) {
        break;
      }

      std::packaged_task<void(t_params...)> _task{
          [this, &_params] { std::apply(m_worker, std::move(_params)); }};

      std::future<void> _future{_task.get_future()};

      if (this->m_stopped) {
        break;
      }

      if (this->m_breaker()) {
        break;
      }

      std::thread m_worker_thread{std::move(_task)};

      if (_future.wait(this->m_timeout) == std::future_status::timeout) {
        m_worker_thread.detach();
      } else {
        m_worker_thread.join();
      }
    }
  }

private:
  worker m_worker;
  provider m_provider;
};

/// #### 2 ####
template <typename t_log>
struct async_loop_t<t_log, void> : public async_loop_base_t<t_log> {

  typedef std::function<void()> worker;

  template <typename t_time>
  async_loop_t(t_time p_timeout, timeout_callback p_timeout_callback,
               breaker p_breaker, worker p_worker)
      : async_loop_base_t<t_log>(p_timeout, p_timeout_callback, p_breaker),
        m_worker(p_worker) {}

protected:
  void loop() override {
    while (true) {

      std::packaged_task<void()> _task{m_worker};

      std::future<void> _future{_task.get_future()};

      if (this->m_stopped) {
        break;
      }

      if (this->m_breaker()) {
        break;
      }

      std::thread m_worker_thread{std::move(_task)};

      if (_future.wait(this->m_timeout) == std::future_status::timeout) {
        m_worker_thread.detach();
      } else {
        m_worker_thread.join();
      }
    }
  }

private:
  worker m_worker;
};

} // namespace concurrent
} // namespace tenacitas

#endif
