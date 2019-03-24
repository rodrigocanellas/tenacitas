#ifndef TENACITAS_CONCURRENT_BUS_SLEEPING_LOOP_H
#define TENACITAS_CONCURRENT_BUS_SLEEPING_LOOP_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com
///
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

#include <concurrent/bus/internal/async_loop.h>
#include <concurrent/bus/traits.h>
#include <logger/bus/cerr.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {
/// \brief namespace of the class group
namespace bus {

///
/// \brief sleeping_loop allows a function (object) to be called in a loop that
/// is executed asyncronously at a user defined period of time.
///
/// \param t_data is the type of the data to be handled. If it is not \p void,
/// it must be:
///    - default constructible
///    - move constructible
///
template<typename t_data>
struct sleeping_loop
{
    ///
    /// \brief work_t is the type of work function, i.e., the function that will
    /// be called in a loop in order to execute some work
    ///
    typedef typename loop_traits<t_data>::work_t work_t;

    ///
    /// \brief provide_t is the type of function that provides data to the work
    /// function during the loop execution, if \p t_data is not void
    ///
    typedef typename loop_traits<t_data>::provide_t provide_t;

    ///
    /// \brief sleeping_loop creates a \p sleeping_loop object, when <tt>bt_data
    /// != void</tt>, and a \p provider is necessary
    ///
    /// \param p_interval the amount of time that the loop will sleep between
    /// executions
    /// \param p_work function that will be executed each time the loop wakes up
    /// \param p_timeout amount of time that the loop will wait for \p p_work to
    /// execute
    /// \param p_provide function that will provide data to the work function,
    /// each time the loop wakes up
    ///
    sleeping_loop(std::chrono::milliseconds p_interval,
                  work_t&& p_work,
                  std::chrono::milliseconds p_timeout,
                  provide_t&& p_provide)
      : m_async(std::move(p_work),
                std::move(p_timeout),
                [this]() -> bool { return this->break_loop(); },
                std::move(p_provide))
      , m_interval(p_interval)
    {}

    ///
    /// \brief sleeping_loop creates a \p sleeping_loop object, when <tt>bt_data
    /// == void</tt>, and a \p provider is not necessary
    ///
    /// \param p_interval the amount of time that the loop will sleep between
    /// executions
    /// \param p_work function that will be executed each time the loop wakes up
    /// \param p_timeout amount of time that the loop will wait for \p p_work to
    /// execute
    ///
    sleeping_loop(std::chrono::milliseconds p_interval,
                  work_t&& p_work,
                  std::chrono::milliseconds p_timeout)
      : m_async(std::move(p_work),
                std::move(p_timeout),
                [this]() -> bool { return this->break_loop(); },
                []() -> void {})
      , m_interval(p_interval)
    {}

    /// \brief default constructor not allowed
    sleeping_loop() = delete;

    /// \brief copy constructor not allowed
    sleeping_loop(const sleeping_loop&) = delete;

    /// \brief move constructor
    sleeping_loop(sleeping_loop&& p_sleep) noexcept
    {
        move(std::move(p_sleep));
    }

    /// \brief destructor interrupts the loop
    inline ~sleeping_loop()
    {
        cerr_debug(this, " destructor");
        stop();
    }

    /// \brief copy assignment not allowed
    sleeping_loop& operator=(const sleeping_loop&) = delete;

    /// \brief move assignment
    sleeping_loop& operator=(sleeping_loop&& p_sleep) noexcept
    {
        if (this != &p_sleep) {
            move(std::move(p_sleep));
        }
        return *this;
    }

    ///
    /// \brief is_stopped
    /// \return \p true if the loop is not running; \p false othewise
    ///
    inline bool is_stopped() const { return m_async.is_stopped(); }

    ///
    /// \brief get_work
    /// \return a copy of the function that executes a defined work in each
    /// round of the loop
    ///
    inline work_t get_work() const { return m_async.get_work(); }

    ///
    /// \brief get_provide
    /// \return a copy of the function that provides an instance of \p t_data,
    /// if available, to the work function
    ///
    inline provide_t get_provide() const { return m_async.get_provide(); }

    ///
    /// \brief get_timeout
    /// \return the amount of time that the loop will wait for the work function
    /// to finish
    ///
    inline std::chrono::milliseconds get_timeout() const
    {
        return m_async.get_timeout();
    }

    ///
    /// \brief get_interval
    /// \return the interval of loop execution
    ///
    inline std::chrono::milliseconds get_interval() const { return m_interval; }

    /// \brief run starts the loop
    void run()
    {
        if (!m_async.is_stopped()) {
            cerr_debug(this,
                       " not running async loop because it was not stopped");
            return;
        }
        cerr_debug(this, " running async loop");
        m_async.run();
    }

    /// \brief stop stops the loop
    void stop()
    {
        if (m_async.is_stopped()) {
            cerr_debug(this,
                       " not stopping async loop because it was not running");
            return;
        }
        cerr_debug(this, " stop");
        m_cond_var.notify_all();

        cerr_debug(this, " all notified, and m_async = ", &m_async);
        m_async.stop();
    }

  private:
    /// \brief a simpler name
    typedef bus::async_loop<t_data> async_loop_t;

  private:
    ///
    /// \brief move a \p sleeping_loop to this
    /// \param p_sleep the \p sleeping_loop to be moved
    ///
    inline void move(sleeping_loop&& p_sleep) noexcept
    {
        // save if the right side was running
        bool _stopped = p_sleep.is_stopped();
        // stop the right side
        p_sleep.stop();

        // move the interval
        m_interval = std::move(p_sleep.m_interval);

        // move the async_loop, reseting the break loop
        m_async = async_loop_t(std::move(p_sleep.get_work()),
                               std::move(p_sleep.get_timeout()),
                               [this]() -> bool { return this->break_loop(); },
                               std::move(p_sleep.get_provide()));

        // if the right side was not stopped
        if (!_stopped) {
            // run this sleeping_loop
            run();
        }
    }

    ///
    /// \brief break_loop function that defines if the loop should stop
    /// \return \p true if the loop should break; \p false othewise
    ///
    bool break_loop()
    {
        std::unique_lock<std::mutex> _lock(m_mutex);
        if (m_cond_var.wait_for(_lock, m_interval) == std::cv_status::timeout) {
            // timeout, so do not stop
            cerr_debug(this, " must not stop");
            return false;
        }
        // no timeout, so do stop
        cerr_debug(this, " must stop");
        return true;
    }

  private:
    /// \brief m_loop the async_loop used
    async_loop_t m_async;

    /// \brief m_interval
    std::chrono::milliseconds m_interval;

    /// \brief m_mutex
    std::mutex m_mutex;

    /// \brief m_cond_var
    std::condition_variable m_cond_var;
};

} // namespace bus
} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_SLEEPING_LOOP_H
