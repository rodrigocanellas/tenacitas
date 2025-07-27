/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_ASYNC_INTERNAL_HANDLING_H
#define TENACITAS_SRC_ASYNC_INTERNAL_HANDLING_H

#include <condition_variable>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>
#include <typeinfo>
#include <vector>

#include "tenacitas/src/async/handling_name.h"
#include "tenacitas/src/async/internal/handler_id.h"
#include "tenacitas/src/async/internal/handling_id.h"
#include "tenacitas/src/format/fmt.h"
#include "tenacitas/src/traits/async/event.h"
#include "tenacitas/src/traits/async/handling_function.h"
#include "tenacitas/src/traits/container/queue.h"
#include "tenacitas/src/traits/log/logger.h"

namespace tenacitas::src::async::internal
{

template <traits::async::event t_event>
class handling
{
public:
  virtual ~handling() = default;

  virtual void add_event(const t_event &p_event) = 0;

  // virtual void increment_handlers(size_t p_num_handlers) = 0;

  virtual void stop() = 0;

  virtual constexpr bool is_stopped() const = 0;

  [[nodiscard]] virtual constexpr size_t get_amount_handlers() const = 0;

  [[nodiscard]] virtual handling_id get_id() const = 0;

  [[nodiscard]] virtual handling_name get_name() const = 0;

  [[nodiscard]] virtual constexpr size_t get_num_events() const = 0;

  [[nodiscard]] virtual internal::handler_id get_handler_id() const = 0;

  virtual void clear() = 0;
};

template <traits::log::logger t_logger, traits::async::event t_event,
          traits::container::queue<t_event> t_queue,
          traits::async::handling_function<t_event>   t_handler>
class handling_concrete final : public handling<t_event>
{
public:
  using logger  = t_logger;
  using event   = t_event;
  using queue   = t_queue;
  using handler = t_handler;

  handling_concrete(handling_name p_handling_name, t_logger &p_logger,
                    handler &&p_handler, queue &&p_queue,
                    size_t p_num_handlers = 1)
      : m_logger(p_logger),
        m_handling_id(std::hash<handling_name>{}(p_handling_name)),
        m_handler(p_handler), m_queue(std::move(p_queue)),
        m_handler_id(internal::get_handler_id<t_event, t_handler>())
  {
    TNCT_LOG_TRA(m_logger, format::fmt("m_handling_id = ", m_handling_id,
                                       ", m_handler_id = ", m_handler_id,
                                       ", p_num_handlers = ", p_num_handlers));
    increment_handlers(p_num_handlers);
  }

  handling_concrete(const handling_concrete &) = delete;

  handling_concrete(handling_concrete &&p_handling)
      : m_logger(p_handling.m_logger), m_handling_id(p_handling.m_handling_id),
        m_handler(std::move(p_handling.m_handler)),
        m_queue(p_handling.m_logger), m_handler_id(p_handling.m_handler_id)
  {
    const bool _right_handling_was_stopped{p_handling.is_stopped()};
    p_handling.stop();

    m_queued_data.store(p_handling.m_queued_data);
    m_queue = std::move(p_handling.m_queue);
    if (!_right_handling_was_stopped)
    {
      increment_handlers(p_handling.get_amount_handlers());
    }
  }

  [[nodiscard]] internal::handler_id get_handler_id() const override
  {
    return m_handler_id;
  }

  ~handling_concrete() override
  {
    TNCT_LOG_TRA(m_logger, trace("entering destructor"));
    stop();
    TNCT_LOG_TRA(m_logger, trace("leaving destructor"));
  }

  handling_concrete &operator=(const handling_concrete &) = default;
  handling_concrete &operator=(handling_concrete &&)      = default;

  void add_event(const event &p_event) override
  {

    TNCT_LOG_TRA(m_logger, format::fmt("event = ", p_event));

    m_queue.push(p_event);

    std::lock_guard<std::mutex> _lock(m_data_mutex);
    m_data_cond.notify_all();

    ++m_queued_data;
  }

  // \brief Stops this handling
  void stop() override
  {
    TNCT_LOG_TRA(m_logger, trace("entering stop()"));

    if (m_stopped)
    {
      TNCT_LOG_TRA(m_logger, trace("not stopping because it is stopped"));

      std::lock_guard<std::mutex> _lock(m_data_mutex);
      m_data_cond.notify_all();
      return;
    }
    TNCT_LOG_TRA(m_logger, trace("notifying loops to stop"));

    {
      std::lock_guard<std::mutex> _lock(m_data_mutex);
      m_stopped.store(true);
      m_data_cond.notify_all();
    }

    for (std::thread &_thread : m_loops)
    {
      if (_thread.joinable())
      {
        TNCT_LOG_TRA(m_logger, trace("is joinable", _thread.get_id()));

        _thread.join();

        TNCT_LOG_TRA(m_logger, trace("joined", _thread.get_id()));
      }
      else
      {
        TNCT_LOG_TRA(m_logger, trace("is not joinable", _thread.get_id()));
      }
    }
    TNCT_LOG_TRA(m_logger, trace("leaving stop()"));
  }

  // Informs if the publishing is stopped
  constexpr bool is_stopped() const override
  {
    return m_stopped;
  }

  [[nodiscard]] constexpr size_t get_amount_handlers() const override
  {
    return m_handling_handlers.size();
  }

  [[nodiscard]] handling_name get_handling_name() const override
  {
    return m_handling_name;
  }

  [[nodiscard]] handling_id get_id() const override
  {
    return m_handling_id;
  }

  // \return Returns the amount of \p t_event objects in the queue
  [[nodiscard]] constexpr size_t get_num_events() const override
  {
    return m_queue.occupied();
  }

  void clear() override
  {
    std::lock_guard<std::mutex> _lock(m_data_mutex);
    m_queue.clear();
    m_data_cond.notify_all();
  }

  friend std::ostream &operator<<(std::ostream            &p_out,
                                  const handling_concrete &p_handling)
  {
    p_out << "{event " << typeid(event).name() << ", handling address "
          << &p_handling << ", handling id " << p_handling.m_handling_id
          << ", queue { capacity " << p_handling.m_queue.capacity()
          << ", occupied " << p_handling.m_queue.occupied() << " }"
          << ", stopped? " << (p_handling.m_stopped ? 'T' : 'F') << ", #loops "
          << p_handling.m_loops.size() << '}';
    return p_out;
  }

  std::string trace(std::string_view p_text,
                    std::thread::id  p_thread = std::this_thread::get_id())
  {
    std::ostringstream _stream;
    _stream << *this << " - thread = " << p_thread << " - " << p_text;
    return _stream.str();
  }

private:
  // Group of async_loops that asynchronously call the handlers
  using async_loops = std::vector<std::thread>;

  // Group of handlers
  using handling_handlers = std::vector<handler>;

  using handling_handler_pos = typename handling_handlers::size_type;

private:
  // void increment_handlers(size_t p_num_handlers) override {
  //     if (p_num_handlers == 0) {
  //         return;
  //     }

  //     if (m_stopped) {
  //         TNCT_LOG_TRA(m_logger,
  //                      trace("not adding subscriber because handling is
  //                      stopped"));
  //         return;
  //     }

  //     TNCT_LOG_TRA(m_logger, trace(format::fmt("adding ", p_num_handlers,
  //                                              " event handlers")));

  //     std::lock_guard<std::mutex> _lock(m_add_subscriber_mutex);

  //     // std::size_t _total_num_handlers{m_handling_handlers.size()};

  //     for (decltype(p_num_handlers) _i = 0; _i < p_num_handlers; ++_i) {
  //         m_handling_handlers.push_back(m_handler);

  //         const handling_handler_pos
  //         _new_handler_pos{m_handling_handlers.size()
  //                                                     -
  //                                                     1};

  //         TNCT_LOG_TRA(m_logger,
  //                      format::fmt("_new_handler_pos ", _new_handler_pos));

  //         m_loops.push_back(std::thread([this, _new_handler_pos]() -> void {
  //             handler_loop(_new_handler_pos);
  //         }));
  //     }
  // }

  void increment_handlers(size_t p_num_handlers)
  {
    if (p_num_handlers == 0)
    {
      return;
    }

    TNCT_LOG_TRA(m_logger, trace(format::fmt("adding ", p_num_handlers,
                                             " event handlers")));

    // std::size_t _total_num_handlers{m_handling_handlers.size()};

    for (decltype(p_num_handlers) _i = 0; _i < p_num_handlers; ++_i)
    {
      m_handling_handlers.push_back(m_handler);

      const handling_handler_pos _new_handler_pos{m_handling_handlers.size()
                                                  - 1};

      TNCT_LOG_TRA(m_logger,
                   format::fmt("_new_handler_pos ", _new_handler_pos));

      m_loops.push_back(std::thread([this, _new_handler_pos]() -> void
                                    { handler_loop(_new_handler_pos); }));
    }
  }

  // Waits for an event to be available in the event queue, then remove it and
  // call the handler in \p p_handling_handler_pos in \p m_handling_handlers. It
  // exits when \p m_stop is set.
  void handler_loop(handling_handler_pos p_handling_handler_pos)
  {

    // auto _queue_addr{&m_queue};
    auto _loop_id{std::this_thread::get_id()};

    TNCT_LOG_TRA(
        m_logger,
        format::fmt("p_handling_handler_pos = ", p_handling_handler_pos, " ",
                    trace("starting subscriber's loop", _loop_id)));

    // auto _subscriber_id{&m_handling_handlers[p_handling_handler_pos]};

    if (m_stopped)
    {
      TNCT_LOG_TRA(m_logger, trace("already stopped", _loop_id));
      return;
    }

    TNCT_LOG_TRA(m_logger, trace("entering loop"));

    while (true)
    {
      {
        TNCT_LOG_TRA(m_logger, trace("locking", _loop_id));

        std::unique_lock<std::mutex> _lock(m_data_mutex);
        m_data_cond.wait(
            _lock,
            [this, /*_subscriber_id,*/ _loop_id /*,_queue_addr*/]() -> bool
            {
              TNCT_LOG_TRA(m_logger, trace("entering condition", _loop_id));

              if (m_stopped)
              {
                TNCT_LOG_TRA(m_logger, trace("stopped", _loop_id));
                return true;
              }
              if (!m_queue.empty())
              {
                TNCT_LOG_TRA(m_logger, trace("there is data", _loop_id));
                return true;
              }
              TNCT_LOG_TRA(m_logger, trace("waiting", _loop_id));
              return false;
            });
      }

      TNCT_LOG_TRA(m_logger, trace("lock released", _loop_id));

      if (m_stopped)
      {
        TNCT_LOG_TRA(m_logger, trace("stopped due to explicit stop", _loop_id));
        break;
      }

      TNCT_LOG_TRA(m_logger, trace("getting event from the queue", _loop_id));

      std::optional<event> _maybe{m_queue.pop()};
      if (!_maybe.has_value())
      {
        TNCT_LOG_TRA(m_logger, trace("no event in queue", _loop_id));
        continue;
      }
      event _event{std::move(*_maybe)};

      TNCT_LOG_TRA(
          m_logger,
          format::fmt("p_handling_handler_pos = ", p_handling_handler_pos,
                      ", handler address = ",
                      &m_handling_handlers[p_handling_handler_pos], " ",
                      trace("about to call subscriber", _loop_id)));

      m_handling_handlers[p_handling_handler_pos](std::move(_event));

      TNCT_LOG_TRA(m_logger, trace("event handled", _loop_id));
    }

    TNCT_LOG_TRA(m_logger, trace("leaving subscriber's loop", _loop_id));
  }

  void empty_queue(const std::thread::id &p_loop_id, handler p_subscriber)
  {
    TNCT_LOG_TRA(m_logger, trace("entering empty_queue", p_loop_id));

    while (true)
    {
      std::optional<event> _maybe{m_queue.pop()};
      if (!_maybe.has_value())
      {
        break;
      }

      if (m_stopped)
      {
        TNCT_LOG_TRA(m_logger, trace("stop", p_loop_id));
        break;
      }

      event _event{std::move(*_maybe)};

      if (this->m_stopped)
      {
        TNCT_LOG_TRA(m_logger, trace("stop", p_loop_id));
        break;
      }

      TNCT_LOG_TRA(m_logger, trace("calling a subscriber", p_loop_id));

      p_subscriber(std::move(_event));

      TNCT_LOG_TRA(m_logger, trace("event handled", p_loop_id));
    }
  }

private:
  logger &m_logger;

  handling_name m_handling_name;

  handling_id m_handling_id;

  handler m_handler;

  queue m_queue;

  // size_t m_handler_id{0};
  internal::handler_id m_handler_id;

  // Indicates if the dispatcher should continue to run
  std::atomic_bool m_stopped{false};

  // Amount of queued data
  std::atomic_size_t m_queued_data{0};

  handling_handlers m_handling_handlers;

  // Asynchronous async_loops, where events are retrieved from queue, and
  // subscribers are called
  async_loops m_loops;

  // Controls access to the \p m_loops while inserting a new handler
  // std::mutex m_add_subscriber_mutex;

  // Controls access to inserting data
  std::mutex m_data_mutex;

  // Controls access to the data produced
  std::condition_variable m_data_cond;
};

} // namespace tenacitas::src::async::internal

#endif
