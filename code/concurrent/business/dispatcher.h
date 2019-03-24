#ifndef TENACITAS_CONCURRENT_BUS_DISPATCHER_H
#define TENACITAS_CONCURRENT_BUS_DISPATCHER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>
#include <mutex>
#include <thread>
#include <tuple>

#include <concurrent/business/thread_pool.h>
#include <logger/business/cerr.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {
/// \brief namespace of the class group
namespace business {

///
/// \brief dispatcher implements a publisher/subscriber pattern
///
/// \param t_data is the type of the data to be handled. If it is not \p void,
/// it must be:
///    - default constructible
///    - move constructible
///
template<typename t_msg>
class dispatcher
{
  public:
    ///
    /// \brief work_t is the type of work function, i.e., the function that will
    /// be called in a loop in order to execute some work
    ///
    typedef std::function<bool(t_msg&&)> work_t;

    /// \brief dispatcher default constructor not allowed
    dispatcher() = delete;

    /// \brief dispatcher copy constructor not allowed
    dispatcher(const dispatcher&) = delete;

    /// \brief dispatcher move constructor not allowed
    dispatcher(dispatcher&&) noexcept = delete;

    /// \brief dispatcher copy assigment not allowed
    dispatcher& operator=(const dispatcher&) = delete;

    /// \brief dispatcher move assigment not allowed
    dispatcher& operator=(dispatcher&&) noexcept = delete;

    /// \brief dispatcher destructor
    ///
    /// Stops all the \p thread_pool
    ~dispatcher()
    {
        cerr_debug("leaving");
        for (thread_pool_t& _pool : m_thread_pool_list) {
            _pool.stop();
        }
    }

    ///
    /// \brief adds a group of \p work_t objects to subscribe to handle \p
    /// t_msg objects
    /// \param p_work_factory is a function that creates \p work_t objects
    /// \param p_num_handlers is the number of \p work_t objects that we want to
    /// "fight" among each other to handle a \p t_msg object
    /// \param p_work_timeout is the maximum amount of time for a \p work_t
    /// function can take to handle a \p t_msg instance
    static void subscribe(std::function<work_t()> p_work_factory,
                          uint16_t p_num_handlers,
                          std::chrono::milliseconds p_work_timeout)
    {
        // initiating the cerr_log
        tenacitas::logger::business::configure_cerr_log();

        // creating a \p thread_pool
        thread_pool_t _thread_pool;

        // adding the \p work_t functions
        _thread_pool.add_work(p_num_handlers, p_work_factory, p_work_timeout);

        // running the \p thread_pool
        _thread_pool.run();

        // adding the \p thread_pool to the list
        m_thread_pool_list.push_back(std::move(_thread_pool));
    }

    ///
    /// \brief subscribe adds a single \p work_t function to handle \p t_msg
    /// objects
    /// \param p_work the \p work_t function that will handle a \p t_msg object
    /// \param p_work_timeout is the maximum amount of time for a \p work_t
    /// function can take to handle a \p t_msg instance
    ///
    static void subscribe(work_t&& p_work,
                          std::chrono::milliseconds p_work_timeout)
    {
        // initiating the cerr_log
        tenacitas::logger::business::configure_cerr_log();

        // creating a \p thread_pool
        thread_pool_t _thread_pool;

        // adding the \p work_t functions
        _thread_pool.add_work(std::move(p_work), p_work_timeout);

        // running the \p thread_pool
        _thread_pool.run();

        // adding the \p thread_pool to the list
        m_thread_pool_list.push_back(std::move(_thread_pool));
    }

    ///
    /// \brief handle sends a message to the \p work_t objects to be handled
    /// \param p_msg an instance of \p t_msg
    ///
    static void publish(const t_msg& p_msg)
    {
        if (m_thread_pool_list.empty()) {
            cerr_warn("there are no subscribers for ", p_msg);
        }
        for (thread_pool_t& _pool : m_thread_pool_list) {
            _pool.handle(p_msg);
        }
    }

  private:
    ///
    /// \brief thread_pool_t alias for \p thread_pool of \p t_msg
    ///
    typedef thread_pool<t_msg> thread_pool_t;

    ///
    /// \brief thread_pool_list_t alias for list of \p thread_pool_t
    ///
    typedef std::list<thread_pool_t> thread_pool_list_t;

  private:
    ///
    /// \brief m_thread_pool_list the single list of pools object
    ///
    static thread_pool_list_t m_thread_pool_list;
};

///
/// \brief definition of the single list of pools object
///
template<typename t_msg>
typename dispatcher<t_msg>::thread_pool_list_t
  dispatcher<t_msg>::m_thread_pool_list;

} // namespace business
} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_DISPATCHER_H
