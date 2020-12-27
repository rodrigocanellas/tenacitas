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
#include <concurrent/executer.h>
#include <concurrent/internal/constants.h>
#include <concurrent/internal/log.h>
#include <concurrent/thread.h>
#include <concurrent/time_unit.h>
#include <concurrent/timeout_callback.h>

using namespace std::chrono_literals;

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

template <typename t_log> struct async_loop_base_t {

  async_loop_base_t(const async_loop_base_t &) = delete;
  async_loop_base_t(async_loop_base_t &&) = delete;
  async_loop_base_t &operator=(const async_loop_base_t &) = delete;
  async_loop_base_t &operator=(async_loop_base_t &&) = delete;

  virtual ~async_loop_base_t() { stop(); }

  void start() {
    if (!m_stopped) {
      return;
    }
    m_stopped = false;
    m_thread = concurrent::thread([this]() { loop(); });
  }

  void stop() {
    if (m_stopped) {
      return;
    }
    m_stopped = true;
    m_thread.join();
  }

  inline void set_log_debug_level() { m_log.set_debug_level(); }
  inline void set_log_info_level() { m_log.set_info_level(); }
  inline void set_log_warn_level() { m_log.set_warn_level(); }

protected:
  async_loop_base_t(breaker p_breaker = []() -> bool { return false; })
      : m_breaker(p_breaker, 500ms, [this](std::thread::id p_id) -> void {
          concurrent_warn(m_log, "thread ", p_id, " for breaker has timed out");
        }) {}

  virtual void loop() = 0;

protected:
  executer_t<t_log, bool, void> m_breaker;

  bool m_stopped{true};

  concurrent::thread m_thread;

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
      : async_loop_base_t<t_log>(p_breaker),
        m_worker(p_worker, p_timeout, p_timeout_callback),
        m_provider(p_provider, 500ms, [this](std::thread::id p_id) -> void {
          concurrent_warn(this->m_log, "thread ", p_id,
                          " for provider has timed out");
        }) {}

  template <typename t_time>
  async_loop_t(t_time p_timeout, timeout_callback p_timeout_callback,
               worker p_worker, provider p_provider)
      : async_loop_base_t<t_log>(),
        m_worker(p_worker, p_timeout, p_timeout_callback),
        m_provider(p_provider, 500ms, [this](std::thread::id p_id) -> void {
          concurrent_warn(this->m_log, "thread ", p_id,
                          " for provider has timed out");
        }) {}

protected:
  void loop() override {
    while (true) {
      std::optional<std::tuple<t_params...>> _maybe_data = m_provider();

      if (this->m_stopped) {
        break;
      }

      std::optional<bool> _maybe_break = this->m_breaker();
      if ((_maybe_break) && (*_maybe_break)) {
        break;
      }

      if (_maybe_data) {
        std::tuple<t_params...> _params = std::move(*_maybe_data);

        std::apply(m_worker, _params);

        if (this->m_stopped) {
          break;
        }

        std::optional<bool> _maybe_break = this->m_breaker();
        if ((_maybe_break) && (*_maybe_break)) {
          break;
        }
      }
    }
  }

private:
  typedef executer_t<t_log, void, t_params...> worker_executer;
  typedef executer_t<t_log, std::tuple<t_params...>, void> provider_executer;

private:
  worker_executer m_worker;
  provider_executer m_provider;
};

/// #### 2 ####
template <typename t_log>
struct async_loop_t<t_log, void> : public async_loop_base_t<t_log> {

  typedef std::function<void()> worker;

  template <typename t_time>
  async_loop_t(t_time p_timeout, timeout_callback p_timeout_callback,
               breaker p_breaker, worker p_worker)
      : async_loop_base_t<t_log>(p_breaker),
        m_worker(p_worker, p_timeout, p_timeout_callback) {}

  template <typename t_time>
  async_loop_t(t_time p_timeout, timeout_callback p_timeout_callback,
               worker p_worker)
      : async_loop_base_t<t_log>(),
        m_worker(p_worker, p_timeout, p_timeout_callback) {}

protected:
  void loop() override {
    while (true) {

      m_worker();

      if (this->m_stopped) {
        break;
      }

      std::optional<bool> _maybe_break = this->m_breaker();
      if ((_maybe_break) && (*_maybe_break)) {
        break;
      }
    }
  }

private:
  typedef executer_t<t_log, void, void> worker_executer;

private:
  worker_executer m_worker;
};

} // namespace concurrent
} // namespace tenacitas

#endif
