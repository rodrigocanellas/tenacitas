#ifndef TENACITAS_PROGRAM_H
#define TENACITAS_PROGRAM_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <functional>
#include <future>
#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <thread>

#include <tenacitas/async.h>
#include <tenacitas/logger.h>
#include <tenacitas/message.h>

using namespace std::chrono_literals;

/// \brief master namespace
namespace tenacitas {

/// \brief manipulates program related issues
namespace program {

/// \brief Class to parse program options
template <bool use = true> struct options {
  /// \brief name of an option
  typedef std::string name;

  /// \brief value of an option
  typedef std::string value;

  // TODO update program::options example
  /// \brief parse parses the options passed to a program
  /// \param p_argc number of options
  /// \param p_argv vector of strings with the options
  /// \param p_mandatory set of options that are mandatory
  ///
  /// \details an option must be preceded with '--', like '--file-name
  ///  abc.txt'.
  /// There are 3 types of parameter:
  /// \p boolean, where the parameter has no value, like '--print-log'
  ///
  /// \p single, where the parameter has a single value, like '--file-name
  /// abc.txt'
  ///
  /// \p set, where the paramter has a set of values, like '--tests { test1
  /// test2 test3 }
  ///
  ///
  /// \code
  ///#include <iomanip>
  ///#include <iostream>
  ///#include <iterator>
  ///#include <map>
  ///#include <sstream>
  ///#include <string>
  ///#include <utility>
  ///#include <vector>
  ///
  ///#include <tenacitas/program.h>
  ///
  ///
  ///  int main() {
  ///    try {
  ///
  ///      using namespace tenacitas::program;
  ///      using namespace std;
  ///
  ///      const char *argv[] = {"pgm-name", "--set_1",    "{",
  ///                             "v0",       "v1",         "}",
  ///                             "--bool_1", "--single_1", "single_value_1"};
  ///      const int argc = 9;
  ///
  ///      options _pgm_options;
  ///
  ///      _pgm_options.parse(argc, (char **)argv, {"bool_1"});
  ///
  ///      cerr << "options: " << _pgm_options << endl;
  ///
  ///      optional<bool> _bool = _pgm_options.get_bool_param("bool_1");
  ///      if (_bool) {
  ///        cerr << "bool param = " << _bool.value() << endl;
  ///      } else {
  ///        cerr << "ERROR! no value for paramenter 'bool_1' was found" <<
  ///        endl;
  ///      }
  ///
  ///      optional<options::value> _single =
  ///        _pgm_options.get_single_param("single_1");
  ///      if (_single) {
  ///        cerr << "single param = " << _single.value() << endl;
  ///      } else {
  ///        cerr << "ERROR! no value for paramenter 'single_1' was found" <<
  ///        endl;
  ///      }
  ///
  ///      optional<list<options::value>> _set =
  ///      _pgm_options.get_set_param("set_1"); if (_set) {
  ///        if (_set.value().size() != 2) {
  ///          cerr << "ERROR! number of values in set parameter should be 2,
  ///          but it "
  ///                  "is "
  ///               << _set.value().size() << endl;
  ///        } else {
  ///          cerr << "set param 'set_1' = ";
  ///          for (options::value _value : _set.value()) {
  ///            cerr << _value << " ";
  ///          }
  ///          cerr << endl;
  ///        }
  ///      } else {
  ///        cerr << "ERROR! no set of values found for parameter 'set_1'" <<
  ///        endl;
  ///      }
  ///    } catch (std::exception &_ex) {
  ///      cerr << "ERROR! '" << _ex.what() << "'" << endl;
  ///    }
  ///
  ///    return 0;
  ///  }
  /// \endcode
  ///
  /// \throws std::runtime_error
  void parse(int p_argc, char **p_argv,
             std::initializer_list<name> &&p_mandatory = {}) {
    int _last = p_argc - 1;
    int _i = 1;
    while (_i <= _last) {
      if (!is_option(p_argv[_i])) {
        throw std::runtime_error("parameter '" + std::string(p_argv[_i]) +
                                 "' should be an option");
      }

      name _name(
          std::string(&(p_argv[_i][2]), &(p_argv[_i][strlen(p_argv[_i])])));

      if (_i == _last) {
        m_booleans.insert(std::move(_name));
        break;
      }

      ++_i;
      if (is_option(p_argv[_i])) {
        m_booleans.insert(std::move(_name));
      } else {

        if (p_argv[_i][0] == '{') {
          _i = parse_set(std::move(_name), _last, p_argv, _i);
        } else {
          std::string _str(&p_argv[_i][0], &p_argv[_i][strlen(p_argv[_i])]);
          m_singles.insert({std::move(_name), std::move(_str)});
          ++_i;
        }
      }
    }

    for (const name &_name : p_mandatory) {
      if ((!get_bool_param(_name)) && (!get_single_param(_name)) &&
          (!get_set_param(_name))) {
        throw std::runtime_error("parameter '" + _name +
                                 "' should have been defined, but it was not");
      }
    }
  }

  /// \brief Retrieves the bool parameter, if possible
  ///
  /// \param p_name is the name of the parameter
  ///
  /// \return {true} if it was possible to retrieve, and \p p_name exists;
  /// {} otherwise
  std::optional<bool> get_bool_param(const name &p_name) const {
    booleans::const_iterator _ite =
        std::find(m_booleans.begin(), m_booleans.end(), p_name);
    if (_ite == m_booleans.end()) {
      return {};
    }
    return {true};
  }

  /// \brief Retrieves a single parameter, if possible
  ///
  /// \param p_name is the name of the parameter
  ///
  /// \return {<some-value>} if \p p_name was found; {} if
  /// not
  std::optional<value> get_single_param(const name &p_name) const {
    singles::const_iterator _ite =
        std::find_if(m_singles.begin(), m_singles.end(),
                     [p_name](const std::pair<name, value> &p_single) -> bool {
                       return p_single.first == p_name;
                     });
    if (_ite == m_singles.end()) {
      return {};
    }
    return {_ite->second};
  }

  /// \brief Retrieves the values of a parameter
  ///
  /// \param p_name is the name of the parameter
  ///
  /// \return {list with the values} if \p p_name was found; {} if not
  std::optional<std::list<value>> get_set_param(const name &p_name) const {
    sets::const_iterator _ite =
        std::find_if(m_sets.begin(), m_sets.end(),
                     [p_name](const std::pair<name, values> &p_set) -> bool {
                       return p_set.first == p_name;
                     });
    if (_ite == m_sets.end()) {
      return {};
    }
    return {_ite->second};
  }

  /// \brief Output operator
  friend std::ostream &operator<<(std::ostream &p_out,
                                  const options &p_options) {
    for (const options::name &_boolean : p_options.m_booleans) {
      p_out << "[" << _boolean << "] ";
    }

    for (options::singles ::const_iterator _ite = p_options.m_singles.begin();
         _ite != p_options.m_singles.end(); ++_ite) {
      p_out << "[" << _ite->first << "," << _ite->second << "] ";
    }

    for (options::sets::const_iterator _ite = p_options.m_sets.begin();
         _ite != p_options.m_sets.end(); ++_ite) {
      p_out << "[" << _ite->first << " { ";
      for (const options::value &_value : _ite->second) {
        p_out << _value << " ";
      }
      p_out << "} ]";
    }

    return p_out;
  }

private:
  /// \brief Checks if a string is the start of an option
  /// An option must be preceded with '--'
  ///
  /// \param p_str is string to be checked
  ///
  /// \return true if it is, false otherwise
  inline bool is_option(const char *p_str) {
    return ((p_str[0] == '-') && (p_str[1] == '-'));
  }

private:
  /// \brief The type enum defines the type of the option
  enum class type : char { single = 's', boolean = 'b', set = 't' };

  /// \brief booleans type for the options that are boolean, i.e., they do not
  /// have value
  typedef std::set<name> booleans;

  /// \brief singles type for the options that have a single value associated
  typedef std::map<name, value> singles;

  /// \brief values type for the list of values used in the paramters that
  /// define a set of values
  typedef std::list<value> values;

  /// \brief map type for the options that have a set of values associated
  typedef std::map<name, values> sets;

private:
  /// \brief Parses a set of options
  ///
  /// \brief p_name is the name of the option
  ///
  /// \brief p_argv string vector with the set of options
  ///
  /// \brief p_index position in \p p_argv where the set of options starts
  int parse_set(name &&p_name, int p_last, char **p_argv, int p_index) {
    std::string _str(&p_argv[p_index][0],
                     &p_argv[p_index][strlen(p_argv[p_index])]);

    values _values;
    if (_str.length() != 1) {
      if (_str[1] == '}') {
        m_sets.insert({std::move(p_name), std::move(_values)});
        ++p_index;
        return p_index;
      }
      _str = std::string(&p_argv[p_index][1],
                         &p_argv[p_index][strlen(p_argv[p_index])]);
      _values.push_back(std::move(_str));
    }

    ++p_index;

    while (p_index <= p_last) {
      int _len = strlen(p_argv[p_index]);
      if (p_argv[p_index][_len - 1] == '}') {
        if (_len > 1) {
          _values.push_back(
              value(&p_argv[p_index][0], &p_argv[p_index][_len - 1]));
        }
        break;
      } else {
        _values.push_back(value(&p_argv[p_index][0], &p_argv[p_index][_len]));
      }
      ++p_index;
    }
    // && (p_argv[p_index][0] != '}')) {
    //    _values.push_back(p_argv[p_index]);
    //  }
    if (p_index > p_last) {
      throw std::runtime_error("option '" + p_name +
                               "' is a set, but '}' was not found");
    }
    m_sets.insert({std::move(p_name), std::move(_values)});
    ++p_index;

    return p_index;
  }

private:
  /// \brief Booleans options
  booleans m_booleans;

  /// \brief Single value options
  singles m_singles;

  /// \brief Sets options
  sets m_sets;
};

/// \brief Entry point for an application
/// This class allows asynchronously execution of a function, which must use
/// tenacitas::async::messenger_t<tenacitas::message::exit_app> to send a
/// tenacitas::message::exit_app or
/// tenacitas::async::messenger_t<tenacitas::message::halt_app> to send a
/// tenacitas::message::halt_app to end the application
struct application {

  /// \brief Default constructor not allowed
  application() = delete;
  /// \brief Copy constructor not allowed
  application(const application &) = delete;
  /// \brief Move constructor not allowed
  application(application &&) = delete;
  /// \brief Copy assignment not allowed
  application &operator=(const application &) = delete;
  /// \brief Move assignment not allowed
  application &operator=(application &&) = delete;
  /// \brief Allocation not allowed
  void *operator new(size_t) = delete;
  /// \brief Deallocation not allowed
  void operator delete(void *) = delete;

  /// \brief Constructor
  ///
  /// \tparam t_time is the type of type used to define how long will the
  /// application will sleep after receive a tenacitas::message::exit_app
  ///
  /// \param p_wait how long will the
  /// application will sleep after receive a tenacitas::message::exit_app
  ///
  /// \param p_function function to be executed asynchronously
  template <typename t_time>
  application(t_time p_wait, std::function<void()> p_function) {
    using namespace std;
    using namespace std::chrono;

    m_wait = (duration_cast<milliseconds>(p_wait));
    async::id _exit_pool_id{"exit_pool"};

    async::add_queue<message::exit_app>(_exit_pool_id, async::priority::lowest,
                                        milliseconds(m_wait.count() + 2000));
    async::add_handler<message::exit_app>(
        _exit_pool_id, [this](const message::exit_app &p_exit_app) -> void {
          on_exit_app(p_exit_app);
        });

    async::id _halt_pool_id{"halt_pool"};
    async::add_queue<message::halt_app>(_halt_pool_id, async::priority::highest,
                                        milliseconds(m_wait.count() + 2000));
    async::add_handler<message::halt_app>(
        _halt_pool_id, [this](const message::halt_app &p_halt_app) -> void {
          on_halt_app(p_halt_app);
        });

    DEB(m_log, "starting application");

    std::thread _thread(p_function);

    {
      DEB(m_log, "waiting...");
      unique_lock<mutex> _lock(m_mutex);
      m_cond.wait(_lock);
    }

    DEB(m_log, "notified");

    // std::this_thread::sleep_for(m_wait);

    _thread.join();
  }

  ~application() = default;

private:
  /// \brief Handler for the tenacitas::message::exit_app
  void on_exit_app(const message::exit_app &) {
    if (m_on_exit_handled) {
      WAR(m_log, "on_exit already handled");
      return;
    }
    DEB(m_log, "on exit");
    m_on_exit_handled = true;
    std::this_thread::sleep_for(m_wait);
    m_cond.notify_one();
  }

  /// \brief Handler for the tenacitas::message::halt_app
  void on_halt_app(const message::halt_app &) {
    DEB(m_log, "on halt");
    m_cond.notify_one();
  }

private:
  /// \brief Controls application ending
  std::condition_variable m_cond;

  /// \brief Controls application ending
  std::mutex m_mutex;

  /// \brief How long will the application will sleep after receive a
  /// tenacitas::message::exit_app
  std::chrono::milliseconds m_wait;

  /// \brief Indicates that a tenacitas::message::exit_app was already handled
  bool m_on_exit_handled{false};

  /// \brief Logger
  logger::cerr<> m_log{"application"};
};

} // namespace program

} // namespace tenacitas

#endif
