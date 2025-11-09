/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_TST_SLEEPING_LOOP_TEST_H
#define TNCT_ASYNC_TST_SLEEPING_LOOP_TEST_H

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include "tnct/async/bus/sleeping_loop.h"
#include "tnct/format/bus/fmt.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/program/bus/options.h"

using namespace std::chrono_literals;
using namespace tnct;

using logger = log::cerr;

namespace tnct::async::tst
{
struct sleeping_loop_000
{

  static const std::string desc()
  {
    return "'async::sleeping_loop' creation test";
  }

  bool operator()(const program::bus::options &)
  {
    using loop = async::sleeping_loop<logger>;
    logger _logger;

    auto _operation = [&]() -> void { _logger.deb("loop1"); };

    loop _loop(_logger, _operation, 1s);

    return true;
  }
};

struct sleeping_loop_001
{

  typedef uint16_t value;

  static const std::string desc()
  {
    std::stringstream _stream;
    _stream << "'async::sleeping_loop' with interval of " << m_interval_secs
            << "s, increments a counter, and just prints."

            << "\nCounter should be " << m_amount;
    return _stream.str();
  }

  bool operator()(const program::bus::options &)
  {
    using loop = async::sleeping_loop<logger>;

    logger _logger;
    _logger.set_deb();
    operation1 _op(&m_cond, _logger);

    loop _loop(
        _logger, [&_op]() { return _op(); },
        std::chrono::seconds(m_interval_secs));

    _loop.start();

    {
      std::unique_lock<std::mutex> _lock(m_mutex);
      m_cond.wait(_lock, [&_op]() -> bool { return _op.counter >= m_amount; });
    }

    _loop.stop();

    if (_op.counter != m_amount)
    {
      _logger.err(format::bus::fmt("counter should be ", m_amount, ", but it is ",
                              _op.counter));
      return false;
    }

    _logger.tst(format::bus::fmt("counter should be ", m_amount,
                            ", and it really is ", _op.counter));

    return true;
  }

private:
  struct operation1
  {
    operation1(std::condition_variable *p_cond, logger &p_logger)
        : m_cond(p_cond), m_logger(p_logger)
    {
      m_logger.set_deb();
    }

    void operator()()
    {
      if (counter < m_amount)
      {
        ++counter;
        m_logger.deb(format::bus::fmt("counter = ", counter));
        std::this_thread::sleep_for(m_sleep);
      }
      else
      {
        m_cond->notify_one();
      }
    }

    value counter{0};

  private:
    std::condition_variable *m_cond;
    logger                  &m_logger;
  };

private:
  std::mutex              m_mutex;
  std::condition_variable m_cond;

  static constexpr value                     m_interval_secs{1};
  static constexpr value                     m_amount{14};
  static constexpr std::chrono::milliseconds m_sleep{200};
};

struct sleeping_loop_002
{
  static std::string desc()
  {
    return "Tests move constructor";
  }

  bool operator()(const program::bus::options &)
  {
    using loop = async::sleeping_loop<logger>;

    logger _logger;
    _logger.set_tra();

    int16_t _counter{0};

    loop _loop1{_logger, [&_counter]() { ++_counter; }, 1s, "loop1"};

    _loop1.start();

    std::this_thread::sleep_for(4s);

    loop _loop2{std::move(_loop1)};

    std::this_thread::sleep_for(6s);

    return true;
  }
};
} // namespace tnct::async::tst
#endif
