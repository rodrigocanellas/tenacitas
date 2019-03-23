#ifndef TENACITAS_CONCURRENT_BUS_ASYNC_LOOP_H
#define TENACITAS_CONCURRENT_BUS_ASYNC_LOOP_H

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

#include <concurrent/bus/internal/loop.h>
#include <concurrent/bus/thread.h>
#include <concurrent/bus/traits.h>
#include <logger/bus/cerr.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {
/// \brief namespace of the class group
namespace bus {

///
/// \brief allows to execute a loop asyncronously
///
/// \param t_data is the type of the data to be handled. If it is not \p void,
/// it must be:
///    - default constructible
///    - move constructible
///
///
template<typename t_data>
struct async_loop
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
    /// \brief break_t is the type of function that indicates if the loop should
    /// stop
    ///
    typedef typename loop_traits<t_data>::break_t break_t;

    ///
    /// \brief async_loop constructor
    ///
    /// This constructor should be used when \p t_data is not \p void
    ///
    /// \param p_work instance of the function that will execute the defined
    /// work
    /// \param p_timeout amount of time that the loop will wait for \p p_work to
    /// execute
    /// \param p_break instance of the function that will indicate when the loop
    /// must stop
    /// \param p_provide instance of the function that will provide an instance
    /// of \p t_data, if available
    ///
    async_loop(work_t&& p_work,
               std::chrono::milliseconds p_timeout,
               break_t&& p_break,
               provide_t&& p_provide)
      : m_loop(std::move(p_work),
               std::move(p_timeout),
               std::move(p_break),
               std::move(p_provide))
      , m_thread()
    {
        // initiates the log library
        tenacitas::logger::bus::configure_cerr_log();
    }

    ///
    /// \brief async_loop constructor
    ///
    /// This constructor should be used when \p t_data is \p void
    ///
    /// \param p_work instance of the function that will execute the defined
    /// work
    /// \param p_timeout amount of time that the loop will wait for \p p_work to
    /// execute
    /// \param p_break instance of the function that will indicate when the loop
    /// must stop
    ///
    async_loop(work_t&& p_work,
               std::chrono::milliseconds p_timeout,
               break_t&& p_break)
      : m_loop(std::move(p_work),
               std::move(p_timeout),
               std::move(p_break),
               []() -> void {})
      , m_thread()
    {
        tenacitas::logger::bus::configure_cerr_log();
    }

    /// \brief default constructor not allowed
    async_loop() = delete;

    /// \brief copy constructor not allowed
    async_loop(const async_loop&) = delete;

    ///
    /// \brief async_loop
    /// \param p_async an instance o \p async_loop to be moved
    ///
    async_loop(async_loop&& p_async) noexcept
      : m_loop(std::move(p_async.m_loop))
      , m_thread()
    {
        if (!p_async.is_stopped()) {
            run_core();
        }
    }

    /// \brief copy assignment not allowed
    async_loop& operator=(const async_loop&) = delete;

    /// \brief move assignment not allowed
    async_loop& operator=(async_loop&& p_loop) noexcept = delete;

    /// \brief destructor
    inline ~async_loop()
    {
        cerr_debug(this, " destructor");
        stop();
    }

    ///
    /// \brief is_stopped
    /// \return \p true if the loop is not running; \p false othewise
    ///
    inline bool is_stopped() const { return m_loop.is_stopped(); }

    ///
    /// \brief get_work
    /// \return a copy of the function that executes a defined work in each
    /// round of the loop
    ///
    inline work_t get_work() const { return m_loop.get_work(); }

    ///
    /// \brief get_break
    /// \return a copy of the function that can make the loop stop
    ///
    inline break_t get_break() const { return m_loop.get_break(); }

    ///
    /// \brief get_provide
    /// \return a copy of the function that provides an instance of \p t_data,
    /// if available, to the work function
    ///
    inline provide_t get_provide() const { return m_loop.get_provide(); }

    ///
    /// \brief get_timeout
    /// \return the amount of time that the loop will wait for the work function
    /// to finish
    ///
    inline std::chrono::milliseconds get_timeout() const
    {
        return m_loop.get_timeout();
    }

    ///
    /// \brief run starts the loop
    ///
    void run()
    {

        if (!m_loop.is_stopped()) {
            cerr_debug(this,
                       " not starting the loop because it is already running");
            return;
        }
        cerr_debug(this, " starting the loop");
        run_core();
    }

    ///
    /// \brief stop stops the loop
    ///
    void stop()
    {
        if (m_loop.is_stopped()) {
            cerr_debug(this,
                       " not stopping the loop because it was not running");
            return;
        }

        std::lock_guard<std::mutex> _lock(m_mutex);
        cerr_debug(this, " marking to stop");
        m_loop.stop();
        cerr_debug(this, " joining");
    }

  private:
    /// \brief loop_t is an easier name for the loop
    typedef loop<t_data> loop_t;

    /// \brief thread_t is an easier name for our wrapper to std::thread
    typedef concurrent::bus::thread thread_t;

  private:
    /// \brief common method to start the loop
    void run_core()
    {
        std::lock_guard<std::mutex> _lock(m_mutex);
        m_thread =
          concurrent::bus::thread([this]() -> void { m_loop.start(); });
    }

  private:
    /// \brief m_loop is the \p loop to be executed asyncronously
    loop_t m_loop;

    /// \brief m_thread is the thread where the \p loop will run
    thread_t m_thread;

    /// \brief m_mutex protects the start of the \p m_loop execution \p m_thread
    std::mutex m_mutex;
};

} // namespace bus
} // namespace concurrent
} // namespace tenacitas

#endif
