#ifndef TENACITAS_CONCURRENT_LOOP_WRAPPER_H
#define TENACITAS_CONCURRENT_LOOP_WRAPPER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <condition_variable>
#include <functional>
#include <mutex>
#include <optional>
#include <thread>
#include <tuple>

#include <concurrent/runner.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

struct loop_base {

  inline void stop() { m_stopped = true; }

  virtual ~loop_base() { stop(); }

protected:
  bool m_stopped{true};
};

// ################ A ########################################################
template <typename t_log, typename... t_params>
struct loop_with_breaker_t : public loop_base {

  template <typename t_work_executer, typename t_provider_executer,
            typename t_breaker_executer>
  void operator()(t_work_executer &p_work_executer,
                  t_provider_executer &p_provider_executer,
                  t_breaker_executer &p_breaker_executer) {
    if (!m_stopped) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return;
    }

    concurrent_info(m_log, "starting");

    m_stopped = false;
    p_work_executer.start();
    p_provider_executer.start();
    p_breaker_executer.start();

    while (true) {

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      std::optional<std::optional<std::tuple<t_params...>>> _maybe_executed =
          p_provider_executer();

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      if (_maybe_executed) {
        concurrent_debug(m_log, "provider executed");
        std::optional<std::tuple<t_params...>> _maybe_provided =
            _maybe_executed.value();

        if (_maybe_provided) {
          auto _provided = _maybe_provided.value();
          concurrent_debug(m_log, "provider provided: ", _provided);

          std::apply(p_work_executer, _provided);
          if (m_stopped) {
            concurrent_debug(m_log, "stopped!!");
            break;
          }
        }
      }

      concurrent_debug(m_log, "about to call breaker");
      std::optional<bool> _maybe_break = p_breaker_executer();
      if ((_maybe_break) && (_maybe_break.value())) {
        concurrent_debug(m_log, "stopped by breaker");
        m_stopped = true;
        break;
      }
    }
    p_work_executer.stop();
    p_provider_executer.stop();
    p_breaker_executer.stop();
  }

private:
  t_log m_log{"loop_with_breaker"};
};

template <typename t_log, typename... t_params>
struct loop_with_no_breaker_t : public loop_base {

  template <typename t_work_executer, typename t_provider_executer,
            typename t_breaker_executer>
  void operator()(t_work_executer &p_work_executer,
                  t_provider_executer &p_provider_executer,
                  t_breaker_executer &) {
    if (!m_stopped) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return;
    }

    concurrent_info(m_log, "starting");

    m_stopped = false;
    p_work_executer.start();
    p_provider_executer.start();

    while (true) {

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      std::optional<std::optional<std::tuple<t_params...>>> _maybe_executed =
          p_provider_executer();

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      if (_maybe_executed) {
        concurrent_debug(m_log, "provider executed");
        std::optional<std::tuple<t_params...>> _maybe_provided =
            _maybe_executed.value();

        if (_maybe_provided) {
          auto _provided = _maybe_provided.value();
          concurrent_debug(m_log, "provider provided: ", _provided);

          std::apply(p_work_executer, _provided);
          if (m_stopped) {
            concurrent_debug(m_log, "stopped!!");
            break;
          }
        }
      }
    }
    p_work_executer.stop();
    p_provider_executer.stop();
  }

private:
  t_log m_log{"concurrent::loop_with_breaker"};
};

// ################ B ########################################################
template <typename t_log, typename t_param>
struct loop_with_breaker_t<t_log, t_param> : public loop_base {

  template <typename t_work_executer, typename t_provider_executer,
            typename t_breaker_executer>
  void operator()(t_work_executer &p_work_executer,
                  t_provider_executer &p_provider_executer,
                  t_breaker_executer &p_breaker_executer) {
    if (!m_stopped) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return;
    }

    concurrent_info(m_log, "starting");

    m_stopped = false;
    p_work_executer.start();
    p_provider_executer.start();
    p_breaker_executer.start();

    while (true) {

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      std::optional<std::optional<t_param>> _maybe_executed =
          p_provider_executer();

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      if (_maybe_executed) {
        concurrent_debug(m_log, "provider executed");
        std::optional<t_param> _maybe_provided = _maybe_executed.value();

        if (_maybe_provided) {
          auto _provided = _maybe_provided.value();
          concurrent_debug(m_log, "provider provided: ", _provided);

          p_work_executer(_provided);
          if (m_stopped) {
            concurrent_debug(m_log, "stopped!!");
            break;
          }
        }
      }

      concurrent_debug(m_log, "about to call breaker");
      std::optional<bool> _maybe_break = p_breaker_executer();
      if ((_maybe_break) && (_maybe_break.value())) {
        concurrent_debug(m_log, "stopped by breaker");
        m_stopped = true;
        break;
      }
    }
    p_work_executer.stop();
    p_provider_executer.stop();
    p_breaker_executer.stop();
  }

private:
  t_log m_log{"concurrent::loop_with_breaker"};
};

template <typename t_log, typename t_param>
struct loop_with_no_breaker_t<t_log, t_param> : public loop_base {

  template <typename t_work_executer, typename t_provider_executer,
            typename t_breaker_executer>
  void operator()(t_work_executer &p_work_executer,
                  t_provider_executer &p_provider_executer,
                  t_breaker_executer &) {
    if (!m_stopped) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return;
    }

    concurrent_info(m_log, "starting");

    m_stopped = false;
    p_work_executer.start();
    p_provider_executer.start();

    while (true) {

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      std::optional<std::optional<t_param>> _maybe_executed =
          p_provider_executer();

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      if (_maybe_executed) {
        concurrent_debug(m_log, "provider executed");
        std::optional<t_param> _maybe_provided = _maybe_executed.value();

        if (_maybe_provided) {
          auto _provided = _maybe_provided.value();
          concurrent_debug(m_log, "provider provided: ", _provided);

          p_work_executer(_provided);
          if (m_stopped) {
            concurrent_debug(m_log, "stopped!!");
            break;
          }
        }
      }
    }
    p_work_executer.stop();
    p_provider_executer.stop();
  }

private:
  t_log m_log{"concurrent::loop_with_breaker"};
};

// ################ C ########################################################
template <typename t_log> struct loop_with_breaker_t<t_log> : public loop_base {

  template <typename t_work_executer, typename t_provider_executer,
            typename t_breaker_executer>
  void operator()(t_work_executer &p_work_executer, t_provider_executer &,
                  t_breaker_executer &p_breaker_executer) {
    if (!m_stopped) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return;
    }

    concurrent_info(m_log, "starting");

    m_stopped = false;
    p_work_executer.start();
    p_breaker_executer.start();

    while (true) {

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      p_work_executer();
      if (m_stopped) {
        concurrent_debug(m_log, " ---- stopped!!");
        break;
      }

      concurrent_debug(m_log, "about to call breaker");
      std::optional<bool> _maybe_break = p_breaker_executer();
      if ((_maybe_break) && (_maybe_break.value())) {
        concurrent_debug(m_log, "stopped by breaker");
        m_stopped = true;
        break;
      }
    }
    p_work_executer.stop();
    p_breaker_executer.stop();
  }

private:
  t_log m_log{"concurrent::loop_with_breaker"};
};

template <typename t_log>
struct loop_with_no_breaker_t<t_log> : public loop_base {

  template <typename t_work_executer, typename t_provider_executer,
            typename t_breaker_executer>
  void operator()(t_work_executer &p_work_executer, t_provider_executer &,
                  t_breaker_executer &) {
    if (!m_stopped) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return;
    }

    concurrent_info(m_log, "starting");

    m_stopped = false;
    p_work_executer.start();

    while (true) {

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      p_work_executer();
      if (m_stopped) {
        concurrent_debug(m_log, "stopped!!");
        break;
      }
    }
    p_work_executer.stop();
  }

private:
  t_log m_log{"concurrent::loop_with_breaker"};
};

// ################ 1 ########################################################

template <typename t_log, typename t_time, typename... t_params>
struct loop_wrapper_t {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  typedef std::function<void(t_params...)> worker;

  /// \brief provider is the type of function that provides data to the work
  /// function
  ///
  /// \return \p an optional tuple of objects needed by the \p worker
  //  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

  struct dummy_provider {

    inline std::optional<std::tuple<>> operator()() { return {}; }
  };

  static constexpr bool tuple_not_empty{
      std::tuple_size<std::tuple<t_params...>>::value != 0};

  typedef typename std::conditional<
      tuple_not_empty, std::function<std::optional<std::tuple<t_params...>>()>,
      dummy_provider>::type provider;

  typedef std::function<bool()> breaker;

  /// \brief used to notify about timeout of \p worker
  typedef std::function<void(std::thread::id)> timeout_callback;

  inline loop_wrapper_t(t_time p_timeout, worker p_worker, breaker p_breaker,
                        provider p_provider,
                        timeout_callback p_timeout_callback)
      : m_work_executer(p_timeout, p_worker, p_timeout_callback),
        m_provider_executer(std::chrono::milliseconds(m_provider_timeout),
                            p_provider),
        m_has_breaker(true),
        m_breaker_executer(std::chrono::milliseconds(m_breaker_timeout),
                           p_breaker) {}

  inline loop_wrapper_t(t_time p_timeout, worker p_worker, breaker p_breaker,
                        provider p_provider)
      : m_work_executer(p_timeout, p_worker, [](std::thread::id) -> void {}),
        m_provider_executer(std::chrono::milliseconds(m_provider_timeout),
                            p_provider),
        m_has_breaker(true),
        m_breaker_executer(std::chrono::milliseconds(m_breaker_timeout),
                           p_breaker) {}

  inline loop_wrapper_t(t_time p_timeout, worker p_worker, provider p_provider,
                        timeout_callback p_timeout_callback)
      : m_work_executer(p_timeout, p_worker, p_timeout_callback),
        m_provider_executer(std::chrono::milliseconds(m_provider_timeout),
                            p_provider),
        m_has_breaker(false),
        m_breaker_executer(std::chrono::milliseconds(m_breaker_timeout),
                           []() -> bool { return false; }) {}

  inline loop_wrapper_t(t_time p_timeout, worker p_worker, provider p_provider)
      : m_work_executer(p_timeout, p_worker, [](std::thread::id) -> void {}),
        m_provider_executer(std::chrono::milliseconds(m_provider_timeout),
                            p_provider),
        m_has_breaker(false),
        m_breaker_executer(std::chrono::milliseconds(m_breaker_timeout),
                           []() -> bool { return false; }) {}
  // ****

  inline loop_wrapper_t(t_time p_timeout, worker p_worker, breaker p_breaker,
                        timeout_callback p_timeout_callback)
      : m_work_executer(p_timeout, p_worker, p_timeout_callback),
        m_provider_executer(std::chrono::milliseconds(1), dummy_provider()),
        m_has_breaker(true),
        m_breaker_executer(std::chrono::milliseconds(m_breaker_timeout),
                           p_breaker) {}

  inline loop_wrapper_t(t_time p_timeout, worker p_worker, breaker p_breaker)
      : m_work_executer(p_timeout, p_worker, [](std::thread::id) -> void {}),
        m_provider_executer(std::chrono::milliseconds(1), dummy_provider()),
        m_has_breaker(true),
        m_breaker_executer(std::chrono::milliseconds(m_breaker_timeout),
                           p_breaker) {}

  inline loop_wrapper_t(t_time p_timeout, worker p_worker,
                        timeout_callback p_timeout_callback)
      : m_work_executer(p_timeout, p_worker, p_timeout_callback),
        m_provider_executer(std::chrono::milliseconds(1), dummy_provider()),
        m_has_breaker(false),
        m_breaker_executer(std::chrono::milliseconds(m_breaker_timeout),
                           []() -> bool { return false; }) {}

  inline loop_wrapper_t(t_time p_timeout, worker p_worker)
      : m_work_executer(p_timeout, p_worker, [](std::thread::id) -> void {}),
        m_provider_executer(std::chrono::milliseconds(1), dummy_provider()),
        m_has_breaker(false),
        m_breaker_executer(std::chrono::milliseconds(m_breaker_timeout),
                           []() -> bool { return false; }) {}

  /// \brief retrieves the timeout for the Work function
  ///
  /// \return the timeout
  inline t_time get_timeout() const { return m_work_executer.get_timeout(); }

  /// \brief redefines the value of the timeout
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  inline void set_timeout(t_time p_timeout) {
    m_work_executer.set_timeout(p_timeout);
  }

  void start() {

    if (!m_stopped) {
      concurrent_info(m_log, this,
                      "not starting the loop because it is already running");
      return;
    }
    concurrent_debug(m_log, "starting the loop");

    if (m_has_breaker) {
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_thread = thread([this]() -> void {
        m_loop_with_breaker(m_work_executer, m_provider_executer,
                            m_breaker_executer);
      });
    } else {
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_thread = thread([this]() -> void {
        m_loop_with_no_breaker(m_work_executer, m_provider_executer,
                               m_breaker_executer);
      });
    }
    m_stopped = false;
  }

  void stop() {
    if (m_stopped) {
      concurrent_warn(m_log,
                      "not stopping the loop because it was not running");
      return;
    }

    concurrent_debug(m_log, "marking to stop");
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_loop_with_breaker.stop();
    m_loop_with_no_breaker.stop();
    m_thread.join();
    m_stopped = true;
  }

  inline bool is_stopped() const { return m_stopped; }

private:
  struct provider_executer_tuple_empty {
    provider_executer_tuple_empty(std::chrono::milliseconds, dummy_provider) {}

    inline std::optional<std::optional<std::tuple<>>> operator()() {
      return {std::make_tuple<>()};
    }
  };

  typedef runner_t<t_log, std::chrono::milliseconds,
                   std::optional<std::tuple<t_params...>>>
      provider_executer_tuple_not_empty;

  typedef typename std::conditional<
      tuple_not_empty, provider_executer_tuple_not_empty,
      provider_executer_tuple_empty>::type provider_executer;

  typedef runner_t<t_log, t_time, void, t_params...> work_executer;

  typedef runner_t<t_log, std::chrono::milliseconds, bool> breaker_executer;

private:
  work_executer m_work_executer;

  provider_executer m_provider_executer;

  bool m_has_breaker{false};

  breaker_executer m_breaker_executer;

  bool m_stopped{true};

  /// \brief m_thread is the thread where the \p loop will run
  concurrent::thread m_thread;

  /// \brief m_mutex protects the start of the \p m_loop execution \p m_thread
  std::mutex m_mutex;

  t_log m_log{"concurrent::loop_wrapper"};

  loop_with_breaker_t<t_log, t_params...> m_loop_with_breaker;
  loop_with_no_breaker_t<t_log, t_params...> m_loop_with_no_breaker;

  static constexpr std::chrono::milliseconds m_provider_timeout{300};
  static constexpr std::chrono::milliseconds m_breaker_timeout{300};
};

} // namespace concurrent
} // namespace tenacitas

#endif // HELPER_H
