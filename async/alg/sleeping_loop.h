#ifndef TENACITAS_LIB_ASYNC_SLEEPING_LOOP_H
#define TENACITAS_LIB_ASYNC_SLEEPING_LOOP_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example sleeping_loop_000/main.cpp

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <thread>

#include <tenacitas.lib/async/cpt/concepts.h>
#include <tenacitas.lib/calendar/cpt/chrono_convertible.h>
#include <tenacitas.lib/log/alg/logger.h>
#include <tenacitas.lib/number/typ/id.h>

using namespace std::chrono_literals;

/// \brief master namespace
namespace tenacitas::lib::async::alg {

/// \brief Periodically executes a function
struct sleeping_loop {
  /// \brief Signature of the function that will be called in each round of
  /// the loop
  using function = std::function<void()>;

  /// \brief Constructor
  ///
  /// \tparam t_interval is the type of time used to define the amount of time
  /// that the function will be called
  ///
  /// \param p_function to be executed at a certain interval
  ///
  /// \param p_interval interval that \p p_function will be executed
  template <calendar::cpt::convertible_to_ns t_interval>
  sleeping_loop(function p_function, t_interval p_interval);

  sleeping_loop() = delete;
  sleeping_loop(const sleeping_loop &) = delete;
  sleeping_loop &operator=(const sleeping_loop &) = delete;

  /// \brief Destructor
  /// The loops stops calling the function
  ~sleeping_loop();

  /// \brief Move constructor
  sleeping_loop(sleeping_loop &&p_loop);

  /// \brief Move assignment
  sleeping_loop &operator=(sleeping_loop &&p_loop);

  /// \brief Starts calling the function periodically
  void start();

  /// \brief Stops the loop, if it was started
  void stop();

  /// \brief Retrieves if the loop was stopped
  bool is_stopped() const;

private:
  /// \brief Helper move function
  void move(sleeping_loop &&p_loop);

  /// \brief Loop where the function will be called
  void loop();

private:
  /// \brief Function that will be called in each round of the loop
  function m_function;

  /// \brief Interval for calling m_function
  std::chrono::nanoseconds m_interval;

  /// \brief Indicates that the loop must stop
  std::atomic<bool> m_stopped{true};

  /// \brief Thread where the \p loop method will run
  std::thread m_thread;

  /// \brief Protects joining the thread
  std::mutex m_mutex_join;

  /// \brief Controls the interval execution
  std::mutex m_mutex_interval;

  /// \brief Controls the interval execution
  std::condition_variable m_cond;

  /// \brief Identifier of the slepping_loop, to help debugging
  number::typ::id m_id;
};

template <calendar::cpt::convertible_to_ns t_interval>
inline sleeping_loop::sleeping_loop(function p_function, t_interval p_interval)
    : m_function(p_function),
      m_interval(std::chrono::duration_cast<decltype(m_interval)>(p_interval)) {
  TNCT_LOG_TRA("sleeping loop ", m_id, " - creating with function ",
               &m_function);
}

sleeping_loop::~sleeping_loop() {
  TNCT_LOG_TRA("sleeping loop ", m_id, " - destructor");
  stop();
  if (m_thread.get_id() == std::thread::id()) {
    TNCT_LOG_TRA("sleeping loop ", m_id,
                 " - not joining because m_thread.get_id() == "
                 "std::thread::id()");
    return;
  }
  TNCT_LOG_TRA("sleeping loop ", m_id,
               " - m_thread.get_id() != std::thread::id()");

  if (!m_thread.joinable()) {
    TNCT_LOG_TRA("sleeping loop ", m_id,
                 " - not joining because m_thread is not joinable");
    return;
  }
  TNCT_LOG_TRA("sleeping loop ", m_id, " - m_thread.joinable()");

  {
    std::lock_guard<std::mutex> _lock(m_mutex_join);
    m_thread.join();
  }
}

inline sleeping_loop::sleeping_loop(sleeping_loop &&p_loop) {
  TNCT_LOG_TRA("sleeping loop ", m_id, " move constructor");
  move(std::move(p_loop));
}

sleeping_loop &sleeping_loop::operator=(sleeping_loop &&p_loop) {
  TNCT_LOG_TRA("sleeping loop ", m_id, " move assignment");
  if (this != &p_loop) {
    move(std::move(p_loop));
  }
  return *this;
}

void sleeping_loop::start() {
  if (!m_stopped) {
    return;
  }
  m_stopped = false;

  TNCT_LOG_TRA("sleeping loop ", m_id, " - starting loop thread");
  m_thread = std::thread([this]() { loop(); });
}

void sleeping_loop::stop() {
  if (m_stopped) {
    TNCT_LOG_TRA("sleeping loop ", m_id,
                 " - not stopping because it is stopped");
    return;
  }
  TNCT_LOG_TRA("sleeping loop ", m_id, " - stopping");
  m_stopped = true;
  m_cond.notify_one();

  TNCT_LOG_TRA("sleeping loop ", m_id, " - leaving stop");
}

inline bool sleeping_loop::is_stopped() const { return m_stopped; }

void sleeping_loop::move(sleeping_loop &&p_loop) {
  bool _stopped = p_loop.is_stopped();
  number::typ::id _other = p_loop.m_id;
  TNCT_LOG_TRA("sleeping loop ", m_id, " - moving ", _other, " which was",
               (_stopped ? " " : " not "), "stopped");

  p_loop.stop();

  m_function = p_loop.m_function;
  m_interval = std::move(p_loop.m_interval);
  m_stopped = true;

  if (!_stopped) {
    TNCT_LOG_TRA("sleeping loop ", m_id, " - starting it because ", _other,
                 " was running");
    start();
  } else {
    TNCT_LOG_TRA("sleeping loop ", m_id, " - NOT starting it because ", _other,
                 " was NOT running");
  }
}

void sleeping_loop::loop() {

  while (true) {
    if (m_stopped) {
      TNCT_LOG_TRA("sleeping loop ", m_id, " - stop");
      break;
    }

    TNCT_LOG_TRA("sleeping loop ", m_id, " - calling subscriber ", &m_function);

    m_function();

    if (m_stopped) {
      TNCT_LOG_TRA("sleeping loop ", m_id, " - stop");
      break;
    }

    TNCT_LOG_TRA("sleeping loop ", m_id, " - waiting for ", m_interval.count(),
                 " microsecs to elaps, or a stop order");
    {
      std::unique_lock<std::mutex> _lock(m_mutex_interval);
      if ((m_cond.wait_for(_lock, m_interval) == std::cv_status::no_timeout) ||
          (m_stopped)) {
        TNCT_LOG_TRA("sleeping loop ", m_id, " - ordered to stop");
        break;
      }
    }
    TNCT_LOG_TRA("sleeping loop ", m_id, " - ", m_interval.count(),
                 " ms elapsed");
  }
  TNCT_LOG_TRA("sleeping loop ", m_id, " - leaving loop");
}
} // namespace tenacitas::lib::async::alg

#endif
