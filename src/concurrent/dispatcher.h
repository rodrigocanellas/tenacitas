#ifndef TENACITAS_CONCURRENT_DISPATCHER_H
#define TENACITAS_CONCURRENT_DISPATCHER_H

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

#include <calendar/epoch.h>
#include <concurrent/circular_unlimited_size_queue.h>
#include <concurrent/internal/log.h>
#include <concurrent/producer_consumer.h>
#include <concurrent/queue.h>
#include <concurrent/timeout_callback.h>
#include <concurrent/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief dispatcher implements a publisher/subscriber pattern
///
/// \param t_data is the type of the data to be handled. If it is not \p void,
/// it must be:
///    - default constructible
///    - move constructible
///
/// \tparam t_log provides log funcionality:
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
///
template <typename t_log, typename t_data> class dispatcher_t {
public:
  typedef t_log log;
  typedef t_data data;

  /// \brief worker type
  typedef std::function<void(data &&)> worker;

  /// \brief name for a group of \p worker functions
  typedef std::string consumers_group;

  /// \brief dispatcher default constructor not allowed
  dispatcher_t() = delete;

  /// \brief dispatcher copy constructor not allowed
  dispatcher_t(const dispatcher_t &) = delete;

  /// \brief dispatcher move constructor not allowed
  dispatcher_t(dispatcher_t &&) = delete;

  /// \brief dispatcher copy assigment not allowed
  dispatcher_t &operator=(const dispatcher_t &) = delete;

  /// \brief dispatcher move assigment not allowed
  dispatcher_t &operator=(dispatcher_t &&) = delete;

  /// \brief dispatcher destructor
  ///
  /// Stops all the \p producer_consumer
  ~dispatcher_t() {
    concurrent_debug(m_log, "leaving");

    for (producer_consumer_ptr _producer_consumer : m_producer_consumer_list) {
      _producer_consumer->stop();
    }
  }

  //  /// \brief Creates a queue for \p data messages, and defines a single \p
  //  /// worker to handle them
  //  static void create(t_data_queue &&p_queue) {

  //    const consumers_group _workers{
  //        std::to_string(calendar::epoch::microsecs())};

  //    create(_workers, std::move(p_queue));
  //  }

  //  /// \brief Creates a queue for \p data messages, and associates a name of
  //  /// a group of \p worker objects
  //  static void create(const consumers_group &p_workers, t_data_queue
  //  &&p_queue) {
  //    producer_consumer_ptr _producer_consumer(
  //        std::make_shared<producer_consumer>(std::move(p_queue)));
  //    m_producer_consumer_list.insert({p_workers, _producer_consumer});
  //  }

  /// \brief Adds \p worker objects to a already created group of \p worker
  /// objects to handle \p data messages

  //  template <typename t_time>
  //  static void subscribe(const consumers_group &p_workers,
  //                        std::function<worker()> p_work_factory,
  //                        uint16_t p_num_handlers, t_time p_work_timeout) {

  //    typename producer_consumer_list::iterator _ite =
  //        m_producer_consumer_list.find(p_workers);

  //    producer_consumer_ptr _producer_consumer;
  //    if (_ite == m_producer_consumer_list.end()) {
  //      concurrent_fatal(m_log, "no " + p_workers + " defined");
  //      throw std::runtime_error("no " + p_workers + " defined");
  //    }

  //    _producer_consumer = _ite->second;

  //    // adding the \p work_t functions
  //    _producer_consumer->add(p_num_handlers, p_work_factory, p_work_timeout,
  //                            m_timeout_callback);

  //    // running the \p producer_consumer
  //    _producer_consumer->start();

  //    // adding the \p producer_consumer to the list
  //    m_producer_consumer_list.insert({p_workers, _producer_consumer});
  //  }

  //  /// \brief Adds a \p worker to a already created group of \p worker object
  //  to
  //  /// handle messages \p data
  //  template <typename t_time>
  //  static void subscribe(const consumers_group &p_workers, worker p_work,
  //                        t_time p_work_timeout) {

  //    typename producer_consumer_list::iterator _ite =
  //        m_producer_consumer_list.find(p_workers);

  //    producer_consumer_ptr _producer_consumer;
  //    if (_ite == m_producer_consumer_list.end()) {
  //      concurrent_fatal(m_log, "no " + p_workers + " defined");
  //      throw std::runtime_error("no " + p_workers + " defined");
  //    }

  //    _producer_consumer = _ite->second;

  //    // adding the \p work_t functions
  //    _producer_consumer->add(p_work, p_work_timeout, m_timeout_callback);

  //    // running the \p producer_consumer
  //    _producer_consumer->start();

  //    // adding the \p producer_consumer to the list
  //    m_producer_consumer_list.insert({p_workers, _producer_consumer});
  //  }

  static void subscribe(worker p_worker, size_t p_queue_size = 20) {

    typedef circular_unlimited_size_queue_t<log, data> queue;

    const consumers_group _workers{
        std::to_string(calendar::epoch::microsecs())};

    producer_consumer_ptr _producer =
        std::make_shared<producer_consumer>(queue::create(p_queue_size));

    _producer->add(p_worker);

    _producer->start();

    m_producer_consumer_list.insert({_workers, _producer});
  }

  //  static void subscribe(worker p_worker, size_t p_queue_size = 20)
  //  {

  //    typedef circular_unlimited_size_queue_t<log, data> queue;

  //    const consumers_group _workers{ std::to_string(
  //      calendar::epoch::microsecs()) };

  //    producer_consumer_ptr _producer =
  //      std::make_shared<producer_consumer>(queue::create(p_queue_size));

  //    _producer->add(p_worker, [](std::thread::id) -> void {});

  //    m_producer_consumer_list.insert(_workers, _producer);
  //  }

  /// \brief handle sends a message to the \p work_t objects to be handled
  ///
  /// \param p_msg an instance of \p t_data
  static void publish(const data &p_msg) {
    typename producer_consumer_list::iterator _end =
        m_producer_consumer_list.end();
    for (typename producer_consumer_list::iterator _ite =
             m_producer_consumer_list.begin();
         _ite != _end; ++_ite) {
      _ite->second->add(p_msg);
    }
  }

  /// \brief handle sends a message to the \p work_t objects to be handled
  ///
  /// \tparam t_params are the types of parameters to create a \p t_data object
  ///
  /// \param p_params are theparameters values to create a \p t_data object
  template <typename... t_params>
  inline static void publish(t_params... p_params) {
    const data _msg(p_params...);
    publish(_msg);
  }

  /// \brief stops the \p subscriber, which stops \p worker objects to handle
  /// new messages
  static void stop() {
    typename producer_consumer_list::iterator _end =
        m_producer_consumer_list.end();
    for (typename producer_consumer_list::iterator _ite =
             m_producer_consumer_list.begin();
         _ite != _end; ++_ite) {
      _ite->second->stop();
    }
  }

  static void set_timeout_callback(timeout_callback p_timeout_callback) {
    m_timeout_callback = p_timeout_callback;
  }

private:
  /// \brief producer_consumer_t alias for \p producer_consumer of \p t_data
  typedef producer_consumer_t<t_log, t_data> producer_consumer;

  typedef typename std::shared_ptr<producer_consumer> producer_consumer_ptr;

  /// \brief producer_consumer_list_t alias for list of \p producer_consumer_t
  typedef std::map<consumers_group, producer_consumer_ptr>
      producer_consumer_list;

private:
  /// \brief m_producer_consumer_list the single list of pools object
  static producer_consumer_list m_producer_consumer_list;

  static timeout_callback m_timeout_callback;
  static t_log m_log;
};

/// \brief definition of the single list of pools object
template <typename t_log, typename t_queue>
typename dispatcher_t<t_log, t_queue>::producer_consumer_list
    dispatcher_t<t_log, t_queue>::m_producer_consumer_list;

template <typename t_log, typename t_queue>
timeout_callback dispatcher_t<t_log, t_queue>::m_timeout_callback =
    [](std::thread::id) -> void {};

template <typename t_log, typename t_queue>
t_log dispatcher_t<t_log, t_queue>::m_log{"consumer::dispatcher"};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_DISPATCHER_H
