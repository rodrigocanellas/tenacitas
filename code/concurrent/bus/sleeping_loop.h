#ifndef TENACITAS_CONCURRENT_BUS_SLEEPING_LOOP_H
#define TENACITAS_CONCURRENT_BUS_SLEEPING_LOOP_H

///\author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \date oct/2018

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
namespace bus {

///
/// \brief sleeping_loop allows a function (object) to be called in a loop that
/// is executed at a user defined period of time.
///
///
template<typename t_data>
struct sleeping_loop
{

    typedef typename loop_traits<t_data>::work_t work_t;
    typedef typename loop_traits<t_data>::provide_t provide_t;
    //    typedef typename loop_traits<t_data>::break_t break_t;

    ///
    /// \brief sleeping_loop creates a \p sleeping_loop object, when \bt_data !=
    /// void\b, where a \p provider is set
    ///
    /// \param p_interval the amount of time that the loop will sleep between
    /// executions
    /// \param p_work function that will be executed each time the loop wakes up
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

    sleeping_loop(std::chrono::milliseconds p_interval,
                  work_t&& p_work,
                  std::chrono::milliseconds p_timeout)
      : m_async(std::move(p_work),
                std::move(p_timeout),
                [this]() -> bool { return this->break_loop(); },
                []() -> void {})
      , m_interval(p_interval)
    {}

    sleeping_loop() = delete;

    sleeping_loop(const sleeping_loop&) = delete;
    sleeping_loop(sleeping_loop&& p_sleep) noexcept = delete;

    ///
    /// \brief destructor interrupts the loop
    ///
    inline ~sleeping_loop()
    {
        cerr_debug(this, " destructor");
        stop();
    }

    sleeping_loop& operator=(const sleeping_loop&) = delete;
    sleeping_loop& operator=(sleeping_loop&&) noexcept = delete;

    inline bool is_stopped() const { return m_async.is_stopped(); }
    inline work_t get_work() const { return m_async.get_work(); }
    inline provide_t get_provide() const { return m_async.get_provide(); }
    inline std::chrono::milliseconds get_timeout() const
    {
        return m_async.get_timeout();
    }
    inline std::chrono::milliseconds get_interval() const { return m_interval; }

    ///
    /// \brief run
    ///
    inline void run()
    {
        if (!m_async.is_stopped()) {
            cerr_debug(this,
                       " not running async loop because it was not stopped");
            return;
        }
        cerr_debug(this, " running async loop");
        m_async.run();
    }

    ///
    /// \brief stop
    ///
    inline void stop()
    {
        if (m_async.is_stopped()) {
            cerr_debug(this,
                       " not stopping async loop because it was not running");
            return;
        }
        cerr_debug(this, " stop");
        m_cond_var.notify_all();
        //        m_break_loop.stop_loop();
        cerr_debug(this, " all notified, and m_async = ", &m_async);
        m_async.stop();
    }

  private:
    typedef bus::async_loop<t_data> async_loop_t;

  private:
    void* operator new(size_t p_size) { return std::malloc(p_size); }

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
    ///
    /// \brief m_loop the async_loop used
    ///
    async_loop_t m_async;

    //    bus::sleeping_loop_break m_break_loop;

    ///
    /// \brief m_interval
    ///
    std::chrono::milliseconds m_interval;

    ///
    /// \brief m_mutex
    ///
    std::mutex m_mutex;

    ///
    /// \brief m_cond_var
    ///
    std::condition_variable m_cond_var;
};

} // namespace bus
} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_SLEEPING_LOOP_H
