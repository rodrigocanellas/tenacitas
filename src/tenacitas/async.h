#ifndef TENACITAS_ASYNC_H
#define TENACITAS_ASYNC_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <list>
#include <mutex>
#include <optional>
#include <set>
#include <thread>
#include <vector>

#include <tenacitas/calendar.h>
#include <tenacitas/logger.h>
#include <tenacitas/macros.h>

using namespace std::chrono_literals;

/// TODO improve documentation

/// \brief master namespace
namespace tenacitas {

/// \brief support for async (asynchronous) programming
namespace async {

typedef std::function<bool()> breaker;

/// \brief Type of time used to define timeout
typedef std::chrono::milliseconds timeout;

/// \brief Type of time used to define interval
typedef std::chrono::milliseconds interval;

struct id {
  id() : m_value(std::to_string(number::uuid())) {}

  explicit id(const std::string &p_value) : m_value(p_value) {}

  explicit id(std::string &&p_value) : m_value(std::move(p_value)) {}

  explicit id(const char *p_value) : m_value(p_value) {}

  id(const id &) = default;
  id(id &&) = default;

  ~id() = default;

  friend std::ostream &operator<<(std::ostream &p_out, id p_id) {
    p_out << p_id.m_value;
    return p_out;
  }

  bool operator==(const id &p_id) const { return m_value == p_id.m_value; }

  bool operator!=(const id &p_id) const { return m_value != p_id.m_value; }

  id &operator=(const id &) = default;
  id &operator=(id &&) = delete;

private:
  std::string m_value;
};

struct priority {

  priority() = delete;

  priority(const priority &) = default;
  priority(priority &&) = default;

  ~priority() = default;

  friend std::ostream &operator<<(std::ostream &p_out, priority p_priority) {
    p_out << static_cast<uint16_t>(p_priority.m_value);
    return p_out;
  }

  bool operator<(priority p_priority) const {
    return m_value < p_priority.m_value;
  }

  bool operator>(priority p_priority) const {
    return m_value > p_priority.m_value;
  }

  bool operator==(priority p_priority) const {
    return m_value == p_priority.m_value;
  }

  bool operator!=(priority p_priority) const {
    return m_value != p_priority.m_value;
  }

  priority &operator=(const priority &) = default;
  priority &operator=(priority &&) = default;

  static const priority lowest;

  static const priority low;

  static const priority low_middle;

  static const priority middle;

  static const priority middle_high;

  static const priority high;

  static const priority highest;

private:
  explicit priority(uint8_t p_value) : m_value(p_value) {}

private:
  uint8_t m_value;
};

const priority priority::lowest{1};
const priority priority::low{45};
const priority priority::low_middle{90};
const priority priority::middle{135};
const priority priority::middle_high{180};
const priority priority::high{225};
const priority priority::highest{255};

namespace internal {

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
///#include <tenacitas/async.h>
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
///    std::optional<double> _maybe = async::execute(300ms, f1, 8);
///    if (_maybe) {
///      std::cout << *_maybe;
///    } else {
///      std::cout << " timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    std::optional<double> _maybe = async::execute(300ms, f2, 8, 4.2f);
///    if (_maybe) {
///      std::cout << *_maybe;
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    std::optional<double> _maybe = async::execute(300ms, f3);
///    if (_maybe) {
///      std::cout << *_maybe;
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    if (async::execute(300ms, f4, -8)) {
///      std::cout << "ok";
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    if (async::execute(300ms, f5, -8, 3.56)) {
///      std::cout << "ok";
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    if (async::execute(300ms, f6)) {
///      std::cout << "ok";
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    std::optional<double> _maybe = async::execute(300ms, f7, 8);
///    if (_maybe) {
///      std::cout << "with exec: " << *_maybe;
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    std::optional<double> _maybe = async::execute(300ms, f8, 8, 4.2f);
///    if (_maybe) {
///      std::cout << *_maybe;
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    std::optional<double> _maybe = async::execute(300ms, f9);
///    if (_maybe) {
///      std::cout << *_maybe;
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    if (async::execute(300ms, f10, -18)) {
///      std::cout << "ok";
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    if (async::execute(300ms, f11, -18, 3.3)) {
///      std::cout << "ok";
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    if (async::execute(300ms, f12)) {
///      std::cout << "ok";
///    } else {
///      std::cout << "timeout!";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    bool b{false};
///    if (async::execute(300ms, f13, std::ref(b))) {
///      std::cout << "ok";
///    } else {
///      std::cout << "timeout";
///    }
///    std::cout << std::endl;
///  }
///
///  {
///    bool b{false};
///    if (async::execute(300ms, f14, std::move(b))) {
///      std::cout << "ok";
///    } else {
///      std::cout << "timeout";
///    }
///    std::cout << std::endl;
///  }
///  {
///    bool b{false};
///    if (async::execute(300ms, f15, std::ref(b))) {
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

namespace internal {

/// \brief Base class for asynchronous loop
/// An asynchronous loop class runs a loop apart from the caller code. A
/// function is set to be executed in each round of the loop.
///
/// \tparam t_function function to be executed in each round of the loop
///
/// \tparam t_on_timeout function to be called if the function being executed in
/// the loop times out
template <typename t_function, typename t_on_timeout> struct loop_base_t {

  /// \brief default contructor not allowed
  loop_base_t() = delete;

  /// \brief copy contructor not allowed
  loop_base_t(const loop_base_t &) = delete;

  /// \brief move contructor not allowed
  loop_base_t(loop_base_t &&) = delete;

  /// \brief copy assignment operator not allowed
  loop_base_t &operator=(const loop_base_t &) = delete;

  /// \brief move assignment operator not allowed
  loop_base_t &operator=(loop_base_t &&) = delete;

  /// \brief destructor
  virtual ~loop_base_t() {
    DEB(m_log, m_id, " - destructor");
    stop();
    DEB(m_log, m_id, " - leaving destructor");
  }

  /// \brief Retieves the identifier of the current object
  /// The identifier is (was) used for debuging purposes
  ///
  /// \return identifier of the current object
  inline uint64_t get_id() const { return m_id; }

  /// \brief Starts the loop asynchronously, if the loop was already started
  void start() {
    if (!m_stopped) {
      return;
    }
    m_stopped = false;

    DEB(m_log, m_id, " - starting loop thread");
    m_thread = std::thread([this]() { loop(); });
  }

  /// \brief Stops the loop, if it was started
  void stop() {

    if (m_stopped) {
      DEB(m_log, m_id, " - not stopping because it is stopped");
      return;
    }
    DEB(m_log, m_id, " - stopping");
    m_stopped = true;

    if (m_thread.get_id() == std::thread::id()) {
      DEB(m_log, m_id,
          " - not joining because m_thread.get_id() == std::thread::id()");
      return;
    }
    DEB(m_log, m_id, " - m_thread.get_id() != std::thread::id()");
    if (!m_thread.joinable()) {
      DEB(m_log, m_id, " - not joining because m_thread is not joinable");
      return;
    }
    DEB(m_log, m_id, " - m_thread.joinable()");
    {
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_thread.join();
    }
    DEB(m_log, "leaving stop");
  }

  /// \brief Informs if the loop is stopped
  inline bool is_stopped() const { return m_stopped; }

  /// \brief Defines the timeout for the function being executed in the loop
  ///
  /// \tparam t_time is the type of time used to define the timeout. It must be
  /// one of the defined in std::chrono, like std::chrono::seconds
  ///
  /// \param p_timeout is the value of the timeout
  template <typename t_time> inline void set_timeout(t_time p_timeout) {
    m_timeout = calendar::convert<timeout>(p_timeout);
  }

  /// \brief Retrieves the timeout for the function being executed in the loop
  ///
  /// \tparam t_time is the type of time to which the timeout should be
  /// converted to. It must be one of the defined in std::chrono, like
  /// std::chrono::seconds
  template <typename t_time> inline t_time get_timeout() const {
    return calendar::convert<t_time>(m_timeout);
  }

protected:
  /// \brief Constuctor
  ///
  /// \tparam t_time is the type of time used to define the timeout. It must be
  /// one of the defined in std::chrono, like std::chrono::seconds
  ///
  /// \param p_function is the function to be executed in each run of the loop
  ///
  /// \param p_time is the amount of time \p p_function has to executed in each
  /// round of the loop
  ///
  /// \param p_on_timeout is the function to be called it \p p_function fails to
  /// execute in \p p_time
  template <typename t_time>
  loop_base_t(t_function p_function, t_time p_time, t_on_timeout p_on_timeout)
      : m_function(p_function), m_timeout(calendar::convert<timeout>(p_time)),
        m_on_timeout(p_on_timeout), m_breaker([]() -> bool { return false; }) {}

  /// \brief Constuctor
  ///
  /// \tparam t_time is the type of time used to define the timeout. It must be
  /// one of the defined in std::chrono, like std::chrono::seconds
  ///
  /// \param p_function is the function to be executed in each run of the loop
  ///
  /// \param p_time is the amount of time \p p_function has to executed in each
  /// round of the loop
  ///
  /// \param p_on_timeout is the function to be called it \p p_function fails to
  /// execute in \p p_time
  ///
  /// \param p_breaker is a function that will also be execute at each round of
  /// the loop, allowing it to be "breaked"
  template <typename t_time>
  loop_base_t(t_function p_function, t_time p_time, t_on_timeout p_on_timeout,
              breaker p_breaker)
      : m_function(p_function), m_timeout(calendar::convert<timeout>(p_time)),
        m_on_timeout(p_on_timeout), m_breaker(p_breaker) {}

  /// \brief Method that implements the loop that runs aynchronously
  virtual void loop() = 0;

protected:
  /// \brief Function that will be called in each round of the loop
  t_function m_function;

  /// \brief Maximum time that \p m_function will have to execute
  timeout m_timeout;

  /// \brief Function that will be called if \p m_function takes more than \p
  /// m_timeout to execute
  t_on_timeout m_on_timeout;

  /// \brief Function that will also be execute at each round of the loop,
  /// allowing it to be "breaked"
  breaker m_breaker;

  /// \brief Indicates that the loop should be stopped
  std::atomic<bool> m_stopped{true};

  /// \brief Thread where the \p loop method will run
  std::thread m_thread;

  /// \brief Logger
  logger::cerr<> m_log{"loop"};

  /// \brief Synchronizes execution
  std::mutex m_mutex;

  /// \brief Identifier of this object, used for debugging purposes
  uint64_t m_id{number::uuid()};
};

/// \brief Asynchronous loop with function that takes parameters
///
/// \tparams are the types of the parameters that the function that will be
/// executed in each round of the loop expects
///
/// \sa loop_base_t
template <typename... t_params> struct loop_t {

  /// \brief Signature of the function that will be called in each round of the
  /// loop
  typedef std::function<void(t_params...)> function;

  /// \brief Signature of the function that will be called if the function
  /// called in the loop times out
  typedef std::function<void(t_params...)> on_timeout;

  /// \brief Signature of the function that will be called in the loop to
  /// provide the parameters for the function
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

  /// \brief Constructor
  ///
  /// \tparam t_time is the type of time used to define the timeout. It must be
  /// one of the defined in std::chrono, like std::chrono::seconds
  ///
  /// \param p_function is the function to be executed in each run of the loop
  ///
  /// \param p_time is the amount of time \p p_function has to executed in each
  /// round of the loop
  ///
  /// \param p_on_timeout is the function to be called it \p p_function fails to
  /// execute in \p p_time
  ///
  /// \param p_provider is the function that will be called in the loop to
  /// provide the parameters for the function
  template <typename t_time>
  loop_t(function p_function, t_time p_timeout, on_timeout p_on_timeout,
         provider p_provider)
      : m_impl(std::make_unique<implementation>(p_function, p_timeout,
                                                p_on_timeout, p_provider)) {}

  /// \brief Constructor
  ///
  /// \tparam t_time is the type of time used to define the timeout. It must be
  /// one of the defined in std::chrono, like std::chrono::seconds
  ///
  /// \param p_function is the function to be executed in each run of the loop
  ///
  /// \param p_time is the amount of time \p p_function has to executed in each
  /// round of the loop
  ///
  /// \param p_on_timeout is the function to be called it \p p_function fails to
  /// execute in \p p_time
  ///
  /// \param p_provider is the function that will be called in the loop to
  /// provide the parameters for the function
  ///
  /// \param p_breaker is a function that will also be execute at each round of
  /// the loop, allowing it to be "breaked"
  template <typename t_time>
  loop_t(function p_function, t_time p_timeout, on_timeout p_on_timeout,
         provider p_provider, breaker p_breaker)
      : m_impl(std::make_unique<implementation>(
            p_function, p_timeout, p_on_timeout, p_provider, p_breaker)) {}

  /// \brief Default constructor not allowed
  loop_t() = delete;

  ~loop_t() = default;

  /// \brief Copy constructor not allowed
  loop_t(const loop_t &) = delete;

  loop_t(loop_t &&p_async_loop) = default;

  loop_t &operator=(loop_t &&p_async_loop) = default;

  /// \brief Copy assignment allowed
  loop_t &operator=(const loop_t &) = delete;

  /// \brief Starts the loop
  inline void start() { m_impl->start(); }

  /// \brief Retieves the identifier of the current object
  /// The identifier is (was) used for debuging purposes
  ///
  /// \return identifier of the current object
  inline uint64_t get_id() const { return m_impl->get_id(); }

  /// \brief Stops the loop
  inline void stop() { m_impl->stop(); }

  /// \brief Informs if the loop is stopped
  inline bool is_stopped() const { return m_impl->is_stopped(); }

  /// \brief Defines the timeout for the function being executed in the loop
  ///
  /// \tparam t_time is the type of time used to define the timeout. It must be
  /// one of the defined in std::chrono, like std::chrono::seconds
  ///
  /// \param p_timeout is the value of the timeout
  template <typename t_time> inline void set_timeout(t_time p_timeout) {
    m_impl->set_timeout(p_timeout);
  }

  /// \brief Retrieves the timeout for the function being executed in the loop
  ///
  /// \tparam t_time is the type of time to which the timeout should be
  /// converted to. It must be one of the defined in std::chrono, like
  /// std::chrono::seconds
  template <typename t_time> inline t_time get_timeout() const {
    return m_impl->template get_timeout<t_time>();
  }

  /// \brief Informs if the loop is valid
  inline operator bool() const { return m_impl != nullptr; }

private:
  /// \brief Implementation of the loop
  /// Using an internal implentation makes it easy to move loop objects
  struct implementation : public loop_base_t<function, on_timeout> {

    /// \brief Constructor
    ///
    /// \tparam t_time is the type of time used to define the timeout. It must
    /// be one of the defined in std::chrono, like std::chrono::seconds
    ///
    /// \param p_function is the function to be executed in each run of the loop
    ///
    /// \param p_time is the amount of time \p p_function has to executed in
    /// each round of the loop
    ///
    /// \param p_on_timeout is the function to be called it \p p_function fails
    /// to execute in \p p_time
    ///
    /// \param p_provider is the function that will be called in the loop to
    /// provide the parameters for the function
    template <typename t_time>
    implementation(function p_function, t_time p_timeout,
                   on_timeout p_on_timeout, provider p_provider)
        : internal::loop_base_t<function, on_timeout>(p_function, p_timeout,
                                                      p_on_timeout),
          m_provider(p_provider) {}

    /// \brief Constructor
    ///
    /// \tparam t_time is the type of time used to define the timeout. It must
    /// be one of the defined in std::chrono, like std::chrono::seconds
    ///
    /// \param p_function is the function to be executed in each run of the loop
    ///
    /// \param p_time is the amount of time \p p_function has to executed in
    /// each round of the loop
    ///
    /// \param p_on_timeout is the function to be called it \p p_function fails
    /// to execute in \p p_time
    ///
    /// \param p_provider is the function that will be called in the loop to
    /// provide the parameters for the function
    ///
    /// \param p_breaker is a function that will also be execute at each round
    /// of the loop, allowing it to be "breaked"
    template <typename t_time>
    implementation(function p_function, t_time p_timeout,
                   on_timeout p_on_timeout, provider p_provider,
                   breaker p_breaker)
        : internal::loop_base_t<function, on_timeout>(p_function, p_timeout,
                                                      p_on_timeout, p_breaker),
          m_provider(p_provider) {}

  protected:
    /// \brief Specific implementation of the loop virtual method
    void loop() override {
      DEB(this->m_log, this->m_id, " - entering loop");

      while (true) {
        std::lock_guard<std::mutex> _lock(this->m_mutex);

        if (this->m_stopped || this->m_breaker()) {
          DEB(this->m_log, this->m_id, " - stopped? ", this->m_stopped);
          break;
        }

        DEB(this->m_log, this->m_id, " - calling provider");
        std::optional<std::tuple<t_params...>> _maybe_data{m_provider()};

        if (this->m_stopped || this->m_breaker()) {
          DEB(this->m_log, this->m_id, " - stopped? ", this->m_stopped);
          break;
        }

        if (_maybe_data) {
          std::tuple<t_params...> _params{std::move(*_maybe_data)};

          DEB(this->m_log, this->m_id, " - data provided = ", _params);

          if (this->m_stopped || this->m_breaker()) {
            DEB(this->m_log, this->m_id, " - stopped? ", this->m_stopped);
            break;
          }

          DEB(this->m_log, this->m_id, " - still here");

          if (!execute(this->m_timeout, [this, &_params]() -> void {
                if (this->m_stopped || this->m_breaker()) {
                  DEB(this->m_log, this->m_id, " - stopped? ", this->m_stopped);
                  return;
                }
                DEB(this->m_log, this->m_id, " - still here");
                std::apply(this->m_function, _params);
              })) {
            WAR(this->m_log, this->m_id,
                " - timeout for worker with data = ", _params);

            if (this->m_stopped || this->m_breaker()) {
              DEB(this->m_log, this->m_id, " - stopped? ", this->m_stopped);
              break;
            }

            auto _on_timeout = [this, &_params]() {
              std::apply(this->m_on_timeout, std::move(_params));
            };

            if (this->m_stopped || this->m_breaker()) {
              DEB(this->m_log, this->m_id, " - stopped? ", this->m_stopped);
              break;
            }

            std::thread([_on_timeout]() -> void { _on_timeout(); }).detach();
          }
        }
      }
      DEB(this->m_log, this->m_id, " - leaving loop");
    }

  private:
    /// \brief Function that will be called in the loop to provide the
    /// parameters for the function
    provider m_provider;
  };

private:
  /// \brief Pointer to the implementation
  std::unique_ptr<implementation> m_impl;
};

/// \brief Asynchronous loop with function that takes no parameters
///
/// \sa loop_base_t
template <> struct loop_t<void> {

  /// \brief Signature of the function that will be called in each round of the
  /// loop
  typedef std::function<void()> function;

  /// \brief Signature of the function that will be called if the function
  /// called in the loop times out
  typedef std::function<void()> on_timeout;

  /// \brief Default constructor not allowed
  loop_t() = delete;

  /// \brief Constructor
  ///
  /// \tparam t_time is the type of time used to define the timeout. It must
  /// be one of the defined in std::chrono, like std::chrono::seconds
  ///
  /// \param p_function is the function to be executed in each run of the loop
  ///
  /// \param p_time is the amount of time \p p_function has to executed in
  /// each round of the loop
  ///
  /// \param p_on_timeout is the function to be called it \p p_function fails
  /// to execute in \p p_time
  template <typename t_time>
  loop_t(function p_function, t_time p_timeout, on_timeout p_on_timeout)
      : m_impl(std::make_unique<implementation>(p_function, p_timeout,
                                                p_on_timeout)) {}

  /// \brief Constructor
  ///
  /// \tparam t_time is the type of time used to define the timeout. It must
  /// be one of the defined in std::chrono, like std::chrono::seconds
  ///
  /// \param p_function is the function to be executed in each run of the loop
  ///
  /// \param p_time is the amount of time \p p_function has to executed in
  /// each round of the loop
  ///
  /// \param p_on_timeout is the function to be called it \p p_function fails
  /// to execute in \p p_time
  ///
  /// \param p_breaker is a function that will also be execute at each round
  /// of the loop, allowing it to be "breaked"
  template <typename t_time>
  loop_t(function p_function, t_time p_timeout, on_timeout p_on_timeout,
         breaker p_breaker)
      : m_impl(std::make_unique<implementation>(p_function, p_timeout,
                                                p_on_timeout, p_breaker)) {}

  ~loop_t() = default;

  /// \brief Copy constructor not allowed
  loop_t(const loop_t &) = delete;

  loop_t(loop_t &&) = default;

  /// \brief Move constructor not allowed
  loop_t &operator=(loop_t &&) = default;

  loop_t &operator=(const loop_t &) = delete;

  /// \brief Retieves the identifier of the current object
  /// The identifier is (was) used for debuging purposes
  ///
  /// \return identifier of the current object
  inline uint64_t get_id() const { return m_impl->get_id(); }

  /// \brief Starts the loop
  inline void start() { m_impl->start(); }

  /// \brief Stops the loop
  inline void stop() { m_impl->stop(); }

  /// \brief Informs if the loop is stopped
  inline bool is_stopped() const { return m_impl->is_stopped(); }

  /// \brief Defines the timeout for the function being executed in the loop
  ///
  /// \tparam t_time is the type of time used to define the timeout. It must be
  /// one of the defined in std::chrono, like std::chrono::seconds
  ///
  /// \param p_timeout is the value of the timeout
  template <typename t_time> inline void set_timeout(t_time p_timeout) {
    m_impl->set_timeout(p_timeout);
  }

  /// \brief Retrieves the timeout for the function being executed in the loop
  ///
  /// \tparam t_time is the type of time to which the timeout should be
  /// converted to. It must be one of the defined in std::chrono, like
  /// std::chrono::seconds
  template <typename t_time> inline t_time get_timeout() const {
    return m_impl->template get_timeout<t_time>();
  }

  /// \brief Informs if the loop is valid
  inline operator bool() const { return m_impl != nullptr; }

private:
  /// \brief Implementation of the loop
  /// Using an internal implentation makes it easy to move loop objects
  struct implementation : public loop_base_t<function, on_timeout> {

    /// \brief Constructor
    ///
    /// \tparam t_time is the type of time used to define the timeout. It must
    /// be one of the defined in std::chrono, like std::chrono::seconds
    ///
    /// \param p_function is the function to be executed in each run of the loop
    ///
    /// \param p_time is the amount of time \p p_function has to executed in
    /// each round of the loop
    ///
    /// \param p_on_timeout is the function to be called it \p p_function fails
    /// to execute in \p p_time
    template <typename t_time>
    implementation(function p_function, t_time p_timeout,
                   on_timeout p_on_timeout)
        : loop_base_t<function, on_timeout>(p_function, p_timeout,
                                            p_on_timeout) {}

    /// \brief Constructor
    ///
    /// \tparam t_time is the type of time used to define the timeout. It must
    /// be one of the defined in std::chrono, like std::chrono::seconds
    ///
    /// \param p_function is the function to be executed in each run of the loop
    ///
    /// \param p_time is the amount of time \p p_function has to executed in
    /// each round of the loop
    ///
    /// \param p_on_timeout is the function to be called it \p p_function fails
    /// to execute in \p p_time
    ///
    /// \param p_breaker is a function that will also be execute at each round
    /// of the loop, allowing it to be "breaked"
    template <typename t_time>
    implementation(function p_function, t_time p_timeout,
                   on_timeout p_on_timeout, breaker p_breaker)
        : loop_base_t<function, on_timeout>(p_function, p_timeout, p_on_timeout,
                                            p_breaker) {}

  protected:
    /// \brief Specific implementation of the loop virtual method
    void loop() override {
      DEB(m_log, this->m_id, " - entering loop");

      auto _worker = [this]() -> void { this->m_function(); };

      while (true) {

        if (this->m_stopped || this->m_breaker()) {
          DEB(this->m_log, this->m_id, " - stopped? ", this->m_stopped);
          break;
        }

        if (!execute(this->m_timeout, _worker)) {
          std::thread([this]() -> void { this->m_on_timeout(); }).detach();
        }
      }
      DEB(m_log, this->m_id, " - leaving loop");
    }
  };

private:
  /// \brief Pointer to the implementation
  std::unique_ptr<implementation> m_impl;
};

/// \brief Executes function in fixed period of times
///
/// \tparam t_on_timeout is a function that will be executed if the function
/// times out
///
/// \tparam t_params are the types of parameters that the function expects
template <typename t_on_timeout, typename... t_params>
struct sleeping_loop_base_t {

  /// \brief Default constructor not allowed
  sleeping_loop_base_t() = delete;

  /// \brief Copy constructor not allowed
  sleeping_loop_base_t(const sleeping_loop_base_t &) = delete;

  /// \brief Move constructor not allowed
  sleeping_loop_base_t(sleeping_loop_base_t &&p_sleep) = delete;

  /// \brief Copy assignment not allowed
  sleeping_loop_base_t &operator=(const sleeping_loop_base_t &) = delete;

  /// \brief Move assignment not allowed
  sleeping_loop_base_t &operator=(sleeping_loop_base_t &&p_sleep) = delete;

  /// \brief Destructor
  /// Interrupts the loop
  inline virtual ~sleeping_loop_base_t() {
    DEB(m_log, "destructor");
    stop();
    DEB(m_log, "leaving destructor");
  }

  /// \brief Retrieves the interval defined for the loop to sleep between
  /// the execution of the worker
  ///
  /// \tparam t_time is the type of time one wants the interval converted to.
  /// It must be one of the defined in std::chrono, like std::chrono::seconds
  ///
  /// \return interval of repetition of the loop, in \p t_time measure
  template <typename t_time> inline t_time get_interval() const {
    return calendar::convert<t_time>(m_interval);
  }

  /// \brief Redefines the value of the execution interval
  ///
  /// \tparam t_time is the type of time used to define the interval. It
  /// must be one of the defined in std::chrono, like std::chrono::seconds
  ///
  /// \param p_interval value of the interval of repetition
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  template <typename t_time> inline void set_interval(t_time p_interval) {
    m_interval = calendar::convert<interval>(p_interval);
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

  /// \brief Retrieves if the loop is stopped
  inline bool is_stopped() const { return m_async.is_stopped(); }

  /// \brief Retrieves the timeout for the worker
  ///
  /// \tparam t_time is the type of time one wants the timeout converted to.
  /// It must be one of the defined in std::chrono, like std::chrono::seconds
  ///
  /// \return timeout for the function executed in the loop
  template <typename t_time> inline t_time get_timeout() const {
    return m_async.template get_timeout<t_time>();
  }

  /// \brief Redefines the value of the timeout
  ///
  /// \tparam t_time is the type of time used to define the timeout. It
  /// must be one of the defined in std::chrono, like std::chrono::seconds
  ///
  /// \param p_timeout is the value for the timeout of the function executed in
  /// the loop
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  template <typename t_time> inline void set_timeout(t_time p_timeout) {
    m_async.set_timeout(p_timeout);
  }

protected:
  /// \brief Constructor
  ///
  /// \tparam t_timeout is the type of time used to define the timeout. It
  /// must be one of the defined in std::chrono, like std::chrono::seconds
  ///
  /// \tparam t_interval is the type of time one wants the interval converted
  /// to. It must be one of the defined in std::chrono, like
  /// std::chrono::seconds
  ///
  /// \tparam t_function is the function executed in the loop, each time it is
  /// executed
  ///
  /// \param p_interval is the value of the interval of time that the loop will
  /// execute
  ///
  /// \param p_function is the function that will be executed in the loop, at
  /// each \p p_interval
  ///
  /// \param p_timeout is the maximum amount of time that \p p_function has to
  /// execute
  ///
  /// \param p_on_timeout is the function that will be execute, if \p p_function
  /// does not execute in \p p_timeout
  template <typename t_interval, typename t_timeout, typename t_function>
  sleeping_loop_base_t(t_interval p_interval, t_function p_function,
                       t_timeout p_timeout, t_on_timeout p_on_timeout)
      : m_interval(calendar::convert<interval>(p_interval)),
        m_async(p_function, calendar::convert<timeout>(p_timeout), p_on_timeout,
                [this]() -> bool { return breaker(); }) {
    DEB(m_log, "constructor without provider called");
  }

  /// \brief Constructor
  ///
  /// \tparam t_timeout is the type of time used to define the timeout. It
  /// must be one of the defined in std::chrono, like std::chrono::seconds
  ///
  /// \tparam t_interval is the type of time one wants the interval converted
  /// to. It must be one of the defined in std::chrono, like
  /// std::chrono::seconds
  ///
  /// \tparam t_function is the function executed in the loop, each time it is
  /// executed
  ///
  /// \tparam t_provider is the type of function that will provide the
  /// parameters that the function expects
  ///
  /// \param p_interval is the value of the interval of time that the loop will
  /// execute
  ///
  /// \param p_function is the function that will be executed in the loop, at
  /// each \p p_interval
  ///
  /// \param p_timeout is the maximum amount of time that \p p_function has to
  /// execute
  ///
  /// \param p_on_timeout is the function that will be execute, if \p p_function
  /// does not execute in \p p_timeout
  ///
  /// \param p_provider is the function that will provide the parameters that
  /// the function expects
  template <typename t_timeout, typename t_interval, typename t_function,
            typename t_provider>
  sleeping_loop_base_t(t_interval p_interval, t_function p_function,
                       t_timeout p_timeout, t_on_timeout p_on_timeout,
                       t_provider p_provider)
      : m_interval(calendar::convert<interval>(p_interval)),
        m_async(p_function, calendar::convert<timeout>(p_timeout), p_on_timeout,
                p_provider, [this]() -> bool { return breaker(); }) {
    DEB(m_log, "constructor with provider called");
  }

  /// \brief Constructor
  /// If the function times out, the on_timeout will ignore its execution
  ///
  /// \tparam t_timeout is the type of time used to define the timeout. It
  /// must be one of the defined in std::chrono, like std::chrono::seconds
  ///
  /// \tparam t_interval is the type of time one wants the interval converted
  /// to. It must be one of the defined in std::chrono, like
  /// std::chrono::seconds
  ///
  /// \tparam t_function is the function executed in the loop, each time it is
  /// executed
  ///
  /// \param p_interval is the value of the interval of time that the loop will
  /// execute
  ///
  /// \param p_function is the function that will be executed in the loop, at
  /// each \p p_interval
  ///
  /// \param p_timeout is the maximum amount of time that \p p_function has to
  /// execute
  template <typename t_timeout, typename t_interval, typename t_function>
  sleeping_loop_base_t(t_interval p_interval, t_function p_function,
                       t_timeout p_timeout)
      : m_interval(calendar::convert<interval>(p_interval)),
        m_async(
            p_function, calendar::convert<timeout>(p_timeout), []() {},
            [this]() -> bool { return breaker(); }) {
    DEB(m_log, "constructor without provider called");
  }

  /// \brief Constructor
  /// If the function times out, the on_timeout will ignore its execution
  ///
  /// \tparam t_timeout is the type of time used to define the timeout. It
  /// must be one of the defined in std::chrono, like std::chrono::seconds
  ///
  /// \tparam t_interval is the type of time one wants the interval converted
  /// to. It must be one of the defined in std::chrono, like
  /// std::chrono::seconds
  ///
  /// \tparam t_function is the function executed in the loop, each time it is
  /// executed
  ///
  /// \tparam t_provider is the type of function that will provide the
  /// parameters that the function expects
  ///
  /// \param p_interval is the value of the interval of time that the loop will
  /// execute
  ///
  /// \param p_function is the function that will be executed in the loop, at
  /// each \p p_interval
  ///
  /// \param p_timeout is the maximum amount of time that \p p_function has to
  /// execute
  ///
  /// \param p_on_timeout is the function that will be execute, if \p p_function
  /// does not execute in \p p_timeout
  ///
  /// \param p_provider is the function that will provide the parameters that
  /// the function expects
  template <typename t_timeout, typename t_interval, typename t_function,
            typename t_provider>
  sleeping_loop_base_t(t_interval p_interval, t_function p_function,
                       t_timeout p_timeout, t_provider p_provider)
      : m_interval(calendar::convert<interval>(p_interval)),
        m_async(
            p_function, calendar::convert<timeout>(p_timeout), []() {},
            p_provider, [this]() -> bool { return breaker(); }) {
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
      //      DEB(m_log, "must not stop");
      return false;
    }
    // no timeout, which means the loop was stopped
    DEB(m_log, "must stop");

    return true;
  }

protected:
  /// \brief Type of loop used
  typedef internal::loop_t<t_params...> async_loop;

protected:
  /// \brief Interval of the execution of the loop
  interval m_interval;

  /// \brief Asynchronous loop used
  async_loop m_async;

  /// \brief Controls asynchronous execution
  std::mutex m_mutex;

  /// \brief Controls asynchronous execution
  std::condition_variable m_cond;

  /// \brief Indicates if the loop should stop
  std::atomic<bool> m_stopped{true};

  /// \brief Logger
  logger::cerr<> m_log{"sleeping_loop"};
};

#if 0
/// \brief
enum class queue_type : uint8_t {
  /// \brief
  CIRCULAR_FIXED_SIZE = 0,
  /// \brief
  CIRCULAR_UNLIMITED_SIZE = 1
};
#endif

/// \brief
template <typename t_data> struct queue_t {

  virtual ~queue_t() {}

  queue_t(const queue_t &) = default;
  queue_t(queue_t &&) = default;
  queue_t &operator=(const queue_t &) = default;
  queue_t &operator=(queue_t &&) = default;

  virtual void add(const t_data &) = 0;
  virtual void add(t_data &&p_data) = 0;
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
// namespace internal

#if 0
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

#endif

/// \brief Implements a circular queue which size is increased if it becomes
/// full
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

  ~circular_unlimited_size_queue_t() override {}

  /// \brief Default constructor not allowed
  circular_unlimited_size_queue_t() = delete;

  /// \brief Copy constructor not allowed
  circular_unlimited_size_queue_t(const circular_unlimited_size_queue_t &) =
      delete;

  /// \brief Move constructor not allowed
  circular_unlimited_size_queue_t(circular_unlimited_size_queue_t &&) = default;

  /// \brief Copy assignment not allowed
  circular_unlimited_size_queue_t &
  operator=(const circular_unlimited_size_queue_t &) = delete;

  /// \brief Move assignment not allowed
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
  ///
  /// \param p_data is a t_data to be added
  void add(const t_data &p_data) override {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (!full()) {
      DEB(m_log, "not adding more slots for ", p_data, " - size: ", capacity(),
          ", occupied: ", occupied());
      m_write->m_data = p_data;
      m_write = m_write->m_next;
    } else {
      DEB(m_log, "adding more slots: ", capacity(), ":", occupied());

      insert(m_write->m_prev, p_data);
    }
    ++m_amount;
  }

  /// \brief Movs a t_data object to the queue
  ///
  /// \param p_data is a t_data to be added
  void add(t_data &&p_data) override {
    std::lock_guard<std::mutex> _lock(m_mutex);
    if (!full()) {
      DEB(m_log, "not adding more slots for ", p_data, " - size: ", capacity(),
          ", occupied: ", occupied());
      m_write->m_data = p_data;
      m_write = m_write->m_next;
    } else {
      DEB(m_log, "adding more slots: ", capacity(), ":", occupied());

      insert(m_write->m_prev, std::move(p_data));
    }
    ++m_amount;
  }

  /// \brief Gets the first t_data obect added to the queue, if there is one
  ///
  /// \return a t_data object, if there was any to be retrieved
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

    /// \brief Copy constructor not allowed
    node(const node &) = delete;

    node(node &&) = default;

    /// \brief Copy assignment not allowed
    node &operator=(const node &) = delete;

    node &operator=(node &&) = default;

    /// \brief Constructor
    ///
    /// \param p_data is a t_data to be moved into the node
    node(t_data &&p_data) : m_data(std::move(p_data)) {}

    /// \brief Constructor
    ///
    /// \param p_data is a t_data to be copied into the node
    node(const t_data &p_data) : m_data(p_data) {}

    /// \brief data in the node
    t_data m_data;

    /// \brief next node
    ptr m_next;

    /// \brief previous node
    ptr m_prev;
  };

  /// \brief Alias for the pointer to a node
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

  /// \brief Inserts a node in the list after a node
  ///
  /// \param p_node which the new node will be inserted in front of
  ///
  /// \param p_data data inserted in the new node
  ///
  /// \return the new node
  node_ptr insert(node_ptr p_node, t_data &&p_data) {
    node_ptr _new_node = create_node(std::move(std::move(p_data)));

    _new_node->m_next = p_node->m_next;
    _new_node->m_prev = p_node;

    p_node->m_next->m_prev = _new_node;
    p_node->m_next = _new_node;
    ++this->m_size;
    return _new_node;
  }

  /// \brief Creates a new node
  ///
  /// \return the new node
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

#if 0
/// \brief
template <typename t_data, typename t_size>
std::unique_ptr<internal::queue_t<t_data>>
queue_factory(queue_type p_queue_type, t_size p_size) {
  std::unique_ptr<internal::queue_t<t_data>> _res;
  switch (p_queue_type) {
  case queue_type::CIRCULAR_FIXED_SIZE: {
    typedef circular_fixed_size_queue_t<t_data> queue;
    _res = std::unique_ptr<internal::queue_t<t_data>>(new queue(p_size));
  } break;
  case queue_type::CIRCULAR_UNLIMITED_SIZE: {
    typedef circular_unlimited_size_queue_t<t_data> queue;
    _res = std::unique_ptr<internal::queue_t<t_data>>(new queue(p_size));
  } break;
  }
  return _res;
}
#endif

} // namespace internal

namespace internal {

/// \brief A group of functions that compete to handle a data added to a queue
///
/// \param t_data type of data to be processed
template <typename t_data> class handler_group_t {
public:
  /// \brief Type of function that will handle the data
  typedef std::function<void(const t_data &)> handler;

  /// \brief Type type of function that will be called if the function times out
  typedef std::function<void(const t_data &)> on_timeout;

public:
  /// \brief
  handler_group_t() = delete;

  /// \brief
  template <typename t_time>
  handler_group_t(const id &p_id, const priority &p_priority, t_time p_timeout,
                  on_timeout p_on_timeout)
      : m_impl(std::make_unique<implementation>(p_id, p_priority, p_timeout,
                                                p_on_timeout)) {}

  /// \brief
  template <typename t_time>
  handler_group_t(const id &p_id, const priority &p_priority, t_time p_timeout)
      : m_impl(std::make_unique<implementation>(p_id, p_priority, p_timeout)) {}

  /// \brief
  template <typename t_time>
  handler_group_t(const id &p_id, t_time p_timeout, on_timeout p_on_timeout)
      : m_impl(
            std::make_unique<implementation>(p_id, p_timeout, p_on_timeout)) {}

  /// \brief
  template <typename t_time>
  handler_group_t(const id &p_id, t_time p_timeout)
      : m_impl(std::make_unique<implementation>(p_id, p_timeout)) {}

  /// \brief
  template <typename t_time>
  handler_group_t(const priority &p_priority, t_time p_timeout,
                  on_timeout p_on_timeout)
      : m_impl(std::make_unique<implementation>(p_priority, p_timeout,
                                                p_on_timeout)) {}

  /// \brief
  template <typename t_time>
  handler_group_t(const priority &p_priority, t_time p_timeout)
      : m_impl(std::make_unique<implementation>(p_priority, p_timeout)) {}

  /// \brief
  template <typename t_time>
  handler_group_t(t_time p_timeout, on_timeout p_on_timeout)
      : m_impl(std::make_unique<implementation>(p_timeout, p_on_timeout)) {}

  /// \brief
  template <typename t_time>
  handler_group_t(t_time p_timeout)
      : m_impl(std::make_unique<implementation>(p_timeout)) {}

  /// \brief copy constructor not allowed
  handler_group_t(const handler_group_t &) = delete;

  /// \brief
  handler_group_t(handler_group_t &&p_wp) = default;

  /// \brief copy assignemnt not allowed
  handler_group_t &operator=(const handler_group_t &) = delete;

  /// \brief
  handler_group_t &operator=(handler_group_t &&p_wp) = default;

  /// \brief destructor
  ///
  /// If 'stop' was not called, empties the queue, waiting for all the data to
  /// be processed
  ~handler_group_t() = default;
  //  {
  //    DEB(m_log, "destructor");
  //    stop();
  //    DEB(m_log, "leaving destructor");
  //  }

  /// \brief adds data to be passed to a worker
  ///
  /// \p data will only be added if \p start() was called
  ///
  /// \param p_data is the data to be passed to a worker
  ///
  /// \return \p true if it was added, \p false otherwise
  inline bool add_data(const t_data &p_data) {
    return m_impl->add_data(p_data);
  }

  /// \brief informs the amount of data added
  inline size_t amount_added() const { return m_impl->amount_added(); }

  /// \brief add adds one \p worker
  ///
  /// \tparam t_time type of time used to define timeout for the worker
  ///
  /// \param p_function the \p worker to be added
  ///
  /// \param p_timeout timeout of \p worker
  /// \brief common function called to add a \p worker
  ///
  /// \param p_loop the new \p worker to be added
  inline void add_worker(handler p_function) { m_impl->add_worker(p_function); }

  /// \brief adds a bunch of \p worker
  ///
  /// \tparam t_time type of time used to define timeout for the worker
  ///
  /// \param p_num_works the number of \p workers to be added
  ///
  /// \param p_function_factory a function that creates \p workers
  ///
  /// \param p_timeout timeout for the workers
  ///
  /// \param p_on_timeout function to be called when the worker times
  /// out
  inline void add_worker(uint16_t p_num_works,
                         std::function<handler()> p_function_factory) {
    m_impl->add_worker(p_num_works, p_function_factory);
  }

  /// \brief informs if the worker is stopped
  inline bool is_stopped() const { return m_impl->is_stopped(); }

  //  /// \brief starts the worker
  //  ///
  //  /// From this call on, the workers will compete among each other, in order
  //  /// to process any instance of \p data that was inserted into the queue
  //  ///
  //  /// \return \p false if it not start because no \p worker was added
  //  ///         \p true if it was already started, or if it started
  //  successfully bool start() {
  //    if (m_impl) {
  //      return m_impl->start();
  //    }
  //    return false;
  //  }

  /// \brief stops the \p worker
  ///
  /// From this call on, the \p workers will stop competing among each other,
  /// in order to process any instance of \p data that was inserted into the
  /// queue
  inline void stop() { m_impl->stop(); }

  /// \brief retrieves the capacity if the queue
  inline size_t capacity() const { return m_impl->capacity(); }

  /// \brief the amount of slots occupied in the \p queue
  inline size_t occupied() const { return m_impl->occupied(); }

  /// \brief
  inline timeout get_timeout() const { return m_impl->get_timeout(); }

  /// \brief
  inline const id &get_id() const { return m_impl->get_id(); }

  /// \brief
  inline const priority &get_priority() const { return m_impl->get_priority(); }

  /// \brief
  inline void set_priority(const priority &p_priority) {
    m_impl->set_priority(p_priority);
  }

  /// \brief
  inline bool operator<(const handler_group_t &p_function_pool) const {

    return m_impl->get_priority() < p_function_pool.m_impl->get_priority();
  }

  /// \brief
  inline bool operator>(const handler_group_t &p_function_pool) const {
    return m_impl->get_priority() > p_function_pool.m_impl->get_priority();
  }

  /// \brief
  inline bool operator!=(const handler_group_t &p_function_pool) const {
    return m_impl->get_id() != p_function_pool.m_impl->get_id();
  }

  /// \brief
  inline bool operator==(const handler_group_t &p_function_pool) const {
    return m_impl->get_id() == p_function_pool.m_impl->get_id();
  }

  inline size_t get_size() const { return m_impl->get_size(); }

  inline size_t get_occupied() const { return m_impl->get_occupied(); }

  inline void empty_queue() { m_impl->empty_queue(); }

private:
  struct implementation {
    /// \brief
    template <typename t_time>
    implementation(const id &p_id, const priority &p_priority, t_time p_timeout,
                   on_timeout p_on_timeout)
        : m_id(p_id), m_priority(p_priority), m_queue(10),
          m_timeout(calendar::convert<timeout>(p_timeout)),
          m_on_timeout(p_on_timeout) {}

    /// \brief
    template <typename t_time>
    implementation(const id &p_id, const priority &p_priority, t_time p_timeout)
        : m_id(p_id), m_priority(p_priority), m_queue(10),
          m_timeout(calendar::convert<timeout>(p_timeout)),
          m_on_timeout(
              [this](const t_data &p_data) -> void { add_data(p_data); }) {}

    /// \brief
    template <typename t_time>
    implementation(const id &p_id, t_time p_timeout, on_timeout p_on_timeout)
        : m_id(p_id), m_priority(async::priority::lowest), m_queue(10),
          m_timeout(calendar::convert<timeout>(p_timeout)),
          m_on_timeout(p_on_timeout) {}

    /// \brief
    template <typename t_time>
    implementation(const id &p_id, t_time p_timeout)
        : m_id(p_id), m_priority(async::priority::lowest), m_queue(10),
          m_timeout(calendar::convert<timeout>(p_timeout)),
          m_on_timeout(
              [this](const t_data &p_data) -> void { add_data(p_data); }) {}

    /// \brief
    template <typename t_time>
    implementation(const priority &p_priority, t_time p_timeout,
                   on_timeout p_on_timeout)
        : m_id(), m_priority(p_priority), m_queue(10),
          m_timeout(calendar::convert<timeout>(p_timeout)),
          m_on_timeout(p_on_timeout) {}

    /// \brief
    template <typename t_time>
    implementation(const priority &p_priority, t_time p_timeout)
        : m_id(), m_priority(p_priority), m_queue(10),
          m_timeout(calendar::convert<timeout>(p_timeout)),
          m_on_timeout(
              [this](const t_data &p_data) -> void { add_data(p_data); }) {}

    /// \brief
    template <typename t_time>
    implementation(t_time p_timeout, on_timeout p_on_timeout)
        : m_id(), m_priority(async::priority::lowest), m_queue(10),
          m_timeout(calendar::convert<timeout>(p_timeout)),
          m_on_timeout(p_on_timeout) {}

    /// \brief
    template <typename t_time>
    implementation(t_time p_timeout)
        : m_id(), m_priority(async::priority::lowest), m_queue(10),
          m_timeout(calendar::convert<timeout>(p_timeout)),
          m_on_timeout(
              [this](const t_data &p_data) -> void { add_data(p_data); }) {}

    /// \brief move constructor not allowed
    implementation(handler_group_t &&) = delete;

    /// \brief move assignemnt not allowed
    implementation &operator=(implementation &&) = delete;

    /// \brief destructor
    ~implementation() {
      if (m_stopped) {
        DEB(m_log, "not stopping because it is stopped");
        return;
      }

      //      empty_queue();

      set_stopped();
      DEB(m_log, "notifying all providers");
      m_data_produced.notify_all();
      for (async_loop &_loop : m_loops) {
        DEB(m_log, "stopping loop ", _loop.get_id());
        _loop.stop();
      }
      DEB(m_log, "leaving");
    }

    void empty_queue() {
      DEB(m_log, "empty queue");
      while (!m_queue.empty()) {
        m_data_produced.notify_all();
        //        std::unique_lock<std::mutex> _lock(m_mutex_data);
        //        m_data_consumed.wait(_lock, [] { return true; });
      }
    }

    bool add_data(const t_data &p_data) {

      m_queue.add(p_data);
      m_data_produced.notify_all();
      {
        std::lock_guard<std::mutex> _lock(m_mutex_data);
        ++m_queued_data;
      }
      return true;
    }

    /// \brief informs the amount of data added
    inline size_t amount_added() const { return m_queued_data; }

    /// \brief add adds one \p worker
    ///
    /// \tparam t_time type of time used to define timeout for the worker
    ///
    /// \param p_function the \p worker to be added
    ///
    /// \param p_timeout timeout of \p worker
    /// \brief common function called to add a \p worker
    ///
    /// \param p_loop the new \p worker to be added
    void add_worker(handler p_function) {
      auto _provider = [this]() -> std::optional<std::tuple<t_data>> {
        return this->provider();
      };
      std::lock_guard<std::mutex> _lock(m_add_work);

      async_loop _async_loop{p_function, m_timeout, m_on_timeout, _provider};

      _async_loop.start();

      m_loops.emplace(m_loops.begin(), std::move(_async_loop));

      set_stopped(false);
    }

    /// \brief adds a bunch of \p worker
    ///
    /// \tparam t_time type of time used to define timeout for the worker
    ///
    /// \param p_num_works the number of \p workers to be added
    ///
    /// \param p_function_factory a function that creates \p workers
    ///
    /// \param p_timeout timeout for the workers
    ///
    /// \param p_on_timeout function to be called when the worker times
    /// out
    void add_worker(uint16_t p_num_works,
                    std::function<handler()> p_function_factory) {
      for (auto _i = 0; _i < p_num_works; ++_i) {
        add_worker(p_function_factory());
      }
    }

    /// \brief informs if the worker is stopped
    inline bool is_stopped() const { return m_stopped; }

    //    /// \brief starts the worker
    //    ///
    //    /// From this call on, the workers will compete among each other, in
    //    order
    //    /// to process any instance of \p data that was inserted into the
    //    queue
    //    ///
    //    /// \return \p false if it not start because no \p worker was added
    //    ///         \p true if it was already started, or if it started
    //    successfully bool start() {

    //      //      if (!m_stopped) {
    //      //        ERR(m_log, "not starting because it is already
    //      running");
    //      //        return true;
    //      //      }

    //      if (m_loops.empty()) {
    //        WAR(m_log, "can't run because there are no workers");
    //        return false;
    //      }

    //      //      {
    //      //        DEB(m_log, "waiting for locking");
    //      //        std::unique_lock<std::mutex> _lock(m_mutex_stop);
    //      //        m_stopped = false;
    //      //      }

    //      std::unique_lock<std::mutex> _lock(m_mutex_stop);
    //      DEB(m_log, "starting");
    //      m_stopped = false;
    //      for (async_loop &_loop : m_loops) {
    //        DEB(m_log, "starting loop");
    //        _loop.start();
    //      }

    //      //      DEB(m_log, "started");
    //      return true;
    //    }

    /// \brief stops the \p worker
    ///
    /// From this call on, the \p workers will stop competing among each other,
    /// in order to process any instance of \p data that was inserted into the
    /// queue
    //    void stop() {
    //      if (m_stopped) {
    //        DEB(m_log, "not stopping because it is stopped");
    //        return;
    //      }

    //      std::unique_lock<std::mutex> _lock(m_mutex_stop);
    //      m_stopped = true;
    //      m_data_produced.notify_all();
    //      for (async_loop &_loop : m_loops) {
    //        DEB(m_log, "stopping loop");
    //        //        std::async(std::launch::async, [&_loop]() -> void {
    //        //        _loop.stop(); });
    //        _loop.stop();
    //      }
    //    }

    /// \brief retrieves the capacity if the queue
    inline size_t capacity() const { return m_queue.capacity(); }

    /// \brief the amount of slots occupied in the \p queue
    inline size_t occupied() const { return m_queue.occupied(); }

    /// \brief
    inline timeout get_timeout() const { return m_timeout; }

    /// \brief
    inline const id &get_id() const { return m_id; }

    /// \brief
    inline const priority &get_priority() const { return m_priority; }

    /// \brief
    inline void set_priority(const priority &p_priority) {
      m_priority = p_priority;
    }

    size_t get_size() const { return m_queue.capacity(); }
    size_t get_occupied() const { return m_queue.occupied(); }

    /// \brief
    inline bool operator<(const implementation &p_impl) const {
      return m_priority < p_impl.m_priority;
    }

    /// \brief
    inline bool operator>(const implementation &p_impl) const {
      return m_priority > p_impl.m_priority;
    }

    /// \brief
    inline bool operator!=(const implementation &p_impl) const {
      return m_id != p_impl.m_id;
    }

    /// \brief
    inline bool operator==(const implementation &p_impl) const {
      return m_id == p_impl.m_id;
    }

  private:
    /// \brief loop_t is a \p async_loop where a \p worker will be
    /// running
    typedef loop_t<t_data> async_loop;

    /// \brief async_loops_t is the collection of \p async_loop pointers
    typedef typename std::vector<async_loop> asyncs_loops;

    typedef circular_unlimited_size_queue_t<t_data> queue;

  private:
    inline void set_stopped(bool p_value = true) {
      std::unique_lock<std::mutex> _lock(m_mutex_stop);
      m_stopped = p_value;
    }

    /// \brief provider provides data, if available, to a \p worker
    std::optional<std::tuple<t_data>> provider() {
      using namespace std;

      if (m_stopped) {
        DEB(m_log, "stopped");
        return {};
      }

      DEB(m_log, "waiting for data...");
      std::unique_lock<std::mutex> _lock(m_mutex_data);
      m_data_produced.wait(_lock, [this]() -> bool {
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

      if (m_stopped) {
        DEB(m_log, "stopped and notifying");
        //        m_data_consumed.notify_all();
        return {};
      }

      std::optional<t_data> _maybe{m_queue.get()};
      if (_maybe) {
        t_data _data(std::move(*_maybe));

        DEB(m_log, "getting ", _data, " and notifying");

        //        m_data_consumed.notify_all();

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
    /// \brief
    id m_id;

    /// \brief
    priority m_priority;

    /// \brief queue where data will be inserted for the worker to compete for
    queue m_queue;

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
    //    std::condition_variable m_data_consumed;

    /// \brief controls access to the data produced
    std::condition_variable m_data_produced;

    /// \brief asynchronous loops, where the workers are running
    asyncs_loops m_loops;

    /// \brief indicates if the worker is running
    std::atomic<bool> m_stopped{true};

    /// \brief m_destroying indicates that the \p produce_consumer should stop
    std::atomic<bool> m_destroying{false};

    /// \brief amount of queued data
    size_t m_queued_data{0};

    /// \brief
    logger::cerr<> m_log{"worker_pool"};
  };

private:
  /// \brief
  std::unique_ptr<implementation> m_impl;

  logger::cerr<> m_log{"worker_pool"};
};

/// \brief
template <typename t_data> struct messenger_t {

  /// \brief Type of worker that a worker will execute
  typedef std::function<void(const t_data &)> worker;

  /// \brief
  typedef std::function<void(const t_data &)> on_timeout;

  /// \brief
  ~messenger_t() = default;

  /// \brief
  template <typename t_time>
  static inline void add_worker_pool(const id &p_id, const priority &p_priority,
                                     t_time p_timeout,
                                     on_timeout p_on_timeout) {
    insert({p_id, p_priority, p_timeout, p_on_timeout});
  }

  /// \brief
  template <typename t_time>
  static inline void add_worker_pool(const id &p_id, const priority &p_priority,
                                     t_time p_timeout) {
    insert({p_id, p_priority, p_timeout});
  }

  /// \brief
  template <typename t_time>
  static inline void add_worker_pool(const id &p_id, t_time p_timeout,
                                     on_timeout p_on_timeout) {
    insert({p_id, p_timeout, p_on_timeout});
  }

  /// \brief
  template <typename t_time>
  static inline void add_worker_pool(const id &p_id, t_time p_timeout) {
    insert({p_id, p_timeout});
  }

  /// \brief
  template <typename t_time>
  static id add_worker_pool(const priority &p_priority, t_time p_timeout,
                            on_timeout p_on_timeout) {
    worker_pool _worker_pool{p_priority, p_timeout, p_on_timeout};
    id _id{_worker_pool.get_id()};
    insert(std::move(_worker_pool));
    return _id;
  }

  /// \brief
  template <typename t_time>
  static id add_worker_pool(const priority &p_priority, t_time p_timeout) {
    worker_pool _worker_pool{p_priority, p_timeout};
    id _id{_worker_pool.get_id()};
    insert(std::move(_worker_pool));
    return _id;
  }

  /// \brief
  template <typename t_time>
  static id add_worker_pool(t_time p_timeout, on_timeout p_on_timeout) {
    worker_pool _worker_pool{p_timeout, p_on_timeout};
    id _id{_worker_pool.get_id()};
    insert(std::move(_worker_pool));
    return _id;
  }

  /// \brief
  template <typename t_time> static id add_worker_pool(t_time p_timeout) {
    worker_pool _worker_pool{p_timeout};
    id _id{_worker_pool.get_id()};
    insert(std::move(_worker_pool));
    return _id;
  }

  /// \brief
  static void set_priority(const id &p_id, priority p_priority) {
    iterator _ite = find(p_id);
    if (_ite != m_list.end()) {
      _ite->set_priority(p_priority);
      sort();
    }
  }

  /// \brief
  static std::optional<priority> get_priority(const id &p_id) {
    iterator _ite = find(p_id);
    if (_ite != m_list.end()) {
      return {_ite->get_priority()};
    }
    return {};
  }

  /// \brief
  static void publish(const t_data &p_data) {
    for (worker_pool &_work_pool : m_list) {
      DEB(m_log, "publishing data to pool '", _work_pool.get_id(), "'");
      _work_pool.add_data(p_data);
    }
  }

  /// \brief
  static void add_subscriber(const id &p_id, worker p_function) {
    auto _ite = find(p_id);
    auto _end = m_list.end();
    if (_ite != _end) {
      DEB(m_log, "adding another worker for ", p_id);
      _ite->add_worker(p_function);
    }
  }

  /// \brief
  static void add_subscriber(const id &p_id, uint16_t p_num_workers,
                             std::function<worker()> p_factory) {
    iterator _ite = find(p_id);
    if (_ite != m_list.end()) {
      _ite->add_worker(p_num_workers, p_factory);
    }
  }

  static void
  traverse(std::function<void(const id &, priority, timeout)> p_visitor) {
    for (const worker_pool &_work_pool : m_list) {
      p_visitor(_work_pool.get_id(), _work_pool.get_priority(),
                _work_pool.get_timeout());
    }
  }

  static size_t size(const id &p_id) {
    iterator _ite = find(p_id);
    if (_ite != m_list.end()) {
      return _ite->get_size();
    }
    return 0;
  }

  static size_t occupied(const id &p_id) {
    iterator _ite = find(p_id);
    if (_ite != m_list.end()) {
      return _ite->get_occupied();
    }
    return 0;
  }

  /// \brief
  static void wait() {
    DEB(m_log, "starting to wait");
    for (worker_pool &_worker_pool : m_list) {
      _worker_pool.empty_queue();
    }
    DEB(m_log, "finished waiting");
  }

private:
  /// \brief
  typedef internal::handler_group_t<t_data> worker_pool;

  /// \brief
  typedef std::list<worker_pool> list;

  /// \brief
  typedef typename list::iterator iterator;

private:
  /// \brief
  static auto find(const id &p_id) {
    auto _cmp = [&p_id](const worker_pool &p_work_pool) -> bool {
      return p_id == p_work_pool.get_id();
    };
    return std::find_if(m_list.begin(), m_list.end(), _cmp);
  }

  static inline void insert(worker_pool &&p_function_pool) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_list.push_back(std::move(p_function_pool));
    //    m_list.back().start();
    sort();
  }

  static inline void sort() {
    m_list.sort([](const worker_pool &p_i1, const worker_pool &p_i2) -> bool {
      return p_i1 < p_i2;
    });
  }

private:
  /// \brief
  static list m_list;
  static logger::cerr<> m_log;
  static std::mutex m_mutex;
};

/// \brief
template <typename t_data>
typename messenger_t<t_data>::list messenger_t<t_data>::m_list;

template <typename t_data>
logger::cerr<> messenger_t<t_data>::m_log{"messenger"};

template <typename t_data> std::mutex messenger_t<t_data>::m_mutex;

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
  /// \param p_function the work function to be executed at \p p_interval
  ///
  /// \param p_on_timeout function to be executed if \p p_function times out
  ///
  /// \param p_provider function that provides the parameters to the \p
  /// p_function

  template <typename t_timeout, typename t_interval>
  sleeping_loop_t(t_timeout p_timeout, t_interval p_interval, worker p_function,
                  on_timeout p_on_timeout, provider p_provider)
      : m_impl(std::make_unique<implementation>(
            p_timeout, p_interval, p_function, p_on_timeout, p_provider)) {}

  template <typename t_timeout, typename t_interval>
  sleeping_loop_t(t_timeout p_timeout, t_interval p_interval, worker p_function,
                  provider p_provider)
      : m_impl(std::make_unique<implementation>(p_timeout, p_interval,
                                                p_function, p_provider)) {}

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

  template <typename t_time> inline t_time get_interval() const {
    return m_impl->template get_interval<t_time>();
  }

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
    implementation(t_timeout p_timeout, t_interval p_interval,
                   worker p_function, on_timeout p_on_timeout,
                   provider p_provider)
        : internal::sleeping_loop_base_t<on_timeout, t_params...>(
              p_interval, p_function, p_timeout, p_on_timeout, p_provider) {
      DEB(this->m_log, "timeout = ", p_timeout.count(),
          ", interval = ", p_interval.count());
    }

    template <typename t_timeout, typename t_interval>
    implementation(t_timeout p_timeout, t_interval p_interval,
                   worker p_function, provider p_provider)
        : internal::sleeping_loop_base_t<on_timeout, t_params...>(
              p_interval, p_function, p_timeout, p_provider) {
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
  /// \param p_function the worker to be executed at \p p_interval
  template <typename t_timeout, typename t_interval>
  sleeping_loop_t(t_timeout p_timeout, t_interval p_interval, worker p_function,
                  on_timeout p_on_timeout)
      : m_impl(std::make_unique<implementation>(p_timeout, p_interval,
                                                p_function, p_on_timeout)) {}

  template <typename t_timeout, typename t_interval>
  sleeping_loop_t(t_timeout p_timeout, t_interval p_interval, worker p_function)
      : m_impl(std::make_unique<implementation>(p_timeout, p_interval,
                                                p_function)) {}

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

  template <typename t_time> inline t_time get_interval() const {
    return m_impl->template get_interval<t_time>();
  }

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
  template <typename t_time> inline t_time get_timeout() const {
    return m_impl->template get_timeout<t_time>();
  }

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
    implementation(t_timeout p_timeout, t_interval p_interval,
                   worker p_function, on_timeout p_on_timeout)
        : internal::sleeping_loop_base_t<on_timeout, void>(
              p_interval, p_function, p_timeout, p_on_timeout) {

      DEB(this->m_log, "timeout = ", p_timeout.count(),
          ", interval = ", p_interval.count());
    }

    template <typename t_timeout, typename t_interval>
    implementation(t_timeout p_timeout, t_interval p_interval,
                   worker p_function)
        : internal::sleeping_loop_base_t<on_timeout, void>(
              p_interval, p_function, p_timeout) {

      DEB(this->m_log, "timeout = ", p_timeout.count(),
          ", interval = ", p_interval.count());
    }
  };

private:
  std::unique_ptr<implementation> m_impl;
};

// #########################################

/// \brief Adds a queue to receive messages to be handled
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_id is the identifier of the queue
///
/// \param p_priority is the priority of the queue. This defines the order that
/// the message will be placed in the queue.
///
/// \param p_timeout is the maximum amount of time that the handler function
/// will have to complete its work.
///
/// \param p_on_timeout is the function that will be called to handle the
/// message that the handler function could not handle in time.
template <typename t_msg, typename t_time>
static inline void add_queue(const id &p_id, const priority &p_priority,
                             t_time p_timeout,
                             std::function<void(const t_msg &)> p_on_timeout) {
  internal::messenger_t<t_msg>::add_worker_pool(p_id, p_priority, p_timeout,
                                                p_on_timeout);
}

/// \brief Adds a queue to receive messages to be handled
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_id is the identifier of the queue
///
/// \param p_priority is the priority of the queue. This defines the order that
/// the message will be placed in the queue.
///
/// \param p_timeout is the maximum amount of time that the handler function
/// will have to complete its work.
///
/// \details In this configuration, if any handler times out, the message will
/// be added again to the queue
template <typename t_msg, typename t_time>
static inline void add_queue(const id &p_id, const priority &p_priority,
                             t_time p_timeout) {
  internal::messenger_t<t_msg>::add_worker_pool(p_id, p_priority, p_timeout);
}

/// \brief Adds a queue to receive messages to be handled
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_id is the identifier of the queue
///
/// \param p_timeout is the maximum amount of time that the handler function
/// will have to complete its work.
///
/// \param p_on_timeout is the function that will be called to handle the
/// message that the handler function could not handle in time.
///
/// \details In this configuration, this queue has the lowest priority, i.e.,
/// this queue will be the last to receive a copy of the message.
template <typename t_msg, typename t_time>
static inline void add_queue(const id &p_id, t_time p_timeout,
                             std::function<void(const t_msg &)> p_on_timeout) {
  internal::messenger_t<t_msg>::add_worker_pool(p_id, p_timeout, p_on_timeout);
}

/// \brief Adds a queue to receive messages to be handled
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_id is the identifier of the queue
///
/// \param p_timeout is the maximum amount of time that the handler function
/// will have to complete its work.
///
/// \details In this configuration, this queue has the lowest priority, i.e.,
/// this queue will be the last to receive a copy of the message; and if any
/// handler times out, the message will be added again to the queue
template <typename t_msg, typename t_time>
static inline void add_queue(const id &p_id, t_time p_timeout) {
  internal::messenger_t<t_msg>::add_worker_pool(p_id, p_timeout);
}

/// \brief Adds a queue to receive messages to be handled
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_id is the identifier of the queue
///
/// \param p_priority is the priority of the queue. This defines the order that
/// the message will be placed in the queue.
///
/// \param p_timeout is the maximum amount of time that the handler function
/// will have to complete its work.
///
/// \param p_on_timeout is the function that will be called to handle the
/// message that the handler function could not handle in time.
///
/// \return An auto generated \p id of the queue
template <typename t_msg, typename t_time>
static id add_queue(const priority &p_priority, t_time p_timeout,
                    std::function<void(const t_msg &)> p_on_timeout) {
  return internal::messenger_t<t_msg>::add_worker_pool(p_priority, p_timeout,
                                                       p_on_timeout);
}

/// \brief Adds a queue to receive messages to be handled
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_id is the identifier of the queue
///
/// \param p_priority is the priority of the queue. This defines the order that
/// the message will be placed in the queue.
///
/// \param p_timeout is the maximum amount of time that the handler function
/// will have to complete its work.
///
/// \param p_on_timeout is the function that will be called to handle the
/// message that the handler function could not handle in time.
///
/// \return An auto generated \p id of the queue
///
///
/// \details In this configuration, if any handler times out, the message will
/// be added again to the queue
template <typename t_msg, typename t_time>
static id add_queue(const priority &p_priority, t_time p_timeout) {
  return internal::messenger_t<t_msg>::add_worker_pool(p_priority, p_timeout);
}

/// \brief Adds a queue to receive messages to be handled
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_id is the identifier of the queue
///
/// \param p_priority is the priority of the queue. This defines the order that
/// the message will be placed in the queue.
///
/// \param p_timeout is the maximum amount of time that the handler function
/// will have to complete its work.
///
/// \param p_on_timeout is the function that will be called to handle the
/// message that the handler function could not handle in time.
///
/// \return An auto generated \p id of the queue
///
/// \details In this configuration, this queue has the lowest priority, i.e.,
/// this queue will be the last to receive a copy of the message.
template <typename t_msg, typename t_time>
static id add_queue(t_time p_timeout,
                    std::function<void(const t_msg &)> p_on_timeout) {
  return internal::messenger_t<t_msg>::add_worker_pool(p_timeout, p_on_timeout);
}

/// \brief Adds a queue to receive messages to be handled
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_id is the identifier of the queue
///
/// \param p_priority is the priority of the queue. This defines the order that
/// the message will be placed in the queue.
///
/// \param p_timeout is the maximum amount of time that the handler function
/// will have to complete its work.
///
/// \param p_on_timeout is the function that will be called to handle the
/// message that the handler function could not handle in time.
///
/// \return An auto generated \p id of the queue
///
/// \details In this configuration, this queue has the lowest priority, i.e.,
/// this queue will be the last to receive a copy of the message; and if any
/// handler times out, the message will be added again to the queue
template <typename t_msg, typename t_time>
static id add_queue(t_time p_timeout) {
  return internal::messenger_t<t_msg>::add_worker_pool(p_timeout);
}

/// \brief Defines the priority of a message queue
/// The priority of the queue defines the order in which a message will be
/// added.
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \param p_id is the identifier of the queue
///
/// \param p_priority is the priority of the queue. This defines the order that
/// the message will be placed in the queue.
template <typename t_msg>
static void set_priority(const id &p_id, priority p_priority) {
  internal::messenger_t<t_msg>::set_priority(p_id, p_priority);
}

/// \brief Retrieves the priority of a message queue
/// The priority of the queue defines the order in which a message will be
/// added.
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \param p_id is the identifier of the queue
///
/// \return The priority of the queue
template <typename t_msg>
static std::optional<priority> get_priority(const id &p_id) {
  return internal::messenger_t<t_msg>::get_priority(p_id);
}

/// \brief Sends a message to all the queues associated to a message type
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \param p_msg is the message to be copied to all the queues
template <typename t_msg> static void send(const t_msg &p_msg) {
  internal::messenger_t<t_msg>::publish(p_msg);
}

/// \brief Adds a handler to a queue
/// It is possible to add many handlers to the same queue. Those handlers will
/// compete with each other to handle a added message.
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \param p_id is the identifier of the queue to which this handler will be
/// added to
///
/// \param p_handler is the function that will handle a message added to the
/// queue
template <typename t_msg>
static void add_handler(const id &p_id,
                        std::function<void(const t_msg &)> p_handler) {
  internal::messenger_t<t_msg>::add_subscriber(p_id, p_handler);
}

/// \brief Adds a handler to a queue
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_handler is the function that will handle a message added to the
/// queue
///
/// \param p_on_timeout is the function that will be called to handle the
/// message that the handler function could not handle in time.
///
/// \return An auto generated \p id of the queue
///
/// \details In this configuration, this queue has the lowest priority, i.e.,
/// this queue will be the last to receive a copy of the message; and if any
/// handler times out, the message will be added again to the queue
template <typename t_msg, typename t_time = std::chrono::minutes>
static id add_handler(std::function<void(const t_msg &)> p_handler,
                      t_time p_timeout = 10min) {
  id _id = add_queue(p_timeout);
  add_handler(_id, p_handler);
  return _id;
}

/// \brief Adds a bunch of handlers to a queue
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \param p_id is the identifier of the queue to which this handler will be
/// added to
///
/// \param p_num_handlers is the number of handlers to be added
///
/// \param p_handler_factory is the function that will create handler functions
template <typename t_msg>
static void add_handler(
    const id &p_id, uint16_t p_num_handlers,
    std::function<std::function<void(const t_msg &)>> p_handler_factory) {
  internal::messenger_t<t_msg>::add_subscriber(p_id, p_num_handlers,
                                               p_handler_factory);
}

} // namespace async
} // namespace tenacitas

#endif
