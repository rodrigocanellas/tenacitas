#ifndef TENACITAS_CONCURRENT_H
#define TENACITAS_CONCURRENT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <condition_variable>
#include <functional>
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

///// \brief The thread class is a wrapper for the std::thread, which joins in
///// destructor and move assignment
// template <typename t_log, bool use = true> class thread {
// public:
//  /// \brief thread constructor
//  ///
//  /// \param p_func function that will run in a separated thread
//  inline explicit thread(std::function<void()> &&p_func,
//                         const std::string &p_id = "")
//      : m_thread(std::move(p_func)),
//        m_log((p_id.empty() ? "concurrent::thread"
//                            : "concurrent::thread " + p_id)) {}

//  inline thread() = default;
//  thread(const thread &) = delete;
//  inline thread(thread &&) noexcept = default;
//  thread &operator=(const thread &) = delete;

//  ///
//  /// \brief operator = move joins the \p this thread, if the thread is still
//  /// running
//  ///
//  /// \param p_th thread to be move to \p this
//  ///
//  /// \return the new thread
//  inline thread &operator=(thread &&p_th) noexcept {
//    DEB(m_log, "move assignment");
//    join();
//    m_thread = std::move(p_th.m_thread);
//    return *this;
//  }

//  inline friend std::ostream &operator<<(std::ostream &out,
//                                         const thread &p_thread) {
//    out << "(" << &p_thread << ", " << p_thread.m_thread.get_id() << ")";
//    return out;
//  }

//  /// \brief join waits for the thread to finish
//  inline void join() {
//    if (m_thread.joinable()) {
//      DEB(m_log, "joining");
//      m_thread.join();
//    }
//  }

//  inline void detach() {
//    DEB(m_log, "detaching");
//    m_thread.detach();
//  }

//  inline std::thread::id get_id() const { return m_thread.get_id(); }

//  /// \brief ~thread joins if the thread is still running
//  inline ~thread() {
//    //    DEB(m_log, "destructor");
//    join();
//  }

//  inline bool joinable() const { return m_thread.joinable(); }

// private:
//  /// \brief m_thread the wrappered std::thread
//  std::thread m_thread;

//  t_log m_log;
//};

/// \brief Type of function executed when a function times out
typedef std::function<void(std::thread::id)> timeout_callback;

/// \brief Base class to allow a function to be executed with timeout control,
/// but not starting a thread for each execution
template <typename t_log> struct executer_base_t {

    template <typename t_time>
    executer_base_t(std::function<void()> p_work, t_time p_timeout,
                    timeout_callback p_timeout_callback)
        : m_work(p_work), m_timeout(to_timeout(p_timeout)),
          m_timeout_callback(p_timeout_callback) {
        start();
    }

    executer_base_t() = delete;
    executer_base_t(const executer_base_t &) = delete;
    executer_base_t(executer_base_t &&) = delete;

    executer_base_t &operator=(const executer_base_t &) = delete;
    executer_base_t &operator=(executer_base_t &&) = delete;

    ~executer_base_t() { stop(); }

    inline void set_log_debug_level() { m_log.set_debug_level(); }
    inline void set_log_info_level() { m_log.set_info_level(); }
    inline void set_log_level() { m_log.set_warn_level(); }



    template <typename t_result>
    t_result call(std::function<t_result()> p_ok,
                  std::function<t_result()> p_not_ok,
                  std::function<void()> p_save_params) {
        std::lock_guard<std::mutex> _lock(m_mutex_call);
        if (m_stopped) {
            ERR(m_log, "must start");
            return p_not_ok();
        }

        p_save_params();

        {
            DEB(m_log, "waiting for permission to send notification to work");
            std::unique_lock<std::mutex> _lock(m_mutex_notify);
            m_cond_notify.wait(_lock, [this]() -> bool { return m_can_notify; });
        }

        {
            std::lock_guard<std::mutex> _lock(m_mutex_working);
            if (m_stopped) {
                DEB(m_log, "stopping");
                return p_not_ok();
            }
            DEB(m_log, "notifying work to be done");
            m_cond_working.notify_one();
        }

        DEB(m_log, "waiting for work to return");
        {
            std::unique_lock<std::mutex> _lock(m_mutex_worked);
            if (m_stopped) {
                DEB(m_log, "stopping");
                return p_not_ok();
            }
            if (m_cond_worked.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
                WAR(m_log, "timeout");
                m_loops.begin()->second = true;
                m_stopped = true;
                start();
                std::thread([this]() -> void {
                                m_timeout_callback(m_loops.begin()->first.get_id());
                            }).detach();
                return p_not_ok();
            }
            DEB(m_log, "no timeout");
            return p_ok();
        }
    }

private:
    typedef std::pair<std::thread, bool> item;
    typedef std::list<item> loops;

private:
    void loop(bool &p_abandoned) {
        while (true) {
            if ( m_stopped || p_abandoned) {
                DEB(m_log, "stopped or abandoned... stopped? ", m_stopped);
                break;
            }

            {
                DEB(m_log, "notifying permission to send notification to work");
                std::lock_guard<std::mutex> _lock(m_mutex_notify);
                if ( m_stopped || p_abandoned) {
                    DEB(m_log, "stopped or abandoned... stopped? ", m_stopped);
                    break;
                }
                m_can_notify = true;
                m_cond_notify.notify_one();
            }

            {
                std::unique_lock<std::mutex> _lock(m_mutex_working);
                if ( m_stopped || p_abandoned) {
                    DEB(m_log, "stopped or abandoned... stopped? ", m_stopped);
                    break;
                }
                DEB(m_log, "waiting for work to do");
                m_cond_working.wait(_lock);
            }
            if ( m_stopped || p_abandoned) {
                DEB(m_log, "stopped or abandoned... stopped? ", m_stopped);
                break;
            }

            m_can_notify = false;

            DEB(m_log, "working");
            m_work();

            {
                std::lock_guard<std::mutex> _lock(m_mutex_worked);
                if ( m_stopped || p_abandoned) {
                    DEB(m_log, "stopped or abandoned... stopped? ", m_stopped);
                    break;
                }
                DEB(m_log, "notifying work done");
                m_cond_worked.notify_one();
            }
        }
    }

    void start() {
        DEB(m_log, "starting");
        m_stopped = false;
        m_loops.push_front({std::thread(), false});
        m_loops.begin()->first =
                std::thread([this]() -> void { loop(m_loops.begin()->second); });
        std::this_thread::sleep_for(50ms);
    }

    void stop() {
        DEB(m_log, "stopping");
        m_stopped = true;
        m_cond_working.notify_all();

        for (item &_item : m_loops) {
            if (_item.first.joinable()) {
                _item.first.join();
            }
        }
    }

private:
    std::function<void()> m_work;
    timeout m_timeout;
    timeout_callback m_timeout_callback;
    bool m_stopped{true};

    std::mutex m_mutex_call;

    std::mutex m_mutex_working;
    std::condition_variable m_cond_working;

    std::mutex m_mutex_worked;
    std::condition_variable m_cond_worked;

    std::mutex m_mutex_notify;
    std::condition_variable m_cond_notify;
    bool m_can_notify{false};

    loops m_loops;

    t_log m_log{"executer"};
};

// ########## 1 ##########
template <typename t_log, typename t_result, typename... t_params>
struct executer_t {

    typedef std::function<t_result(t_params...)> worker;

    template <typename t_time>
    executer_t(worker p_worker, t_time p_timeout,
               timeout_callback p_timeout_callback)
        : m_exec(
              [this, p_worker]() -> void {
        m_result = std::apply(p_worker, std::move(m_params));
    },
    p_timeout, p_timeout_callback) {}

    std::optional<t_result> operator()(t_params... p_params) {
        auto ok = [this]() -> std::optional<t_result> { return {m_result}; };
        auto not_ok = []() -> std::optional<t_result> { return {}; };
        auto save_params = [this, p_params...]() -> void {
            m_params = {p_params...};
        };

        return m_exec.template call<std::optional<t_result>>(ok, not_ok,
                                                             save_params);
    }

private:
    executer_base_t<t_log> m_exec;

    std::tuple<t_params...> m_params;

    t_result m_result;
};

// ########## 2 ##########
template <typename t_log, typename t_result>
struct executer_t<t_log, t_result, void> {
    typedef std::function<t_result(void)> worker;

    template <typename t_time>
    executer_t(worker p_worker, t_time p_timeout,
               timeout_callback p_timeout_callback)
        : m_exec([this, p_worker]() -> void { m_result = p_worker(); }, p_timeout,
    p_timeout_callback) {}

    std::optional<t_result> operator()() {
        auto ok = [this]() -> std::optional<t_result> { return {m_result}; };
        auto not_ok = []() -> std::optional<t_result> { return {}; };
        auto save_params = []() -> void {};

        return m_exec.template call<std::optional<t_result>>(ok, not_ok,
                                                             save_params);
    }

private:
    executer_base_t<t_log> m_exec;

    t_result m_result;
};

// ########## 3 ##########
template <typename t_log, typename... t_params>
struct executer_t<t_log, void, t_params...> {
    typedef std::function<void(t_params...)> worker;

    template <typename t_time>
    executer_t(worker p_worker, t_time p_timeout,
               timeout_callback p_timeout_callback)
        : m_exec(
              [this, p_worker]() -> void {
        std::apply(p_worker, std::move(m_params));
    },
    p_timeout, p_timeout_callback) {}

    void operator()(t_params... p_params) {
        auto ok = []() -> void {};
        auto not_ok = []() -> void {};
        auto save_params = [this, p_params...]() -> void {
            m_params = {p_params...};
        };

        return m_exec.template call<void>(ok, not_ok, save_params);
    }

private:
    executer_base_t<t_log> m_exec;

    std::tuple<t_params...> m_params;
};

// ########## 4 ##########
template <typename t_log> struct executer_t<t_log, void, void> {

    typedef std::function<void()> worker;

    template <typename t_time>
    executer_t(worker p_work, t_time p_timeout,
               timeout_callback p_timeout_callback)
        : m_exec(p_work, p_timeout, p_timeout_callback) {}

    void operator()() {
        auto _ok = []() -> void {};
        auto _not_ok = []() -> void {};
        auto _save_params = []() -> void {};
        m_exec.template call<void>(_ok, _not_ok, _save_params);
    }

private:
    executer_base_t<t_log> m_exec;
};

/// \brief Type of function used to inform if a loop should stop
typedef std::function<bool()> breaker;

/// \brief base class for asynchronous loop
template <typename t_log> struct async_loop_base_t {

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
    async_loop_base_t() = default;

    virtual void loop() = 0;

protected:
    bool m_stopped{true};

    std::thread m_thread;

    t_log m_log{"concurrent::async_loop"};
};

/// \brief Used to define if a loop will use a breaker function or not
enum class use_breaker : char { yes = 'y', no = 'n' };

/// \brief Base class for asynchronous loop
template <typename t_log, use_breaker use, typename... t_params>
struct async_loop_t;

/// #### async_loop 1 ####
template <typename t_log, typename... t_params>
struct async_loop_t<t_log, use_breaker::yes, t_params...>
        : public async_loop_base_t<t_log> {

    typedef std::function<void(t_params...)> worker;
    typedef std::function<std::tuple<t_params...>()> provider;

    template <typename t_time>
    async_loop_t(t_time p_timeout, timeout_callback p_timeout_callback,
                 breaker p_breaker, worker p_worker, provider p_provider)
        : async_loop_base_t<t_log>(),
          m_worker(p_worker, p_timeout, p_timeout_callback, "w"),
          m_provider(
              p_provider, 500ms,
              [this](std::thread::id p_id) -> void {
        WAR(this->m_log, "thread ", p_id, " for provider has timed out");
    },
    "p"),
    m_breaker(
            p_breaker, 300ms,
            [this](std::thread::id p_id) -> void {
        WAR(this->m_log, "thread ", p_id, " for breaker has timed out");
    },
    "b") {}

protected:
void loop() override {
    m_worker.start();
    m_provider.start();
    m_breaker.start();
    while (true) {
        if (this->m_stopped) {
            m_worker.stop();
            m_provider.stop();
            m_breaker.stop();
            break;
        }

        std::optional<bool> _maybe_break = m_breaker();
        if ((_maybe_break) && (*_maybe_break)) {
            break;
        }

        std::optional<std::tuple<t_params...>> _maybe_data = m_provider();

        if (this->m_stopped) {
            break;
        }

        _maybe_break = m_breaker();
        if ((_maybe_break) && (*_maybe_break)) {
            break;
        }

        if (_maybe_data) {
            std::tuple<t_params...> _params = std::move(*_maybe_data);

            std::apply(m_worker, _params);
        }
    }
}

private:
typedef executer_t<t_log, void, t_params...> worker_executer;
typedef executer_t<t_log, std::tuple<t_params...>, void> provider_executer;
typedef executer_t<t_log, bool, void> breaker_executer;

private:
worker_executer m_worker;
provider_executer m_provider;
breaker_executer m_breaker;
};

/// #### async_loop 2 ####
template <typename t_log, typename... t_params>
struct async_loop_t<t_log, use_breaker::no, t_params...>
        : public async_loop_base_t<t_log> {

    typedef std::function<void(t_params...)> worker;
    typedef std::function<std::tuple<t_params...>()> provider;

    template <typename t_time>
    async_loop_t(t_time p_timeout, timeout_callback p_timeout_callback,
                 worker p_worker, provider p_provider)
        : async_loop_base_t<t_log>(),
          m_worker(p_worker, p_timeout, p_timeout_callback, "w"),
          m_provider(
              p_provider, 500ms,
              [this](std::thread::id p_id) -> void {
        WAR(this->m_log, "thread ", p_id, " for provider has timed out");
    },
    std::string{"p"}) {}

protected:
void loop() override {
    DEB(this->m_log, "entering loop");

    m_worker.start();
    m_provider.start();

    while (true) {
        DEB(this->m_log, "another loop, m_stopped = ", this->m_stopped);
        if (this->m_stopped) {
            m_worker.stop();
            m_provider.stop();
            break;
        }

        std::optional<std::tuple<t_params...>> _maybe_data = m_provider();

        if (this->m_stopped) {
            break;
        }

        if (_maybe_data) {
            std::tuple<t_params...> _params = std::move(*_maybe_data);

            std::apply(m_worker, _params);
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

/// #### async_loop 3 ####
template <typename t_log>
struct async_loop_t<t_log, use_breaker::yes, void>
        : public async_loop_base_t<t_log> {

    typedef std::function<void()> worker;

    template <typename t_time>
    async_loop_t(t_time p_timeout, timeout_callback p_timeout_callback,
                 breaker p_breaker, worker p_worker)
        : async_loop_base_t<t_log>(),
          m_worker(p_worker, p_timeout, p_timeout_callback, "w"),
          m_breaker(
              p_breaker, 300ms,
              [this](std::thread::id p_id) -> void {
        WAR(this->m_log, "thread ", p_id, " for breaker has timed out");
    },
    "b") {}

protected:
void loop() override {
    m_worker.start();
    m_breaker.start();

    while (true) {
        if (this->m_stopped) {
            m_worker.stop();

            m_breaker.stop();

            break;
        }

        std::optional<bool> _maybe_break = m_breaker();
        if ((_maybe_break) && (*_maybe_break)) {
            break;
        }

        m_worker();
    }
}

private:
typedef executer_t<t_log, void, void> worker_executer;
typedef executer_t<t_log, bool, void> breaker_executer;

private:
worker_executer m_worker;
breaker_executer m_breaker;
};

/// #### async_loop 4 ####
template <typename t_log>
struct async_loop_t<t_log, use_breaker::no, void>
        : public async_loop_base_t<t_log> {

    typedef std::function<void()> worker;

    template <typename t_time>
    async_loop_t(t_time p_timeout, timeout_callback p_timeout_callback,
                 worker p_worker)
        : async_loop_base_t<t_log>(),
          m_worker(p_worker, p_timeout, p_timeout_callback, "w") {}

protected:
    void loop() override {
        m_worker.start();

        while (true) {

            if (this->m_stopped) {
                m_worker.stop();

                break;
            }

            m_worker();
        }
    }

private:
    typedef executer_t<t_log, void, void> worker_executer;

private:
    worker_executer m_worker;
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

/// \brief Base class for sleeping loops, which are loops that sleep during
/// a certain amount of time, then wake up and execute some work
///
/// \tparam t_log
///
/// \tparam t_params are the parameters that the work function
template <typename t_log, typename... t_params>
struct sleeping_loop_t
        : public sleeping_loop_base_t<
        t_log, async_loop_t<t_log, use_breaker::yes, t_params...>> {

    /// \brief type of work executed in a loop in time intervals
    typedef std::function<void(t_params...)> worker;

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
    /// \param p_timeout_callback function to be executed if \p p_worker times
    /// out
    ///
    /// \param p_provider function that provides the parameters to the \p
    /// p_worker
    template <typename t_timeout, typename t_interval>
    sleeping_loop_t(t_timeout p_timeout, t_interval p_interval, worker p_worker,
                    timeout_callback p_timeout_callback, provider p_provider)
        : sleeping_loop_base_t<
          t_log, async_loop_t<t_log, use_breaker::yes, t_params...>>(
              p_interval, p_timeout, p_timeout_callback,
              [this]() -> bool { return this->breaker(); }, p_worker,
              p_provider) {
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
        : public sleeping_loop_base_t<t_log,
        async_loop_t<t_log, use_breaker::yes, void>> {

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
                    timeout_callback p_timeout_callback)
        : sleeping_loop_base_t<t_log,
          async_loop_t<t_log, use_breaker::yes, void>>(
              p_interval, p_timeout, p_timeout_callback,
              [this]() -> bool { return this->breaker(); }, p_worker) {

        DEB(this->m_log, "timeout = ", p_timeout.count(),
            ", interval = ", p_interval.count());
    }
};
} // namespace concurrent

} // namespace tenacitas

#endif
