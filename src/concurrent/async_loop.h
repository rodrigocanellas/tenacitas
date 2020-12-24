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

protected:
  async_loop_base_t(breaker p_breaker) : m_breaker(p_breaker) {}

  virtual void loop() = 0;

protected:
  breaker m_breaker;

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
        m_provider(p_provider, 500ms, [](std::thread::id) -> void {}) {}

protected:
  void loop() override {
    while (true) {

      std::optional<std::tuple<t_params...>> _maybe = m_provider();

      if (this->m_stopped) {
        break;
      }

      if (this->m_breaker()) {
        break;
      }

      if (_maybe) {
        std::tuple<t_params...> _params = std::move(*_maybe);

        std::apply(m_worker, _params);

        if (this->m_stopped) {
          break;
        }

        if (this->m_breaker()) {
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

protected:
  void loop() override {
    while (true) {

      m_worker();

      if (this->m_stopped) {
        break;
      }

      if (this->m_breaker()) {
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
