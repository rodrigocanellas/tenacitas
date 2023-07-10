#ifndef TENACITAS_LIB_ASYNC_INTERNAL_TYP_QUEUE_H
#define TENACITAS_LIB_ASYNC_INTERNAL_TYP_QUEUE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <condition_variable>
#include <memory>
#include <mutex>
#include <typeindex>

#include <tenacitas.lib/src/async/cpt/event.h>
#include <tenacitas.lib/src/async/typ/priority.h>
#include <tenacitas.lib/src/async/typ/queue_id.h>
#include <tenacitas.lib/src/async/typ/subscriber.h>
#include <tenacitas.lib/src/container/typ/circular_queue.h>
#include <tenacitas.lib/src/log/alg/logger.h>

namespace tenacitas::lib::async::internal::typ {

// template <cpt::event t_event> class queue_t;

// A publishing for an event
template <cpt::event t_event> class event_queue {

public:
  event_queue() = delete;

  event_queue(async::typ::priority p_priority = async::typ::priority ::medium);

  event_queue(const event_queue &) = delete;

  event_queue(event_queue &&p_queue) {
    m_id = std::move(p_queue.m_id);
    m_priority = std::move(p_queue.m_priority);
    m_loops = std::move(p_queue.m_loops);
    m_stopped = p_queue.m_stopped.load();
    m_queued_data = std::move(p_queue.m_queued_data);
    m_circular_queue = std::move(p_queue.m_circular_queue);
  }

  event_queue &operator=(const event_queue &) = delete;
  event_queue &operator=(event_queue &&) = delete;

  // Adds an event to the queue of events
  bool add_event(const t_event &p_event);

  // Adds a subscriber that will compete with the other existing subscribers for
  // an event in the queue
  void add_subscriber(async::typ::subscriber_t<t_event> p_subscriber);

  // Adds a bunch of subscribers
  void
  add_subscriber(std::unsigned_integral auto p_num_subscribers,
                 std::function<async::typ::subscriber_t<t_event>()> p_factory);

  ~event_queue();

  // \brief Stops this publishing
  void stop() {
    if (m_stopped) {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("publishing ", m_id,
                   " - not stopping because it is stopped");
#endif
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_cond.notify_all();
      return;
    }
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("publishing ", m_id, " -  notifying loops to stop");
#endif
    {
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_stopped = true;
      m_cond.notify_all();
    }

    for (std::thread &_thread : m_loops) {
      if (_thread.joinable()) {
#ifdef TENACITAS_LOG
        TNCT_LOG_TRA("publishing ", m_id, ", thread ", _thread.get_id(),
                     " - is joinable");
#endif
        _thread.join();

#ifdef TENACITAS_LOG
        TNCT_LOG_TRA("publishing ", m_id, ", thread ", _thread.get_id(),
                     " - joined");
#endif
      }
#ifdef TENACITAS_LOG
      else {

        TNCT_LOG_TRA("publishing ", m_id, ", thread ", _thread.get_id(),
                     " - is not joinable");
      }
#endif
    }
  }

  // Amount of events added
  size_t amount_added() const;

  // Informs if the publishing is stopped
  bool is_stopped() const;

  async::typ::queue_id get_id() const;

  constexpr async::typ::priority get_priority() const;

  void set_priority(async::typ::priority p_priority);

  // \return Returns the size of the queue of \p t_event
  size_t get_size() const;

  // \return Returns the amount of \p t_event objects in the queue
  size_t get_occupied() const;

  // publishing is ordered by tenacitas::lib::async::priority
  constexpr bool operator<(const event_queue &p_publishing) const;

  // publishing is ordered by tenacitas::lib::async::priority
  constexpr bool operator>(const event_queue &p_publishing) const;

  // publishing is compared by tenacitas::lib::async::async::typ::publishing_id
  constexpr bool operator!=(const event_queue &p_publishing) const;

  // publishing is compared by tenacitas::lib::async::async::typ::publishing_id
  constexpr bool operator==(const event_queue &p_publishing) const;

  void report(std::ostringstream &p_out) const;

protected:
  // Group of loops that asynchronously call the subscribers
  using loops = std::vector<std::thread>;

  // Queue used to store the events to be handled
  using circular_queue = container::typ::circular_queue_t<t_event>;

protected:
  void subscriber_loop(async::typ::subscriber_t<t_event> p_subscriber);

  void empty_queue(const std::thread::id &p_loop_id,
                   async::typ::subscriber_t<t_event> p_subscriber);

protected:
  // Identifier of this publishing
  size_t m_id;

  // Priority of this publishing
  async::typ::priority m_priority;

  // Asynchronous loops, where events are retrieved from queue, and
  // subscribers are called
  loops m_loops;

  // Indicates if the dispatcher should continue to run
  std::atomic_bool m_stopped{false};

  // Amount of queued data
  size_t m_queued_data{0};

  // Controls access to the \p m_loops while inserting a new subscriber
  std::mutex m_add_subscriber;

  // Controls access to inserting data
  std::mutex m_mutex;

  // Controls access to the data produced
  std::condition_variable m_cond;

  circular_queue m_circular_queue;
};

} // namespace tenacitas::lib::async::internal::typ

namespace tenacitas::lib::async::internal::typ {

template <cpt::event t_event> inline event_queue<t_event>::~event_queue() {
#ifdef TENACITAS_LOG
  TNCT_LOG_TRA("publishing ", m_id, " - destructor");
#endif
  stop();
}

template <cpt::event t_event>
inline bool event_queue<t_event>::is_stopped() const {
  return m_stopped;
}

template <cpt::event t_event>
inline async::typ::queue_id event_queue<t_event>::get_id() const {
  return m_id;
}

template <cpt::event t_event>
inline constexpr async::typ::priority
event_queue<t_event>::get_priority() const {
  return m_priority;
}

template <cpt::event t_event>
inline void
event_queue<t_event>::set_priority(async::typ::priority p_priority) {
  m_priority = p_priority;
}

template <cpt::event t_event>
inline constexpr bool
event_queue<t_event>::operator<(const event_queue &p_publishing) const {
  return m_priority < p_publishing.m_priority;
}

template <cpt::event t_event>
inline constexpr bool
event_queue<t_event>::operator>(const event_queue &p_publishing) const {
  return m_priority > p_publishing.m_priority;
}

template <cpt::event t_event>
inline constexpr bool
event_queue<t_event>::operator!=(const event_queue &p_publishing) const {
  return m_id != p_publishing.m_id;
}

template <cpt::event t_event>
inline constexpr bool
event_queue<t_event>::operator==(const event_queue &p_publishing) const {
  return m_id == p_publishing.m_id;
}

template <cpt::event t_event>
inline event_queue<t_event>::event_queue(async::typ::priority p_priority)
    : m_id(reinterpret_cast<size_t>(&(*this))), m_priority(p_priority) {}

template <cpt::event t_event>
bool event_queue<t_event>::add_event(const t_event &p_event) {
  try {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("event '", typeid(t_event).name(), "', publishing ", m_id,
                 ", queue ", m_circular_queue.get_id(), " - adding event ");
#endif

    m_circular_queue.add(p_event);

    std::lock_guard<std::mutex> _lock(m_mutex);
    m_cond.notify_all();

    ++m_queued_data;

    return true;

  } catch (std::exception &_ex) {
#ifdef TENACITAS_LOG
    TNCT_LOG_ERR("error adding event'", typeid(t_event).name(), "': '",
                 _ex.what(), '\'');
#endif
  }
  return false;
}

template <cpt::event t_event>

void event_queue<t_event>::add_subscriber(
    std::unsigned_integral auto p_num_subscribers,
    std::function<async::typ::subscriber_t<t_event>()> p_factory) {
  for (decltype(p_num_subscribers) _i = 0; _i < p_num_subscribers; ++_i) {
    add_subscriber(p_factory());
  }
}

template <cpt::event t_event>
inline size_t event_queue<t_event>::amount_added() const {
  return m_circular_queue.capacity();
}

template <cpt::event t_event>
inline size_t event_queue<t_event>::get_size() const {
  return m_circular_queue.capacity();
}

template <cpt::event t_event>
inline size_t event_queue<t_event>::get_occupied() const {
  return m_circular_queue.occupied();
}

template <cpt::event t_event>
inline void event_queue<t_event>::report(std::ostringstream &p_out) const {
  p_out << "\tpublishing: " << m_id << '\n'
        << "\t\tnum subscribers: " << m_loops.size()
        << ", priority: " << m_priority << '\n';
}

template <cpt::event t_event>
void event_queue<t_event>::add_subscriber(
    async::typ::subscriber_t<t_event> p_subscriber) {
  if (m_stopped) {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("event '", typeid(t_event).name(), "', publishing ", m_id,
                 ", queue ", m_circular_queue.get_id(),
                 " - not adding subscriber because m_stopped = ", m_stopped);
#endif
    return;
  }
  std::lock_guard<std::mutex> _lock(m_add_subscriber);

  m_loops.push_back(std::thread(
      [this, p_subscriber]() -> void { subscriber_loop(p_subscriber); }));
}

template <cpt::event t_event>
void event_queue<t_event>::subscriber_loop(
    async::typ::subscriber_t<t_event> p_subscriber) {
  auto _queue_id{m_circular_queue.get_id()};
  auto _loop_id{std::this_thread::get_id()};

  if (!p_subscriber) {
    std::stringstream _stream;
    _stream << "invalid event subscriber for " << '(' << typeid(t_event).name()
            << ',' << m_id << ',' << _queue_id << "," << _loop_id << ')';
    const std::string _str{_stream.str()};
    TNCT_LOG_FAT(_str);
    throw std::runtime_error(_str);
  }

  auto _subscriber_id{&p_subscriber};

  if (m_stopped) {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA('(', typeid(t_event).name(), ',', m_id, ',', _queue_id, ",",
                 _subscriber_id, ',', _loop_id, ')',
                 " - leaving because it was already stopped");
#endif
    return;
  }

#ifdef TENACITAS_LOG
  TNCT_LOG_TRA('(', typeid(t_event).name(), ',', m_id, ',', _queue_id, ",",
               _subscriber_id, ',', _loop_id, ')',
               " - entering loop, with m_stopped = ", m_stopped);
#endif
  while (true) {
    {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA('(', typeid(t_event).name(), ',', m_id, ',', _queue_id, ",",
                   _subscriber_id, ',', _loop_id, ')', " - locking");
#endif
      std::unique_lock<std::mutex> _lock(m_mutex);
      m_cond.wait(_lock, [this, _subscriber_id, _loop_id, _queue_id]() -> bool {
#ifdef TENACITAS_LOG
        TNCT_LOG_TRA('(', typeid(t_event).name(), ',', m_id, ',', _queue_id,
                     ",", _subscriber_id, ',', _loop_id, ')',
                     " - entering condition, with m_stop = ", m_stopped,
                     ", and m_queue.empty()? ", m_circular_queue.empty());
#endif
        if (m_stopped) {
#ifdef TENACITAS_LOG
          TNCT_LOG_TRA('(', typeid(t_event).name(), ',', m_id, ',', _queue_id,
                       ",", _subscriber_id, ',', _loop_id, ')', " - stopped");
#endif
          return true;
        }
        if (!m_circular_queue.empty()) {
#ifdef TENACITAS_LOG
          TNCT_LOG_TRA('(', typeid(t_event).name(), ',', m_id, ',', _queue_id,
                       ",", _subscriber_id, ',', _loop_id, ')',
                       " - there is data");
#endif
          return true;
        }
        if (m_stopped) {
#ifdef TENACITAS_LOG
          TNCT_LOG_TRA('(', typeid(t_event).name(), ',', m_id, ',', _queue_id,
                       ",", _subscriber_id, ',', _loop_id, ')', " - stopped");
#endif
          return true;
        }
#ifdef TENACITAS_LOG
        TNCT_LOG_TRA('(', typeid(t_event).name(), ',', m_id, ',', _queue_id,
                     ",", _subscriber_id, ',', _loop_id, ')', " - waiting");
#endif
        return false;
      });
    }

#ifdef TENACITAS_LOG
    TNCT_LOG_TRA('(', typeid(t_event).name(), ',', m_id, ',', _queue_id, ",",
                 _subscriber_id, ',', _loop_id, ')', " - lock released");
#endif

    if (m_stopped) {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA('(', typeid(t_event).name(), ',', m_id, ',', _queue_id, ",",
                   _subscriber_id, ',', _loop_id, ')',
                   " - stopped due to explicit stop");
#endif
      break;
    }

#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("about to get the event from the queue");
#endif

    std::optional<t_event> _maybe{m_circular_queue.get()};
    if (!_maybe.has_value()) {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA('(', typeid(t_event).name(), ',', m_id, ',', _queue_id, ",",
                   _subscriber_id, ',', _loop_id, ')', " - no event in queue");
#endif
      continue;
    }

    if (m_stopped) {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA('(', typeid(t_event).name(), ',', m_id, ',', _queue_id, ",",
                   _subscriber_id, ',', _loop_id, ')', " - stop");
#endif
      break;
    }

    t_event _event{std::move(*_maybe)};
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("got event ", typeid(t_event).name(), " from the queue");
#endif

    if (m_stopped) {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA('(', typeid(t_event).name(), ',', m_id, ',', _queue_id, ",",
                   _subscriber_id, ',', _loop_id, ')', " - stop");
#endif
      break;
    }

#ifdef TENACITAS_LOG
    //      std::stringstream _stream;
    //      _stream << _event;
    TNCT_LOG_TRA('(', typeid(t_event).name(), ',', m_id, ',', _queue_id, ",",
                 _subscriber_id, ',', _loop_id, ')', " - event ",
                 typeid(t_event).name(), " to be passed to a subscriber");
#endif
    if (!p_subscriber) {
      std::stringstream _stream;
      _stream << "invalid event subscriber for " << '('
              << typeid(t_event).name() << ',' << m_id << ',' << _queue_id
              << "," << _subscriber_id << ',' << _loop_id << ')';
      const std::string _str{_stream.str()};
      TNCT_LOG_FAT(_str);
      throw std::runtime_error(_str);

    } else {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA('(', typeid(t_event).name(), ',', m_id, ',', _queue_id, ",",
                   _subscriber_id, ',', _loop_id, ')',
                   " - about to call subscriber");

#endif
      p_subscriber(std::move(_event));
    }

#ifdef TENACITAS_LOG
    TNCT_LOG_TRA('(', typeid(t_event).name(), ',', m_id, ',', _queue_id, ",",
                 _subscriber_id, ',', _loop_id, ')', " - event handled");
#endif
  }
#ifdef TENACITAS_LOG
  TNCT_LOG_TRA('(', typeid(t_event).name(), ',', m_id, ',', _queue_id, ",",
               _subscriber_id, ',', _loop_id, ") - leaving");
#endif
}

template <cpt::event t_event>
void event_queue<t_event>::empty_queue(
    const std::thread::id &p_loop_id,
    async::typ::subscriber_t<t_event> p_subscriber) {
  while (true) {
    std::optional<t_event> _maybe{m_circular_queue.get()};
    if (!_maybe.has_value()) {
      break;
    }

    if (m_stopped) {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("event '", typeid(t_event), "', publishing ", m_id,
                   ", loop ", p_loop_id, " - stop");
#endif
      break;
    }

    t_event _event{std::move(*_maybe)};

    if (this->m_stopped) {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("event '", typeid(t_event).name(), "', publishing ", m_id,
                   ", loop ", p_loop_id, " - stop");
#endif
      break;
    }

#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("event '", typeid(t_event).name(), "', publishing ", m_id,
                 ", loop ", p_loop_id, " - event to be passed to a subscriber");
#endif
    p_subscriber(std::move(_event));
  }
}

} // namespace tenacitas::lib::async::internal::typ

#endif
