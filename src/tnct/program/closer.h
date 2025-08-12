/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_PROGRAM_CLOSER_H
#define TNCT_PROGRAM_CLOSER_H

#include <condition_variable>
#include <mutex>

#include "tnct/async/handling_priority.h"
#include "tnct/async/result.h"
#include "tnct/format/fmt.h"

#include "tnct/async/cpt/is_dispatcher.h"
#include "tnct/log/cpt/logger.h"
#include "tnct/program/exit.h"
#include "tnct/tuple/cpt/contains_tuple.h"

namespace tnct::program
{

template <log::cpt::logger t_logger, async::cpt::is_dispatcher t_dispatcher>

requires(tuple::cpt::contains_tuple<typename t_dispatcher::events,
                                    std::tuple<program::exit>>)

struct closer
{
  using events_subscribed = std::tuple<program::exit>;

  closer(t_logger &p_logger, t_dispatcher &p_dispatcher)
      : m_logger(p_logger), m_dispatcher(p_dispatcher)
  {
    auto _result(m_dispatcher.template subscribe<closer, program::exit>(
        "exit", *this, async::handling_priority::highest, 1 /*number_handlers*/,
        5 /*queue_size*/));

    if (_result != async::result::OK)
    {
      m_logger.err(format::fmt(_result));
    }
  }

  void operator()()
  {
    TNCT_LOG_DEB(m_logger, "waiting");
    std::unique_lock<std::mutex> _lock(m_mutex);
    m_cond.wait(_lock, [&]() { return m_close == true; });
    TNCT_LOG_DEB(m_logger, "done waiting");
  }

  ~closer()
  {
  }

  void handle(program::exit &&)
  {
    TNCT_LOG_DEB(m_logger, "'closer' handling exit");
    m_close.store(true);
    m_cond.notify_one();
  }

private:
  t_logger     &m_logger;
  t_dispatcher &m_dispatcher;

  std::mutex              m_mutex;
  std::condition_variable m_cond;

  std::atomic_bool m_close{false};
};

} // namespace tnct::program

#endif
