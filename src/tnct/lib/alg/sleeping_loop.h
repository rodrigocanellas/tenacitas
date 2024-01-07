/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LIB_ALG_SLEEPING_LOOP_H
#define TNCT_LIB_ALG_SLEEPING_LOOP_H

#include <future>

#ifdef TENACITAS_LOG
#include <tnct/lib/alg/log.h>
#endif
#include <tnct/lib/cpt/chrono_convertible.h>

namespace tnct::lib::alg {

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
  template <tlc::convertible_to_nano t_interval>
  sleeping_loop(function p_function, t_interval p_interval);

  sleeping_loop() = delete;
  sleeping_loop(const sleeping_loop &) = delete;
  sleeping_loop &operator=(const sleeping_loop &) = delete;

  /// \brief Destructor
  /// The loops stops calling the function
  ~sleeping_loop() {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id, " - destructor");
#endif
    stop();
    if (m_thread.get_id() == std::thread::id()) {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("sleeping loop ", m_id,
                   " - not joining because m_thread.get_id() == "
                   "std::thread::id()");
#endif
      return;
    }
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id,
                 " - m_thread.get_id() != std::thread::id()");
#endif
    if (!m_thread.joinable()) {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("sleeping loop ", m_id,
                   " - not joining because m_thread is not joinable");
#endif
      return;
    }
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id, " - m_thread.joinable()");
#endif
    {
      std::lock_guard<std::mutex> _lock(m_mutex_join);
      m_thread.join();
    }
  }

  /// \brief Move constructor
  sleeping_loop(sleeping_loop &&p_loop);

  /// \brief Move assignment
  sleeping_loop &operator=(sleeping_loop &&p_loop) {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id, " move assignment");
#endif
    if (this != &p_loop) {
      move(std::move(p_loop));
    }
    return *this;
  }

  /// \brief Starts calling the function periodically
  void start() {
    if (!m_stopped) {
      return;
    }
    m_stopped = false;

#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id, " - starting loop thread");
#endif
    m_thread = std::thread([this]() { loop(); });
  }

  /// \brief Stops the loop, if it was started
  void stop() {
    if (m_stopped) {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("sleeping loop ", m_id,
                   " - not stopping because it is stopped");
#endif
      return;
    }
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id, " - stopping");
#endif
    m_stopped = true;
    m_cond.notify_one();

#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id, " - leaving stop");
#endif
  }

  /// \brief Retrieves if the loop was stopped
  bool is_stopped() const;

private:
  /// \brief Helper move function
  void move(sleeping_loop &&p_loop) {
    bool _stopped = p_loop.is_stopped();
    id_t _other = p_loop.m_id;
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id, " - moving ", _other, " which was",
                 (_stopped ? " " : " not "), "stopped");
#endif

    p_loop.stop();

    m_function = p_loop.m_function;
    m_interval = std::move(p_loop.m_interval);
    m_stopped = true;

    if (!_stopped) {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("sleeping loop ", m_id, " - starting it because ", _other,
                   " was running");
#endif
      start();
    }
#ifdef TENACITAS_LOG
    else {
      TNCT_LOG_TRA("sleeping loop ", m_id, " - NOT starting it because ",
                   _other, " was NOT running");
    }
#endif
  }

  /// \brief Loop where the function will be called
  void loop() {
    while (true) {
      if (m_stopped) {
#ifdef TENACITAS_LOG
        TNCT_LOG_TRA("sleeping loop ", m_id, " - stop");
#endif
        break;
      }

#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("sleeping loop ", m_id, " - calling subscriber ",
                   &m_function);
#endif

      m_function();

      if (m_stopped) {
#ifdef TENACITAS_LOG
        TNCT_LOG_TRA("sleeping loop ", m_id, " - stop");
#endif
        break;
      }
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("sleeping loop ", m_id, " - waiting for ",
                   m_interval.count(), " microsecs to elaps, or a stop order");
#endif
      {
        std::unique_lock<std::mutex> _lock(m_mutex_interval);
        if ((m_cond.wait_for(_lock, m_interval) ==
             std::cv_status::no_timeout) ||
            (m_stopped)) {
#ifdef TENACITAS_LOG
          TNCT_LOG_TRA("sleeping loop ", m_id, " - ordered to stop");
#endif
          break;
        }
      }
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("sleeping loop ", m_id, " - ", m_interval.count(),
                   " ms elapsed");
#endif
    }
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id, " - leaving loop");
#endif
  }

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
  id_t m_id;
};

template <tlc::convertible_to_nano t_interval>
inline sleeping_loop::sleeping_loop(function p_function, t_interval p_interval)
    : m_function(p_function),
      m_interval(std::chrono::duration_cast<decltype(m_interval)>(p_interval)) {
#ifdef TENACITAS_LOG
  TNCT_LOG_TRA("sleeping loop ", m_id, " - creating with function ",
               &m_function);
#endif
}

inline sleeping_loop::sleeping_loop(sleeping_loop &&p_loop) {
#ifdef TENACITAS_LOG
  TNCT_LOG_TRA("sleeping loop ", m_id, " move constructor");
#endif
  move(std::move(p_loop));
}

bool sleeping_loop::is_stopped() const { return m_stopped; }

} // namespace tnct::lib::alg

namespace tla = tnct::lib::alg;

#endif
