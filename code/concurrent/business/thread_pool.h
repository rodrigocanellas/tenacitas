#ifndef TENACITAS_CONCURRENT_BUS_THREAD_POOL_H
#define TENACITAS_CONCURRENT_BUS_THREAD_POOL_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

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

#include <concurrent/business/internal/async_loop.h>
#include <concurrent/business/traits.h>
#include <logger/business/cerr.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {
/// \brief namespace of the class group
namespace business {

///
/// \brief thread_pool implements a thread pool, which allows to use the
/// producer/consumer pattern
///
/// \tparam t_data is the type of data inserted into the pool, so that
/// registered work functions can "fight" among each other to get the data to
/// process it. \p t_data must be default constructable, moveable and copiable
///
/// \param t_data is the type of the data to be handled. If it is not \p void,
/// it must be:
///    - default constructible
///    - move constructible
///
template<typename t_data>
class thread_pool
{
  public:
    ///
    /// \brief work_t is the type of work function, i.e., the function that will
    /// be called in a loop in order to execute some work
    ///
    typedef typename loop_traits<t_data>::work_t work_t;

    /// \brief thread_pool constructor
    thread_pool()
      : m_loops()
      , m_values()
      , m_stopped(true)
      , m_destroying(false)
    {
        // initiating the cerr_log
        tenacitas::logger::business::configure_cerr_log();
    }

    /// \brief move constructor
    thread_pool(thread_pool&& p_pool) noexcept
    {
        // save if the right side was stopped
        bool _stopped = p_pool.is_stopped();

        // stop the right side
        p_pool.stop();

        // move the \p t_data collection
        m_values = std::move(p_pool.m_values);

        // build the work function collection, moving the work function from the
        // right side, and reseting the provide and break function
        for (async_loop_t& _loop_right : p_pool.m_loops) {
            async_loop_t _loop(
              std::move(_loop_right.get_work()),
              _loop_right.get_timeout(),
              [this]() -> bool { return this->stop_condition(); },
              [this]() -> std::pair<bool, t_data> { return this->data(); });

            add_work(std::move(_loop));
        }

        // seting the flags
        m_destroying = false;

        m_stopped = true;

        // if the right side was not stopped
        if (!_stopped) {
            // run this thread pool
            run();
        }
    }

    /// \brief copy constructor not allowed
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

    /// \brief move assignment
    thread_pool& operator=(thread_pool&& p_pool) noexcept
    {
        if (this != &p_pool) {
            // save if the right side was stopped
            bool _stopped = p_pool.is_stopped();

            // stop the right side
            p_pool.stop();

            // move the \p t_data collection
            m_values = std::move(p_pool.m_values);

            // build the work function collection, moving the work function from
            // the right side, and reseting the provide and break function
            for (async_loop_t& _loop_right : p_pool.m_loops) {
                async_loop_t _loop(
                  std::move(_loop_right.get_work()),
                  _loop_right.get_timeout(),
                  [this]() -> bool { return this->stop_condition(); },
                  [this]() -> std::pair<bool, t_data> { return this->data(); });

                add_work(std::move(_loop));
            }

            // seting the flags
            m_destroying = false;

            m_stopped = true;

            // if the right side was not stopped
            if (!_stopped) {
                // run this thread pool
                run();
            }
        }
        return *this;
    }

    /// \brief copy assignment not allowed
    thread_pool& operator=(const thread_pool&) = delete;

    ///
    /// \brief is_stopped
    /// \return \p true if the loop is not running; \p false othewise
    ///
    bool is_stopped() const { return m_stopped; }

    ///
    /// \brief add_work adds a bunch of \p work_t functions
    /// \param p_num_works the number of \p work_t functions to be added
    /// \param p_work_factory a function that creates \p work_t functions
    /// \param p_work_timeout timeout for the \p work_t functions
    ///
    void add_work(uint16_t p_num_works,
                  std::function<work_t()> p_work_factory,
                  std::chrono::milliseconds p_work_timeout)
    {
        for (uint16_t _i = 0; _i < p_num_works; ++_i) {
            async_loop_t _loop(
              p_work_factory(),
              p_work_timeout,
              [this]() -> bool { return this->stop_condition(); },
              [this]() -> std::pair<bool, t_data> { return this->data(); });

            add_work(std::move(_loop));
        }
    }

    ///
    /// \brief add_work adds one \p work_t function
    /// \param p_work the \p work_t fuction to be added
    /// \param p_work_timeout timeout of this \p work_t function
    ///
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
    /// \brief operator ()
    ///

    ///
    /// \brief run starts the thread_pool
    ///
    /// From this call on, the \p work_t functions will to "fight" among each
    /// other, in order to process any instance of \p t_data that was inserted,
    /// using the \p handle method, into the pool
    ///
    inline void run()
    {
        if (m_stopped) {
            run_common();
        }
    }

    ///
    /// \brief interrupt stops the \p thread_pool
    ///
    /// From this call on, the \p work_t functions will stop "fighting" among
    /// each other, in order to process any instance of \p t_data that was
    /// inserted, using the \p handle method, into the pool
    ///
    inline void stop()
    {

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
    /// \brief provide_t is a simpler name for the provide function, which
    /// removes the \p t_data from \p m_values, and passes to a \p work_t
    /// function
    typedef typename loop_traits<t_data>::provide_t provide_t;

    /// \brief the collection of values
    typedef std::list<t_data> values_t;

    /// \brief async_loop_t is an simples for the \p async_loop where a \p
    /// work_t function will be running
    typedef business::async_loop<t_data> async_loop_t;

    /// \brief async_loops_t is the collection of \p async_loop
    typedef std::vector<async_loop_t> async_loops_t;

  private:
    /// \brief run_common is called from other functions, in order to start the
    /// \p thread_pool
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

    ///
    /// \brief add_work common function called to add a \p work_t function
    /// \param p_loop the new \p work_t function to be added
    ///
    void add_work(async_loop_t&& p_loop)
    {
        std::lock_guard<std::mutex> _lock(m_add_work);
        m_loops.push_back(std::move(p_loop));
    }

    ///
    /// \brief stop_condition
    /// \return \p true if the flag indicating that the \p thread_pool should
    /// stop is \p true; \p false otherwise
    ///
    bool stop_condition()
    {

        cerr_debug(this, " stopped = ", m_stopped);
        return m_stopped;
    }

    ///
    /// \brief data is the \p provide_t function, which returns data, if
    /// available, to a \p work_t function \return
    ///
    /// \return (true, a filled \p t_data object), if there is any instance of
    /// \p data available; of (false, t_data()) otherwise
    ///
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

    ///
    /// \brief add_data adds a instance of \p t_data to the pool
    /// \param p_value is an instance of \p t_data to be added
    ///
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
    /// \brief m_loops the pool of async_loops
    async_loops_t m_loops;

    /// \brief m_values is the collection of instances of \p t_data
    values_t m_values;

    /// \brief m_stopped indicates if the \p thread_pool should stop
    bool m_stopped = true;

    /// \brief m_mutex_data controls access to \p m_values
    std::mutex m_mutex_data;
    /// \brief m_cv_data signalizes if there is \p t_data available
    std::condition_variable m_cv_data;

    /// \brief m_mutex_stop controls access to attributes while the \p
    /// thread_pool is stopping
    std::mutex m_mutex_stop;

    /// \brief m_mutex_poped controls acesss to \p m_values while the \p t_data
    /// instances are being consumed during the destruction of the \p
    /// thread_pool
    std::mutex m_mutex_poped;

    /// \brief m_cv_poped signalizes if there is \p t_data available the
    /// destruction of the \p thread_pool
    std::condition_variable m_cv_poped;

    /// \brief m_add_work controls access to the \p m_loops while inserting a
    /// new \p t_work function
    std::mutex m_add_work;

    /// \brief m_destroying indicates that the \p thread_pool should stop
    bool m_destroying = false;
};

} // namespace business
} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_THREAD_POOL_H
