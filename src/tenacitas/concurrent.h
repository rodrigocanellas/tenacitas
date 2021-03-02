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
#include <vector>

#include <tenacitas/calendar.h>
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

namespace internal {

/// \brief base class for asynchronous loop
///
/// \tparam t_log
///
/// \tparam t_time
///
/// \tparam t_worker
template <typename t_log, typename t_on_timeout, typename t_worker>
struct async_loop_base_t {

  async_loop_base_t() = delete;
  async_loop_base_t(const async_loop_base_t &) = delete;
  async_loop_base_t(async_loop_base_t &&p_async_loop) {
    m_worker = std::move(p_async_loop.m_worker);
    m_timeout = std::move(p_async_loop.m_timeout);
    m_on_timeout = std::move(p_async_loop.m_on_timeout);
    m_stopped = p_async_loop.m_stopped;
    m_thread = std::move(p_async_loop.m_thread);
  }
  async_loop_base_t &operator=(const async_loop_base_t &) = delete;
  async_loop_base_t &operator=(async_loop_base_t &&p_async_loop) {
    if (this != &p_async_loop) {
      if (!p_async_loop.is_stopped()) {
        p_async_loop.stop();
      }
      m_worker = std::move(p_async_loop.m_worker);
      m_timeout = std::move(p_async_loop.m_timeout);
      m_on_timeout = std::move(p_async_loop.m_on_timeout);
      m_stopped = p_async_loop.m_stopped;
      m_thread = std::move(p_async_loop.m_thread);
      if (!m_stopped) {
        start();
      }
    }
    return *this;
  }

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

  template <typename t_time> inline void set_timeout(t_time p_timeout) {
    m_timeout = internal::to_timeout(p_timeout);
  }

  inline timeout get_timeout() const { return m_timeout; }

protected:
  template <typename t_time>
  async_loop_base_t(t_worker p_worker, t_time p_time, t_on_timeout p_on_timeout)
      : m_worker(p_worker), m_timeout(internal::to_timeout(p_time)),
        m_on_timeout(p_on_timeout) {}

  virtual void loop() = 0;

protected:
  t_worker m_worker;

  timeout m_timeout;

  t_on_timeout m_on_timeout;

  bool m_stopped{true};

  std::thread m_thread;

  t_log m_log{"async_loop"};

  //  const timeout m_breaker_timeout{200ms};
  //  const timeout m_provider_timeout{300ms};
};
} // namespace internal
/// \brief Base class for asynchronous loop
template <typename t_log, typename... t_params> struct async_loop_t;

/// #### async_loop 1 ####
template <typename t_log, typename... t_params>
struct async_loop_t
    : public internal::async_loop_base_t<t_log,
                                         std::function<void(t_params &&...)>,
                                         std::function<void(t_params &&...)>> {

  typedef std::function<void(t_params &&...)> on_timeout;
  typedef std::function<void(t_params &&...)> worker;
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

  template <typename t_time>
  async_loop_t(worker p_worker, t_time p_timeout, on_timeout p_on_timeout,
               provider p_provider)
      : internal::async_loop_base_t<t_log, on_timeout, worker>(
            p_worker, p_timeout, p_on_timeout),
        m_provider(p_provider) {}

  async_loop_t(async_loop_t &&p_async_loop)
      : internal::async_loop_base_t<t_log, on_timeout, worker>(
            std::move(p_async_loop)) {
    m_provider = std::move(p_async_loop.m_provider);
  }

  async_loop_t &operator=(async_loop_t &&p_async_loop) {
    if (this != &p_async_loop) {
      //      internal::async_loop_base_t<t_log, worker>::operator=(
      //          std::move(p_async_loop));
      m_provider = std::move(p_async_loop.m_provider);
    }
    return *this;
  }

protected:
  void loop() override {
    DEB(this->m_log, "entering loop");

    while (true) {

      if (this->m_stopped) {
        DEB(this->m_log, "another loop, m_stopped = ", this->m_stopped);
        break;
      }

      std::optional<std::tuple<t_params...>> _maybe_data = m_provider();

      if (_maybe_data) {
        std::tuple<t_params...> _params = std::move(*_maybe_data);

        DEB(this->m_log, "data provided = ", _params);

        auto _worker = [this, &_params]() -> void {
          std::apply(this->m_worker, std::move(_params));
        };

        if (!execute(this->m_timeout, _worker)) {
          DEB(this->m_log, "timeout for worker");

          std::tuple<t_params...> _copy{_params};

          auto _on_timeout = [this, &_copy]() {
            std::apply(this->m_on_timeout, std::move(_copy));
          };

          std::thread([_on_timeout]() -> void { _on_timeout(); }).detach();
        }
      }
    }
  }

private:
  provider m_provider;
};

/// #### async_loop 2 ####
template <typename t_log>
struct async_loop_t<t_log, void>
    : public internal::async_loop_base_t<t_log, std::function<void()>,
                                         std::function<void()>> {

  typedef std::function<void()> on_timeout;
  typedef std::function<void()> worker;

  template <typename t_time>
  async_loop_t(worker p_worker, t_time p_timeout, on_timeout p_on_timeout)
      : internal::async_loop_base_t<t_log, on_timeout, worker>(
            p_worker, p_timeout, p_on_timeout) {}

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
template <typename t_log, typename t_on_timeout, typename... t_params>
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
    m_thread = std::thread([this]() -> void { breaker(); });
    m_async.start();
  }

  /// \brief Stops the loop
  void stop() {

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
                       t_timeout p_timeout, t_on_timeout p_on_timeout)
      : m_interval(internal::to_interval(p_interval)),
        m_async(p_worker, internal::to_timeout(p_timeout), p_on_timeout) {
    DEB(m_log, "constructor without provider called");
  }

  template <typename t_timeout, typename t_interval, typename t_worker,
            typename t_provider>
  sleeping_loop_base_t(t_interval p_interval, t_worker p_worker,
                       t_timeout p_timeout, t_on_timeout p_on_timeout,
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
  typedef async_loop_t<t_log, t_params...> async_loop;

protected:
  interval m_interval;

  async_loop m_async;

  std::thread m_thread;

  /// \brief controls asynchronous execution
  std::mutex m_mutex;

  /// \brief controls asynchronous execution
  std::condition_variable m_cond;

  bool m_stopped{true};

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
    : public internal::sleeping_loop_base_t<
          t_log, std::function<void(t_params &&...)>, t_params...> {

  /// \brief type of work executed in a loop in time intervals
  typedef std::function<void(t_params &&...)> worker;

  typedef std::function<void(t_params &&...)> on_timeout;

  /// \brief Provider is the type of function that provides data to the
  /// work function
  ///
  /// \return \p an optional tuple of objects needed by the \p operation
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

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
      : internal::sleeping_loop_base_t<t_log, on_timeout, t_params...>(
            p_interval, p_worker, p_timeout, p_on_timeout, p_provider) {
    DEB(this->m_log, "timeout = ", p_timeout.count(),
        ", interval = ", p_interval.count());
  }
};

/// \brief
///
/// \tparam t_log
template <typename t_log>
struct sleeping_loop_t<t_log, void>
    : public internal::sleeping_loop_base_t<t_log, std::function<void()>,
                                            void> {

  /// \brief type of work executed in a loop in time intervals
  typedef std::function<void()> worker;

  typedef std::function<void()> on_timeout;

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
      : internal::sleeping_loop_base_t<t_log, on_timeout, void>(
            p_interval, p_worker, p_timeout, p_on_timeout) {

    DEB(this->m_log, "timeout = ", p_timeout.count(),
        ", interval = ", p_interval.count());
  }
};

enum class queue_type : uint8_t {
  CIRCULAR_FIXED_SIZE = 0,
  CIRCULAR_UNLIMITED_SIZE = 1
};

/// \brief Implements a circular queue with fixed size
///
/// Unlike the usual implementation, if the buffer is full, the insertion of an
/// element *does not* override the first element
///
/// \tparam t_log
///
/// \tparam t_data defines the types of the data contained in the buffer
///
///
template <typename t_log, typename t_data> struct circular_fixed_size_queue_t {

  /// \brief Alias for the data
  typedef t_data data;

  /// \brief Alias for the log
  typedef t_log log;

  /// \brief Constructor
  ///
  /// \param p_size the number of slots in the queue
  circular_fixed_size_queue_t(size_t p_size)
      : m_size(p_size), m_values(p_size) {}

  /// \brief Adds a t_data object to the queue
  void add(const t_data &p_data) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (!full()) {
      m_values[m_write++] = p_data;
      ++m_amount;
      if (m_write == m_size) {
        m_write = 0;
      }
    } else {
      DEB(m_log, "could not add ", p_data, ", because it is full");
    }
  }

  /// \brief Adds a t_data object to the queue
  void add(data &&p_data) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (!full()) {
      m_values[m_write++] = std::move(p_data);
      ++m_amount;
      if (m_write == m_size) {
        m_write = 0;
      }
    } else {
      DEB(m_log, "could not add ", p_data, ", because it is full");
    }
  }

  /// \brief Gets the first t_data obect added to the queue, if there is one
  std::optional<data> get() {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (empty()) {
      DEB(m_log, "could not get because it is empty");
      return {};
    }
    data _data(m_values[m_read++]);
    --m_amount;
    if (m_read == m_size) {
      m_read = 0;
    }
    return {_data};
  }

  /// \brief Traverses the queue
  ///
  /// \param p_function will be called for every data in the queue
  void traverse(std ::function<void(const data &)> p_function) const {
    size _i = m_read;
    size _counter = 0;
    while (_counter < m_amount) {
      if (_i == m_size) {
        _i = 0;
      }
      p_function(m_values[_i++]);
      ++_counter;
    }
  }

  /// \brief Informs if the queue is full
  inline bool full() const { return m_amount == m_size; }

  /// \brief Informs if the queue is empty
  inline bool empty() const { return m_amount == 0; }

  /// \brief Informs the total capacity of the queue
  inline size_t capacity() const { return m_size; }

  /// \brief Informs the current number of slots occupied in the queue
  inline size_t occupied() const { return m_amount; }

private:
  /// \brief Queue implemented as a vector
  typedef std::vector<t_data> values;

  /// \brief Type of size of the queue
  typedef typename values::size_type size;

private:
  /// \brief Size of the buffer
  size m_size = 0;

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

  t_log m_log{"circular_fixed_size_queue_t"};
};

/// \brief Implements a circular queue which size is increased if it becomes
/// full
///
/// \tparam t_log
///
/// \tparam t_data defines the types of the data contained in the buffer
template <typename t_log, typename t_data>
struct circular_unlimited_size_queue_t {

  /// \brief Alias for the data
  typedef t_data data;

  /// \brief Alias for the log
  typedef t_log log;

  /// \brief Constructor
  ///
  /// \param p_size the number of initial slots in the queue
  circular_unlimited_size_queue_t(size_t p_size) {
    m_root = create_node();
    node_ptr _p = m_root;
    for (size_t _i = 1; _i < p_size; ++_i) {
      _p = insert(_p, data());
    }
    m_size = p_size;
    m_write = m_root;
    m_read = m_root;
  }

  /// \brief Traverses the queue
  ///
  /// \param p_function will be called for every data in the queue
  void traverse(std::function<void(const t_data &)> p_visitor) const {
    node_ptr _p = m_root;
    while (_p && (_p->m_next != m_root)) {
      p_visitor(_p->m_data);
      _p = _p->m_next;
    }
    p_visitor(_p->m_data);
  }

  /// \brief Adds a t_data object to the queue
  void add(data &&p_data) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (!full()) {
      DEB(m_log, "not adding more slots");
      m_write->m_data = std::move(p_data);
      m_write = m_write->m_next;
    } else {
      DEB(m_log, "adding more slots");
      insert(m_write->m_prev, std::move(p_data));
    }
    ++m_amount;
  }

  /// \brief Adds a t_data object to the queue
  void add(const data &p_data) {
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
  std::optional<data> get() {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (empty()) {
      DEB(m_log, "empty");
      return {};
    }
    DEB(m_log, "not empty");
    data _data(m_read->m_data);
    m_read = m_read->m_next;
    --m_amount;
    return {_data};
  }

  /// \brief Informs if the queue is full
  inline bool full() const { return m_amount == m_size; }

  /// \brief Informs if the queue is empty
  inline bool empty() const { return m_amount == 0; }

  /// \brief Informs the total capacity of the queue
  inline size_t capacity() const { return m_size; }

  /// \brief Informs the current number of slots occupied in the queue
  inline size_t occupied() const { return m_amount; }

private:
  /// \brief Node of the linked list used to implement the queue
  struct node {
    /// \brief Type of pointer
    typedef std::shared_ptr<node> ptr;

    node() = default;
    node(const node &) = delete;
    node(node &&) = delete;
    node &operator=(const node &) = delete;
    node &operator=(node &&) = delete;

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
  node_ptr insert(node_ptr p_node, t_data &&p_data) {
    node_ptr _new_node = create_node(std::move(p_data));

    _new_node->m_next = p_node->m_next;
    _new_node->m_prev = p_node;

    p_node->m_next->m_prev = _new_node;
    p_node->m_next = _new_node;
    ++m_size;
    return _new_node;
  }

  /// \brief Inserts a node in the list after a node
  ///
  /// \param p_node which the new node will be inserted in front of
  ///
  /// \param p_data data inserted in the new node
  ///
  /// \return the new node
  node_ptr insert(node_ptr p_node, const t_data &p_data) {
    node_ptr _new_node = create_node(p_data);

    _new_node->m_next = p_node->m_next;
    _new_node->m_prev = p_node;

    p_node->m_next->m_prev = _new_node;
    p_node->m_next = _new_node;
    ++m_size;
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
  node_ptr create_node(t_data &&p_data) {
    node_ptr _p(std::make_shared<node>(std::move(p_data)));
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

  /// \brief Size of the queue
  size_t m_size;

  /// \brief Amount of nodes actually used
  size_t m_amount{0};

  /// \brief Controls insertion
  std::mutex m_mutex;

  t_log m_log{"circular_unlimited_size_queue"};
};

template <typename t_log, typename t_data, queue_type qt> struct queue_traits;

template <typename t_log, typename t_data>
struct queue_traits<t_log, t_data, queue_type::CIRCULAR_FIXED_SIZE> {
  typedef circular_fixed_size_queue_t<t_log, t_data> type;
};

template <typename t_log, typename t_data>
struct queue_traits<t_log, t_data, queue_type::CIRCULAR_UNLIMITED_SIZE> {
  typedef circular_unlimited_size_queue_t<t_log, t_data> type;
};

/// \brief worker implements the producer/consumer pattern
///
/// A worker uses some type of queue to cache data in order a bunch of
/// operation compete for the first instance of data inserted into the queue
///
/// \tparam t_log
///
/// \param t_data type of data to be processed
template <typename t_log, queue_type p_queue_type, typename t_data>
class message_queue_t {
public:
  /// \brief Alias for the data
  typedef t_data data;

  /// \brief Alias for a generic queue type, used to order data to be processed
  typedef typename queue_traits<t_log, t_data, p_queue_type>::type queue;

  /// \brief Type of operation that a worker will execute
  typedef std::function<void(data &&)> operation;

  /// \brief Type for the identifier of the worker
  typedef std::string id;

  /// \brief Type fot the priority of a worker
  typedef int16_t priority;

  typedef std::function<void(data &&)> on_timeout;

public:
  /// \brief Constructor
  ///
  /// \param p_id identifier of the worker
  ///
  /// \param p_queue_ptr pointer to the queue used to cache data to be processed
  template <typename t_size>
  message_queue_t(const id &p_id, t_size p_queue_size)
      : m_id(p_id), m_queue(p_queue_size) {}

  /// \brief Constructor
  ///
  /// In this constructor, which is epoch in microseconds is set as the
  /// identifier
  ///
  /// \param p_queue_ptr pointer to the queue used to cache data to be processed
  template <typename t_size>
  message_queue_t(t_size p_queue_size)
      : m_id(std::to_string(uuid())), m_queue(p_queue_size) {}

  /// \brief copy constructor not allowed
  message_queue_t(const message_queue_t &) = delete;

  /// \brief move constructor not allowed
  message_queue_t(message_queue_t &&) = delete;

  /// \brief copy assignemnt not allowed
  message_queue_t &operator=(const message_queue_t &) = delete;

  /// \brief move assignemnt not allowed
  message_queue_t &operator=(message_queue_t &&p_pc) = delete;

  /// \brief destructor
  ///
  /// If 'stop' was not called, empties the queue, waiting for all the data to
  /// be processed
  ~message_queue_t() {
    DEB(m_log, "destructor");
    if (!all_loops_stopped()) {
      if (!m_stopped) {
        while (!m_queue.empty()) {
          m_data_produced.notify_all();
          std::unique_lock<std::mutex> _lock(m_mutex_data);
          m_data_consumed.wait(_lock, [this] { return true; });
        }
        stop();
      }
    }
    DEB(m_log, "leaving destructor");
  }

  /// \brief Equal-to operator
  ///
  /// a worker is equal to another if theirs ids are equals
  bool operator==(const message_queue_t &p_operation) const {
    return m_id == p_operation.m_id;
  }

  /// \brief Not-equal-to operator
  ///
  /// a worker is equal to another if theirs ids are not equals
  bool operator!=(const message_queue_t &p_operation) const {
    return m_id != p_operation.m_id;
  }

  /// \brief Less-than operator
  ///
  /// a worker is less than another if its priority is *higher* than the other;
  /// this makes worker with higher priority numbers to become before workers
  /// with lower priority, i.e., a worker with priority 5 comes before a worker
  /// with priority 2
  bool operator<(const message_queue_t &p_operation) const {
    return m_priority > p_operation.m_priority;
  }

  /// \brief Greater-than operator
  ///
  /// a worker is less than another if its priority is *lower* than the other;
  /// this makes worker with higher priority numbers to become before workers
  /// with lower priority, i.e., a worker with priority 5 comes before a worker
  /// with priority 2
  bool operator>(const message_queue_t &p_operation) const {
    return m_priority < p_operation.m_priority;
  }

  /// \brief Defines the priority for this worker
  void set_priority(priority p_priority) { m_priority = p_priority; }

  /// \brief Defines the priority for this worker
  priority get_priority() { return m_priority; }

  /// \brief adds data to be passed to an operation
  ///
  /// \p data will only be added if \p start() was called
  ///
  /// \param p_data is the data to be passed to an operation
  ///
  /// \return \p true if it was added, \p false otherwise
  bool add(const data &p_data) {
    data _data{p_data};
    if (m_stopped) {
      ERR(m_log, "could not add data ", _data,
          " because 'worker' is not running; call 'start()' first");
      return false;
    }

    std::unique_lock<std::mutex> _lock_stop(m_mutex_stop);

    DEB(m_log, "waiting for room to add ", _data);
    {
      std::unique_lock<std::mutex> _lock(m_mutex_data);
      m_data_consumed.wait(_lock,
                           [this]() { return (!m_queue.full() || m_stopped); });

      if (m_stopped) {
        DEB(m_log, "stopped when about to add ", _data);
      } else {
        DEB(m_log, "adding ", _data);
        m_queue.add(_data);
        ++m_queued_data;
      }
    }
    // notifyng that new data is available
    DEB(m_log, "notifying");
    m_data_produced.notify_all();

    return true;
  }

  /// \brief adds data to be passed to an operation
  ///
  /// \p data will only be added if \p start() was called
  ///
  /// \param p_data is the data to be passed to an operation
  ///
  /// \return \p true if it was added, \p false otherwise
  bool add(data &&p_data) {
    data _data{p_data};
    if (m_stopped) {
      ERR(m_log, "could not add data ", _data,
          " because 'worker' is not running; call 'start()' first");
      return false;
    }

    std::unique_lock<std::mutex> _lock_stop(m_mutex_stop);

    DEB(m_log, "waiting for room to add ", _data);
    {
      std::unique_lock<std::mutex> _lock(m_mutex_data);
      m_data_consumed.wait(_lock,
                           [this]() { return (!m_queue.full() || m_stopped); });

      if (m_stopped) {
        DEB(m_log, "stopped when about to add ", _data);
      } else {
        DEB(m_log, "adding ", _data);
        m_queue.add(std::move(_data));
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

  /// \brief add adds one \p operation
  ///
  /// \tparam t_time type of time used to define timeout for the operation
  ///
  /// \param p_operation the \p operation to be added
  ///
  /// \param p_timeout timeout of \p operation
  /// \brief common function called to add a \p operation
  ///
  /// \param p_loop the new \p operation to be added
  template <typename t_time>
  void add(operation p_operation, t_time p_timeout, on_timeout p_on_timeout) {
    auto _provider = [this]() -> std::optional<std::tuple<data>> {
      return this->provider();
    };
    std::lock_guard<std::mutex> _lock(m_add_work);

    m_loops.push_back({p_operation, p_timeout, p_on_timeout, _provider});
  }

  /// \brief adds a bunch of \p operation
  ///
  /// \tparam t_time type of time used to define timeout for the operation
  ///
  /// \param p_num_works the number of \p operations to be added
  ///
  /// \param p_operation_factory a function that creates \p operations
  ///
  /// \param p_timeout timeout for the operations
  ///
  /// \param p_on_timeout function to be called when the operation times
  /// out
  template <typename t_time>
  void add(uint16_t p_num_works, std::function<operation()> p_operation_factory,
           t_time p_timeout, on_timeout p_on_timeout) {

    std::unique_lock<std::mutex> _lock_stop(m_mutex_stop);

    auto _provider = [this]() -> std::optional<std::tuple<data>> {
      return this->provider();
    };

    for (auto _i = 0; _i < p_num_works; ++_i) {

      add(p_operation_factory(), p_timeout, p_on_timeout);
    }
  }

  /// \brief informs if the worker is stopped
  inline bool is_stopped() const { return m_stopped; }

  /// \brief starts the worker
  ///
  /// From this call on, the operations will compete among each other, in order
  /// to process any instance of \p data that was inserted into the queue
  ///
  /// \return \p false if it not start because no \p operation was added
  ///         \p true if it was already started, or if it started successfully
  bool start() {
    if (!m_stopped) {
      ERR(m_log, "not starting because it is already running");
      return true;
    }

    std::unique_lock<std::mutex> _lock(m_mutex_stop);
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
  /// From this call on, the \p operations will stop competing among each other,
  /// in order to process any instance of \p data that was inserted into the
  /// queue
  void stop() {

    std::unique_lock<std::mutex> _lock(m_mutex_stop);
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
  inline auto capacity() const { return m_queue.capacity(); }

  /// \brief the amount of slots occupied in the \p queue
  inline auto occupied() const { return m_queue.occupied(); }

private:
  /// \brief async_loop_t is a \p async_loop where a \p operation will be
  /// running
  typedef async_loop_t<t_log, data> async_loop;

  /// \brief async_loops_t is the collection of \p async_loop pointers
  typedef typename std::vector<async_loop> asyncs_loops;

private:
  /// \brief provider provides data, if available, to a \p operation
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
      if (!m_queue.empty()) {
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

    std::optional<data> _maybe = m_queue.get();
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
  /// \brief identifier of the worker
  id m_id;

  /// \brief priority of this worker
  priority m_priority{0};

  /// \brief queue where data will be inserted for the operation to compete for
  queue m_queue;

  /// \brief controls access to the \p m_loops while inserting a new \p
  /// operation
  std::mutex m_add_work;

  /// \brief controls access to inserting data
  std::mutex m_mutex_data;

  /// \brief controls access to attributes while the operation is stopping
  std::mutex m_mutex_stop;

  /// \brief controls access to the data consumed
  std::condition_variable m_data_consumed;

  /// \brief controls access to the data produced
  std::condition_variable m_data_produced;

  /// \brief asynchronous loops, where the operations are running
  asyncs_loops m_loops;

  /// \brief indicates if the worker is running
  bool m_stopped{true};

  /// \brief m_destroying indicates that the \p produce_consumer should stop
  bool m_destroying{false};

  /// \brief amount of queued data
  size_t m_queued_data{0};

  t_log m_log{"message_queue"};
};

} // namespace concurrent
} // namespace tenacitas

#endif
