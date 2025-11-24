/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TST_CIRCULAR_QUEUE_TEST_H
#define TNCT_CONTAINER_TST_CIRCULAR_QUEUE_TEST_H

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

#include "tnct/container/dat/circular_queue.h"
#include "tnct/format/bus/fmt.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/program/bus/options.h"

namespace tnct::container::tst
{

struct circular_queue_001
{
  static std::string desc()
  {
    return "Adds " + std::to_string(m_amount)
           + " strings of 4k in a queue with " + std::to_string(m_initial_size)
           + " initial size";
  }
  bool operator()(const program::bus::options &)
  {
    log::cerr _logger;
    _logger.set_inf();
    try
    {

      using queue = container::dat::circular_queue<log::cerr, std::string>;

      std::optional<queue> _queue{
          queue::create(_logger, m_initial_size, m_initial_size / 2)};

      if (_queue)
      {
        return false;
      }

      std::string data(4 * 1024, 'z');

      for (uint32_t i = 0; i < m_amount; ++i)
      {
        _queue->push(data);
      }

      uint32_t j{0};
      while (!_queue->empty())
      {
        std::optional<std::string> maybe{_queue->pop()};
        if (!maybe)
        {
          _logger.err(format::bus::fmt("error getting data # ", j));
          return false;
        }
        _logger.tst(format::bus::fmt("capacity = ", _queue->capacity(),
                                     ", occupied = ", _queue->occupied(),
                                     ", data # ", j++));
      }
    }
    catch (std::exception &ex)
    {
      _logger.err(ex.what());
      return false;
    }
    catch (...)
    {
      _logger.err("unknown error");
      return false;
    }

    return true;
  }

private:
  static constexpr uint32_t m_amount{1000000};
  static constexpr size_t   m_initial_size{static_cast<size_t>(m_amount / 10)};
};

struct circular_queue_003
{
  static std::string desc()
  {
    return "Move constructor";
  }

  bool operator()(program::bus::options &)
  {
    using queue = container::dat::circular_queue<log::cerr, int32_t>;
    log::cerr            _logger;
    std::optional<queue> _queue_1(queue::create(_logger, 100, 50));

    queue _queue_2(std::move(*_queue_1));

    return true;
  }
};

// struct circular_queue_test
// {

//   static std::string desc()
//   {
//     return "Running tests configured in a ini file passed on parameter "
//            "'--test-cfg'. You can choose a specific test define in the .ini
//            " "file by passing its name to '--run' parameter, or just run
//            all " "passing '--run' with no parameter";
//   }

//   bool operator()(program::bus::options &p_options)
//   {
//     bool      _success(true);
//     log::cerr _logger;
//     try
//     {

//       // which .ini file?
//       std::string _ini_file_name(
//           p_options.get_single_param("test-cfg").value_or(""));

//       if (_ini_file_name.empty())
//       {
//         _logger.tst("Parameter 'test-cfg' is not defined");
//         return false;
//       }

//       ini_file _ini_file(_logger);

//       // which tests should run?
//       std::list<ini_file::value> _tests_to_run;
//       {
//         std::optional<std::list<ini_file::value>> _maybe_run(
//             p_options.get_set_param("run"));

//         if (_maybe_run)
//         {
//           _tests_to_run = std::move(_maybe_run.value());
//         }
//         else if (!p_options.get_bool_param("run"))
//         {
//           _logger.tst("Parameter 'run' is not defined");
//           return false;
//         }
//       }

//       // reading sections from ini
//       auto _maybe_sections(_ini_file.read(_ini_file_name));
//       if (!_maybe_sections)
//       {
//         _logger.tst(format::bus::fmt("Failed to read sections from ini file
//         ",
//                                      _ini_file_name));
//         return false;
//       }
//       auto _sections(std::move(_maybe_sections.value()));
//       if (_sections.size() == 0)
//       {
//         _logger.tst(format::bus::fmt("No sections read from ini file ",
//                                      _ini_file_name));
//         return false;
//       }

//       // it is certain that there was a 'run' parameter passed, and if the
//       list
//       // of tests to run is empty it means all the tests should be executed
//       bool _run_all_tests(_tests_to_run.empty());

//       for (const auto &_value_section : _sections)
//       {
//         _logger.tst(format::bus::fmt("############ -> ",
//         _value_section.first));

//         if (!_run_all_tests
//             && (std::find_if(_tests_to_run.begin(), _tests_to_run.end(),
//                              [&](const std::string &p_test_name)
//                              { return p_test_name == _value_section.first;
//                              })
//                 == _tests_to_run.end()))
//         {
//           _logger.tst(format::bus::fmt("Test ", _value_section.first,
//                                        " is not in the parameter 'run'
//                                        list"));
//         }
//         else
//         {
//           steps _steps;

//           if (parse(_value_section.second, _steps, _logger))
//           {
//             if (!run(_value_section.first, _steps, _logger))
//             {
//               _logger.tst(format::bus::fmt(_value_section.first, " FAIL"));
//               _success = false;
//             }
//             else
//             {
//               _logger.tst(format::bus::fmt(_value_section.first, "
//               SUCCESS"));
//             }
//           }
//           else
//           {
//             _logger.err(format::bus::fmt("error parsing section ",
//                                          _value_section.first));
//             _success = false;
//           }
//         }
//         _logger.tst(format::bus::fmt("############ <- ",
//         _value_section.first));
//       }
//     }
//     catch (std::exception *_ex)
//     {
//       _logger.err(_ex->what());
//       _success = false;
//     }
//     return _success;
//   }

// private:
//   using logger = log::cerr;

//   using ini_file = parser::bus::ini_file<logger>;

//   template <typename t_content>
//   struct step_type
//   {
//     enum class action : char
//     {
//       push = 'u',
//       pop  = 'o',
//       none = 'n'
//     };
//     enum class report : char
//     {
//       none  = 'n',
//       full  = 'f',
//       brief = 's'
//     };

//     step_type() = delete;
//     step_type(logger &p_logger) : m_logger(p_logger)
//     {
//     }

//     friend std::ostream &operator<<(std::ostream    &p_out,
//                                     const step_type &p_step)
//     {
//       p_out << "action = " << (p_step.act == action::push ? "push" : "pop")
//             << ", amount = " << p_step.amount
//             << ", capacity expected = " << p_step.capacity_expected
//             << ", occupied expected = " << p_step.occupied_expected
//             << ", head expected = " << p_step.head_expected
//             << ", tail expected = " << p_step.tail_expected;
//       if (p_step.repor == report::none)
//       {
//         p_out << ", report = none";
//       }
//       else if (p_step.repor == report::brief)
//       {
//         p_out << ", report = brief";
//       }
//       else
//       {
//         p_out << ", report = full";
//       }
//       return p_out;
//     }

//     bool parse(const std::string &p_step)
//     {
//       const std::regex _regex(
//           R"(action\s*:\s*(push|pop)\s*amount\s*:\s*(\d*)\s*capacity\s*:\s*(\d*)\s*occupied\s*:\s*(\d*)\s*head\s*:\s*(\d*)\s*tail\s*:\s*(\d*)\s*report\s*:\s*(brief|full|none)\s*contents\s*:\s*(.*))");
//       std::smatch _match;

//       if (!std::regex_match(p_step, _match, _regex))
//       {
//         return false;
//       }
//       auto _num_matches(_match.size());
//       if (_num_matches != 9)
//       {
//         m_logger.tst(format::bus::fmt("# matches in step ", p_step,
//                                       " should be 9, but it is ",
//                                       _num_matches));
//         return false;
//       }
//       act    = (_match[1].str() == "push" ? action::push : action::pop);
//       amount = std::stoul(_match[2].str());
//       capacity_expected = std::stoul(_match[3].str());
//       occupied_expected = std::stoul(_match[4].str());
//       head_expected     = std::stoul(_match[5].str());
//       tail_expected     = std::stoul(_match[6].str());
//       std::string _report(_match[7].str());
//       if (_report == "full")
//       {
//         repor = report::full;
//       }
//       else if (_report == "brief")
//       {
//         repor = report::brief;
//       }
//       std::string _contents(_match[8].str());

//       const std::regex _regex_contents(R"((-?\d+))");

//       std::string::const_iterator _search_start(_contents.cbegin());
//       std::smatch                 _match_contents;
//       while (std::regex_search(_search_start, _contents.cend(),
//       _match_contents,
//                                _regex_contents))
//       {
//         contents.push_back(std::stol(_match_contents[0]));
//         _search_start =
//             _match_contents.suffix().first; // Update the search position
//       }

//       if (contents.size() != capacity_expected)
//       {
//         m_logger.err(format::bus::fmt("capacity expected = ",
//         capacity_expected,
//                                       ", but ", contents.size(),
//                                       " items were found"));
//         return false;
//       }

//       return true;
//     }

//     action                 act{action::none};
//     size_t                 amount{0};
//     size_t                 capacity_expected{0};
//     size_t                 occupied_expected{0};
//     size_t                 head_expected{0};
//     size_t                 tail_expected{0};
//     report                 repor{report::none};
//     std::vector<t_content> contents;

//   private:
//     logger &m_logger;
//   };

//   using queue = container::dat::circular_queue<logger, int32_t, 8>;

//   using step = step_type<queue::data>;

//   using steps = std::vector<step>;

// private:
//   bool run(std::string_view /*p_queue_id*/, const steps &p_steps,
//            logger &p_logger)
//   {

//     queue _queue(p_logger);

//     queue::data _data(-1);

//     steps::size_type _step_counter(0);
//     for (const auto &_step : p_steps)
//     {
//       if (_step.act == step::action::push)
//       {
//         for (decltype(step::amount) _i = 0; _i < _step.amount; ++_i)
//         {
//           _queue.push(_data--);
//         }
//       }
//       else
//       {
//         for (decltype(step::amount) _i = 0; _i < _step.amount; ++_i)
//         {
//           _queue.pop();
//         }
//       }
//       if (_step.repor == step::report::brief)
//       {
//         p_logger.tst(format::bus::fmt("step ", ++_step_counter, ": ",
//                                       _queue.brief_report()));
//       }
//       else if (_step.repor == step::report::full)
//       {
//         p_logger.tst(format::bus::fmt("step ", ++_step_counter, ": ",
//                                       _queue.full_report()));
//       }
//       else
//       {
//         p_logger.tst(format::bus::fmt("step ", ++_step_counter));
//       }
//       if ((_queue.occupied() != _step.occupied_expected)
//           || (_queue.head() != _step.head_expected)
//           || (_queue.tail() != _step.tail_expected))
//       {
//         p_logger.err(format::bus::fmt(
//             "queue - occupied: expected ", _step.occupied_expected, ", got
//             ", _queue.occupied(), ", head: expected ", _step.head_expected,
//             ", got ", _queue.head(), ", tail: expected ",
//             _step.tail_expected,
//             ", got ", _queue.tail()));
//         return false;
//       }
//       for (decltype(_step.capacity_expected) _i = 0;
//            _i < _step.capacity_expected; ++_i)
//       {
//         if (_step.contents[_i] != _queue[_i])
//         {
//           p_logger.err(format::bus::fmt("content is ", _queue[_i], ", but
//           ",
//                                         _step.contents[_i], " was
//                                         expected"));
//           return false;
//         }
//       }
//     }
//     return true;
//   }

//   bool parse(const ini_file::properties &p_properties, steps &p_steps,
//              logger &p_logger)
//   {

//     for (const auto &_property : p_properties)
//     {
//       auto             _key(_property.first);
//       const std::regex _regex(R"(Step_(\d*))");
//       std::smatch      _base_match;

//       if (std::regex_match(_key, _base_match, _regex))
//       {
//         if (_base_match.size() == 2)
//         {
//           auto _number = _base_match[1].str();
//           step _step(p_logger);
//           if (_step.parse(_property.second))
//           {
//             p_steps.push_back(std::move(_step));
//           }
//           else
//           {
//             p_logger.tst(format::bus::fmt("error parsing step '", _key,
//             '\'')); return false;
//           }
//         }
//       }
//     }
//     return true;
//   }

//   // bool validate_content(const queue &p_queue, queue::data p_data_begin,
//   //                       queue::data p_data_end, size_t p_head_begin,
//   //                       size_t p_head_end, size_t p_tail_begin,
//   //                       size_t p_tail_end, size_t p_occupied_begin,
//   //                       size_t p_occupied_end, size_t p_capacity_begin,
//   //                       size_t p_capacity_end) {
//   //   return true;
//   // }
// };

} // namespace tnct::container::tst
#endif
