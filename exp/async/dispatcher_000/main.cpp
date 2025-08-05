/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example dispatcher/dispatcher_000/main.cpp

#include <array>
#include <chrono>
#include <condition_variable>
#include <mutex>

#include "tenacitas/exp/async/dispatcher_000/configuration.h"
#include "tenacitas/exp/async/dispatcher_000/event.h"
#include "tenacitas/exp/async/dispatcher_000/event_handled.h"
#include "tenacitas/exp/async/dispatcher_000/handler.h"
#include "tenacitas/exp/async/dispatcher_000/logger.h"
#include "tenacitas/exp/async/dispatcher_000/publisher.h"
#include "tenacitas/exp/async/dispatcher_000/results.h"
#include "tenacitas/src/async/dispatcher.h"
#include "tenacitas/src/container/circular_queue.h"
#include "tenacitas/src/format/fmt.h"

using namespace std::chrono_literals;
using namespace tenacitas;

struct pgm
{
  using event_a       = exp::async::event<'a'>;
  using event_handled = exp::async::event_handled;

  using events_handled = std::tuple<event_a, event_handled>;

  int operator()(int argc, char **argv)
  {
    exp::async::logger _logger;

    try
    {
      if (argc != 2)
      {
        syntax(argv[0]);
        return 1;
      }

      exp::async::configuration _configuration(_logger, argv[1]);

      if (_configuration.amount_events_to_publish == 0)
      {
        TNCT_LOG_TST(_logger, "amount of events to be published = 0");
        return 0;
      }

      if (_configuration.interval_for_events_publishing
          == std::chrono::milliseconds::zero())
      {
        TNCT_LOG_TST(_logger, "interval for publishing events = 0");
        return 0;
      }

      if (!at_least_one_handling_configured(_configuration))
      {
        TNCT_LOG_TST(_logger, "no handling configured");
        return 0;
      }

      TNCT_LOG_TST(_logger, src::format::fmt("\n", _configuration));

      dispatcher _dispatcher(_logger);

      exp::async::results _results;

      const size_t _total_to_be_published{
          _configuration.amount_events_to_publish};
      const size_t _total_to_be_handled{
          define_total_to_be_handled(_configuration)};
      TNCT_LOG_TST(_logger, src::format::fmt("# of events to be handled: ",
                                             _total_to_be_handled));
      size_t       _total_handled{0};
      const size_t _percentage_to_display{
          percentage_to_display(_total_to_be_handled)};

      std::condition_variable _cond_all_handled;
      std::mutex              _mutex_all_handled;

      event_handled_handler _event_handled_handler{_logger,
                                                   _total_handled,
                                                   _results,
                                                   _percentage_to_display,
                                                   _total_to_be_handled,
                                                   _cond_all_handled};

      // _dispatcher.add_handling<event_handled, event_handled_handler>(
      //     "event-handled", std::move(_event_handled_handler),
      //     queue_event_handled{_logger}, 1);

      // async::handling_id _event_handled_id{"event-handled"};

      _dispatcher.add_handling<event_handled>(
          "event-handled", queue_event_handled{_logger},
          std::move(_event_handled_handler),
          src::async::handling_priority::medium, 1);

      publisher _publisher{_dispatcher, _logger,
                           _configuration.interval_for_events_publishing,
                           _total_to_be_published, "pub 1"};

      define_handlings(_dispatcher, _logger, _configuration);

      const auto _start = std::chrono::high_resolution_clock::now();

      _publisher.start();

      {
        std::unique_lock<std::mutex> _lock{_mutex_all_handled};
        _cond_all_handled.wait(_lock);
      }

      const auto _end = std::chrono::high_resolution_clock::now();

      const std::chrono::duration<double> _diff = _end - _start;

      TNCT_LOG_TST(_logger,
                   src::format::fmt("time = ", _diff.count(), " seconds"));

      std::cout << _results.report() << std::endl;
    }
    catch (std::exception &_ex)
    {
      TNCT_LOG_ERR(_logger, _ex.what());
    }

    return 0;
  }

private:
  using logger = exp::async::logger;

  using dispatcher = src::async::dispatcher<logger, event_a, event_handled>;

  using queue_event_handled =
      src::container::circular_queue<logger, event_handled, 5000>;

  using publisher = exp::async::publisher<'a', dispatcher>;

  using queue_event_a = src::container::circular_queue<logger, event_a, 5000>;

  static constexpr size_t num_handlings{5};

  using handler_0 = exp::async::handler<'a', 0, dispatcher>;
  using handler_1 = exp::async::handler<'a', 1, dispatcher>;
  using handler_2 = exp::async::handler<'a', 2, dispatcher>;
  using handler_3 = exp::async::handler<'a', 3, dispatcher>;
  using handler_4 = exp::async::handler<'a', 4, dispatcher>;

  using results = exp::async::results;

  using configuration = exp::async::configuration<logger, num_handlings>;

  struct event_handled_handler
  {
    using events_handled = std::tuple<event_handled>;

    event_handled_handler(exp::async::logger &p_logger, size_t &p_total_handled,
                          exp::async::results     &p_results,
                          size_t                   p_percentage_to_display,
                          size_t                   p_total_to_be_handled,
                          std::condition_variable &p_cond_all_handled)
        : m_logger(p_logger), m_total_handled(p_total_handled),
          m_results(p_results),
          m_percentage_to_display(p_percentage_to_display),
          m_total_to_be_handled(p_total_to_be_handled),
          m_cond_all_handled(p_cond_all_handled)
    {
    }

    void operator()(event_handled &&p_event)
    {
      ++m_total_handled;
      m_results.increment(p_event.event_id, p_event.handling_id,
                          p_event.handler_type_id, p_event.handler_id);
      if ((m_total_handled % m_percentage_to_display) == 0)
      {
        TNCT_LOG_TST(m_logger, src::format::fmt("handled ", m_total_handled,
                                                '/', m_total_to_be_handled));
      }
      if (m_total_handled >= m_total_to_be_handled)
      {
        m_cond_all_handled.notify_all();
      }
    }

  private:
    exp::async::logger      &m_logger;
    size_t                  &m_total_handled;
    exp::async::results     &m_results;
    size_t                   m_percentage_to_display;
    size_t                   m_total_to_be_handled;
    std::condition_variable &m_cond_all_handled;
  };

  void define_handlings(dispatcher &p_dispatcher, exp::async::logger &p_logger,
                        const configuration &p_configuration)
  {
    if (p_configuration.handlings_cfg[0].use)
    {
      p_dispatcher.add_handling<event_a>(
          "event-a-0", queue_event_a{p_logger, "event-a-0"},
          handler_0{p_logger, p_dispatcher, "handling-0",
                    p_configuration.handlings_cfg[0].sleep_to_simulate_work},
          src::async::handling_priority::medium,
          p_configuration.handlings_cfg[0].amount_handlers);
    }

    if (p_configuration.handlings_cfg[1].use)
    {
      p_dispatcher.add_handling<event_a>(
          "event-a-1", queue_event_a{p_logger, "event-a-1"},
          handler_1{p_logger, p_dispatcher, "handling-1",
                    p_configuration.handlings_cfg[1].sleep_to_simulate_work},
          src::async::handling_priority::medium,
          p_configuration.handlings_cfg[1].amount_handlers);
    }

    if (p_configuration.handlings_cfg[2].use)
    {
      p_dispatcher.add_handling<event_a>(
          "event-a-2", queue_event_a{p_logger, "event-a-2"},
          handler_2{p_logger, p_dispatcher, "handling-2",
                    p_configuration.handlings_cfg[2].sleep_to_simulate_work},
          src::async::handling_priority::medium,
          p_configuration.handlings_cfg[2].amount_handlers);
    }

    if (p_configuration.handlings_cfg[3].use)
    {
      p_dispatcher.add_handling<event_a>(
          "event-a-3", queue_event_a{p_logger, "event-a-3"},
          handler_3{p_logger, p_dispatcher, "handling-3",
                    p_configuration.handlings_cfg[3].sleep_to_simulate_work},
          src::async::handling_priority::medium,
          p_configuration.handlings_cfg[3].amount_handlers);
    }

    if (p_configuration.handlings_cfg[4].use)
    {
      p_dispatcher.add_handling<event_a>(
          "event-a-4", queue_event_a{p_logger, "event-a-4"},
          handler_4{p_logger, p_dispatcher, "handling-4",
                    p_configuration.handlings_cfg[4].sleep_to_simulate_work},
          src::async::handling_priority::medium,
          p_configuration.handlings_cfg[4].amount_handlers);
    }
  }

  size_t define_total_to_be_handled(const configuration &p_configuration)
  {
    size_t _total_to_be_handled{0};
    for (const auto &_handling_cfg : p_configuration.handlings_cfg)
    {
      if (_handling_cfg.use)
      {
        _total_to_be_handled += p_configuration.amount_events_to_publish;
      }
    }
    return _total_to_be_handled;
  }

  bool at_least_one_handling_configured(const configuration &p_configuration)
  {
    for (const auto &_handling_cfg : p_configuration.handlings_cfg)
    {
      if (_handling_cfg.use && (_handling_cfg.amount_handlers > 0))
      {
        return true;
      }
    }
    return false;
  }

  void syntax(const char *p_pgm_name)
  {
    std::cout << "Syntax: " << p_pgm_name
              << " <ini-file>"
                 "\n\nWhere <ini-file> must have this structure:\n"
                 "[PUBLISHER]\n"
                 "amount_events_to_publish=<number>\n"
                 "interval_for_events_publishing=<time-in-milliseconds>\n"
                 "\n"
                 "[HANDLING_0]\n"
                 "use=<true/false>\n"
                 "amount_handlers=<number>\n"
                 "sleep_to_simulate_work=<time-in-milliseconds>\n"
                 "\n"
                 "[HANDLING_1]\n"
                 "use=<true/false>\n"
                 "amount_handlers=<number>\n"
                 "sleep_to_simulate_work=<time-in-milliseconds>\n"
                 "\n"
                 "[HANDLING_2]\n"
                 "use=<true/false>\n"
                 "amount_handlers=<number>\n"
                 "sleep_to_simulate_work=<time-in-milliseconds>\n"
                 "\n"
                 "[HANDLING_3]\n"
                 "use=<true/false>\n"
                 "amount_handlers=<number>\n"
                 "sleep_to_simulate_work=<time-in-milliseconds>\n"
                 "\n"
                 "[HANDLING_4]\n"
                 "use=<true/false>\n"
                 "amount_handlers=<number>\n"
                 "sleep_to_simulate_work=<time-in-milliseconds>\n"

              << std::endl;
  }

  std::size_t percentage_to_display(std::size_t p_total_to_be_handled)
  {
    auto _aux{static_cast<size_t>(p_total_to_be_handled * 0.1)};
    return (_aux == 0 ? 1 : _aux);
  }
};

int main(int argc, char **argv)
{
  pgm _pgm;

  _pgm(argc, argv);
}
