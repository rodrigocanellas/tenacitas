/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_PROGRAM_CLOSER_H
#define TENACITAS_SRC_PROGRAM_CLOSER_H

#include <condition_variable>
#include <mutex>

#include "tenacitas/src/async/handling_priority.h"
#include "tenacitas/src/async/result.h"
#include "tenacitas/src/format/fmt.h"

#include "tenacitas/src/program/exit.h"
#include "tenacitas/src/traits/async/dispatcher.h"
#include "tenacitas/src/traits/log/logger.h"
#include "tenacitas/src/traits/tuple/contains_tuple.h"

namespace tenacitas::src::program {

template <src::traits::log::logger t_logger, traits::async::dispatcher t_dispatcher>

requires(traits::contains_tuple<typename t_dispatcher::events,
                                      std::tuple<program::exit>>)

    struct closer {
  using events_subscribed = std::tuple<program::exit>;

  closer(t_logger &p_logger, t_dispatcher &p_dispatcher)
      : m_logger(p_logger), m_dispatcher(p_dispatcher) {
    auto _result(m_dispatcher.template subscribe<closer, program::exit>(
        "exit", *this, async::handling_priority::highest, 1 /*number_handlers*/,
        5 /*queue_size*/));

    if (_result != async::result::OK) {
      m_logger.err(format::fmt(_result));
    }
  }

  void operator()() {
    TNCT_LOG_DEB(m_logger, "waiting");
    std::unique_lock<std::mutex> _lock(m_mutex);
    m_cond.wait(_lock, [&]() { return m_close == true; });
    TNCT_LOG_DEB(m_logger, "done waiting");
  }

  ~closer() {}

  void handle(program::exit &&) {
    TNCT_LOG_DEB(m_logger, "'closer' handling exit");
    m_close.store(true);
    m_cond.notify_one();
  }

private:
  t_logger &m_logger;
  t_dispatcher &m_dispatcher;

  std::mutex m_mutex;
  std::condition_variable m_cond;

  std::atomic_bool m_close{false};
};

} // namespace tenacitas::src::program

#endif
