#ifndef TENACITAS_CONCURRENT_ASYNC_H
#define TENACITAS_CONCURRENT_ASYNC_H

#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <utility>

#include <calendar/bus/epoch.h>
#include <concurrent/bus/thread.h>
#include <logger/bus/cerr.h>

namespace tenacitas {
namespace concurrent {

inline void
async_reset(bool& p_finished, std::thread& p_work, std::thread& p_wait)
{
    cerr_debug("reseting...");
    p_finished = false;
    if (p_work.joinable()) {
        cerr_debug("joining work");
        p_work.join();
    }

    if (p_wait.joinable()) {
        cerr_debug("joining wait");
        p_wait.join();
    }
    cerr_debug("done reseting");
}

inline bool
async_wait(bool p_finished,
           std::chrono::milliseconds p_time,
           std::mutex& p_mutex,
           std::condition_variable& p_cond)
{
    cerr_debug("starting to wait");
    if (p_finished) {
        cerr_debug("no need to wait");
        return false;
    }

    std::unique_lock<std::mutex> _lock(p_mutex);
    if (p_cond.wait_for(_lock, p_time) == std::cv_status::timeout) {
        cerr_debug("timeout!");
        return true;
    }
    cerr_debug("no timeout");
    return false;
}

template<typename t_result>
struct async
{
    async() = default;

    ~async() { async_reset(m_finished, m_work, m_wait); }
    async(const async&) = delete;
    async(async&&) noexcept = delete;

    async& operator=(const async&) = delete;
    async& operator=(async&&) noexcept = delete;

    void operator()(std::function<t_result()> p_function,
                    std::chrono::milliseconds p_time)
    {
        async_reset(m_finished, m_work, m_wait);

        m_work = std::thread([this, p_function] {
            m_result = p_function();
            m_finished = true;
            m_cond.notify_all();
        });

        wait(p_time);
    }

    std::pair<bool, t_result> timeout()
    {
        if (m_wait.joinable()) {
            m_wait.join();
        }
        return std::make_pair(std::move(m_timeout), std::move(m_result));
    }

  private:
    void wait(std::chrono::milliseconds p_time)
    {
        m_timeout = async_wait(m_finished, p_time, m_mutex, m_cond);

        if (m_timeout) {
            m_result = t_result();
        }
    }

  private:
    std::thread m_work;
    std::thread m_wait;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    t_result m_result;
    bool m_timeout = { false };
    bool m_finished = { false };
};

template<>
struct async<void>
{
    async() = default;

    ~async() { async_reset(m_finished, m_work, m_wait); }
    async(const async&) = delete;
    async(async&&) noexcept = delete;

    async& operator=(const async&) = delete;
    async& operator=(async&&) noexcept = delete;

    ///
    ///
    ///
    void operator()(std::function<void()> p_function,
                    std::chrono::milliseconds p_time)
    {
        async_reset(m_finished, m_work, m_wait);

        m_work = std::thread([this, p_function] {
            p_function();
            m_finished = true;
            m_cond.notify_all();
        });

        wait(p_time);
    }

    bool timeout()
    {
        if (m_wait.joinable()) {
            m_wait.join();
        }
        return m_timeout;
    }

  private:
    void wait(std::chrono::milliseconds p_time)
    {
        m_timeout = async_wait(m_finished, p_time, m_mutex, m_cond);
    }

  private:
    std::thread m_work;
    std::thread m_wait;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    bool m_timeout = { false };
    bool m_finished = { false };
};

} // namespace concurrent
} // namespace tenacitas

#endif // TIME_OUT_H
