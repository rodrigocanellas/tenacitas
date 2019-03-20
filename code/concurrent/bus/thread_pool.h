#ifndef TENACITAS_CONCURRENT_BUS_THREAD_POOL_H
#define TENACITAS_CONCURRENT_BUS_THREAD_POOL_H

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \date nov/2018

#include <cassert>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include <concurrent/bus/traits.h>
#include <concurrent/bus/internal//async_loop.h>
#include <logger/bus/cerr.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {
namespace bus {

///
/// \brief thread_pool implements a thread pool, which allows to use the
/// producer/consumer pattern
///
/// \tparam t_data is the type of data inserted into the pool, so that
/// registered functions can "fight" among each other to get the data to process
/// it. \p t_data must be default constructable, moveable and copiable
///
/// \example
/// \code
//#include <cstdint>
//#include <functional>
//#include <iomanip>
//#include <iostream>
//#include <sstream>
//#include <thread>
//#include <tuple>

//#include <concurrent/bus/thread.h>
//#include <concurrent/bus/thread_pool.h>
//#include <logger/bus/cerr.h>
//#include <string/bus/max_str_length.h>

// typedef tenacitas::concurrent::bus::thread_pool<int16_t> thread_pool_t;
// typedef tenacitas::concurrent::bus::thread thread_t;

// struct work
//{
//    bool operator()(int16_t&& p_i)
//    {
//        std::stringstream _stream;
//        _stream <<
//        std::setw(tenacitas::string::bus::max_str_length<int16_t>())
//                << std::right << std::setfill('0') << p_i;
//        cerr_debug(_stream.str());
//        return true;
//    }
//};

// int
// main(int argc, char **argv)
//{
//    tenacitas::logger::bus::configure_cerr_log();
//    cerr_set_debug();

//    thread_pool_t _pool(3, [] { return work(); });

//    _pool();

//    thread_t _thread = thread_t([&_pool]() {
//        for (int16_t _i = 0; _i < 40; ++_i) {
//            _pool.handle(_i);
//        }
//        std::this_thread::sleep_for(std::chrono::seconds(2));
//    });
//}
/// \endcode
///
template<typename t_data>
class thread_pool
{
  public:
    ///
    /// \brief work_t is the type of function that will handle an instance of \p
    /// t_data
    ///
    typedef typename loop_traits<t_data>::work_t work_t;

    ///
    /// \brief thread_pool constructor
    /// \param p_num_works number of \p work_t objects registered in the pool
    /// \param p_work_factory a specialized function that creates \p work_t
    /// objects
    ///
    thread_pool()
      : m_loops()
      , m_values()
      , m_stopped(true)
      , m_destroying(false)
    {}

    thread_pool(thread_pool&& p_pool) noexcept
    {
        bool _stopped = p_pool.is_stopped();

        p_pool.stop();

        m_values = std::move(p_pool.m_values);

        for (async_loop_t& _loop_right : p_pool.m_loops) {
            async_loop_t _loop(
              std::move(_loop_right.get_work()),
              _loop_right.get_timeout(),
              [this]() -> bool { return this->stop_condition(); },
              [this]() -> std::pair<bool, t_data> { return this->data(); });

            add_work(std::move(_loop));
        }

        m_destroying = false;

        m_stopped = true;

        if (!_stopped) {
            run();
        }
    }

    thread_pool(const thread_pool&) = delete;

    ///
    /// \brief if 'stop ' was not called, empties the pool, waiting for all the
    /// data to be processed
    ///
    ~thread_pool()
    {
        m_destroying = true;
        cerr_debug(this, " m_values.size() = ", m_values.size());

        if (!m_stopped) {
            while (!m_values.empty()) {
                cerr_debug(this, " m_values.size() = ", m_values.size());
                m_cv_data.notify_all();
                cerr_debug(this, " waiting for poping, ", this);
                std::unique_lock<std::mutex> _lock(m_mutex_poped);
                m_cv_poped.wait(_lock, [this] {
                    cerr_debug(this, " poping signal arrived, ", this);
                    return true;
                });
                cerr_debug(this, " lock free");
            }
            cerr_debug("this, m_values.size() = ", m_values.size());
            stop();
        }
    }

    thread_pool& operator=(thread_pool&&) noexcept = delete;
    thread_pool& operator=(const thread_pool&) = delete;

    bool is_stopped() const { return m_stopped; }

    void add_work(uint16_t p_num_works,
                  std::function<work_t()> p_work_factory,
                  std::chrono::milliseconds p_work_timeout)
    {

        cerr_debug(this, "p_num_works = ", p_num_works);
        for (uint16_t _i = 0; _i < p_num_works; ++_i) {
            async_loop_t _loop(
              p_work_factory(),
              p_work_timeout,
              [this]() -> bool { return this->stop_condition(); },
              [this]() -> std::pair<bool, t_data> { return this->data(); });

            add_work(std::move(_loop));
        }
        cerr_debug(this, "there are ", m_loops.size(), " loops");
    }

    void add_work(work_t&& p_work, std::chrono::milliseconds p_work_timeout)
    {

        async_loop_t _loop(
          std::move(p_work),
          p_work_timeout,
          [this]() -> bool { return this->stop_condition(); },
          [this]() -> std::pair<bool, t_data> { return this->data(); });

        add_work(std::move(_loop));
    }

    ///
    /// \brief handle sends an instance of \p t_data to be handled
    /// \param p_value the value to be handled
    ///
    inline void handle(const t_data& p_value) { add_data(p_value); }

    ///
    /// \brief operator () starts the thread_pool, i.e., allows the \p work_t
    /// functions to "fight" among each other, in order to process any instance
    /// of \p t_data that is inxserted, using the \p handle method, into the
    /// pool
    ///
    inline void run()
    {
        if (m_stopped) {
            run_common();
        }
    }

    ///
    /// \brief interrupt stops the \p work_t instances
    ///
    inline void stop()
    {

        cerr_debug("stopping ", this);

        std::unique_lock<std::mutex> _lock(m_mutex_stop);
        m_stopped = true;
        if (m_loops.empty()) {
            return;
        }

        m_cv_data.notify_all();
        for (async_loop_t& _loop : m_loops) {
            cerr_debug("stopping loop ", &_loop);
            _loop.stop();
        }
    }

  private:
    typedef typename loop_traits<t_data>::provide_t provide_t;

    typedef std::list<t_data> values_t;

    ///
    /// \brief async_loop_t an alias for async_loop, specialized with
    /// thred_pool_controller
    ///
    typedef bus::async_loop<t_data> async_loop_t;

    typedef std::vector<async_loop_t> async_loops_t;

  private:
    void run_common()
    {

        cerr_debug(this, " running ", this);
        std::unique_lock<std::mutex> _lock(m_mutex_stop);
        if (m_loops.empty()) {
            cerr_debug(this, " cant run because it is empty");
            return;
        }
        m_stopped = false;
        cerr_debug(this, " m_stopped = ", m_stopped);

        for (async_loop_t& _loop : m_loops) {
            cerr_debug(this, " starting loop ", &_loop);
            _loop.run();
        }
        cerr_debug(this, " started ");
    }

    void add_work(async_loop_t&& p_loop)
    {

        std::lock_guard<std::mutex> _lock(m_add_work);
        m_loops.push_back(std::move(p_loop));
    }

    bool stop_condition()
    {

        cerr_debug(this, " stopped = ", m_stopped);
        return m_stopped;
    }

    std::pair<bool, t_data> data()
    {

        using namespace std;

        if (m_stopped) {
            cerr_debug(this,
                       " not waiting for more data because it is stopped ");

            return std::make_pair(false, t_data());
        }

        unique_lock<std::mutex> _lock(m_mutex_data);
        m_cv_data.wait(_lock, [this] {
            cerr_debug(this, " waiting");
            return (!m_values.empty() || m_stopped);
        });

        if (m_stopped) {
            cerr_debug(this, " m_stopped, returning 'false'");
            return std::make_pair(false, t_data());
        }

        std::pair<bool, t_data> _return =
          std::make_pair(true, std::move(m_values.front()));
        cerr_debug(this, " data to be handled ", _return.second);
        m_values.pop_front();

        if (m_destroying && !m_values.empty()) {
            cerr_debug(this, " signaling poped");
            m_cv_poped.notify_all();
        }

        return _return;
    }

    void add_data(const t_data& p_value)
    {
        cerr_debug(this, " adding ", p_value);
        {
            std::lock_guard<std::mutex> _lock(m_mutex_data);
            m_values.push_back(p_value);
        }
        m_cv_data.notify_all();
        cerr_debug(this, " added and notified ", this);
    }

  private:
    ///
    /// \brief m_loops the pool of async_loops
    ///
    async_loops_t m_loops;
    values_t m_values;
    bool m_stopped = true;

    std::mutex m_mutex_data;
    std::condition_variable m_cv_data;
    std::mutex m_mutex_stop;

    std::mutex m_mutex_poped;
    std::condition_variable m_cv_poped;

    std::mutex m_add_work;
    bool m_destroying = false;
};

} // namespace bus
} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_THREAD_POOL_H
