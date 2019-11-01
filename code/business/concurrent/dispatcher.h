#ifndef TENACITAS_BUSINESS_CONCURRENT_DISPATCHER_H
#define TENACITAS_BUSINESS_CONCURRENT_DISPATCHER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <mutex>
#include <thread>
#include <tuple>

#include <business/concurrent/internal/log.h>
#include <business/concurrent/thread_pool.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace business {
/// \brief namespace of the class group
namespace concurrent {

///
/// \brief dispatcher implements a publisher/subscriber pattern
///
/// \param t_data is the type of the data to be handled. If it is not \p void,
/// it must be:
///    - default constructible
///    - move constructible
///
/// \tparam t_log provides log funcionality:
/// static void debug(const std::string & p_file, int p_line, const
/// t_params&... p_params)
/// static void info(const std::string & p_file, int p_line, const t_params&...
/// p_params)
/// static void warn(const std::string & p_file, int p_line, const t_params&...
/// p_params)
/// static void error(const std::string & p_file, int p_line, const
/// t_params&... p_params)
/// static void fatal(const std::string & p_file, int p_line, const
/// t_params&... p_params)
template<typename t_msg, typename t_log>
class dispatcher_t
{
public:
  /// \brief work_t is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  /// \param t_data is an instance of the data to be handled
  ///
  /// \return concurrent::result::stop if the loop where this function is being
  /// called should stop, or concurrent::result::dont_stop if it should continue
  typedef typename traits_t<t_msg>::worker worker;

  ///
  /// \brief queue name of a queue of messages
  ///
  typedef std::string queue;

  ///
  /// \brief log alias for @p t_log
  ///
  typedef t_log log;

  /// \brief dispatcher default constructor not allowed
  dispatcher_t() = delete;

  /// \brief dispatcher copy constructor not allowed
  dispatcher_t(const dispatcher_t&) = delete;

  /// \brief dispatcher move constructor not allowed
  dispatcher_t(dispatcher_t&&) = delete;

  /// \brief dispatcher copy assigment not allowed
  dispatcher_t& operator=(const dispatcher_t&) = delete;

  /// \brief dispatcher move assigment not allowed
  dispatcher_t& operator=(dispatcher_t&&) = delete;

  /// \brief dispatcher destructor
  ///
  /// Stops all the \p thread_pool
  ~dispatcher_t()
  {
    concurrent_log_debug(log, "leaving");
    for (thread_pool& _pool : m_thread_pool_list) {
      _pool.stop();
    }
  }

  ///
  /// \brief adds a group of \p work_t objects to handle \p t_msg objects
  /// \param p_work_factory is a function that creates \p work_t objects
  /// \param p_num_handlers is the number of \p work_t objects that we want to
  /// "fight" among each other to handle a \p t_msg object
  /// \param p_work_timeout is the maximum amount of time for a \p work_t
  /// function can take to handle a \p t_msg instance
  static void subscribe(const queue& p_queue,
                        std::function<worker()> p_work_factory,
                        uint16_t p_num_handlers,
                        std::chrono::milliseconds p_work_timeout)
  {

    // creating a \p thread_pool
    thread_pool _thread_pool;

    // adding the \p work_t functions
    _thread_pool.add_work(p_num_handlers, p_work_factory, p_work_timeout);

    // running the \p thread_pool
    _thread_pool.run();

    // adding the \p thread_pool to the list
    //    m_thread_pool_list.push_back(std::move(_thread_pool));
    m_thread_pool_list.insert({ p_queue, std::move(_thread_pool) });
  }

  ///
  /// \brief subscribe adds a single \p work_t function to handle \p t_msg
  /// objects
  /// \param p_work the \p work_t function that will handle a \p t_msg object
  /// \param p_work_timeout is the maximum amount of time for a \p work_t
  /// function can take to handle a \p t_msg instance
  ///
  static void subscribe(const queue& p_queue,
                        worker&& p_work,
                        std::chrono::milliseconds p_work_timeout)
  {
    // creating a \p thread_pool
    thread_pool _thread_pool;

    // adding the \p work_t functions
    _thread_pool.add_work(std::move(p_work), p_work_timeout);

    // running the \p thread_pool
    _thread_pool.run();

    // adding the \p thread_pool to the list
    m_thread_pool_list.insert({ p_queue, std::move(_thread_pool) });
    //    m_thread_pool_list.push_back(std::move(_thread_pool));
  }

  ///
  /// \brief handle sends a message to the \p work_t objects to be handled
  /// \param p_msg an instance of \p t_msg
  ///
  static void publish(const t_msg& p_msg)
  {
    if (m_thread_pool_list.empty()) {
      concurrent_log_warn(log, "there are no subscribers for ", p_msg);
    }
    concurrent_log_debug(log, "# of pools: ", m_thread_pool_list.size());
    //    for (thread_pool& _pool : m_thread_pool_list) {
    //      _pool.handle(p_msg);
    //    }
    typename thread_pool_list::iterator _end = m_thread_pool_list.end();
    for (typename thread_pool_list::iterator _ite = m_thread_pool_list.begin();
         _ite != _end;
         ++_ite) {
      _ite->second.handle(p_msg);
    }
  }

private:
  ///
  /// \brief thread_pool_t alias for \p thread_pool of \p t_msg
  ///
  typedef thread_pool_t<t_msg, t_log> thread_pool;

  ///
  /// \brief thread_pool_list_t alias for list of \p thread_pool_t
  ///
  typedef std::map<queue, thread_pool> thread_pool_list;

private:
  ///
  /// \brief m_thread_pool_list the single list of pools object
  ///
  static thread_pool_list m_thread_pool_list;
};

///
/// \brief definition of the single list of pools object
///
template<typename t_msg, typename t_log>
typename dispatcher_t<t_msg, t_log>::thread_pool_list
  dispatcher_t<t_msg, t_log>::m_thread_pool_list;

} // namespace business
} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_DISPATCHER_H
