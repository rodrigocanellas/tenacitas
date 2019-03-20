#ifndef TENACITAS_CONCURRENT_BUS_DISPATCHER_H
#define TENACITAS_CONCURRENT_BUS_DISPATCHER_H

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \date dez/2018

#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>
#include <mutex>
#include <thread>
#include <tuple>

#include <concurrent/bus/thread_pool.h>
#include <logger/bus/cerr.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {
namespace bus {

///
/// \brief dispatcher implements a publisher/subscriber pattern
///
/// \tparam t_msg is the type of message to be dispatched to the subscribers.
/// \p t_data must be default constructable, moveable and copiable
///
template<typename t_msg>
class dispatcher
{
  public:
    ///
    /// \brief work_t type of function that must handle a \p t_msg type
    ///
    typedef std::function<bool(t_msg&&)> work_t;

    dispatcher() = delete;
    dispatcher(const dispatcher&) = delete;
    dispatcher(dispatcher&&) noexcept = delete;
    dispatcher& operator=(const dispatcher&) = delete;
    dispatcher& operator=(dispatcher&&) noexcept = delete;

    ~dispatcher()
    {
        cerr_debug("leaving");
        for (thread_pool_t& _pool : m_thread_pool_list) {
            _pool.stop();
        }
    }

    ///
    /// \brief subscribe allows \p work_t objects to subscribe
    /// \param p_work_factory is a function that creates \p work_t objects
    /// \param p_num_handlers is the number of \p work_t objects that we want to
    /// "fight" among each other to handle a \p t_msg object
    ///
    static void subscribe(std::function<work_t()> p_work_factory,
                          uint16_t p_num_handlers,
                          std::chrono::milliseconds p_work_timeout)
    {
        tenacitas::logger::bus::configure_cerr_log();

        thread_pool_t _thread_pool;
        _thread_pool.add_work(p_num_handlers, p_work_factory, p_work_timeout);
        _thread_pool.run();
        m_thread_pool_list.push_back(std::move(_thread_pool));
    }

    static void subscribe(work_t&& p_work,
                          std::chrono::milliseconds p_work_timeout)
    {
        tenacitas::logger::bus::configure_cerr_log();

        thread_pool_t _thread_pool;
        _thread_pool.add_work(std::move(p_work), p_work_timeout);
        _thread_pool.run();
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

} // namespace bus
} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_DISPATCHER_H
