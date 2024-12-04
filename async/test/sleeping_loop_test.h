/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TEST_ASYNC_SLEPPING_LOOP_H
#define TENACITAS_LIB_TEST_ASYNC_SLEPPING_LOOP_H

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include "tenacitas.lib/async/sleeping_loop.h"
#include "tenacitas.lib/format/fmt.h"
#include "tenacitas.lib/log/cerr.h"
#include "tenacitas.lib/program/options.h"
#include "tenacitas.lib/tester/test.h"

using namespace std::chrono_literals;
using namespace tenacitas::lib;

using logger = log::cerr;

struct sleeping_loop_000 {

  static const std::string desc() {
    return "'async::sleeping_loop' creation test";
  }

  bool operator()(const program::options &) {
    using loop = async::sleeping_loop<logger>;
    logger _logger;

    auto _operation = [&]() -> void { _logger.deb("loop1"); };

    loop _loop(_logger, _operation, 1s);

    return true;
  }
};

struct sleeping_loop_001 {

  typedef uint16_t value;

  static const std::string desc() {
    std::stringstream _stream;
    _stream << "'async::sleeping_loop' with interval of " << m_interval_secs
            << "s, increments a counter, and just prints."

            << "\nCounter should be " << m_amount;
    return _stream.str();
  }

  bool operator()(const program::options &) {
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

    if (_op.counter != m_amount) {
      _logger.err(format::fmt("counter should be ", m_amount, ", but it is ",
                              _op.counter));
      return false;
    }

    _logger.tst(format::fmt("counter should be ", m_amount,
                            ", and it really is ", _op.counter));

    return true;
  }

private:
  struct operation1 {
    operation1(std::condition_variable *p_cond, logger &p_logger)
        : m_cond(p_cond), m_logger(p_logger) {
      m_logger.set_deb();
    }

    void operator()() {
      if (counter < m_amount) {
        ++counter;
        m_logger.deb(format::fmt("counter = ", counter));
        std::this_thread::sleep_for(m_sleep);
      } else {
        m_cond->notify_one();
      }
    }

    value counter{0};

  private:
    std::condition_variable *m_cond;
    logger &m_logger;
  };

private:
  std::mutex m_mutex;
  std::condition_variable m_cond;

  static constexpr value m_interval_secs{1};
  static constexpr value m_amount{14};
  static constexpr std::chrono::milliseconds m_sleep{200};
};

#endif
