#ifndef TENACITAS_CONCURRENT_BUS_ASYNC_LOOP_H
#define TENACITAS_CONCURRENT_BUS_ASYNC_LOOP_H

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \date ago/2018

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

namespace tenacitas {
namespace concurrent {
namespace bus {

///
///
///
template<typename t_data>
struct async_loop
{
    typedef typename loop_traits<t_data>::work_t work_t;
    typedef typename loop_traits<t_data>::provide_t provide_t;
    typedef typename loop_traits<t_data>::break_t break_t;

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
        tenacitas::logger::bus::configure_cerr_log();
    }

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

    async_loop() = delete;
    async_loop(const async_loop&) = delete;
    async_loop(async_loop&& p_async) noexcept
      : m_loop(std::move(p_async.m_loop))
      , m_thread()
    {
        if (!p_async.is_stopped()) {
            run_core();
        }
    }

    async_loop& operator=(const async_loop&) = delete;
    async_loop& operator=(async_loop&& p_loop) noexcept = delete;

    inline ~async_loop()
    {
        cerr_debug(this, " destructor");
        stop();
    }

    inline bool is_stopped() const { return m_loop.is_stopped(); }
    inline work_t get_work() const { return m_loop.get_work(); }
    inline break_t get_break() const { return m_loop.get_break(); }
    inline provide_t get_provide() const { return m_loop.get_provide(); }
    inline std::chrono::milliseconds get_timeout() const
    {
        return m_loop.get_timeout();
    }

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
        //        m_thread.join();
    }

  private:
    typedef loop<t_data> loop_t;
    typedef concurrent::bus::thread thread_t;

  private:
    void* operator new(size_t p_size) { return std::malloc(p_size); }

    void run_core()
    {
        std::lock_guard<std::mutex> _lock(m_mutex);
        m_thread =
          concurrent::bus::thread([this]() -> void { m_loop.start(); });
    }

  private:
    loop_t m_loop;
    thread_t m_thread;
    std::mutex m_mutex;
};

} // namespace bus
} // namespace concurrent
} // namespace tenacitas

#endif
