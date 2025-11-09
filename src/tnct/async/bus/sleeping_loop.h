/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_SLEEPING_LOOP_H
#define TNCT_ASYNC_SLEEPING_LOOP_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <string>
#include <string_view>

#include "tnct/format/bus/fmt.h"

#include "tnct/time/cpt/chrono_convertible.h"
#include "tnct/log/cpt/logger.h"
#include "tnct/log/cpt/macros.h"

namespace tnct::async
{

/// \brief Periodically executes a function
template <log::cpt::logger t_logger>
struct sleeping_loop
{
  /// \brief Signature of the function that will be called in each round of
  /// the loop
  using function = std::function<void()>;

  using logger = t_logger;

  /// \brief Constructor
  ///
  /// \tparam t_interval is the type of time used to define the amount of time
  /// that the function will be called
  ///
  /// \param p_function to be executed at a certain interval
  ///
  /// \param p_interval interval that \p p_function will be executed
  template <time::cpt::convertible_to_nano t_interval>
  sleeping_loop(logger &p_logger, function p_function, t_interval p_interval,
                std::string_view p_id = "no-id")
      : m_logger(p_logger), m_id(p_id), m_function(p_function),
        m_interval(std::chrono::duration_cast<decltype(m_interval)>(p_interval))
  {

    TNCT_LOG_TRA(m_logger, format::bus::fmt("sleeping loop ", m_id,
                                       " - creating with function ",
                                       &m_function, " and interval of ",
                                       m_interval.count(), " nanosecs"));
  }

  sleeping_loop()                                 = delete;
  sleeping_loop(const sleeping_loop &)            = delete;
  sleeping_loop &operator=(const sleeping_loop &) = delete;

  /// \brief Destructor
  /// The loops stops calling the function
  ~sleeping_loop()
  {
    TNCT_LOG_TRA(m_logger,
                 format::bus::fmt("sleeping loop ", m_id, " - destructor"));

    stop();
    if (m_thread.get_id() == std::thread::id())
    {
      TNCT_LOG_TRA(m_logger,
                   format::bus::fmt("sleeping loop ", m_id,
                               " - not joining because m_thread.get_id() == "
                               "std::thread::id()"));

      return;
    }
    TNCT_LOG_TRA(m_logger,
                 format::bus::fmt("sleeping loop ", m_id,
                             " - m_thread.get_id() != std::thread::id()"));

    if (!m_thread.joinable())
    {
      TNCT_LOG_TRA(
          m_logger,
          format::bus::fmt("sleeping loop ", m_id,
                      " - not joining because m_thread is not joinable"));

      return;
    }
    TNCT_LOG_TRA(m_logger,
                 format::bus::fmt("sleeping loop ", m_id, " - m_thread.joinable()"));

    {
      std::lock_guard<std::mutex> _lock(m_mutex_join);
      m_thread.join();
    }
  }

  /// \brief Move constructor
  sleeping_loop(sleeping_loop &&p_loop)
      : m_logger(p_loop.m_logger), m_id(std::move(p_loop.m_id)),
        m_function(std::move(p_loop.m_function)), m_interval(p_loop.m_interval)
  {
    TNCT_LOG_TRA(m_logger,
                 format::bus::fmt("sleeping loop ", m_id, " move constructor from ",
                             &p_loop, " to ", &(*this)));
    bool _stopped(p_loop.is_stopped());

    p_loop.stop();
    m_stopped = _stopped;

    //    m_thread = std::move(p_loop.m_thread);
    if (!m_stopped)
    {
      m_stopped = true;
      start();
    }
  }

  /// \brief Move assignment
  sleeping_loop &operator=(sleeping_loop &&) = delete;

  /// \brief Starts calling the function periodically
  void start()
  {
    if (!m_stopped)
    {
      TNCT_LOG_TRA(m_logger,
                   format::bus::fmt("sleeping loop ", m_id,
                               " - not starting because it is not stopped"));
      return;
    }
    m_stopped = false;

    TNCT_LOG_TRA(m_logger,
                 format::bus::fmt("sleeping loop ", m_id,
                             " - starting loop thread in ", &(*this)));

    m_thread = std::thread([this]() { loop(); });
  }

  /// \brief Stops the loop, if it was started
  void stop()
  {
    if (m_stopped)
    {
      TNCT_LOG_TRA(m_logger,
                   format::bus::fmt("sleeping loop ", m_id,
                               " - not stopping because it is stopped"));

      return;
    }
    TNCT_LOG_TRA(m_logger, format::bus::fmt("sleeping loop ", m_id,
                                       " - stopping in ", &(*this)));

    m_stopped = true;
    m_cond.notify_one();

    TNCT_LOG_TRA(m_logger, format::bus::fmt("sleeping loop ", m_id,
                                       " - leaving stop in ", &(*this)));
  }

  /// \brief Retrieves if the loop was stopped
  bool is_stopped() const
  {
    return m_stopped;
  }

  const std::string &get_id() const
  {
    return m_id;
  }

private:
  /// \brief Helper move function
  // void move(sleeping_loop &&p_loop) {
  //   bool _stopped = p_loop.is_stopped();

  //   TNCT_LOG_TRA(m_logger, format::bus::fmt("sleeping loop ", m_id, " - moving ",
  //                                       p_loop.m_id, " which was",
  //                                       (_stopped ? " " : " not "),
  //                                       "stopped"));

  //   p_loop.stop();

  //   m_function = p_loop.m_function;
  //   m_interval = std::move(p_loop.m_interval);
  //   m_stopped = true;

  //   if (!_stopped) {
  //     TNCT_LOG_TRA(m_logger, format::bus::fmt("sleeping loop ", m_id,
  //                                         " - starting it because ",
  //                                         p_loop.m_id, " was running"));

  //     start();
  //   }

  //   else {
  //     TNCT_LOG_TRA(m_logger, format::bus::fmt("sleeping loop ", m_id,
  //                                         " - NOT starting it because ",
  //                                         p_loop.m_id, " was NOT running"));
  //   }
  // }

  /// \brief Loop where the function will be called
  void loop()
  {
    while (true)
    {
      if (m_stopped)
      {
        TNCT_LOG_TRA(m_logger, format::bus::fmt("sleeping loop ", m_id, " - stop"));

        break;
      }

      TNCT_LOG_TRA(m_logger,
                   format::bus::fmt("sleeping loop ", m_id, " - calling function ",
                               &m_function, " in ", &(*this)));

      m_function();

      TNCT_LOG_TRA(m_logger, format::bus::fmt("sleeping loop ", m_id, " - ",
                                         &m_function, " called"));

      if (m_stopped)
      {
        TNCT_LOG_TRA(m_logger, format::bus::fmt("sleeping loop ", m_id, " - stop"));

        break;
      }
      TNCT_LOG_TRA(m_logger,
                   format::bus::fmt("sleeping loop ", m_id, " - waiting for ",
                               m_interval.count(),
                               " microsecs to elaps, or a stop order"));

      {
        std::unique_lock<std::mutex> _lock(m_mutex_interval);
        if ((m_cond.wait_for(_lock, m_interval) == std::cv_status::no_timeout)
            || (m_stopped))
        {

          TNCT_LOG_TRA(m_logger,
                       format::bus::fmt("sleeping loop ", m_id,
                                   " - ordered to stop in ", &(*this)));

          break;
        }
      }
      TNCT_LOG_TRA(m_logger, format::bus::fmt("sleeping loop ", m_id, " - ",
                                         m_interval.count(), " ms elapsed"));
    }
    TNCT_LOG_TRA(m_logger, format::bus::fmt("sleeping loop ", m_id,
                                       " - leaving loop in ", &(*this)));
  }

private:
  logger &m_logger;

  /// \brief Identifier of the slepping_loop, to help debugging
  std::string m_id;

  /// \brief Function that will be called in each round of the loop
  function m_function;

  /// \brief Interval for calling m_function
  std::chrono::nanoseconds m_interval;

  /// \brief Indicates that the loop must stop
  // std::atomic<bool> m_stopped{true};
  bool m_stopped{true};

  /// \brief Thread where the \p loop method will run
  std::thread m_thread;

  /// \brief Protects joining the thread
  std::mutex m_mutex_join;

  /// \brief Controls the interval execution
  std::mutex m_mutex_interval;

  /// \brief Controls the interval execution
  std::condition_variable m_cond;
};

} // namespace tnct::async

#endif
