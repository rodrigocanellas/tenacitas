#ifndef TENACITAS_CONCURRENT_BUS_LOOP_H
#define TENACITAS_CONCURRENT_BUS_LOOP_H

#include <chrono>
#include <future>

#include <concurrent/bus/traits.h>
#include <logger/bus/cerr.h>

namespace tenacitas {
namespace concurrent {
namespace bus {

template<typename t_data>
struct loop
{
    typedef typename loop_traits<t_data>::work_t work_t;
    typedef typename loop_traits<t_data>::provide_t provide_t;
    typedef typename loop_traits<t_data>::break_t break_t;

    loop(work_t&& p_work,
         std::chrono::milliseconds p_timeout,
         break_t&& p_break,
         provide_t&& p_provide)
      : m_work(std::move(p_work))
      , m_timeout(std::move(p_timeout))
      , m_break(std::move(p_break))
      , m_provide(std::move(p_provide))
      , m_stopped(true)
    {}

    loop() = delete;
    ~loop() = default;
    loop(const loop&) = delete;
    loop(loop&& p_loop) noexcept
      : m_work(std::move(p_loop.m_work))
      , m_timeout(std::move(p_loop.m_timeout))
      , m_break(std::move(p_loop.m_break))
      , m_provide(std::move(p_loop.m_provide))
      , m_stopped(true)
    {}

    loop& operator=(const loop&) = delete;
    loop& operator=(loop&&) noexcept = delete;

    inline bool is_stopped() const { return m_stopped; }
    inline work_t get_work() const { return m_work; }
    inline break_t get_break() const { return m_break; }
    inline provide_t get_provide() const { return m_provide; }
    inline std::chrono::milliseconds get_timeout() const { return m_timeout; }

    inline void stop() { m_stopped = true; }

    void start()
    {
        if (!m_stopped) {
            cerr_debug(this, " not starting because it was not stopped");
            return;
        }

        m_stopped = false;

        while (true) {

            cerr_debug(this, " one more loop");

            if (m_stopped) {
                cerr_debug(this, " stopping loop");
                break;
            }

            cerr_debug(this, " waiting for data");
            std::pair<bool, t_data> _provide = m_provide();
            if (!_provide.first) {
                cerr_debug(this, " breaking because there is no data");
                break;
            }

            cerr_debug(this, " data received ", _provide.second);

            std::future<bool> _future = std::async(
              std::launch::async, std::ref(m_work), std::move(_provide.second));
            if (_future.wait_for(m_timeout) == std::future_status::ready) {
                if (!_future.get()) {
                    cerr_debug(this, " breaking because there is no more work");
                    m_stopped = true;
                    break;
                }
            } else {
                cerr_warn(this, " timeout for data ", _provide.second);
            }

            if (m_stopped) {
                cerr_debug(this, " stopping loop");
                break;
            }

            if (m_break()) {
                cerr_debug(this, " stopping because breaker said so");
                m_stopped = true;
                break;
            }
        }
    }

  private:
    work_t m_work;
    std::chrono::milliseconds m_timeout;
    break_t m_break;
    provide_t m_provide;
    bool m_stopped;
};

template<>
struct loop<void>
{
    typedef typename loop_traits<void>::work_t work_t;
    typedef typename loop_traits<void>::provide_t provide_t;
    typedef typename loop_traits<void>::break_t break_t;

    loop(work_t&& p_work,
         std::chrono::milliseconds p_timeout,
         break_t&& p_break,
         provide_t&& p_provide)
      : m_work(std::move(p_work))
      , m_timeout(std::move(p_timeout))
      , m_break(std::move(p_break))
      , m_provide(std::move(p_provide))
      , m_stopped(true)
    {}

    loop() = delete;
    ~loop() = default;
    loop(const loop&) = delete;
    loop(loop&& p_loop) noexcept
      : m_work(std::move(p_loop.m_work))
      , m_timeout(std::move(p_loop.m_timeout))
      , m_break(std::move(p_loop.m_break))
      , m_provide(std::move(p_loop.m_provide))
      , m_stopped(true)
    {}

    loop& operator=(const loop&) = delete;
    loop& operator=(loop&&) noexcept = delete;

    inline bool is_stopped() const { return m_stopped; }
    inline work_t get_work() const { return m_work; }
    inline break_t get_break() const { return m_break; }
    inline provide_t get_provide() const { return m_provide; }
    inline std::chrono::milliseconds get_timeout() const { return m_timeout; }

    inline void stop() { m_stopped = true; }

    void start()
    {
        if (m_stopped == false) {
            cerr_debug(this, " not starting beacause it was not stopped");
            return;
        }

        m_stopped = false;

        while (true) {

            cerr_debug(this, " one more loop");

            if (m_stopped) {
                cerr_debug(this, " stopping loop");
                break;
            }

            cerr_debug(this, " calling work");
            std::future<bool> _future =
              std::async(std::launch::async, std::ref(m_work));
            if (_future.wait_for(m_timeout) == std::future_status::ready) {
                if (!_future.get()) {
                    cerr_debug(this, " breaking because there is no more work");
                    m_stopped = true;
                    break;
                }
            } else {
                cerr_warn(this, " timeout");
            }

            if (m_stopped) {
                cerr_debug(this, " stopping loop");
                break;
            }

            if (m_break()) {
                cerr_debug(this, " breaker said to stop");
                m_stopped = true;
                break;
            }
        }
    }

  private:
    work_t m_work;
    std::chrono::milliseconds m_timeout;
    break_t m_break;
    provide_t m_provide;
    bool m_stopped;
};

} // namespace bus
} // namespace concurrent
} // namespace tenacitas

#endif // LOOP_H
