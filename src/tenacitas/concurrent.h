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

/// \brief master namespace
namespace tenacitas {

/// \brief support for concurrent (asynchronous) programming
namespace concurrent {

/// \brief Type of time used to define timeout
typedef std::chrono::milliseconds timeout;

/// \brief Type of time used to define interval
typedef std::chrono::milliseconds interval;

/// \brief Converts any type of time defined in std::chrono to \p timeout
template <typename t_time> inline timeout to_timeout(t_time p_time) {
    return std::chrono::duration_cast<timeout>(p_time);
}

/// \brief Converts any type of time defined in std::chrono to \p interval
template <typename t_time> inline interval to_interval(t_time p_time) {
    return std::chrono::duration_cast<interval>(p_time);
}

/// \brief helper class for \p execute function
///
/// \tparam t_type type returned by the executed function
///
/// \tparam t_time type of time used to control timeout of the function being executed
///
/// \tparam t_params... possible types of the arguments of the function being executed
template <typename t_type, typename t_time, typename... t_params>
struct executer_helper_t {

    /// \brief returned by the executed function
    typedef t_type type;

    /// \brief result of the time controlled execution of the function
    typedef std::optional<type> result;

    /// \brief executes a function, controlling its timeout
    ///
    /// \param p_function function to be executed
    ///
    /// \param p_timeout maximum amount of time for \p p_function to execute
    ///
    /// \param p_params... possible parameters required by \p p_function
    ///
    /// \return std::optional<t_type> with value if not timeout, {} otherwise
    static result call(std::function<t_type(t_params...)> p_function,
                       t_time p_timeout,
                       t_params... p_params) {

        std::future<t_type> _future =
                std::async(std::launch::async, p_function, p_params...);
        if (_future.wait_for(p_timeout) != std::future_status::timeout) {
            return {_future.get()};
        }
        return {};
    }
};

/// \brief helper class for \p execute function, when the function being executed returns void
///
/// \tparam t_time type of time used to control timeout of the function being executed
///
/// \tparam t_params... possible types of the arguments of the function being executed
template <typename t_time, typename... t_params>
struct executer_helper_t<void, t_time, t_params...> {

    /// \brief returned by the executed function
    typedef void type ;

    /// \brief result of the time controlled execution of the function
    typedef bool result;


    /// \brief executes a function, controlling its timeout
    ///
    /// \param p_function function to be executed
    ///
    /// \param p_timeout maximum amount of time for \p p_function to execute
    ///
    /// \param p_params... possible parameters required by \p p_function
    ///
    /// \return true if no timeout; false otherwise
    static result call(std::function<void(t_params...)> p_function,
                       t_time p_timeout,
                       t_params... p_params) {

        std::future<void> _future =
                std::async(std::launch::async, p_function, p_params...);
        if (_future.wait_for(p_timeout) != std::future_status::timeout) {
            return true;
        }
        return false;
    }
};

/// \brief executes a callable object in a maximum amount of time
///
/// \tparam t_time type of time used to control timeout of the function being executed
///
/// \tparam t_function type of callable object
///
/// \tparam t_params possible types of the arguments of the function being executed
///
/// \param p_timeout maximum amount of time for the function to execute
///
/// \param p_function callable object
///
/// \param p_params... possible parameters required by \p p_function
///
/// \return if the callable object returns void, \p execute returns true if no timeout; false otherwise
///         if the callable object does not return void, \p execute std::optional<type-returned> with value if not timeout, {} otherwise
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
///using namespace std::chrono_literals;
///
///double f1(int i) {
///    std::cout << "f1: ";
///    return i / 4;
///}
///
///double f2(int i, float f) {
///    std::cout << "f2: ";
///    return f * i / 4;
///}
///
///double f3() {
///    std::cout << "f3: ";
///    return 3.14;
///}
///
///void f4(int i) { std::cout << "f4: " << i / 4 << " "; }
///
///void f5(int i, float f) {
///    std::cout << "f5: " << f * i / 4 << " ";
///}
///
///void f6() {
///    std::cout << "f6: " << 3.14 << " ";
///}
///
///double f7(int i) {
///    std::this_thread::sleep_for(2s);
///    std::cout << "f7: ";
///    return i / 4;
///}
///
///double f8(int i, float f) {
///    std::this_thread::sleep_for(2s);
///    std::cout << "f8: ";
///    return f * i / 4;
///}
///
///double f9() {
///    std::this_thread::sleep_for(2s);
///    std::cout << "f9: ";
///    return 3.14;
///}
///
///void f10(int i) {
///    std::this_thread::sleep_for(2s);
///    std::cout << "f10: " << i / 4 << " ";
///}
///
///void f11(int i, float f) {
///    std::this_thread::sleep_for(2s);
///    std::cout << "f11: " << f * i / 4 << " ";
///}
///
///void f12() {
///    std::this_thread::sleep_for(2s);
///    std::cout << "f6: " << 3.14 << " ";
///}
///
////// ##################
///
///int main() {
///    using namespace tenacitas;
///
///    {
///        std::optional<double> _maybe = concurrent::execute(300ms, f1, 8);
///        if (_maybe) {
///            std::cout << "with exec: " << *_maybe;
///        } else {
///            std::cout << "timeout!";
///        }
///        std::cout << std::endl;
///    }
///
///    {
///        std::optional<double> _maybe = concurrent::execute(300ms, f2, 8, 4.2f);
///        if (_maybe) {
///            std::cout << *_maybe;
///        } else {
///            std::cout << "timeout!";
///        }
///        std::cout << std::endl;
///    }
///
///    {
///        std::optional<double> _maybe = concurrent::execute(300ms, f3);
///        if (_maybe) {
///            std::cout << *_maybe;
///        } else {
///            std::cout << "timeout!";
///        }
///        std::cout << std::endl;
///    }
///
///    {
///        if (concurrent::execute(300ms, f4,  -8)) {
///            std::cout << "ok";
///        }
///        else {
///            std::cout<< "timeout!";
///        }
///        std::cout << std::endl;
///
///    }
///
///    {
///        if (concurrent::execute(300ms, f5,  -8, 3.56))  {
///            std::cout << "ok";
///        }
///        else {
///            std::cout<< "timeout!";
///        }
///        std::cout << std::endl;
///
///    }
///
///    {
///        if (concurrent::execute(300ms, f6))  {
///            std::cout << "ok";
///        }
///        else {
///            std::cout<< "timeout!";
///        }
///        std::cout << std::endl;
///    }
///
///    {
///        std::optional<double> _maybe = concurrent::execute(300ms, f7, 8);
///        if (_maybe) {
///            std::cout << "with exec: " << *_maybe;
///        } else {
///            std::cout << "timeout!";
///        }
///        std::cout << std::endl;
///    }
///
///    {
///        std::optional<double> _maybe = concurrent::execute(300ms, f8,  8, 4.2f);
///        if (_maybe) {
///            std::cout << *_maybe;
///        } else {
///            std::cout << "timeout!";
///        }
///        std::cout << std::endl;
///    }
///
///    {
///        std::optional<double> _maybe = concurrent::execute(300ms, f9);
///        if (_maybe) {
///            std::cout << *_maybe;
///        } else {
///            std::cout << "timeout!";
///        }
///        std::cout << std::endl;
///    }
///
///    {
///        if (concurrent::execute(300ms, f10,  -18))  {
///            std::cout << "ok";
///        }
///        else {
///            std::cout<< "timeout!";
///        }
///        std::cout << std::endl;
///    }
///
///    {
///        if (concurrent::execute(300ms, f11,  -18, 3.3))   {
///            std::cout << "ok";
///        }
///        else {
///            std::cout<< "timeout!";
///        }
///        std::cout << std::endl;
///    }
///
///    {
///        if (concurrent::execute(300ms, f12) ) {
///            std::cout << "ok";
///        }
///        else {
///            std::cout<< "timeout!";
///        }
///        std::cout << std::endl;
///    }
///}
///
/// \endcode
template <typename t_time, typename t_function, typename... t_params>
inline typename executer_helper_t<std::invoke_result_t<t_function, t_params...>,
t_time,
t_params...>::result
execute(t_time p_timeout, t_function p_function, t_params... p_params) {

    typedef std::invoke_result_t<t_function, t_params...> type ;

    typedef executer_helper_t<type, t_time, t_params...> executer_helper;

    return executer_helper::call(p_function, p_timeout, p_params...);
}


/// \brief Type of function executed when a function times out
typedef std::function<void(std::thread::id)> timeout_callback;

/// \brief Type of function used to inform if a loop should stop
typedef std::function<bool()> breaker;

/// \brief base class for asynchronous loop
template <typename t_log, typename t_time> struct async_loop_base_t {

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

protected:
    async_loop_base_t(t_time p_time, timeout_callback p_timeout_callback)
        : m_timeout(to_timeout(p_time)), m_timeout_callback(p_timeout_callback) {}

    virtual void loop() = 0;

    void launch_timeout_callback() {
        std::thread(m_timeout_callback, m_thread.get_id()).detach();
    }
protected:
    timeout m_timeout;

    timeout_callback m_timeout_callback;

    bool m_stopped{true};

    std::thread m_thread;

    const timeout m_breaker_timeout {200ms};
    const timeout m_provider_timeout {300ms};


    t_log m_log{"concurrent::async_loop"};
};

/// \brief Used to define if a loop will use a breaker function or not
enum class use_breaker : char { yes = 'y', no = 'n' };

/// \brief Base class for asynchronous loop
template <typename t_log, typename t_time, use_breaker use,
          typename... t_params>
struct async_loop_t;

/// #### async_loop 1 ####
template <typename t_log, typename t_time, typename... t_params>
struct async_loop_t<t_log, t_time, use_breaker::yes, t_params...>
        : public async_loop_base_t<t_log, t_time> {

    typedef std::function<void(t_params...)> worker;
    typedef std::function<std::tuple<t_params...>()> provider;

    async_loop_t(t_time p_timeout, timeout_callback p_timeout_callback,
                 breaker p_breaker, worker p_worker, provider p_provider)
        : async_loop_base_t<t_log, t_time>(p_timeout, p_timeout_callback),
          m_worker(p_worker), m_provider(p_provider), m_breaker(p_breaker) {}

protected:
    void loop() override {


        while (true) {
            if (this->m_stopped) {
                break;
            }

            std::optional<bool> _maybe_break = execute(this->m_breaker_timeout, m_breaker);
            if ((_maybe_break) && (*_maybe_break)) {
                break;
            }

            std::optional<std::tuple<t_params...>> _maybe_data =
                    execute(this->m_provider_timeout, m_provider);

            if (this->m_stopped) {
                break;
            }

            _maybe_break = execute(this->m_breaker_timeout, m_breaker);
            if ((_maybe_break) && (*_maybe_break)) {
                break;
            }

            if (_maybe_data) {
                std::tuple<t_params...> _params = std::move(*_maybe_data);
                auto _worker = [this, _params]() -> void {
                    std::apply(m_worker, _params);
                };

                if (!execute(this->m_timeout, _worker)) {
                    this->launch_timeout_callback();
                }
            }
        }
    }

private:
    worker m_worker;
    provider m_provider;
    breaker m_breaker;
};

/// #### async_loop 2 ####
template <typename t_log, typename t_time, typename... t_params>
struct async_loop_t<t_log, t_time, use_breaker::no, t_params...>
        : public async_loop_base_t<t_log, t_time> {

    typedef std::function<void(t_params...)> worker;
    typedef std::function<std::tuple<t_params...>()> provider;

    async_loop_t(t_time p_timeout, timeout_callback p_timeout_callback,
                 worker p_worker, provider p_provider)
        : async_loop_base_t<t_log, t_time>(p_timeout, p_timeout_callback),
          m_worker(p_worker),
          m_provider(p_provider) {}

protected:
    void loop() override {
        DEB(this->m_log, "entering loop");


        while (true) {
            DEB(this->m_log, "another loop, m_stopped = ", this->m_stopped);
            if (this->m_stopped) {
                break;
            }

            std::optional<std::tuple<t_params...>> _maybe_data =
                    execute(this->m_provider_timeout, m_provider);

            if (this->m_stopped) {
                break;
            }

            if (_maybe_data) {
                std::tuple<t_params...> _params = std::move(*_maybe_data);
                auto _worker = [this, _params]() -> void {
                    std::apply(m_worker, _params);
                };

                if (!execute(this->m_timeout, _worker)) {
                    this->launch_timeout_callback();
                }
            }
        }
    }


private:
    worker m_worker;
    provider m_provider;
};

/// #### async_loop 3 ####
template <typename t_log, typename t_time>
struct async_loop_t<t_log, t_time, use_breaker::yes, void>
        : public async_loop_base_t<t_log, t_time> {

    typedef std::function<void()> worker;

    async_loop_t(t_time p_timeout, timeout_callback p_timeout_callback,
                 breaker p_breaker, worker p_worker)
        : async_loop_base_t<t_log, t_time>(p_timeout, p_timeout_callback),
          m_worker(p_worker),
          m_breaker(p_breaker) {}

protected:
    void loop() override {


        while (true) {
            if (this->m_stopped) {
                break;
            }

            std::optional<bool> _maybe_break =
                    execute(this->m_breaker_timeout, m_breaker);
            if ((_maybe_break) && (*_maybe_break)) {
                break;
            }

            if (!execute(this->m_timeout, m_worker)) {
                this->launch_timeout_callback();
            }
        }
    }


private:
    worker m_worker;
    breaker m_breaker;
};

/// #### async_loop 4 ####
template <typename t_log, typename t_time>
struct async_loop_t<t_log, t_time, use_breaker::no, void>
        : public async_loop_base_t<t_log, t_time> {

    typedef std::function<void()> worker;

    async_loop_t(t_time p_timeout, timeout_callback p_timeout_callback,
                 worker p_worker)
        : async_loop_base_t<t_log, t_time>(p_timeout, p_timeout_callback),
          m_worker(p_worker) {}

protected:
    void loop() override {

        while (true) {

            if (this->m_stopped) {
                break;
            }

            if (!execute(this->m_timeout, m_worker)) {
                this->launch_timeout_callback();
            }

        }
    }


private:
    worker m_worker;
};

/// \brief Executes a work function in fixed period of times
template <typename t_log, typename t_async_loop> struct sleeping_loop_base_t {

    /// \brief used to notify about timeout of \p operation
    typedef std::function<void(std::thread::id)> timeout_callback;

    /// \brief default constructor not allowed
    sleeping_loop_base_t() = delete;

    /// \brief copy constructor not allowed
    sleeping_loop_base_t(const sleeping_loop_base_t &) = delete;

    /// \brief move constructor
    sleeping_loop_base_t(sleeping_loop_base_t &&p_sleep) = delete;

    /// \brief copy assignment not allowed
    sleeping_loop_base_t &operator=(const sleeping_loop_base_t &) = delete;

    /// \brief move assignment
    sleeping_loop_base_t &operator=(sleeping_loop_base_t &&p_sleep) = delete;

    /// \brief destructor interrupts the loop
    inline virtual ~sleeping_loop_base_t() {
        //    DEB(this->m_log, "destructor");
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
        m_interval = to_interval(p_interval);
    }

    //  /// \brief Retrieves the operation defined to be executed
    //  inline typename t_async_loop::worker get_worker() const {
    //    return m_async.get_worker();
    //  }

    /// \brief Stops the loop, and starts it again
    void restart() {
        DEB(this->m_log, "restart");
        stop();
        start();
    }

    /// \brief run starts the loop
    void start() {
        if (!m_async.is_stopped()) {
            DEB(this->m_log, "not running async loop because it was not stopped");
            return;
        }
        DEB(this->m_log, "running async loop");
        m_stopped = false;
        m_async.start();
    }

    /// \brief stop stops the loop
    void stop() {

        if (m_async.is_stopped()) {
            DEB(this->m_log, "not stopping async loop because it was not running");
            return;
        }

        m_stopped = true;

        m_cond_var.notify_all();

        m_async.stop();
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

    template <typename t_interval, typename... t_async_params>
    sleeping_loop_base_t(t_interval p_interval,
                         t_async_params &&... p_async_params)
        : m_interval(p_interval), m_async(p_async_params...) {}

    /// \brief breaker defines if the loop should stop
    ///
    /// \return \p true if the loop should break; \p false othewise
    bool breaker() {
        DEB(m_log, "interval = ", m_interval.count());

        std::unique_lock<std::mutex> _lock(m_mutex);
        if (m_cond_var.wait_for(_lock, m_interval) == std::cv_status::timeout) {
            // timeout, so do not stop
            DEB(m_log, "must not stop");
            return false;
        }
        // no timeout, which means the loop was stopped
        DEB(m_log, "must stop");
        return true;
    }

protected:
    interval m_interval;

    t_async_loop m_async;

    /// \brief controls asynchronous execution
    std::mutex m_mutex;

    /// \brief controls asynchronous execution
    std::condition_variable m_cond_var;

    bool m_stopped{true};

    /// \brief log for the class
    t_log m_log{"concurrent::sleeping_loop"};
};

///// \brief Base class for sleeping loops, which are loops that sleep during
///// a certain amount of time, then wake up and execute some work
/////
///// \tparam t_log
/////
///// \tparam t_params are the parameters that the work function
//template <typename t_log, typename... t_params>
//struct sleeping_loop_t
//        : public sleeping_loop_base_t<
//        t_log, async_loop_t<t_log, use_breaker::yes, t_params...>> {

//    /// \brief type of work executed in a loop in time intervals
//    typedef std::function<void(t_params...)> worker;

//    /// \brief Provider is the type of function that provides data to the
//    /// work function
//    ///
//    /// \return \p an optional tuple of objects needed by the \p operation
//    typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

//    /// \brief Constructor
//    ///
//    /// \tparam t_timeout type of time used to define the timeout of the
//    /// operation
//    ///
//    /// \tparam t_interval type of time used to define the execution interval
//    /// of the operation
//    ///
//    /// \param p_timeout time used to define the timeout of the operation
//    ///
//    /// \param p_interval time used to define the execution interval of the
//    /// operation
//    ///
//    /// \param p_worker the work function to be executed at \p p_interval
//    ///
//    /// \param p_timeout_callback function to be executed if \p p_worker times
//    /// out
//    ///
//    /// \param p_provider function that provides the parameters to the \p
//    /// p_worker
//    template <typename t_timeout, typename t_interval>
//    sleeping_loop_t(t_timeout p_timeout, t_interval p_interval, worker p_worker,
//                    timeout_callback p_timeout_callback, provider p_provider)
//        : sleeping_loop_base_t<
//          t_log, async_loop_t<t_log, use_breaker::yes, t_params...>>(
//              p_interval, p_timeout, p_timeout_callback,
//              [this]() -> bool { return this->breaker(); }, p_worker,
//    p_provider) {
//        DEB(this->m_log, "timeout = ", p_timeout.count(),
//            ", interval = ", p_interval.count());
//    }
//};

///// \brief Base class for sleeping loops, which are loops that sleep during
///// a certain amount of time, then wake up and execute some work
/////
///// \tparam t_log
/////
///// \tparam t_params are the parameters that the work function
//template <typename t_log>
//struct sleeping_loop_t<t_log, void>
//        : public sleeping_loop_base_t<t_log,
//        async_loop_t<t_log, use_breaker::yes, void>> {

//    /// \brief type of work executed in a loop in time intervals
//    typedef std::function<void()> worker;

//    /// \brief Constructor
//    ///
//    /// \tparam t_interval type of time used to define the execution interval
//    /// of the operation
//    ///
//    /// \param p_interval time used to define the execution interval of the
//    /// operation
//    ///
//    /// \param p_operation the operation to be executed at \p p_interval
//    template <typename t_timeout, typename t_interval>
//    sleeping_loop_t(t_timeout p_timeout, t_interval p_interval, worker p_worker,
//                    timeout_callback p_timeout_callback)
//        : sleeping_loop_base_t<t_log,
//          async_loop_t<t_log, use_breaker::yes, void>>(
//              p_interval, p_timeout, p_timeout_callback,
//              [this]() -> bool { return this->breaker(); }, p_worker) {

//        DEB(this->m_log, "timeout = ", p_timeout.count(),
//            ", interval = ", p_interval.count());
//    }
//};

} // namespace concurrent
} // namespace tenacitas

#endif
