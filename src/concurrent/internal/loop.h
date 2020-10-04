#ifndef TENACITAS_CONCURRENT_LOOP_H
#define TENACITAS_CONCURRENT_LOOP_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <chrono>
#include <future>

#include <concurrent/executer.h>
#include <concurrent/internal/log.h>
#include <concurrent/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

struct loop_base {

  inline void stop() { m_stopped = true; }

  virtual ~loop_base() { stop(); }

protected:
  bool m_stopped{true};
};

// ################ A ########################################################
template <typename t_log, typename... t_params>
struct loop_with_breaker_t : public loop_base {

  template <typename t_work_executer, typename t_provider_executer,
            typename t_breaker_executer>
  void operator()(t_work_executer &p_work_executer,
                  t_provider_executer &p_provider_executer,
                  t_breaker_executer &p_breaker_executer) {
    if (!m_stopped) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return;
    }

    concurrent_info(m_log, "starting");

    m_stopped = false;
    p_work_executer.start();
    p_provider_executer.start();
    p_breaker_executer.start();

    while (true) {

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      std::optional<std::optional<std::tuple<t_params...>>> _maybe_executed =
          p_provider_executer();

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      if (_maybe_executed) {
        concurrent_debug(m_log, "provider executed");
        std::optional<std::tuple<t_params...>> _maybe_provided =
            _maybe_executed.value();

        if (_maybe_provided) {
          auto _provided = _maybe_provided.value();
          concurrent_debug(m_log, "provider provided: ", _provided);

          std::apply(p_work_executer, _provided);
          if (m_stopped) {
            concurrent_debug(m_log, "stopped!!");
            break;
          }
        }
      }

      concurrent_debug(m_log, "about to call breaker");
      std::optional<bool> _maybe_break = p_breaker_executer();
      if ((_maybe_break) && (_maybe_break.value())) {
        concurrent_debug(m_log, "stopped by breaker");
        m_stopped = true;
        break;
      }
    }
    p_work_executer.stop();
    p_provider_executer.stop();
    p_breaker_executer.stop();
  }

private:
  t_log m_log{"loop_with_breaker"};
};

template <typename t_log, typename... t_params>
struct loop_with_no_breaker_t : public loop_base {

  template <typename t_work_executer, typename t_provider_executer,
            typename t_breaker_executer>
  void operator()(t_work_executer &p_work_executer,
                  t_provider_executer &p_provider_executer,
                  t_breaker_executer &) {
    if (!m_stopped) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return;
    }

    concurrent_info(m_log, "starting");

    m_stopped = false;
    p_work_executer.start();
    p_provider_executer.start();

    while (true) {

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      std::optional<std::optional<std::tuple<t_params...>>> _maybe_executed =
          p_provider_executer();

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      if (_maybe_executed) {
        concurrent_debug(m_log, "provider executed");
        std::optional<std::tuple<t_params...>> _maybe_provided =
            _maybe_executed.value();

        if (_maybe_provided) {
          auto _provided = _maybe_provided.value();
          concurrent_debug(m_log, "provider provided: ", _provided);

          std::apply(p_work_executer, _provided);
          if (m_stopped) {
            concurrent_debug(m_log, "stopped!!");
            break;
          }
        }
      }
    }
    p_work_executer.stop();
    p_provider_executer.stop();
  }

private:
  t_log m_log{"concurrent::loop_with_breaker"};
};

// ################ B ########################################################
template <typename t_log, typename t_param>
struct loop_with_breaker_t<t_log, t_param> : public loop_base {

  template <typename t_work_executer, typename t_provider_executer,
            typename t_breaker_executer>
  void operator()(t_work_executer &p_work_executer,
                  t_provider_executer &p_provider_executer,
                  t_breaker_executer &p_breaker_executer) {
    if (!m_stopped) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return;
    }

    concurrent_info(m_log, "starting");

    m_stopped = false;
    p_work_executer.start();
    p_provider_executer.start();
    p_breaker_executer.start();

    while (true) {

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      std::optional<std::optional<t_param>> _maybe_executed =
          p_provider_executer();

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      if (_maybe_executed) {
        concurrent_debug(m_log, "provider executed");
        std::optional<t_param> _maybe_provided = _maybe_executed.value();

        if (_maybe_provided) {
          auto _provided = _maybe_provided.value();
          concurrent_debug(m_log, "provider provided: ", _provided);

          p_work_executer(_provided);
          if (m_stopped) {
            concurrent_debug(m_log, "stopped!!");
            break;
          }
        }
      }

      concurrent_debug(m_log, "about to call breaker");
      std::optional<bool> _maybe_break = p_breaker_executer();
      if ((_maybe_break) && (_maybe_break.value())) {
        concurrent_debug(m_log, "stopped by breaker");
        m_stopped = true;
        break;
      }
    }
    p_work_executer.stop();
    p_provider_executer.stop();
    p_breaker_executer.stop();
  }

private:
  t_log m_log{"concurrent::loop_with_breaker"};
};

template <typename t_log, typename t_param>
struct loop_with_no_breaker_t<t_log, t_param> : public loop_base {

  template <typename t_work_executer, typename t_provider_executer,
            typename t_breaker_executer>
  void operator()(t_work_executer &p_work_executer,
                  t_provider_executer &p_provider_executer,
                  t_breaker_executer &) {
    if (!m_stopped) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return;
    }

    concurrent_info(m_log, "starting");

    m_stopped = false;
    p_work_executer.start();
    p_provider_executer.start();

    while (true) {

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      std::optional<std::optional<t_param>> _maybe_executed =
          p_provider_executer();

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      if (_maybe_executed) {
        concurrent_debug(m_log, "provider executed");
        std::optional<t_param> _maybe_provided = _maybe_executed.value();

        if (_maybe_provided) {
          auto _provided = _maybe_provided.value();
          concurrent_debug(m_log, "provider provided: ", _provided);

          p_work_executer(_provided);
          if (m_stopped) {
            concurrent_debug(m_log, "stopped!!");
            break;
          }
        }
      }
    }
    p_work_executer.stop();
    p_provider_executer.stop();
  }

private:
  t_log m_log{"concurrent::loop_with_breaker"};
};

// ################ C ########################################################
template <typename t_log> struct loop_with_breaker_t<t_log> : public loop_base {

  template <typename t_work_executer, typename t_provider_executer,
            typename t_breaker_executer>
  void operator()(t_work_executer &p_work_executer, t_provider_executer &,
                  t_breaker_executer &p_breaker_executer) {
    if (!m_stopped) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return;
    }

    concurrent_info(m_log, "starting");

    m_stopped = false;
    p_work_executer.start();
    p_breaker_executer.start();

    while (true) {

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      p_work_executer();
      if (m_stopped) {
        concurrent_debug(m_log, " ---- stopped!!");
        break;
      }

      concurrent_debug(m_log, "about to call breaker");
      std::optional<bool> _maybe_break = p_breaker_executer();
      if ((_maybe_break) && (_maybe_break.value())) {
        concurrent_debug(m_log, "stopped by breaker");
        m_stopped = true;
        break;
      }
    }
    p_work_executer.stop();
    p_breaker_executer.stop();
  }

private:
  t_log m_log{"concurrent::loop_with_breaker"};
};

template <typename t_log>
struct loop_with_no_breaker_t<t_log> : public loop_base {

  template <typename t_work_executer, typename t_provider_executer,
            typename t_breaker_executer>
  void operator()(t_work_executer &p_work_executer, t_provider_executer &,
                  t_breaker_executer &) {
    if (!m_stopped) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return;
    }

    concurrent_info(m_log, "starting");

    m_stopped = false;
    p_work_executer.start();

    while (true) {

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      p_work_executer();
      if (m_stopped) {
        concurrent_debug(m_log, "stopped!!");
        break;
      }
    }
    p_work_executer.stop();
  }

private:
  t_log m_log{"concurrent::loop_with_breaker"};
};

} // namespace concurrent
} // namespace tenacitas

#endif // LOOP_H
