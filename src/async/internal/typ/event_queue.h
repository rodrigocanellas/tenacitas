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

#ifdef TENACITAS_LOG
#include <tenacitas.lib/src/log/alg/logger.h>
#include <tenacitas.lib/src/number/alg/format.h>
#endif

namespace tenacitas::lib::async::internal::typ {

// template <cpt::event t_event> class queue_t;

// A publishing for an event
template <cpt::event t_event> class event_queue {

public:
  event_queue() = delete;

  event_queue(async::typ::priority p_priority = async::typ::priority::medium);

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
  void add_subscriber(async::typ::subscriber<t_event> p_subscriber);

  // Adds a bunch of subscribers
  void
  add_subscriber(std::unsigned_integral auto p_num_subscribers,
                 std::function<async::typ::subscriber<t_event>()> p_factory);

  ~event_queue();

  // \brief Stops this publishing
  void stop() {

    trace(__LINE__, "entering stop()");

    if (m_stopped) {
      trace(__LINE__, "not stopping because it is stopped");
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_cond.notify_all();
      return;
    }

    trace(__LINE__, "notifying loops to stop");

    {
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_stopped = true;
      m_cond.notify_all();
    }

    for (std::thread &_thread : m_loops) {
      if (_thread.joinable()) {

        trace(__LINE__, "is joinable", _thread.get_id());

        _thread.join();

        trace(__LINE__, "joined", _thread.get_id());
      } else {
        trace(__LINE__, "is not joinable", _thread.get_id());
      }
    }
    trace(__LINE__, "leaving stop()");
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

  void clear() {
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_paused = true;
    m_circular_queue.clear();
    m_paused = false;
    m_cond.notify_all();
  }

  // publishing is ordered by tenacitas::lib::async::priority
  constexpr bool operator<(const event_queue &p_publishing) const;

  // publishing is ordered by tenacitas::lib::async::priority
  constexpr bool operator>(const event_queue &p_publishing) const;

  // publishing is compared by tenacitas::lib::async::async::typ::publishing_id
  constexpr bool operator!=(const event_queue &p_publishing) const;

  // publishing is compared by tenacitas::lib::async::async::typ::publishing_id
  constexpr bool operator==(const event_queue &p_publishing) const;

  void report(std::ostringstream &p_out) const;

  //  void clear() {
  //    std::lock_guard<std::mutex> _lock(m_mutex);
  //    stop();
  //    m_circular_queue.clear();
  //  }

  void trace(uint16_t p_line, std::string_view p_text,
             std::thread::id p_thread = std::this_thread::get_id()) {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA(number::alg::format(p_line, uint8_t{5}), " - E ",
                 typeid(t_event).name(), ", Q {", this, ',', m_id, "}, C {",
                 m_circular_queue.get_id(), ",", m_circular_queue.occupied(),
                 "}, # L ", m_loops.size(), ", S? ",
                 (m_stopped.load() ? 'T' : 'F'), ", T ", p_thread, " - ",
                 p_text);
#endif
  }

protected:
  // Group of loops that asynchronously call the subscribers
  using loops = std::vector<std::thread>;

  // Queue used to store the events to be handled
  using circular_queue = container::typ::circular_queue<t_event>;

protected:
  void subscriber_loop(async::typ::subscriber<t_event> p_subscriber);

  void empty_queue(const std::thread::id &p_loop_id,
                   async::typ::subscriber<t_event> p_subscriber);

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

  std::atomic_bool m_paused{false};

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
  trace(__LINE__, "entering destructor");
  stop();
  trace(__LINE__, "leaving destructor");
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
    : m_id(number::typ::create<decltype(m_id)>()), m_priority(p_priority) {
  trace(__LINE__, "creating queue");
}

template <cpt::event t_event>
bool event_queue<t_event>::add_event(const t_event &p_event) {
  try {
    trace(__LINE__, "adding event");

    m_circular_queue.add(p_event);

    std::lock_guard<std::mutex> _lock(m_mutex);
    m_cond.notify_all();

    ++m_queued_data;

    return true;

  } catch (std::exception &_ex) {
    trace(__LINE__, "error adding event: " + std::string(_ex.what()));
  }
  return false;
}

template <cpt::event t_event>
void event_queue<t_event>::add_subscriber(
    std::unsigned_integral auto p_num_subscribers,
    std::function<async::typ::subscriber<t_event>()> p_factory) {
  trace(__LINE__, "adding many subscribers");
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
    async::typ::subscriber<t_event> p_subscriber) {
  if (m_stopped) {
    trace(__LINE__, "not adding subscriber because stopped");
    return;
  }

  trace(__LINE__, "adding subscriber");

  std::lock_guard<std::mutex> _lock(m_add_subscriber);

  m_loops.push_back(std::thread(
      [this, p_subscriber]() -> void { subscriber_loop(p_subscriber); }));
}

template <cpt::event t_event>
void event_queue<t_event>::subscriber_loop(
    async::typ::subscriber<t_event> p_subscriber) {
  auto _queue_id{m_circular_queue.get_id()};
  auto _loop_id{std::this_thread::get_id()};

  trace(__LINE__, "starting subscriber's loop", _loop_id);

  if (!p_subscriber) {
    std::stringstream _stream;
    _stream << "event " << typeid(t_event).name() << "<< queue {" << this
            << "<<" << m_id << "}<< circular " << m_circular_queue.get_id()
            << "<< queued_data = " << m_queued_data << "<< priority "
            << m_priority << "<< loops = " << &m_loops
            << "<< stopped = " << m_stopped.load() << "<< thread "
            << std::this_thread::get_id() << " - "
            << "invalid event subscriber";
    const std::string _str{_stream.str()};
    TNCT_LOG_FAT(_str);
    throw std::runtime_error(_str);
  }

  auto _subscriber_id{&p_subscriber};

  if (m_stopped) {
    trace(__LINE__, "leaving because it was already stopped", _loop_id);
    return;
  }

  trace(__LINE__, "entering loop");

  while (true) {
    {
      trace(__LINE__, "locking", _loop_id);

      std::unique_lock<std::mutex> _lock(m_mutex);
      m_cond.wait(_lock, [this, _subscriber_id, _loop_id, _queue_id]() -> bool {
        trace(__LINE__, "entering condition", _loop_id);
        if (m_paused) {
          trace(__LINE__, "stopped", _loop_id);
          return false;
        }
        if (m_stopped) {
          trace(__LINE__, "stopped", _loop_id);
          return true;
        }
        if (!m_circular_queue.empty()) {
          trace(__LINE__, "there is data", _loop_id);
          return true;
        }
        trace(__LINE__, "waiting", _loop_id);
        return false;
      });
    }

    trace(__LINE__, "lock released", _loop_id);

    if (m_stopped) {
      trace(__LINE__, "stopped due to explicit stop", _loop_id);
      break;
    }

    trace(__LINE__, "about to get the event from the queue", _loop_id);

    std::optional<t_event> _maybe{m_circular_queue.get()};
    if (!_maybe.has_value()) {
      trace(__LINE__, "no event in queue", _loop_id);
      continue;
    }
    t_event _event{std::move(*_maybe)};

    trace(__LINE__, "about to call subscriber", _loop_id);
    p_subscriber(std::move(_event));

    trace(__LINE__, "event handled", _loop_id);
  }
  trace(__LINE__, "leaving subscriber's loop", _loop_id);
}

template <cpt::event t_event>
void event_queue<t_event>::empty_queue(
    const std::thread::id &p_loop_id,
    async::typ::subscriber<t_event> p_subscriber) {

  trace(__LINE__, "entering empty_queue", p_loop_id);
  while (true) {
    std::optional<t_event> _maybe{m_circular_queue.get()};
    if (!_maybe.has_value()) {
      break;
    }

    if (m_stopped) {
      trace(__LINE__, "stop", p_loop_id);
      break;
    }

    t_event _event{std::move(*_maybe)};

    if (this->m_stopped) {
      trace(__LINE__, "stop", p_loop_id);
      break;
    }

    trace(__LINE__, "event to be passed to a subscriber", p_loop_id);
    p_subscriber(std::move(_event));
    trace(__LINE__, "event handled", p_loop_id);
  }
}

} // namespace tenacitas::lib::async::internal::typ

#endif
