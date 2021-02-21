#ifndef TENACITAS_CONCURRENT_H
#define TENACITAS_CONCURRENT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <list>
#include <mutex>
#include <optional>
#include <thread>

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

/// \brief Type of function executed when a function times out
typedef std::function<void()> on_timeout;

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
  static result call(std::function<t_type(t_params &&...)> p_work,
                     t_time p_timeout, t_params &&... p_params) {

    std::packaged_task<type(t_params && ...)> _task(
        [p_work](t_params &&... p_params) -> t_type {
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
    std::invoke_result_t<t_function, t_params &&...>, t_time,
    t_params &&...>::result
execute(t_time p_timeout, t_function &&p_function, t_params &&... p_params) {

  typedef std::invoke_result_t<t_function, t_params &&...> type;

  typedef internal::executer_helper_t<type, t_time, t_params &&...>
      executer_helper;

  return executer_helper::call(p_function, p_timeout,
                               std::forward<t_params>(p_params)...);
}

/// \brief Type of function used to inform if a loop should stop
typedef std::function<bool()> breaker;

///// \brief Used to define if a loop will use a breaker function or not
// enum class use_breaker : char { yes = 'y', no = 'n' };

namespace internal {

/// \brief base class for asynchronous loop
///
/// \tparam t_log
///
/// \tparam t_time
///
/// \tparam t_worker
template <typename t_log, typename t_worker> struct async_loop_base_t {

  async_loop_base_t() = delete;
  async_loop_base_t(const async_loop_base_t &) = delete;
  async_loop_base_t(async_loop_base_t &&p_async_loop) = delete;
  async_loop_base_t &operator=(const async_loop_base_t &) = delete;
  async_loop_base_t &operator=(async_loop_base_t &&p_async_loop) = delete;

  virtual ~async_loop_base_t() { stop(); }

  void start() {
    if (!m_stopped) {
      return;
    }
    m_stopped = false;
    m_thread = std::thread([this]() { loop(); });
  }

  void stop() {
    if (m_stopped) {
      return;
    }
    m_stopped = true;
    if (m_thread.joinable()) {
      m_thread.join();
    }
  }

  inline bool is_stopped() const { return m_stopped; }
  inline void set_log_debug() { m_log.set_debug_level(); }
  inline void set_log_info() { m_log.set_info_level(); }
  inline void set_log_warn() { m_log.set_warn_level(); }

  template <typename t_time> inline void set_timeout(t_time p_timeout) {
    m_timeout = internal::to_timeout(p_timeout);
  }

  inline timeout get_timeout() const { return m_timeout; }

protected:
  template <typename t_time>
  async_loop_base_t(t_worker p_worker, t_time p_time, on_timeout p_on_timeout)
      : m_worker(p_worker), m_timeout(internal::to_timeout(p_time)),
        m_on_timeout(p_on_timeout) {}

  virtual void loop() = 0;

protected:
  t_worker m_worker;

  timeout m_timeout;

  on_timeout m_on_timeout;

  bool m_stopped{true};

  std::thread m_thread;

  const timeout m_breaker_timeout{200ms};
  const timeout m_provider_timeout{300ms};

  t_log m_log{"async_loop"};
};
} // namespace internal
/// \brief Base class for asynchronous loop
template <typename t_log /*, use_breaker use*/, typename... t_params>
struct async_loop_t;

///// #### async_loop 1 ####
// template <typename t_log, typename... t_params>
// struct async_loop_t<t_log, use_breaker::yes, t_params...>
//    : public internal::async_loop_base_t<t_log,
//                                         std::function<void(t_params &&...)>>
//                                         {

//  typedef std::function<void(t_params &&...)> worker;

//  typedef std::function<std::tuple<t_params...>()> provider;

//  template <typename t_time>
//  async_loop_t(worker p_worker, t_time p_timeout, on_timeout p_on_timeout,
//               breaker p_breaker, provider p_provider)
//      : internal::async_loop_base_t<t_log, worker>(p_worker, p_timeout,
//                                                   p_on_timeout),
//        m_provider(p_provider), m_breaker(p_breaker) {}

// protected:
//  void loop() override {

//    while (true) {
//      if (this->m_stopped) {
//        break;
//      }

//      std::optional<bool> _maybe_break =
//          execute(this->m_breaker_timeout, m_breaker);

//      if (this->m_stopped) {
//        DEB(this->m_log, "stopped before checking it should break");
//        break;
//      }

//      if ((_maybe_break) && (*_maybe_break)) {
//        DEB(this->m_log, "breaking loop");
//        break;
//      }

//      std::optional<std::tuple<t_params...>> _maybe_data =
//          execute(this->m_provider_timeout, m_provider);

//      if (this->m_stopped) {
//        DEB(this->m_log, "stopped before checking if data was provided");
//        break;
//      }

//      _maybe_break = execute(this->m_breaker_timeout, m_breaker);

//      if (this->m_stopped) {
//        DEB(this->m_log, "stopped before checking it should break");
//        break;
//      }

//      if ((_maybe_break) && (*_maybe_break)) {
//        DEB(this->m_log, "breaking before checking if data was provided");
//        break;
//      }

//      if (_maybe_data) {

//        std::tuple<t_params...> _params = std::move(*_maybe_data);
//        DEB(this->m_log, "data was provided: ", _params);

//        auto _worker = [this, &_params]() -> void {
//          std::apply(this->m_worker, std::move(_params));
//        };

//        if (!execute(this->m_timeout, _worker)) {
//          DEB(this->m_log, "timeout for worker");
//          std::thread([this]() -> void { this->m_on_timeout(); }).detach();
//        }
//      }
//    }
//  }

// private:
//  provider m_provider;
//  breaker m_breaker;
//};

/// #### async_loop 2 ####
template <typename t_log, typename... t_params>
struct async_loop_t /*<t_log, use_breaker::no, t_params...>*/
    : public internal::async_loop_base_t<t_log,
                                         std::function<void(t_params &&...)>> {

  typedef std::function<void(t_params &&...)> worker;
  typedef std::function<std::tuple<t_params...>()> provider;

  template <typename t_time>
  async_loop_t(worker p_worker, t_time p_timeout, on_timeout p_on_timeout,
               provider p_provider)
      : internal::async_loop_base_t<t_log, worker>(p_worker, p_timeout,
                                                   p_on_timeout),
        m_provider(p_provider) {}

protected:
  void loop() override {
    DEB(this->m_log, "entering loop");

    while (true) {

      if (this->m_stopped) {
        DEB(this->m_log, "another loop, m_stopped = ", this->m_stopped);
        break;
      }

      std::optional<std::tuple<t_params...>> _maybe_data =
          execute(this->m_provider_timeout, m_provider);

      if (this->m_stopped) {
        DEB(this->m_log, "stopped before checking if data was provided");
        break;
      }

      if (_maybe_data) {
        std::tuple<t_params...> _params = std::move(*_maybe_data);
        DEB(this->m_log, "data provided = ", _params);

        auto _worker = [this, &_params]() -> void {
          std::apply(this->m_worker, std::move(_params));
        };

        if (!execute(this->m_timeout, _worker)) {
          DEB(this->m_log, "timeout for worker");
          std::thread([this]() -> void { this->m_on_timeout(); }).detach();
        }
      }
    }
  }

private:
  provider m_provider;
};

///// #### async_loop 3 ####
// template <typename t_log>
// struct async_loop_t<t_log, use_breaker::yes, void>
//    : public internal::async_loop_base_t<t_log, std::function<void()>> {

//  typedef std::function<void()> worker;

//  template <typename t_time>
//  async_loop_t(worker p_worker, t_time p_timeout, on_timeout p_on_timeout,
//               breaker p_breaker)
//      : internal::async_loop_base_t<t_log, worker>(p_worker, p_timeout,
//                                                   p_on_timeout),
//        m_breaker(p_breaker) {}

// protected:
//  void loop() override {

//    auto _worker = [this]() -> void { this->m_worker(); };

//    while (true) {
//      if (this->m_stopped) {
//        break;
//      }

//      std::optional<bool> _maybe_break =
//          execute(this->m_breaker_timeout, m_breaker);
//      if ((_maybe_break) && (*_maybe_break)) {
//        break;
//      }

//      if (!execute(this->m_timeout, _worker)) {
//        std::thread([this]() -> void { this->m_on_timeout(); }).detach();
//      }
//    }
//  }

// private:
//  breaker m_breaker;
//};

/// #### async_loop 4 ####
template <typename t_log>
struct async_loop_t<t_log, /*use_breaker::no,*/ void>
    : public internal::async_loop_base_t<t_log, std::function<void()>> {

  typedef std::function<void()> worker;

  template <typename t_time>
  async_loop_t(worker p_worker, t_time p_timeout, on_timeout p_on_timeout)
      : internal::async_loop_base_t<t_log, worker>(p_worker, p_timeout,
                                                   p_on_timeout) {}

protected:
  void loop() override {

    auto _worker = [this]() -> void { this->m_worker(); };

    while (true) {

      if (this->m_stopped) {
        break;
      }

      if (!execute(this->m_timeout, _worker)) {
        std::thread([this]() -> void { this->m_on_timeout(); }).detach();
      }
    }
  }
};

namespace internal {

/// \brief Executes a work function in fixed period of times
template <typename t_log, typename... t_params> struct sleeping_loop_base_t {

  sleeping_loop_base_t() = delete;
  sleeping_loop_base_t(const sleeping_loop_base_t &) = delete;
  sleeping_loop_base_t(sleeping_loop_base_t &&p_sleep) = delete;
  sleeping_loop_base_t &operator=(const sleeping_loop_base_t &) = delete;
  sleeping_loop_base_t &operator=(sleeping_loop_base_t &&p_sleep) = delete;

  /// \brief destructor interrupts the loop
  inline virtual ~sleeping_loop_base_t() {
    DEB(this->m_log, "destructor");
    stop();
  }

  /// \brief retrieves the interval defined for the loop to sleep between
  /// the execution of the operation
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
    DEB(this->m_log, "restart");
    stop();
    start();
  }

  /// \brief Starts the loop
  void start() {
    if (!m_async.is_stopped()) {
      DEB(this->m_log, "not starting async loop because it is running");
      return;
    }
    if (!m_stopped) {
      DEB(this->m_log, "not starting because it is running");
    }
    DEB(this->m_log, "running async loop");
    m_stopped = false;
    m_thread = std::thread([this]() -> void { breaker(); });
    m_async.start();
  }

  /// \brief Stops the loop
  void stop() {

    //    if (m_stopped) {
    //      if (m_thread.joinable()) {
    //        m_thread.join();
    //      }
    //      return;
    //    }

    //    if (m_async.is_stopped()) {
    //      DEB(this->m_log, "not stopping async loop because it was not
    //      running"); if (m_thread.joinable()) {
    //        m_thread.join();
    //      }
    //      return;
    //    }

    m_stopped = true;

    m_async.stop();

    DEB(m_log, "notifying");
    m_cond.notify_one();

    if (m_thread.joinable()) {
      m_thread.join();
    }
    DEB(m_log, "stopping");
  }

  /// \brief retrieves if the loop is stopped
  inline bool is_stopped() const { return m_async.is_stopped(); }

  /// \brief retrieves the timeout for the operation
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
                       t_timeout p_timeout, on_timeout p_on_timeout)
      : m_interval(internal::to_interval(p_interval)),
        m_async(p_worker, internal::to_timeout(p_timeout), p_on_timeout) {
    DEB(m_log, "constructor without provider called");
  }

  template <typename t_timeout, typename t_interval, typename t_worker,
            typename t_provider>
  sleeping_loop_base_t(t_interval p_interval, t_worker p_worker,
                       t_timeout p_timeout, on_timeout p_on_timeout,
                       t_provider p_provider)
      : m_interval(internal::to_interval(p_interval)),
        m_async(p_worker, internal::to_timeout(p_timeout), p_on_timeout,
                p_provider) {
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
      //      if (m_stopped) {
      //        DEB(m_log, "breaking because it is stopped");
      //        m_async.stop();
      //        return true;
      //      }

      // timeout, so do not stop
      DEB(m_log, "must not stop");
      return false;
    }
    // no timeout, which means the loop was stopped
    DEB(m_log, "must stop");
    m_async.stop();
    return true;
  }

protected:
  typedef async_loop_t<t_log, /*use_breaker::no,*/ t_params...> async_loop;

protected:
  interval m_interval;

  async_loop m_async;

  std::thread m_thread;

  /// \brief controls asynchronous execution
  std::mutex m_mutex;

  /// \brief controls asynchronous execution
  std::condition_variable m_cond;

  bool m_stopped{true};

  /// \brief log for the class
  t_log m_log{"sleeping_loop"};
};

} // namespace internal

/// \brief Base class for sleeping loops, which are loops that sleep during
/// a certain amount of time, then wake up and execute some work
///
/// \tparam t_log
///
/// \tparam t_params are the parameters that the work function
template <typename t_log, typename... t_params>
struct sleeping_loop_t
    : public internal::sleeping_loop_base_t<t_log, t_params...> {

  /// \brief type of work executed in a loop in time intervals
  typedef std::function<void(t_params...)> worker;

  /// \brief Provider is the type of function that provides data to the
  /// work function
  ///
  /// \return \p an optional tuple of objects needed by the \p operation
  typedef std::function<std::tuple<t_params...>()> provider;

  /// \brief Constructor
  ///
  /// \tparam t_timeout type of time used to define the timeout of the
  /// operation
  ///
  /// \tparam t_interval type of time used to define the execution interval
  /// of the operation
  ///
  /// \param p_timeout time used to define the timeout of the operation
  ///
  /// \param p_interval time used to define the execution interval of the
  /// operation
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
      : internal::sleeping_loop_base_t<t_log, t_params...>(
            p_interval, p_worker, p_timeout, p_on_timeout, p_provider) {
    DEB(this->m_log, "timeout = ", p_timeout.count(),
        ", interval = ", p_interval.count());
  }
};

/// \brief Base class for sleeping loops, which are loops that sleep during
/// a certain amount of time, then wake up and execute some work
///
/// \tparam t_log
///
/// \tparam t_params are the parameters that the work function
template <typename t_log>
struct sleeping_loop_t<t_log, void>
    : public internal::sleeping_loop_base_t<t_log, void> {

  /// \brief type of work executed in a loop in time intervals
  typedef std::function<void()> worker;

  /// \brief Constructor
  ///
  /// \tparam t_interval type of time used to define the execution interval
  /// of the operation
  ///
  /// \param p_interval time used to define the execution interval of the
  /// operation
  ///
  /// \param p_operation the operation to be executed at \p p_interval
  template <typename t_timeout, typename t_interval>
  sleeping_loop_t(t_timeout p_timeout, t_interval p_interval, worker p_worker,
                  on_timeout p_on_timeout)
      : internal::sleeping_loop_base_t<t_log, void>(p_interval, p_worker,
                                                    p_timeout, p_on_timeout) {

    DEB(this->m_log, "timeout = ", p_timeout.count(),
        ", interval = ", p_interval.count());
  }
};

} // namespace concurrent
} // namespace tenacitas

#endif
