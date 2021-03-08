#ifndef TENACITAS_CONCURRENT_H
#define TENACITAS_CONCURRENT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <list>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

#include <tenacitas/calendar.h>
#include <tenacitas/logger.h>
#include <tenacitas/macros.h>

using namespace std::chrono_literals;

/// TODO improve documentation

/// \brief master namespace
namespace tenacitas {

/// \brief support for concurrent (asynchronous) programming
namespace concurrent {

/// \brief Type of time used to define timeout
typedef std::chrono::milliseconds timeout;

/// \brief Type of time used to define interval
typedef std::chrono::milliseconds interval;

namespace internal {

/// \brief Converts any type of time defined in std::chrono to \p timeout
template <typename t_time> inline timeout to_timeout(t_time p_time) {
  return std::chrono::duration_cast<timeout>(p_time);
}

/// \brief Converts any type of time defined in std::chrono to \p interval
template <typename t_time> inline interval to_interval(t_time p_time) {
  return std::chrono::duration_cast<interval>(p_time);
}

template <typename t_type> struct executer_helper_result_t {
  /// \brief returned by the executed function
  typedef t_type type;

  /// \brief result of the time controlled execution of the function
  typedef std::optional<type> result;

  static std::optional<t_type> ok(std::future<t_type> &&p_future) {
    return p_future.get();
  }
  static std::optional<t_type> not_ok(std::future<t_type> &&) { return {}; }
};

template <> struct executer_helper_result_t<void> {
  /// \brief returned by the executed function
  typedef void type;

  /// \brief result of the time controlled execution of the function
  typedef bool result;

  static bool ok(std::future<void> &&) { return true; }
  static bool not_ok(std::future<void> &&) { return false; }
};

/// \brief helper class for \p execute function
///
/// \tparam t_type type returned by the executed function
///
/// \tparam t_time type of time used to control timeout of the function being
/// executed
///
/// \tparam t_params... possible types of the arguments of the function being
/// executed
template <typename t_type, typename t_time, typename... t_params>
struct executer_helper_t {
  typedef executer_helper_result_t<t_type> executer_helper_result;
  typedef typename executer_helper_result::type type;
  typedef typename executer_helper_result::result result;

  /// \brief executes a function, controlling its timeout
  ///
  /// \param p_function function to be executed
  ///
  /// \param p_timeout maximum amount of time for \p p_function to execute
  ///
  /// \param p_params... possible parameters required by \p p_function
  ///
  /// \return std::optional<t_type> with value if not timeout, {} otherwise
  static result call(std::function<t_type(t_params...)> p_work,
                     t_time p_timeout, t_params... p_params) {

    std::packaged_task<type(t_params...)> _task(
        [p_work](t_params... p_params) -> t_type {
          return p_work(std::forward<t_params>(p_params)...);
        });
    auto _future = _task.get_future();
    std::thread _thr(std::move(_task), std::forward<t_params>(p_params)...);
    if (_future.wait_for(p_timeout) != std::future_status::timeout) {
      _thr.join();
      return executer_helper_result::ok(std::move(_future));
    }
    _thr.detach();

    return executer_helper_result::not_ok(std::move(_future));
  }
};

} // namespace internal

/// \brief executes a callable object in a maximum amount of time
///
/// \tparam t_time type of time used to control timeout of the function being
/// executed
///
/// \tparam t_function type of callable object
///
/// \tparam t_params possible types of the arguments of the function being
/// executed
///
/// \param p_timeout maximum amount of time for the function to execute
///
/// \param p_function callable object
///
/// \param p_params... possible parameters required by \p p_function
///
/// \return if the callable object returns void, \p execute returns true if no
/// timeout; false otherwise
///         if the callable object does not return void, \p execute returns
///         std::optional<type-returned> with value if not timeout, {}
///         otherwise
///
/// \code
///
///#include <thread>
///#include <optional>
///#include <type_traits>
///#include <chrono>
///#include <iostream>
///
///#include <tenacitas/concurrent.h>
///
/// using namespace std::chrono_literals;
///
/// double f1(int i) {
///  std::cout << "f2: ";
///  return i / 4;
///}
///
/// double f2(int i, float f) {
///  std::cout << "f2: ";
///  return f * i / 4;
///}
///
/// double f3() {
///  std::cout << "f3: ";
///  return 3.14;
///}
///
/// void f4(int i) { std::cout << "f4: " << i / 4 << " "; }
///
/// void f5(int i, float f) { std::cout << "f5: " << f * i / 4 << " "; }
///
/// void f6() { std::cout << "f6: " << 3.14 << " "; }
///
/// double f7(int i) {
///  std::cout << "f7: ";
///  std::this_thread::sleep_for(2s);
///
///  return i / 4;
///}
///
/// double f8(int i, float f) {
///
///  std::cout << "f8: ";
///  std::this_thread::sleep_for(2s);
///  return f * i / 4;
///}
///
/// double f9() {
///  std::cout << "f9: ";
///  std::this_thread::sleep_for(2s);
///
///  return 3.14;
///}
///
/// void f10(int i) {
///  std::cout << "f10: ";
///  std::this_thread::sleep_for(2s);
///  std::cout << i / 4 << " ";
///}
///
/// void f11(int i, float f) {
///  std::cout << "f11: ";
///  std::this_thread::sleep_for(2s);
///  std::cout << f * i / 4 << " ";
///}
///
/// void f12() {
///  std::cout << "f6: ";
///  std::this_thread::sleep_for(2s);
///  std::cout << 3.14 << " ";
///}
///
/// void f13(bool &b) { std::cout << "f13: " << (b ? "true" : "false") << " "; }
///
/// void f14(bool &&b) {
///  std::cout << "f14: ";
///  std::this_thread::sleep_for(2s);
///  std::cout << (b ? "true" : "false") << " ";
///}
///
/// void f15(bool &b) {
///  std::cout << "f15: ";
///  std::this_thread::sleep_for(2s);
///  std::cout << (b ? "true" : "false") << " ";
///  b = !b;
///}
///
/// ##########################
/// ########################## main
///
/// int main() {
///  using namespace tenacitas;
///
///  {
///    std::optional<double> _maybe = concurrent::execute(300ms, f1, 8);
///    if (_maybe) {
///      std::cout << *_maybe;
///    } else {
///      std::cout << " timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    std::optional<double> _maybe = concurrent::execute(300ms, f2, 8, 4.2f);
///    if (_maybe) {
///      std::cout << *_maybe;
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    std::optional<double> _maybe = concurrent::execute(300ms, f3);
///    if (_maybe) {
///      std::cout << *_maybe;
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    if (concurrent::execute(300ms, f4, -8)) {
///      std::cout << "ok";
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    if (concurrent::execute(300ms, f5, -8, 3.56)) {
///      std::cout << "ok";
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    if (concurrent::execute(300ms, f6)) {
///      std::cout << "ok";
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    std::optional<double> _maybe = concurrent::execute(300ms, f7, 8);
///    if (_maybe) {
///      std::cout << "with exec: " << *_maybe;
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    std::optional<double> _maybe = concurrent::execute(300ms, f8, 8, 4.2f);
///    if (_maybe) {
///      std::cout << *_maybe;
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    std::optional<double> _maybe = concurrent::execute(300ms, f9);
///    if (_maybe) {
///      std::cout << *_maybe;
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    if (concurrent::execute(300ms, f10, -18)) {
///      std::cout << "ok";
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    if (concurrent::execute(300ms, f11, -18, 3.3)) {
///      std::cout << "ok";
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    if (concurrent::execute(300ms, f12)) {
///      std::cout << "ok";
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    bool b{false};
///    if (concurrent::execute(300ms, f13, std::ref(b))) {
///      std::cout << "ok";
///    } else {
///      std::cout << "timeout";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    bool b{false};
///    if (concurrent::execute(300ms, f14, std::move(b))) {
///      std::cout << "ok";
///    } else {
///      std::cout << "timeout";
///    }
///    std::cout << std::endl;
///  }
///  {
///    bool b{false};
///    if (concurrent::execute(300ms, f15, std::ref(b))) {
///      std::cout << "ok";
///    } else {
///      std::cout << "timeout";
///    }
///    std::cout << std::endl;
///  }
///}
///
/// \endcode
template <typename t_time, typename t_function, typename... t_params>
inline typename internal::executer_helper_t<
    std::invoke_result_t<t_function, t_params...>, t_time, t_params...>::result
execute(t_time p_timeout, t_function p_function, t_params... p_params) {

  typedef std::invoke_result_t<t_function, t_params...> type;

  typedef internal::executer_helper_t<type, t_time, t_params...>
      executer_helper;

  return executer_helper::call(p_function, p_timeout,
                               std::forward<t_params>(p_params)...);
}

/// \brief Type of function used to inform if a loop should stop
typedef std::function<bool()> breaker;

uint64_t uuid() { return calendar::now<>::microsecs_num(); }

typedef std::function<bool()> breaker;

namespace internal {

/// \brief base class for asynchronous loop
///
/// \tparam t_time
///
/// \tparam t_worker
template <typename t_on_timeout, typename t_worker> struct async_loop_base_t {

  /// \brief
  async_loop_base_t() = delete;
  /// \brief
  async_loop_base_t(const async_loop_base_t &) = delete;
  /// \brief
  async_loop_base_t(async_loop_base_t &&) = delete;
  /// \brief
  async_loop_base_t &operator=(const async_loop_base_t &) = delete;
  /// \brief
  async_loop_base_t &operator=(async_loop_base_t &&) = delete;

  /// \brief
  virtual ~async_loop_base_t() {
    DEB(m_log, "destructor");
    stop();
  }

  /// \brief
  void start() {
    if (!m_stopped) {
      return;
    }
    m_stopped = false;
    m_thread = std::thread([this]() { loop(); });
  }

  /// \brief
  void stop() {
    if (m_stopped) {
      return;
    }
    m_stopped = true;
    if (m_thread.joinable()) {
      m_thread.join();
    }
  }

  /// \brief
  inline bool is_stopped() const { return m_stopped; }

  /// \brief
  template <typename t_time> inline void set_timeout(t_time p_timeout) {
    m_timeout = to_timeout(p_timeout);
  }

  /// \brief
  inline timeout get_timeout() const { return m_timeout; }

protected:
  /// \brief
  template <typename t_time>
  async_loop_base_t(t_worker p_worker, t_time p_time, t_on_timeout p_on_timeout)
      : m_worker(p_worker), m_timeout(to_timeout(p_time)),
        m_on_timeout(p_on_timeout), m_breaker([]() -> bool { return false; }) {}

  /// \brief
  template <typename t_time>
  async_loop_base_t(t_worker p_worker, t_time p_time, t_on_timeout p_on_timeout,
                    breaker p_breaker)
      : m_worker(p_worker), m_timeout(to_timeout(p_time)),
        m_on_timeout(p_on_timeout), m_breaker(p_breaker) {}

  /// \brief
  virtual void loop() = 0;

protected:
  /// \brief
  t_worker m_worker;

  /// \brief
  timeout m_timeout;

  /// \brief
  t_on_timeout m_on_timeout;

  /// \brief
  breaker m_breaker;

  /// \brief
  bool m_stopped{true};

  /// \brief
  std::thread m_thread;

  /// \brief
  logger::cerr<> m_log{"async_loop"};
};

} // namespace internal

/// \brief Base class for asynchronous loop
template <typename... t_params> struct async_loop_t;

/// #### async_loop 1 ####
template <typename... t_params> struct async_loop_t {

  /// \brief
  typedef std::function<void(t_params...)> on_timeout;

  /// \brief
  typedef std::function<void(t_params...)> worker;

  /// \brief
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

  /// \brief
  template <typename t_time>
  async_loop_t(worker p_worker, t_time p_timeout, on_timeout p_on_timeout,
               provider p_provider)
      : m_impl(std::make_unique<implementation>(p_worker, p_timeout,
                                                p_on_timeout, p_provider)) {}

  /// \brief
  template <typename t_time>
  async_loop_t(worker p_worker, t_time p_timeout, on_timeout p_on_timeout,
               provider p_provider, breaker p_breaker)
      : m_impl(std::make_unique<implementation>(
            p_worker, p_timeout, p_on_timeout, p_provider, p_breaker)) {}

  ~async_loop_t() = default;

  /// \brief
  async_loop_t(const async_loop_t &) = delete;

  /// \brief
  async_loop_t(async_loop_t &&p_async_loop) = default;

  /// \brief
  async_loop_t &operator=(async_loop_t &&p_async_loop) = default;

  /// \brief
  async_loop_t &operator=(const async_loop_t &) = delete;

  /// \brief
  inline void start() { m_impl->start(); }

  /// \brief
  inline void stop() {
    if (m_impl) {
      m_impl->stop();
    }
  }

  /// \brief
  inline bool is_stopped() const { return m_impl->is_stopped(); }

  /// \brief
  template <typename t_time> inline void set_timeout(t_time p_timeout) {
    m_impl->set_timeout(p_timeout);
  }

  /// \brief
  inline timeout get_timeout() const { return m_impl->get_timeout(); }

  /// \brief
  inline operator bool() const { return m_impl != nullptr; }

private:
  /// \brief
  struct implementation
      : public internal::async_loop_base_t<std::function<void(t_params...)>,
                                           std::function<void(t_params...)>> {
    /// \brief
    template <typename t_time>
    implementation(worker p_worker, t_time p_timeout, on_timeout p_on_timeout,
                   provider p_provider)
        : internal::async_loop_base_t<on_timeout, worker>(p_worker, p_timeout,
                                                          p_on_timeout),
          m_provider(p_provider) {}

    /// \brief
    template <typename t_time>
    implementation(worker p_worker, t_time p_timeout, on_timeout p_on_timeout,
                   provider p_provider, breaker p_breaker)
        : internal::async_loop_base_t<on_timeout, worker>(
              p_worker, p_timeout, p_on_timeout, p_breaker),
          m_provider(p_provider) {}

  protected:
    /// \brief
    void loop() override {
      DEB(this->m_log, "entering loop");

      while (true) {

        if ((this->m_breaker()) || (this->m_stopped)) {
          DEB(this->m_log, "stopped? ", this->m_stopped);
          break;
        }

        std::optional<std::tuple<t_params...>> _maybe_data = m_provider();

        if (_maybe_data) {
          std::tuple<t_params...> _params = std::move(*_maybe_data);

          DEB(this->m_log, "data provided = ", _params);

          auto _worker = [this, &_params]() -> void {
            std::apply(this->m_worker, _params);
          };

          if (!execute(this->m_timeout, _worker)) {
            WAR(this->m_log, "timeout for worker with data = ", _params);

            auto _on_timeout = [this, &_params]() {
              std::apply(this->m_on_timeout, _params);
            };

            std::thread([_on_timeout]() -> void { _on_timeout(); }).detach();
          }
        }
      }
    }

  private:
    /// \brief
    provider m_provider;
  };

private:
  std::unique_ptr<implementation> m_impl;
};

/// #### async_loop 2 ####
template <> struct async_loop_t<void> {

  /// \brief
  typedef std::function<void()> on_timeout;
  /// \brief
  typedef std::function<void()> worker;

  /// \brief
  template <typename t_time>
  async_loop_t(worker p_worker, t_time p_timeout, on_timeout p_on_timeout)
      : m_impl(std::make_unique<implementation>(p_worker, p_timeout,
                                                p_on_timeout)) {}

  /// \brief
  template <typename t_time>
  async_loop_t(worker p_worker, t_time p_timeout, on_timeout p_on_timeout,
               breaker p_breaker)
      : m_impl(std::make_unique<implementation>(p_worker, p_timeout,
                                                p_on_timeout, p_breaker)) {}

  ~async_loop_t() = default;

  /// \brief
  async_loop_t(const async_loop_t &) = delete;

  /// \brief
  async_loop_t(async_loop_t &&) = default;

  /// \brief
  async_loop_t &operator=(async_loop_t &&) = default;

  /// \brief
  async_loop_t &operator=(const async_loop_t &) = delete;

  /// \brief
  inline void start() { m_impl->start(); }

  /// \brief
  inline void stop() {
    if (m_impl) {
      m_impl->stop();
    }
  }
  /// \brief
  inline bool is_stopped() const { return m_impl->is_stopped(); }

  /// \brief
  template <typename t_time> inline void set_timeout(t_time p_timeout) {
    m_impl->set_timeout(p_timeout);
  }

  /// \brief
  inline timeout get_timeout() const { return m_impl->get_timeout(); }
  /// \brief
  inline operator bool() const { return m_impl != nullptr; }

private:
  /// \brief
  struct implementation
      : public internal::async_loop_base_t<std::function<void()>,
                                           std::function<void()>> {
    /// \brief
    template <typename t_time>
    implementation(worker p_worker, t_time p_timeout, on_timeout p_on_timeout)
        : async_loop_base_t<on_timeout, worker>(p_worker, p_timeout,
                                                p_on_timeout) {}

    /// \brief
    template <typename t_time>
    implementation(worker p_worker, t_time p_timeout, on_timeout p_on_timeout,
                   breaker p_breaker)
        : async_loop_base_t<on_timeout, worker>(p_worker, p_timeout,
                                                p_on_timeout, p_breaker) {}

  protected:
    /// \brief
    void loop() override {
      DEB(m_log, "entering loop");

      auto _worker = [this]() -> void { this->m_worker(); };

      while (true) {

        if ((this->m_breaker()) || (this->m_stopped)) {
          DEB(this->m_log, "stopped? ", this->m_stopped);
          break;
        }

        if (!execute(this->m_timeout, _worker)) {
          std::thread([this]() -> void { this->m_on_timeout(); }).detach();
        }
      }
      DEB(m_log, "leaving loop");
    }
  };

private:
  /// \brief
  std::unique_ptr<implementation> m_impl;
};

namespace internal {

/// \brief Executes a work function in fixed period of times
template <typename t_on_timeout, typename... t_params>
struct sleeping_loop_base_t {

  sleeping_loop_base_t() = delete;
  sleeping_loop_base_t(const sleeping_loop_base_t &) = delete;
  sleeping_loop_base_t(sleeping_loop_base_t &&p_sleep) = delete;
  sleeping_loop_base_t &operator=(const sleeping_loop_base_t &) = delete;
  sleeping_loop_base_t &operator=(sleeping_loop_base_t &&p_sleep) = delete;

  /// \brief destructor interrupts the loop
  inline virtual ~sleeping_loop_base_t() {
    DEB(m_log, "destructor");
    stop();
  }

  /// \brief retrieves the interval defined for the loop to sleep between
  /// the execution of the worker
  inline interval get_interval() const { return m_interval; }

  /// \brief redefines the value of the execution interval
  ///
  /// \tparam t_interval is the type of time used to define the interval
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  template <typename t_interval>
  inline void set_interval(t_interval p_interval) {
    m_interval = internal::to_interval(p_interval);
  }

  /// \brief Stops the loop, and starts it again
  void restart() {
    DEB(m_log, "restart");
    stop();
    start();
  }

  /// \brief Starts the loop
  void start() {
    if (!m_async.is_stopped()) {
      DEB(m_log, "not starting async loop because it is running");
      return;
    }
    if (!m_stopped) {
      DEB(m_log, "not starting because it is running");
    }
    DEB(m_log, "running async loop");
    m_stopped = false;
    m_async.start();
  }

  /// \brief Stops the loop
  void stop() {
    if (m_stopped) {
      DEB(m_log, "not stopping because it is stopped");
      return;
    }

    DEB(m_log, "stopping");
    m_stopped = true;

    m_async.stop();

    DEB(m_log, "notifying");
    m_cond.notify_one();

    DEB(m_log, "stopping");
  }

  /// \brief retrieves if the loop is stopped
  inline bool is_stopped() const { return m_async.is_stopped(); }

  /// \brief retrieves the timeout for the worker
  inline timeout get_timeout() const { return m_async.get_timeout(); }

  /// \brief redefines the value of the timeout
  ///
  /// \tparam t_timeout is the type of time used to define timeout for the
  /// worker function
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  template <typename t_timeout> inline void set_timeout(t_timeout p_timeout) {
    m_async.set_timeout(p_timeout);
  }

protected:
  template <typename t_timeout, typename t_interval, typename t_worker>
  sleeping_loop_base_t(t_interval p_interval, t_worker p_worker,
                       t_timeout p_timeout, t_on_timeout p_on_timeout)
      : m_interval(internal::to_interval(p_interval)),
        m_async(p_worker, internal::to_timeout(p_timeout), p_on_timeout,
                [this]() -> bool { return breaker(); }) {
    DEB(m_log, "constructor without provider called");
  }

  template <typename t_timeout, typename t_interval, typename t_worker,
            typename t_provider>
  sleeping_loop_base_t(t_interval p_interval, t_worker p_worker,
                       t_timeout p_timeout, t_on_timeout p_on_timeout,
                       t_provider p_provider)
      : m_interval(internal::to_interval(p_interval)),
        m_async(p_worker, internal::to_timeout(p_timeout), p_on_timeout,
                p_provider, [this]() -> bool { return breaker(); }) {
    DEB(m_log, "constructor with provider called");
  }

  template <typename t_timeout, typename t_interval, typename t_worker>
  sleeping_loop_base_t(t_interval p_interval, t_worker p_worker,
                       t_timeout p_timeout)
      : m_interval(internal::to_interval(p_interval)),
        m_async(
            p_worker, internal::to_timeout(p_timeout), []() {},
            [this]() -> bool { return breaker(); }) {
    DEB(m_log, "constructor without provider called");
  }

  template <typename t_timeout, typename t_interval, typename t_worker,
            typename t_provider>
  sleeping_loop_base_t(t_interval p_interval, t_worker p_worker,
                       t_timeout p_timeout, t_provider p_provider)
      : m_interval(internal::to_interval(p_interval)),
        m_async(
            p_worker, internal::to_timeout(p_timeout), []() {}, p_provider,
            [this]() -> bool { return breaker(); }) {
    DEB(m_log, "constructor with provider called");
  }

  /// \brief breaker defines if the loop should stop
  ///
  /// \return \p true if the loop should break; \p false othewise
  bool breaker() {

    // The predicate version returns pred(), regardless of whether the
    // timeout was triggered (although it can only be false if triggered).

    std::unique_lock<std::mutex> _lock(m_mutex);
    if (!m_cond.wait_for(_lock, m_interval,
                         [this]() -> bool { return m_stopped; })) {
      // timeout, so do not stop
      DEB(m_log, "must not stop");
      return false;
    }
    // no timeout, which means the loop was stopped
    DEB(m_log, "must stop");

    return true;
  }

protected:
  typedef async_loop_t<t_params...> async_loop;

protected:
  interval m_interval;

  async_loop m_async;

  /// \brief controls asynchronous execution
  std::mutex m_mutex;

  /// \brief controls asynchronous execution
  std::condition_variable m_cond;

  bool m_stopped{true};

  logger::cerr<> m_log{"sleeping_loop"};
};

} // namespace internal

/// \brief Base class for sleeping loops, which are loops that sleep during
/// a certain amount of time, then wake up and execute some work
///
/// \tparam t_params are the parameters that the work function
template <typename... t_params> struct sleeping_loop_t {

  /// \brief type of work executed in a loop in time intervals
  typedef std::function<void(t_params...)> worker;

  typedef std::function<void(t_params...)> on_timeout;

  /// \brief Provider is the type of function that provides data to the
  /// work function
  ///
  /// \return \p an optional tuple of objects needed by the \p worker
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

  /// \brief Constructor
  ///
  /// \tparam t_timeout type of time used to define the timeout of the
  /// worker
  ///
  /// \tparam t_interval type of time used to define the execution interval
  /// of the worker
  ///
  /// \param p_timeout time used to define the timeout of the worker
  ///
  /// \param p_interval time used to define the execution interval of the
  /// worker
  ///
  /// \param p_worker the work function to be executed at \p p_interval
  ///
  /// \param p_on_timeout function to be executed if \p p_worker times out
  ///
  /// \param p_provider function that provides the parameters to the \p
  /// p_worker

  template <typename t_timeout, typename t_interval>
  sleeping_loop_t(t_timeout p_timeout, t_interval p_interval, worker p_worker,
                  on_timeout p_on_timeout, provider p_provider)
      : m_impl(std::make_unique<implementation>(p_timeout, p_interval, p_worker,
                                                p_on_timeout, p_provider)) {}

  template <typename t_timeout, typename t_interval>
  sleeping_loop_t(t_timeout p_timeout, t_interval p_interval, worker p_worker,
                  provider p_provider)
      : m_impl(std::make_unique<implementation>(p_timeout, p_interval, p_worker,
                                                p_provider)) {}

  sleeping_loop_t() = delete;
  ~sleeping_loop_t() = default;

  sleeping_loop_t(const sleeping_loop_t &) = delete;
  sleeping_loop_t(sleeping_loop_t &&p_sl) = default;

  sleeping_loop_t &operator=(const sleeping_loop_t &) = delete;
  sleeping_loop_t &operator=(sleeping_loop_t &&p_sl) = default;

  template <typename t_interval>
  inline void set_interval(t_interval p_interval) {
    m_impl->set_interval(p_interval);
  }

  inline interval get_interval() const { return m_impl->get_interval(); }

  inline void start() { m_impl->start(); }

  inline void stop() {
    if (m_impl) {
      m_impl->stop();
    }
  }

  inline void restart() { m_impl->restart(); }

  /// \brief retrieves if the loop is stopped
  inline bool is_stopped() const { return m_impl->is_stopped(); }

  /// \brief retrieves the timeout for the worker
  inline timeout get_timeout() const { return m_impl->get_timeout(); }

  /// \brief redefines the value of the timeout
  ///
  /// \tparam t_timeout is the type of time used to define timeout for the
  /// worker function
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  template <typename t_timeout> inline void set_timeout(t_timeout p_timeout) {
    m_impl->set_timeout(p_timeout);
  }

private:
  struct implementation
      : public internal::sleeping_loop_base_t<std::function<void(t_params...)>,
                                              t_params...> {
    template <typename t_timeout, typename t_interval>
    implementation(t_timeout p_timeout, t_interval p_interval, worker p_worker,
                   on_timeout p_on_timeout, provider p_provider)
        : internal::sleeping_loop_base_t<on_timeout, t_params...>(
              p_interval, p_worker, p_timeout, p_on_timeout, p_provider) {
      DEB(this->m_log, "timeout = ", p_timeout.count(),
          ", interval = ", p_interval.count());
    }

    template <typename t_timeout, typename t_interval>
    implementation(t_timeout p_timeout, t_interval p_interval, worker p_worker,
                   provider p_provider)
        : internal::sleeping_loop_base_t<on_timeout, t_params...>(
              p_interval, p_worker, p_timeout, p_provider) {
      DEB(this->m_log, "timeout = ", p_timeout.count(),
          ", interval = ", p_interval.count());
    }
  };

private:
  std::unique_ptr<implementation> m_impl;
};

/// \brief
///
///
template <> struct sleeping_loop_t<void> {

  /// \brief type of work executed in a loop in time intervals
  typedef std::function<void()> worker;

  typedef std::function<void()> on_timeout;

  /// \brief Constructor
  ///
  /// \tparam t_interval type of time used to define the execution interval
  /// of the worker
  ///
  /// \param p_interval time used to define the execution interval of the
  /// worker
  ///
  /// \param p_worker the worker to be executed at \p p_interval
  template <typename t_timeout, typename t_interval>
  sleeping_loop_t(t_timeout p_timeout, t_interval p_interval, worker p_worker,
                  on_timeout p_on_timeout)
      : m_impl(std::make_unique<implementation>(p_timeout, p_interval, p_worker,
                                                p_on_timeout)) {}

  template <typename t_timeout, typename t_interval>
  sleeping_loop_t(t_timeout p_timeout, t_interval p_interval, worker p_worker)
      : m_impl(std::make_unique<implementation>(p_timeout, p_interval,
                                                p_worker)) {}

  sleeping_loop_t() = delete;
  ~sleeping_loop_t() = default;

  sleeping_loop_t(const sleeping_loop_t &) = delete;
  sleeping_loop_t(sleeping_loop_t &&p_sl) = default;

  sleeping_loop_t &operator=(const sleeping_loop_t &) = delete;
  sleeping_loop_t &operator=(sleeping_loop_t &&p_sl) = default;

  template <typename t_interval>
  inline void set_interval(t_interval p_interval) {
    m_impl->set_interval(p_interval);
  }

  inline interval get_interval() const { return m_impl->get_interval(); }

  inline void start() { m_impl->start(); }

  inline void stop() {
    if (m_impl) {
      m_impl->stop();
    }
  }

  inline void restart() { m_impl->restart(); }

  /// \brief retrieves if the loop is stopped
  inline bool is_stopped() const { return m_impl->is_stopped(); }

  /// \brief retrieves the timeout for the worker
  inline timeout get_timeout() const { return m_impl->get_timeout(); }

  /// \brief redefines the value of the timeout
  ///
  /// \tparam t_timeout is the type of time used to define timeout for the
  /// worker function
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  template <typename t_timeout> inline void set_timeout(t_timeout p_timeout) {
    m_impl->set_timeout(p_timeout);
  }

private:
  struct implementation
      : public internal::sleeping_loop_base_t<std::function<void()>, void> {

    template <typename t_timeout, typename t_interval>
    implementation(t_timeout p_timeout, t_interval p_interval, worker p_worker,
                   on_timeout p_on_timeout)
        : internal::sleeping_loop_base_t<on_timeout, void>(
              p_interval, p_worker, p_timeout, p_on_timeout) {

      DEB(this->m_log, "timeout = ", p_timeout.count(),
          ", interval = ", p_interval.count());
    }

    template <typename t_timeout, typename t_interval>
    implementation(t_timeout p_timeout, t_interval p_interval, worker p_worker)
        : internal::sleeping_loop_base_t<on_timeout, void>(p_interval, p_worker,
                                                           p_timeout) {

      DEB(this->m_log, "timeout = ", p_timeout.count(),
          ", interval = ", p_interval.count());
    }
  };

private:
  std::unique_ptr<implementation> m_impl;
};

/// \brief
enum class queue_type : uint8_t {
  /// \brief
  CIRCULAR_FIXED_SIZE = 0,
  /// \brief
  CIRCULAR_UNLIMITED_SIZE = 1
};

namespace internal {

/// \brief
template <typename t_data> struct queue_t {

  virtual ~queue_t() {}

  queue_t(const queue_t &) = default;
  queue_t(queue_t &&) = default;
  queue_t &operator=(const queue_t &) = default;
  queue_t &operator=(queue_t &&) = default;

  virtual void add(const t_data &) = 0;
  virtual std::optional<t_data> get() = 0;
  virtual void traverse(std ::function<void(const t_data &)>) const = 0;
  virtual bool full() const = 0;
  virtual bool empty() const = 0;
  virtual size_t capacity() const = 0;
  virtual size_t occupied() const = 0;

protected:
  queue_t(size_t p_size = 0) : m_size(p_size) {}

protected:
  size_t m_size{0};
};
} // namespace internal

/// \brief Implements a circular queue with fixed size
///
/// Unlike the usual implementation, if the buffer is full, the insertion of an
/// element *does not* override the first element
///
///
///
/// \tparam t_data defines the types of the data contained in the buffer
///
///
template <typename t_data>
struct circular_fixed_size_queue_t : public internal::queue_t<t_data> {

  /// \brief Alias for the data
  typedef t_data data;

  /// \brief Constructor
  ///
  /// \param p_size the number of slots in the queue
  circular_fixed_size_queue_t(size_t p_size)
      : internal::queue_t<t_data>(p_size), m_values(p_size) {}

  circular_fixed_size_queue_t() = delete;

  circular_fixed_size_queue_t(const circular_fixed_size_queue_t &) = delete;

  circular_fixed_size_queue_t(circular_fixed_size_queue_t &&) = default;

  circular_fixed_size_queue_t &
  operator=(const circular_fixed_size_queue_t &) = delete;

  circular_fixed_size_queue_t &
  operator=(circular_fixed_size_queue_t &&) = default;

  /// \brief Adds a t_data object to the queue
  void add(const t_data &p_data) override {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (!full()) {
      m_values[m_write++] = p_data;
      ++m_amount;
      if (m_write == this->m_size) {
        m_write = 0;
      }
    } else {
      DEB(m_log, "could not add ", p_data, ", because it is full");
    }
  }

  /// \brief Gets the first t_data obect added to the queue, if there is one
  std::optional<data> get() override {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (empty()) {
      DEB(m_log, "could not get because it is empty");
      return {};
    }
    data _data(m_values[m_read++]);
    --m_amount;
    if (m_read == this->m_size) {
      m_read = 0;
    }
    return {_data};
  }

  /// \brief Traverses the queue
  ///
  /// \param p_function will be called for every data in the queue
  void traverse(std ::function<void(const data &)> p_function) const override {
    size _i = m_read;
    size _counter = 0;
    while (_counter < m_amount) {
      if (_i == this->m_size) {
        _i = 0;
      }
      p_function(m_values[_i++]);
      ++_counter;
    }
  }

  /// \brief Informs if the queue is full
  inline bool full() const override { return m_amount == this->m_size; }

  /// \brief Informs if the queue is empty
  inline bool empty() const override { return m_amount == 0; }

  /// \brief Informs the total capacity of the queue
  inline size_t capacity() const override { return this->m_size; }

  /// \brief Informs the current number of slots occupied in the queue
  inline size_t occupied() const override { return m_amount; }

private:
  /// \brief Queue implemented as a vector
  typedef std::vector<t_data> values;

  /// \brief Type of size of the queue
  typedef typename values::size_type size;

private:
  /// \brief Vector with the values
  values m_values;

  /// \brief Position from where to read
  size m_read = 0;

  /// \brief Position where to write
  size m_write = 0;

  /// \brief Amount of elements
  size m_amount = 0;

  /// \brief Controls access to positions
  std::mutex m_mutex;

  logger::cerr<> m_log{"circular_fixed_size_queue_t"};
};

/// \brief Implements a circular queue which size is increased if it becomes
/// full
///
///
///
/// \tparam t_data defines the types of the data contained in the buffer
template <typename t_data>
struct circular_unlimited_size_queue_t : public internal::queue_t<t_data> {

  /// \brief Constructor
  ///
  /// \param p_size the number of initial slots in the queue
  circular_unlimited_size_queue_t(size_t p_size) : internal::queue_t<t_data>() {
    m_root = create_node();
    node_ptr _p = m_root;
    for (size_t _i = 0; _i < p_size; ++_i) {
      _p = insert(_p, t_data());
    }
    //    this->m_size = p_size;
    m_write = m_root;
    m_read = m_root;
  }

  circular_unlimited_size_queue_t() = delete;

  circular_unlimited_size_queue_t(const circular_unlimited_size_queue_t &) =
      delete;

  circular_unlimited_size_queue_t(circular_unlimited_size_queue_t &&) = default;

  circular_unlimited_size_queue_t &
  operator=(const circular_unlimited_size_queue_t &) = delete;

  circular_unlimited_size_queue_t &
  operator=(circular_unlimited_size_queue_t &&) = default;

  /// \brief Traverses the queue
  ///
  /// \param p_function will be called for every data in the queue
  void traverse(std::function<void(const t_data &)> p_visitor) const override {
    node_ptr _p = m_root;
    while (_p && (_p->m_next != m_root)) {
      p_visitor(_p->m_data);
      _p = _p->m_next;
    }
    p_visitor(_p->m_data);
  }

  /// \brief Adds a t_data object to the queue
  void add(const t_data &p_data) override {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (!full()) {
      DEB(m_log, "not adding more slots");
      m_write->m_data = p_data;
      m_write = m_write->m_next;
    } else {
      DEB(m_log, "adding more slots");
      insert(m_write->m_prev, p_data);
    }
    ++m_amount;
  }

  /// \brief Gets the first t_data obect added to the queue, if there is one
  std::optional<t_data> get() override {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (empty()) {
      DEB(m_log, "empty");
      return {};
    }
    DEB(m_log, "not empty");
    t_data _data(m_read->m_data);
    m_read = m_read->m_next;
    --m_amount;
    return {_data};
  }

  /// \brief Informs if the queue is full
  inline bool full() const override { return m_amount == this->m_size; }

  /// \brief Informs if the queue is empty
  inline bool empty() const override { return m_amount == 0; }

  /// \brief Informs the total capacity of the queue
  inline size_t capacity() const override { return this->m_size; }

  /// \brief Informs the current number of slots occupied in the queue
  inline size_t occupied() const override { return m_amount; }

private:
  /// \brief Node of the linked list used to implement the queue
  struct node {
    /// \brief Type of pointer
    typedef std::shared_ptr<node> ptr;

    node() = default;
    node(const node &) = delete;
    node(node &&) = default;
    node &operator=(const node &) = delete;
    node &operator=(node &&) = default;

    node(t_data &&p_data) : m_data(std::move(p_data)) {}

    node(const t_data &p_data) : m_data(p_data) {}

    t_data m_data;
    ptr m_next;
    ptr m_prev;
  };

  typedef typename node::ptr node_ptr;

private:
  /// \brief Inserts a node in the list after a node
  ///
  /// \param p_node which the new node will be inserted in front of
  ///
  /// \param p_data data inserted in the new node
  ///
  /// \return the new node
  node_ptr insert(node_ptr p_node, const t_data &p_data) {
    node_ptr _new_node = create_node(std::move(p_data));

    _new_node->m_next = p_node->m_next;
    _new_node->m_prev = p_node;

    p_node->m_next->m_prev = _new_node;
    p_node->m_next = _new_node;
    ++this->m_size;
    return _new_node;
  }

  /// \brief Creates a new node
  node_ptr create_node() {
    node_ptr _p(std::make_shared<node>());
    _p->m_next = _p;
    _p->m_prev = _p;
    return _p;
  }

  /// \brief Creates a new node, defining its data
  ///
  /// \param p_data is the data inside the new node
  ///
  /// \return the new node
  node_ptr create_node(const t_data &p_data) {
    node_ptr _p(std::make_shared<node>(p_data));
    _p->m_next = _p;
    _p->m_prev = _p;
    return _p;
  }

private:
  /// \brief The first node of the queue
  node_ptr m_root;

  /// \brief The node where the next write, i.e., new data insertion, should be
  /// done
  node_ptr m_write;

  /// \brief The node where the next read, i.e., new data extraction, should be
  /// done
  node_ptr m_read;

  /// \brief Amount of nodes actually used
  size_t m_amount{0};

  /// \brief Controls insertion
  std::mutex m_mutex;

  logger::cerr<> m_log{"circular_unlimited_size_queue"};
};

/// \brief
template <typename t_data, typename t_size>
std::unique_ptr<internal::queue_t<t_data>>
queue_factory(queue_type p_queue_type, t_size p_size) {
  std::unique_ptr<internal::queue_t<t_data>> _res;
  switch (p_queue_type) {
  case queue_type::CIRCULAR_FIXED_SIZE:
    typedef circular_fixed_size_queue_t<t_data> queue_1;
    _res = std::unique_ptr<internal::queue_t<t_data>>(new queue_1(p_size));
    break;
  case queue_type::CIRCULAR_UNLIMITED_SIZE:
    typedef circular_unlimited_size_queue_t<t_data> queue_2;
    queue_2 *_q = new queue_2(p_size);
    _res = std::unique_ptr<internal::queue_t<t_data>>(_q);
    break;
  }
  return _res;
}

/// \brief worker implements the producer/consumer pattern
///
/// A worker uses some type of queue to cache data in order a bunch of
/// worker compete for the first instance of data inserted into the queue
///
///
///
/// \param t_data type of data to be processed
template <typename t_data> class worker_pool_t {
public:
  /// \brief Alias for the data
  typedef t_data data;

  /// \brief Type of worker that a worker will execute
  typedef std::function<void(const data &)> worker;

  /// \brief
  typedef std::function<void(const data &)> on_timeout;

public:
  /// \brief
  worker_pool_t() = delete;

  /// \brief Constructor
  ///
  /// \param p_id identifier of the worker
  ///
  /// \param p_queue_ptr pointer to the queue used to cache data to be processed
  template <typename t_size, typename t_time>
  worker_pool_t(t_size p_queue_size, t_time p_timeout, on_timeout p_on_timeout,
                queue_type p_queue_type = queue_type::CIRCULAR_UNLIMITED_SIZE)
      : m_impl(std::make_unique<implementation>(p_queue_size, p_timeout,
                                                p_on_timeout, p_queue_type)) {}

  /// \brief if worker timesout, the message not handled is reinserted into the
  /// pool
  template <typename t_size, typename t_time>
  worker_pool_t(t_size p_queue_size, t_time p_timeout,
                queue_type p_queue_type = queue_type::CIRCULAR_UNLIMITED_SIZE)
      : m_impl(std::make_unique<implementation>(p_queue_size, p_timeout,
                                                p_queue_type)) {}

  /// \brief copy constructor not allowed
  worker_pool_t(const worker_pool_t &) = delete;

  /// \brief
  worker_pool_t(worker_pool_t &&p_wp) = default;

  /// \brief copy assignemnt not allowed
  worker_pool_t &operator=(const worker_pool_t &) = delete;

  /// \brief
  worker_pool_t &operator=(worker_pool_t &&p_wp) = default;

  /// \brief destructor
  ///
  /// If 'stop' was not called, empties the queue, waiting for all the data to
  /// be processed
  inline ~worker_pool_t() {
    DEB(m_log, "destructor");
    stop();
  }

  /// \brief adds data to be passed to a worker
  ///
  /// \p data will only be added if \p start() was called
  ///
  /// \param p_data is the data to be passed to a worker
  ///
  /// \return \p true if it was added, \p false otherwise
  inline bool add_data(const data &p_data) { return m_impl->add_data(p_data); }

  /// \brief informs the amount of data added
  inline size_t amount_added() const { return m_impl->amount_added(); }

  /// \brief add adds one \p worker
  ///
  /// \tparam t_time type of time used to define timeout for the worker
  ///
  /// \param p_worker the \p worker to be added
  ///
  /// \param p_timeout timeout of \p worker
  /// \brief common function called to add a \p worker
  ///
  /// \param p_loop the new \p worker to be added
  inline void add_worker(worker p_worker) { m_impl->add_worker(p_worker); }

  /// \brief adds a bunch of \p worker
  ///
  /// \tparam t_time type of time used to define timeout for the worker
  ///
  /// \param p_num_works the number of \p workers to be added
  ///
  /// \param p_worker_factory a function that creates \p workers
  ///
  /// \param p_timeout timeout for the workers
  ///
  /// \param p_on_timeout function to be called when the worker times
  /// out
  template <typename t_time>
  inline void add_worker(uint16_t p_num_works,
                         std::function<worker()> p_worker_factory) {
    m_impl->add_worker(p_num_works, p_worker_factory);
  }

  /// \brief informs if the worker is stopped
  inline bool is_stopped() const { return m_impl->is_stopped(); }

  /// \brief starts the worker
  ///
  /// From this call on, the workers will compete among each other, in order
  /// to process any instance of \p data that was inserted into the queue
  ///
  /// \return \p false if it not start because no \p worker was added
  ///         \p true if it was already started, or if it started successfully
  bool start() { return m_impl->start(); }

  /// \brief stops the \p worker
  ///
  /// From this call on, the \p workers will stop competing among each other,
  /// in order to process any instance of \p data that was inserted into the
  /// queue
  inline void stop() {

    if (m_impl) {
      m_impl->stop();
    }
  }

  /// \brief retrieves the capacity if the queue
  inline auto capacity() const { return m_impl->capacity(); }

  /// \brief the amount of slots occupied in the \p queue
  inline auto occupied() const { return m_impl->occupied(); }

private:
  struct implementation {
    /// \brief Constructor
    ///
    /// \param p_id identifier of the worker
    ///
    /// \param p_queue_ptr pointer to the queue used to cache data to be
    /// processed
    template <typename t_size, typename t_time>
    implementation(t_size p_queue_size, t_time p_timeout,
                   on_timeout p_on_timeout, queue_type p_queue_type)
        : m_queue(queue_factory<t_data, t_size>(p_queue_type, p_queue_size)),
          m_timeout(internal::to_timeout(p_timeout)),
          m_on_timeout(p_on_timeout) {}

    /// \brief if worker timesout, the message not handled is reinserted into
    /// the pool
    template <typename t_size, typename t_time>
    implementation(t_size p_queue_size, t_time p_timeout,
                   queue_type p_queue_type)
        : m_queue(queue_factory<t_data, t_size>(p_queue_type, p_queue_size)),
          m_timeout(internal::to_timeout(p_timeout)),
          m_on_timeout(
              [this](const data &p_data) -> void { add_data(p_data); }) {}

    /// \brief move constructor not allowed
    implementation(worker_pool_t &&) = delete;
    //    implementation(worker_pool_t &&p_wp) {
    //      /// \brief queue where data will be inserted for the worker to
    //      compete for m_queue = std::move(p_wp.m_queue);

    //      m_timeout = p_wp.m_timeout;

    //      m_on_timeout = std::move(p_wp.m_on_timeout);

    //      m_loops = std::move(p_wp.m_loops);

    //      /// \brief indicates if the worker is running
    //      m_stopped = p_wp.m_stopped;

    //      /// \brief m_destroying indicates that the \p produce_consumer
    //      should stop m_destroying = p_wp.m_destroying;

    //      /// \brief amount of queued data
    //      m_queued_data = p_wp.m_queued_data;
    //    }

    /// \brief move assignemnt not allowed
    implementation &operator=(implementation &&) = delete;
    //    implementation &operator=(implementation &&p_wp) {
    //      if (this != &p_wp) {
    //        /// \brief queue where data will be inserted for the worker to
    //        compete
    //        /// for
    //        m_queue = std::move(p_wp.m_queue);

    //        m_timeout = p_wp.m_timeout;

    //        m_on_timeout = std::move(p_wp.m_on_timeout);

    //        m_loops = std::move(p_wp.m_loops);

    //        /// \brief indicates if the worker is running
    //        m_stopped = p_wp.m_stopped;

    //        /// \brief m_destroying indicates that the \p produce_consumer
    //        should
    //        /// stop
    //        m_destroying = p_wp.m_destroying;

    //        /// \brief amount of queued data
    //        m_queued_data = p_wp.m_queued_data;
    //      }
    //      return *this;
    //    }

    /// \brief destructor
    ///
    /// If 'stop' was not called, empties the queue, waiting for all the data to
    /// be processed
    ~implementation() {
      DEB(m_log, "destructor");
      if (!all_loops_stopped()) {
        if (!m_stopped) {
          while (!m_queue->empty()) {
            m_data_produced.notify_all();
            std::unique_lock<std::mutex> _lock(m_mutex_data);
            m_data_consumed.wait(_lock, [] { return true; });
          }
          stop();
        }
      }
      DEB(m_log, "leaving destructor");
    }

    /// \brief adds data to be passed to a worker
    ///
    /// \p data will only be added if \p start() was called
    ///
    /// \param p_data is the data to be passed to a worker
    ///
    /// \return \p true if it was added, \p false otherwise
    bool add_data(const data &p_data) {
      std::unique_lock<std::mutex> _lock_stop(m_mutex_stop);

      data _data{p_data};

      if (m_stopped) {
        ERR(m_log, "could not add data ", _data,
            " because 'worker' is not running; call 'start()' first");
        return false;
      }

      DEB(m_log, "waiting for room to add ", _data);
      {
        std::unique_lock<std::mutex> _lock(m_mutex_data);
        m_data_consumed.wait(
            _lock, [this]() { return (!m_queue->full() || m_stopped); });

        if (m_stopped) {
          DEB(m_log, "stopped when about to add ", _data);
        } else {
          DEB(m_log, "adding ", _data);
          m_queue->add(_data);
          ++m_queued_data;
        }
      }
      // notifyng that new data is available
      DEB(m_log, "notifying");
      m_data_produced.notify_all();

      return true;
    }

    /// \brief informs the amount of data added
    inline size_t amount_added() const { return m_queued_data; }

    /// \brief add adds one \p worker
    ///
    /// \tparam t_time type of time used to define timeout for the worker
    ///
    /// \param p_worker the \p worker to be added
    ///
    /// \param p_timeout timeout of \p worker
    /// \brief common function called to add a \p worker
    ///
    /// \param p_loop the new \p worker to be added
    void add_worker(worker p_worker) {
      auto _provider = [this]() -> std::optional<std::tuple<data>> {
        return this->provider();
      };
      std::lock_guard<std::mutex> _lock(m_add_work);

      m_loops.emplace(m_loops.begin(), p_worker, m_timeout, m_on_timeout,
                      _provider);
    }

    /// \brief adds a bunch of \p worker
    ///
    /// \tparam t_time type of time used to define timeout for the worker
    ///
    /// \param p_num_works the number of \p workers to be added
    ///
    /// \param p_worker_factory a function that creates \p workers
    ///
    /// \param p_timeout timeout for the workers
    ///
    /// \param p_on_timeout function to be called when the worker times
    /// out
    template <typename t_time>
    void add_worker(uint16_t p_num_works,
                    std::function<worker()> p_worker_factory) {

      std::unique_lock<std::mutex> _lock_stop(m_mutex_stop);

      auto _provider = [this]() -> std::optional<std::tuple<data>> {
        return this->provider();
      };

      for (auto _i = 0; _i < p_num_works; ++_i) {
        add_worker(p_worker_factory(), m_timeout, m_on_timeout);
      }
    }

    /// \brief informs if the worker is stopped
    inline bool is_stopped() const { return m_stopped; }

    /// \brief starts the worker
    ///
    /// From this call on, the workers will compete among each other, in order
    /// to process any instance of \p data that was inserted into the queue
    ///
    /// \return \p false if it not start because no \p worker was added
    ///         \p true if it was already started, or if it started successfully
    bool start() {

      std::unique_lock<std::mutex> _lock(m_mutex_stop);

      if (!m_stopped) {
        ERR(m_log, "not starting because it is already running");
        return true;
      }

      if (m_loops.empty()) {
        WAR(m_log, "can't run because there are no workers");
        return false;
      }

      DEB(m_log, "starting");
      m_stopped = false;
      for (async_loop &_loop : m_loops) {
        _loop.start();
      }

      DEB(m_log, "started");
      return true;
    }

    /// \brief stops the \p worker
    ///
    /// From this call on, the \p workers will stop competing among each other,
    /// in order to process any instance of \p data that was inserted into the
    /// queue
    void stop() {

      std::unique_lock<std::mutex> _lock(m_mutex_stop);

      if (m_stopped) {
        DEB(m_log, "not stopping because it is stopped");
        return;
      }

      DEB(m_log, "stopping");

      m_stopped = true;
      if (m_loops.empty()) {
        return;
      }

      m_data_produced.notify_all();
      for (async_loop &_loop : m_loops) {
        DEB(m_log, "stopping loop");
        _loop.stop();
      }
    }

    /// \brief retrieves the capacity if the queue
    inline auto capacity() const { return m_queue->capacity(); }

    /// \brief the amount of slots occupied in the \p queue
    inline auto occupied() const { return m_queue->occupied(); }

  private:
    /// \brief async_loop_t is a \p async_loop where a \p worker will be
    /// running
    typedef async_loop_t<data> async_loop;

    /// \brief async_loops_t is the collection of \p async_loop pointers
    typedef typename std::vector<async_loop> asyncs_loops;

    /// \brief Alias for a generic queue type, used to order data to be
    /// processed
    typedef internal::queue_t<t_data> queue;

    /// \brief
    typedef std::unique_ptr<queue> queue_ptr;

  private:
    /// \brief provider provides data, if available, to a \p worker
    std::optional<std::tuple<data>> provider() {
      using namespace std;

      if (m_stopped) {
        DEB(m_log, "stopped");
        return {};
      }

      DEB(m_log, "waiting for data...");
      std::unique_lock<std::mutex> _lock(m_mutex_data);
      m_data_produced.wait(_lock, [this]() -> bool {
        //      return (!m_container.empty() || m_stopped);
        if (m_stopped) {
          DEB(m_log, "stopped");
          return true;
        }
        if (!m_queue->empty()) {
          DEB(m_log, "not empty");
          return true;
        }
        DEB(m_log, "still waiting for data");

        return false;
      });

      DEB(m_log, "either there is data, or it was stopped");

      if (m_stopped) {
        DEB(m_log, "stopped and notifying");
        m_data_consumed.notify_all();
        return {};
      }

      DEB(m_log, "there is data");

      std::optional<data> _maybe = m_queue->get();
      if (_maybe) {
        data _data(std::move(*_maybe));

        DEB(m_log, "getting ", _data, " and notifying");

        m_data_consumed.notify_all();
        return {{_data}};
      }
      DEB(m_log, "it was not possible to get data");
      return {};
    }

    /// \brief informs if all the \p async_loops are stopped
    bool all_loops_stopped() const {
      for (const async_loop &_loop : m_loops) {
        if (!_loop.is_stopped()) {
          return false;
        }
      }
      return true;
    }

  private:
    /// \brief queue where data will be inserted for the worker to compete for
    queue_ptr m_queue;

    /// \brief
    timeout m_timeout;

    /// \brief
    on_timeout m_on_timeout;

    /// \brief controls access to the \p m_loops while inserting a new \p
    /// worker
    std::mutex m_add_work;

    /// \brief controls access to inserting data
    std::mutex m_mutex_data;

    /// \brief controls access to attributes while the worker is stopping
    std::mutex m_mutex_stop;

    /// \brief controls access to the data consumed
    std::condition_variable m_data_consumed;

    /// \brief controls access to the data produced
    std::condition_variable m_data_produced;

    /// \brief asynchronous loops, where the workers are running
    asyncs_loops m_loops;

    /// \brief indicates if the worker is running
    bool m_stopped{true};

    /// \brief m_destroying indicates that the \p produce_consumer should stop
    bool m_destroying{false};

    /// \brief amount of queued data
    size_t m_queued_data{0};

    /// \brief
    logger::cerr<> m_log{"implementation"};
  };

private:
  /// \brief
  std::unique_ptr<implementation> m_impl;

  logger::cerr<> m_log{"worker_pool"};
};

/// \brief
template <typename t_data> struct messenger_t {

  /// \brief Type for the identifier of the worker pool
  typedef std::string pool_id;

  /// \brief Type of worker that a worker will execute
  typedef std::function<void(const t_data &)> worker;

  /// \brief
  typedef std::function<void(const t_data &)> on_timeout;

  /// \brief 0 is the lowest
  typedef uint16_t priority;

  ~messenger_t() {
    DEB(m_log, "destructor");
    stop();
  }

  /// \brief
  template <typename t_size, typename t_time>
  static inline void add_worker_pool(
      const pool_id &p_pool_id, priority p_priority, t_size p_queue_size,
      t_time p_timeout, on_timeout p_on_timeout,
      queue_type p_queue_type = queue_type::CIRCULAR_UNLIMITED_SIZE) {

    //    item _item{p_pool_id,    p_priority, p_queue_size,
    //               p_queue_type, p_timeout,  p_on_timeout};
    //    m_itens.push_back(std::move(_item));

    if (find(p_pool_id) != m_itens.end()) {
      return;
    }

    //    m_itens.emplace(m_itens.begin(), p_pool_id, p_priority, p_queue_size,
    //                    p_queue_type, p_timeout, p_on_timeout);

    item _item{p_pool_id,    p_priority, p_queue_size,
               p_queue_type, p_timeout,  p_on_timeout};
    m_itens.push_back(std::move(_item));
    //    std::sort(m_itens.begin(), m_itens.end());
    m_itens.sort(
        [](const item &p_i1, const item &p_i2) -> bool { return p_i1 < p_i2; });
  }

  /// \brief
  template <typename t_size, typename t_time>
  static pool_id add_worker_pool(
      priority p_priority, t_size p_queue_size, t_time p_timeout,
      on_timeout p_on_timeout,
      queue_type p_queue_type = queue_type::CIRCULAR_UNLIMITED_SIZE) {
    pool_id _pool_id(std::to_string(uuid()));
    add_worker_pool(_pool_id, p_priority, p_queue_size, p_timeout, p_on_timeout,
                    p_queue_type);
    return _pool_id;
  }

  /// \brief
  template <typename t_size, typename t_time>
  static inline void add_worker_pool(
      const pool_id &p_pool_id, priority p_priority, t_size p_queue_size,
      t_time p_timeout,
      queue_type p_queue_type = queue_type::CIRCULAR_UNLIMITED_SIZE) {

    //    item _item{p_pool_id, p_priority, p_queue_size, p_queue_type,
    //    p_timeout}; m_itens.push_back(std::move(_item));
    if (find(p_pool_id) != m_itens.end()) {
      return;
    }

    //    m_itens.emplace(m_itens.begin(), p_pool_id, p_priority, p_queue_size,
    //                    p_queue_type, p_timeout);

    item _item{p_pool_id, p_priority, p_queue_size, p_queue_type, p_timeout};
    m_itens.push_back(std::move(_item));

    //    std::sort(m_itens.begin(), m_itens.end());
    m_itens.sort(
        [](const item &p_i1, const item &p_i2) -> bool { return p_i1 < p_i2; });
  }

  /// \brief
  template <typename t_size, typename t_time>
  static pool_id add_worker_pool(
      priority p_priority, t_size p_queue_size, t_time p_timeout,
      queue_type p_queue_type = queue_type::CIRCULAR_UNLIMITED_SIZE) {

    pool_id _pool_id(std::to_string(uuid()));

    add_worker_pool(_pool_id, p_priority, p_queue_size, p_timeout,
                    p_queue_type);

    return _pool_id;
  }

  /// \brief
  static void set_priority(const pool_id &p_pool_id, priority p_priority) {
    iterator _ite = find(p_pool_id);
    if (_ite != m_itens.end()) {
      _ite->set_priority(p_priority);
    }
    //    std::sort(m_itens.begin(), m_itens.end());
    m_itens.sort(
        [](const item &p_i1, const item &p_i2) -> bool { return p_i1 < p_i2; });
  }

  /// \brief
  static std::optional<priority> get_priority(const pool_id &p_pool_id) {
    iterator _ite = find(p_pool_id);
    if (_ite != m_itens.end()) {
      return {_ite->get_priority()};
    }
    return {};
  }

  /// \brief
  static void publish(const t_data &p_data) {

    iterator _end = m_itens.end();
    for (iterator _ite = m_itens.begin(); _ite != _end; ++_ite) {
      _ite->add_data(p_data);
    }
  }

  /// \brief
  static void add_subscriber(const pool_id &p_pool_id, worker p_worker) {
    iterator _ite = find(p_pool_id);
    if (_ite != m_itens.end()) {
      _ite->add_worker(p_worker);
    }
  }

  /// \brief
  static void add_subscriber(const pool_id &p_pool_id, uint16_t p_num_workers,
                             std::function<worker()> p_factory) {
    iterator _ite = find(p_pool_id);
    if (_ite != m_itens.end()) {
      _ite->add_worker(p_num_workers, p_factory);
    }
  }

  /// \brief
  static void stop() {
    iterator _end = m_itens.end();
    for (iterator _ite = m_itens.begin(); _ite != _end; ++_ite) {
      _ite->stop();
    }
  }

private:
  /// \brief
  struct item {
    /// \brief
    item() = delete;

    /// \brief
    item(const item &) = delete;

    /// \brief
    item(item &&p_item)
        : m_pool_id(std::move(p_item.m_pool_id)), m_priority(p_item.m_priority),
          m_worker_pool(std::move(p_item.m_worker_pool)) {}

    /// \brief
    ~item() {
      DEB(m_log, "destructor");
      m_worker_pool.stop();
    }

    /// \brief
    template <typename t_size, typename t_time>
    item(const pool_id &p_pool_id, priority p_priority, t_size p_size,
         queue_type p_queue_type, t_time p_timeout, on_timeout p_on_timeout)
        : m_pool_id(p_pool_id), m_priority(p_priority),
          m_worker_pool(p_size, p_timeout, p_on_timeout, p_queue_type) {}

    /// \brief
    template <typename t_size, typename t_time>
    item(const pool_id &p_pool_id, priority p_priority, t_size p_size,
         queue_type p_queue_type, t_time p_timeout)
        : m_pool_id(p_pool_id), m_priority(p_priority),
          m_worker_pool(p_size, p_timeout, p_queue_type) {}

    /// \brief
    item &operator=(const item &) = delete;

    /// \brief
    item &operator=(item &&p_item) {
      if (this != &p_item) {
        m_pool_id = std::move(p_item.m_pool_id);
        m_priority = p_item.m_priority;
        m_worker_pool = std::move(p_item.m_work_pool);
      }
      return *this;
    }

    /// \brief
    inline bool operator==(const pool_id &p_pool_id) const {
      return m_pool_id == p_pool_id;
    }

    /// \brief
    inline bool operator!=(const pool_id &p_pool_id) const {
      return m_pool_id == p_pool_id;
    }

    /// \brief
    inline bool operator==(const item &p_item) const {
      return m_pool_id == p_item.m_pool_id;
    }

    /// \brief
    inline bool operator!=(const item &p_item) const {
      return m_pool_id != p_item.m_pool_id;
    }

    /// \brief
    inline bool operator<(const item &p_item) const {
      return m_priority > p_item.m_priority;
    }

    /// \brief
    inline bool operator>(const item &p_item) const {
      return m_priority < p_item.m_priority;
    }

    /// \brief
    inline const pool_id &get_id() const { return m_pool_id; }

    /// \brief
    inline priority get_priority() const { return m_priority; }

    /// \brief
    inline void set_priority(priority p_priority) { m_priority = p_priority; }

    /// \brief
    inline void add_worker(worker p_worker) {
      m_worker_pool.add_worker(p_worker);
      m_worker_pool.start();
    }

    /// \brief
    inline void add_worker(uint16_t p_num_workers,
                           std::function<worker()> p_factory) {
      m_worker_pool.add_worker(p_num_workers, p_factory);
      m_worker_pool.start();
    }

    /// \brief
    inline void add_data(const t_data &p_data) {
      m_worker_pool.add_data(p_data);
    }

    /// \brief
    inline void stop() { m_worker_pool.stop(); }

  private:
    /// \brief
    typedef worker_pool_t<t_data> worker_pool;

  private:
    /// \brief
    pool_id m_pool_id;

    /// \brief
    priority m_priority;

    /// \brief
    worker_pool m_worker_pool;

    logger::cerr<> m_log{"item"};
  };

  /// \brief
  typedef std::list<item> itens;

  /// \brief
  typedef typename itens::iterator iterator;

private:
  /// \brief
  static inline iterator find(const pool_id &p_pool_id) {
    auto _cmp = [&p_pool_id](const item &p_item) -> bool {
      return p_pool_id == p_item.get_id();
    };
    return std::find_if(m_itens.begin(), m_itens.end(), _cmp);
  }

private:
  /// \brief
  static itens m_itens;
  logger::cerr<> m_log{"messenger"};
};

/// \brief
template <typename t_data>
typename messenger_t<t_data>::itens messenger_t<t_data>::m_itens;

} // namespace concurrent
} // namespace tenacitas

#endif
