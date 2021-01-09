#ifndef TENACITAS_H
#define TENACITAS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \details This single header file (will) contain all the 'tenacitas' code.
/// It was inspiered by https://github.com/nlohmann/json
/// I suggest you use some type of outline functionality of your code editor or
/// IDE

#include <algorithm>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdint>
#include <cstring>
#include <functional>
#include <future>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>

using namespace std::chrono_literals;

/// \brief master namespace
namespace tenacitas {

// ############################################################################
//                                 type
/// \brief type traits
namespace type {

template <typename> struct is_tuple : std::false_type {};

template <typename... T> struct is_tuple<std::tuple<T...>> : std::true_type {};

} // namespace type

// ############################################################################
//                                 number
/// \brief manipulates numbers, like formating
namespace number {

/// \brief maximum number of characters needed to represent a type of
/// number
///
/// <tt>max_str_length<uint16_t>()</tt> is 5, while
/// <tt>max_str_length<uint32_t>()</tt> is 10
///
/// \tparam t_num_type is the type of number
///
template <typename t_num_type> inline uint16_t max_str_length() {
  return (
      static_cast<uint16_t>(std::log10(std::pow(2, sizeof(t_num_type) * 8))) +
      1);
}

/// \brief type of alingnment when formating numbers
enum class align : char { left = 'l', right = 'd' };

/// \brief Formats a number with the maximum characters necessary to represent
/// the number to string, filled with \p p_fill and aligned with \p p_align
///
/// \tparam t_num_type is the type of the number to be formated
///
/// \param p_num is the value of the number to be formated
///
/// \param p_fill is the char used to fill the parts of the string not filled
/// with \p p_num digits
///
/// \param p_align defines if \p p_num will be aligned at left, right, or center
/// of the string
///
template <typename t_num_type>
inline std::string format_000(t_num_type p_num, char p_fill = '0',
                              align p_align = align::right) {
  std::stringstream _stream;
  _stream << (p_align == align::right ? std::right : std::left)
          << std::setfill(p_fill) << std::setw(max_str_length<t_num_type>())
          << p_num;
  return _stream.str();
}

/// \brief Specialization for uint8_t of \p tenacitas::number::format_000
///
/// \param p_num is the value of the number to be formated
///
/// \param p_fill is the char used to fill the parts of the string not filled
/// with \p p_num digits
///
/// \param p_align defines if \p p_num will be aligned at left, right, or center
/// of the string
///
inline std::string format_000(uint8_t p_num, char p_fill = '0',
                              align p_align = align::right) {
  std::stringstream _stream;
  _stream << (p_align == align::right ? std::right : std::left)
          << std::setfill(p_fill) << std::setw(max_str_length<uint8_t>())
          << static_cast<uint16_t>(p_num);
  return _stream.str();
}

/// \brief Specialization for int8_t of \p tenacitas::number::format_000
///
/// \param p_num is the value of the number to be formated
///
/// \param p_fill is the char used to fill the parts of the string not filled
/// with \p p_num digits
///
/// \param p_align defines if \p p_num will be aligned at left, right, or center
/// of the string
///
inline std::string format_000(int8_t p_num, char p_fill = '0',
                              align p_align = align::right) {
  std::stringstream _stream;
  _stream << (p_align == align::right ? std::right : std::left)
          << std::setfill(p_fill) << std::setw(max_str_length<uint8_t>())
          << static_cast<int16_t>(p_num);
  return _stream.str();
}
} // namespace number

// ############################################################################
//                                 program
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
  ///#include <tenacitas/tenacitas.h>
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
      if ((!get_bool_param(_name).first) && (!get_single_param(_name).first) &&
          (!get_set_param(_name).first)) {
        throw std::runtime_error("parameter '" + _name +
                                 "' should have been defined, but it was not");
      }
    }
  }

  std::pair<bool, bool> get_bool_param(const name &p_name) const {
    booleans::const_iterator _ite =
        std::find(m_booleans.begin(), m_booleans.end(), p_name);
    if (_ite == m_booleans.end()) {
      return {false, false};
    }
    return {true, true};
  }

  std::pair<bool, value> get_single_param(const name &p_name) const {
    singles::const_iterator _ite =
        std::find_if(m_singles.begin(), m_singles.end(),
                     [p_name](const std::pair<name, value> &p_single) -> bool {
                       return p_single.first == p_name;
                     });
    if (_ite == m_singles.end()) {
      return {false, value()};
    }
    return {true, _ite->second};
  }

  std::pair<bool, std::list<value>> get_set_param(const name &p_name) const {
    sets::const_iterator _ite =
        std::find_if(m_sets.begin(), m_sets.end(),
                     [p_name](const std::pair<name, values> &p_set) -> bool {
                       return p_set.first == p_name;
                     });
    if (_ite == m_sets.end()) {
      return {false, std::list<options::value>()};
    }
    return {true, _ite->second};
  }

  /// \brief operator <<
  /// \param p_out
  /// \param p_options
  /// \return
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
  booleans m_booleans;
  singles m_singles;
  sets m_sets;
};
} // namespace program

// ############################################################################
//                                 calendar
/// \brief about calendar and time
namespace calendar {

/// \brief functions that return 'now' in different time units
template <bool use = true> struct now {

  /// \brief now in microseconds
  template <typename t_int = uint64_t> inline static t_int microsecs_num() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  }

  /// \brief YYYY-MM-DD HH::MM::SS,999999
  static std::string microsecs_str() {
    using namespace std;
    using namespace chrono;

    const auto _microsecs = microsecs_num();
    const time_t _time_t = static_cast<time_t>(_microsecs / 1000000);

    const auto _remainder = _microsecs % 1000000;
    stringstream _stream;
    _stream << put_time(std::localtime(&_time_t), "%Y-%m-%d %H:%M:%S,")
            << std::setw(6) << std::setfill('0') << std::left << _remainder;
    return _stream.str();
  }

  /// \brief now in milliseconds
  template <typename t_int = uint64_t> inline static t_int millisecs_num() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  }

  /// \brief YYYY-MM-DD HH::MM::SS,999
  // TODO test millisecs_str
  static std::string millisecs_str() {
    using namespace std;
    using namespace chrono;

    const auto _microsecs = millisecs_num();
    const time_t _time_t = static_cast<time_t>(_microsecs / 1000);

    const auto _remainder = _microsecs % 1000;
    stringstream _stream;
    _stream << put_time(std::localtime(&_time_t), "%Y-%m-%d %H:%M:%S,")
            << std::setw(3) << std::setfill('0') << std::left << _remainder;
    return _stream.str();
  }

  /// \brief now in seconds
  template <typename t_int = uint64_t> inline static t_int secs_num() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  }

  /// \brief YYYY-MM-DD HH::MM::SS
  // TODO test secs_str
  static std::string secs_str() {
    using namespace std;
    using namespace chrono;

    const time_t _time_t = secs_num<time_t>();

    stringstream _stream;
    _stream << put_time(std::localtime(&_time_t), "%Y-%m-%d %H:%M:%S");
    return _stream.str();
  }

  /// \brief now in minutes
  // TODO test minutes_num
  template <typename t_int = uint64_t> inline static t_int minutes_num() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::minutes>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  }

  /// \brief now in hours
  // TODO test hours_num
  template <typename t_int = uint64_t> inline static t_int hours_num() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::hours>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  }

  /// \brief now in days
  // TODO test days_num
  template <typename t_int = uint64_t> inline static t_int days_num() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::hours>(
            std::chrono::system_clock::now().time_since_epoch())
            .count() /
        24);
  }

  /// \brief now in months
  // TODO test months_num
  template <typename t_int = uint64_t> inline static t_int months_num() {
    return static_cast<t_int>(days_num() / 30);
  }

  /// \brief now in years
  // TODO test years_num
  template <typename t_int = uint64_t> inline static t_int years_num() {
    return static_cast<t_int>(months_num() / 12);
  }
};
} // namespace calendar

// ############################################################################
//                                 tester
/// \brief helps creates tests
namespace tester {

/// \brief Runs a test
///
/// \param tester is an instance of tenacitas::tester::test defined below
///
/// \param test is the name of a class that implements
///
/// \code
/// bool operator()()
///
/// static std::string desc()
/// \endcode
///
#define run_test(tester, test) tester.run<test>(#test)

/// \brief The test struct executes tests implemented in classes
///
/// \code
/// #include <iostream>
/// #include <string>
///
/// #include <tester/test.h>
///
/// using namespace tenacitas;
///
/// struct test_ok {
///   bool operator()() { return true; }
///
///   static std::string desc() { return "an ok test"; }
/// };
///
/// struct test_fail {
///   bool operator()() { return true; }
///   static std::string desc() { return "a fail test"; }
/// };
///
/// struct test_error {
///  bool operator()() {
///    try {
///      throw std::runtime_error("test function raised an exception");
///      return false;
///    } catch (const std::exception &_ex) {
///      std::cerr << "'test_error' raised '" << _ex.what() << "'" << std::endl;
///      return true;
///    }
///  }
///  static std::string desc() { return "an eror test"; }
///};
///
/// int main(int argc, char **argv) {
///  try {
///    tester::test _tester(argc, argv);
///
///    run_test(_tester, test_ok);
///    run_test(_tester, test_fail);
///    run_test(_tester, test_error);
///
///  } catch (std::exception &_ex) {
///    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
///  }
///}
///
/// \endcode
template <bool use = true> struct test {

  /// \brief test constructor
  ///
  /// \param argc number of strings in \p argv
  /// \param argv parameters passed to the program
  ///
  /// \details
  /// If '--desc' is passed, \p operator() will print a description of the
  /// tests.
  /// If '--exec' is passed, \p operator() will execute the tests
  /// If '--exec { <test-name-1> <test-name-2> ... }' is passed, \p operator()
  /// will execute the tests will execute tests  defined between '{' and '}'
  ///
  /// \details the user prints should use \p cerr
  ///
  /// \throw std::runtime_error
  test(int argc, char **argv) : m_argc(argc), m_argv(argv) {
    m_pgm_name = m_argv[0];

    try {

      program::options _options;

      _options.parse(m_argc, m_argv);
      if (_options.get_bool_param("exec").first) {
        m_execute_tests = true;
      } else if (_options.get_bool_param("desc").first) {
        m_print_desc = true;
      } else {

        std::pair<bool, std::list<program::options<>::value>> _tests_to_exec =
            _options.get_set_param("exec");
        if (_tests_to_exec.first) {
          m_execute_tests = true;
          m_tests_to_exec.insert(_tests_to_exec.second.begin(),
                                 _tests_to_exec.second.end());
        }
      }

      if ((!m_execute_tests) && (!m_print_desc)) {
        print_mini_howto();
      }
    } catch (std::exception &_ex) {
      std::cout << "EXCEPTION '" << _ex.what() << "'" << std::endl;
      return;
    }
  }

  test() = delete;
  test(const test &) = delete;
  test(test &&) = delete;

  test &operator=(const test &) = delete;
  test &operator=(test &&) = delete;

  /// \brief run executes the test
  ///
  /// \tparam t_test_class must implement:
  /// \code
  /// bool operator()()
  ///
  /// static std::string desc()
  /// \endcode
  ///
  /// \details You can use the macro 'run_test' defined above, instead of
  /// calling this method
  ///
  /// \details user should use <tt>std::cerr</tt> to print specific test
  /// messages
  ///
  /// \details if the test passes, the message "SUCCESS for <name>" will be
  /// printed; if the test does not pass, the message "FAIL for <name>" will be
  /// printed; if an error occurr while executing the test , the messae "ERROR
  /// for <name> <desc>" will be printed; if an exception occurrs, the message
  /// 'EXCEPTION <description>' will be displayed
  template <typename t_test_class> void run(const std::string p_test_name) {
    using namespace std;

    if (m_print_desc) {
      cout << p_test_name << ": " << t_test_class::desc() << "\n" << endl;
      return;
    }

    if (m_execute_tests) {
      if (!m_tests_to_exec.empty()) {
        if ((std::find(m_tests_to_exec.begin(), m_tests_to_exec.end(),
                       p_test_name)) != m_tests_to_exec.end()) {
          exec<t_test_class>(p_test_name);
        }
      } else {
        exec<t_test_class>(p_test_name);
      }
    }
  }

private:
  template <typename t_test_class> void exec(const std::string p_test_name) {
    using namespace std;
    bool result = false;
    try {
      cerr << "\n############ -> " << p_test_name << " - "
           << t_test_class::desc() << endl;
      result = t_test_class()();
      cout << (result ? "SUCCESS" : "FAIL") << " for " << p_test_name << endl;
    } catch (exception &_ex) {
      cout << "ERROR for " << p_test_name << " '" << _ex.what() << "'" << endl;
    }
    cerr << "############ <- " << p_test_name << endl;
  }

  /// \brief print_mini_howto prints a mini how-to for using the \p test class
  void print_mini_howto() {
    using namespace std;
    cout
        << "Syntax:\n"
        << "\t'" << m_pgm_name
        << " --desc' will display a description of the test\n"
        << "\t'" << m_pgm_name << " --exec' will execute the all the tests\n"
        << "\t'" << m_pgm_name
        << " --exec { <test-name-1> <test-name-2> ...}' will execute tests "
           "defined between '{' and '}'\n"
        << "\t'" << m_pgm_name << "' displays this message\n\n"
        << "For the programmers: \n"
        << "\t1 - Programmers should use 'std::cerr' to print messages\n"
        << "\t2 - If do not want your 'std::cerr' messages to be displayed, "
           "use\n"
        << "\t'" << m_pgm_name
        << " --exec 2> /dev/null' to execute the tests\n\n"
        << "Output:\n"
        << "\tIf the test passes, the message \"SUCCESS for <name>\" will be "
           "printed\n"
        << "\tIf the test does not pass, the message \"FAIL for <name>\" will "
           "be "
           "printed\n"
        << "\tIf an error occurr while executing the test , the message "
           "\"ERROR "
           "for <name> <desc>\" will be printed\n"
        << "\tIf an exception occurrs, the message \"EXCEPTION <description>\" "
           "will be printed"
        << endl;
  }

private:
  /// \brief name of the test program
  std::string m_pgm_name;

  /// \brief indicates if the tests should actually be executed, or if theirs
  /// description should be printed
  bool m_execute_tests = {false};

  /// prints test decription to \p cout
  bool m_print_desc = {false};

  /// \brief number of parameters passed to the \p test object
  int m_argc = {-1};

  /// \brief parameters passed to the \p test object
  char **m_argv = {nullptr};

  std::set<std::string> m_tests_to_exec;
};

} // namespace tester

// ############################################################################
//                                 logger
/// \brief logger classes
namespace logger {

/// \brief The level enum defines the possible logger::log levels
enum class level : int8_t {
  no_log = -1,
  debug = 0,
  info = 1,
  warn = 2,
  error = 3,
  fatal = 4
};

inline std::ostream &operator<<(std::ostream &p_out, level p_level) {

  switch (p_level) {
  case level::debug:
    p_out << "DEB";
    break;
  case level::info:
    p_out << "INF";
    break;
  case level::warn:
    p_out << "WAR";
    break;
  case level::error:
    p_out << "ERR";
    break;
  case level::fatal:
    p_out << "FAT";
    break;
  default:
    p_out << "-N-";
  }
  return p_out;
}

level global_level{level::warn};

void set_debug_level() { global_level = level::debug; }
void set_info_level() { global_level = level::info; }
void set_warn_level() { global_level = level::warn; }

bool can_log(level p_level) { return p_level >= global_level; }

/// \brief log is a class that guarantees thread safe writing to the log writer
///
/// \tparam t_specific_logger is the class that will implement a concrete log
///
template <bool use = true> class log {
public:
  /// \brief function responsible for actually writing the log message to a
  /// writer
  typedef std::function<void(std::string &&)> writer;

public:
  log() = delete;

  log(const log &p_log) : m_class(p_log.m_class) {}

  log(log &&p_log) noexcept : m_class(std::move(p_log.m_class)) {}

  log &operator=(const log &p_log) {
    if (this != &p_log) {
      m_class = p_log.m_class;
    }
    return *this;
  }

  log &operator=(log &&p_log) noexcept {
    if (this != &p_log) {
      m_class = std::move(p_log.m_class);
    }
    return *this;
  }

  virtual ~log() = default;

  /// \brief Constructor
  /// \param p_writer the actual writer. Please, read the \p log class
  /// documentation above to be aware of the methods \p t_writer should
  /// implement
  inline log(writer &&p_writer)
      : m_writer(std::move(p_writer)), m_is_writer_set(true) {}

  /// \brief sets 'this' log level for level::debug
  inline void set_debug_level() { m_level = level::debug; }

  /// \brief sets 'this' log level for level::info
  inline void set_info_level() { m_level = level::info; }

  /// \brief sets 'this' log level for level::warn
  inline void set_warn_level() { m_level = level::warn; }

  /// \brief set_timestamp_as_number makes the timestamp to be printed as a
  /// number, instead of a string
  inline static void set_timestamp_as_number(bool p_value = true) {
    m_timestamp_as_number = p_value;
  }

  /// \brief set_separator defines the separator to be used in the log
  /// messages
  ///
  /// \param p_separator the value of the separator
  inline void set_separator(char p_separator) { m_separator = p_separator; }

  /// \brief get_separator retrieves the separator used in the log messages
  ///
  /// \return the value of the separator
  inline char get_separator() const { return m_separator; }

  /// \brief logs message with \p debug severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message will only be printed if the current log level
  /// is \p level::debug
  template <typename t_line_num, typename... t_params>
  inline void debug(t_line_num p_line, const t_params &... p_params) {
    write(level::debug, p_line, p_params...);
  }

  /// \brief logs message with \p info severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message will only be printed if the current log level
  /// is at least \p level::info
  template <typename t_line_num, typename... t_params>
  inline void info(t_line_num p_line, const t_params &... p_params) {
    write(level::info, p_line, p_params...);
  }

  /// \brief logs message with \p warn severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message will only be printed if the current log level
  /// is at least \p level::warn
  template <typename t_line_num, typename... t_params>
  inline void warn(t_line_num p_line, const t_params &... p_params) {
    write(level::warn, p_line, p_params...);
  }

  /// \brief logs message with \p error severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message with this severity will always be printed
  template <typename t_line_num, typename... t_params>
  inline void error(t_line_num p_line, const t_params &... p_params) {
    write(level::error, p_line, p_params...);
  }

  /// \brief logs message with \p fatal severity
  ///
  /// \tparam t_params are the types of the values to be logged
  ///
  /// \attention each parameter must implement the \code std::ostream &
  /// operator<<(const t &)\endcode operator, where \p t is the type of the
  /// parameter
  ///
  /// \details the log message with this severity will always be printed
  template <typename t_line_num, typename... t_params>
  inline void fatal(t_line_num p_line, const t_params &... p_params) {
    write(level::fatal, p_line, p_params...);
  }

protected:
  log(std::string &&p_class, writer p_writer)
      : m_class(std::move(p_class)), m_writer(p_writer),
        m_level(logger::level::warn) {}

  log(const char *p_class, writer p_writer)
      : m_class(p_class), m_writer(p_writer), m_level(logger::level::warn) {}

private:
  /// \brief write will actually write the message
  ///
  /// tparam t_params are the types of the values to be logged, and each
  /// parameter must implement the \code std::ostream & operator<<(const t
  /// &)\endcode operator, where \p t is the type of the parameter
  ///
  /// \param p_level is the severity level of the message
  ///
  /// \param p_params are the values to be logged
  template <typename... t_params>
  void write(level p_level, uint32_t p_line, const t_params &... p_params) {

    if (can_log(p_level)) {
      std::ostringstream _stream;
      _stream << p_level << m_separator
              << (m_timestamp_as_number
                      ? std::to_string(calendar::now<>::microsecs_num())
                      : calendar::now<>::microsecs_str())
              << m_separator << std::this_thread::get_id() << m_separator
              << m_class << m_separator << number::format_000(p_line);
      format(_stream, m_separator, p_params...);
      _stream << std::endl;
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_writer(_stream.str());
    }
  }

  /// \brief can_log informs if a message with a certain log level can be
  /// printed, considering the current log level
  ///
  /// \param p_level is the value of the level to be compared to the current
  /// log level
  ///
  /// \return \p true if the messsage can be logged, \p false otherwise
  ///
  bool can_log(level p_level) const {
    if (m_level == level::no_log) {
      return false;
    }
    if ((p_level == level::error) || (p_level == level::fatal)) {
      return true;
    }
    return ((p_level >= m_level) || (logger::can_log(p_level)));
  }

  /// \brief format compile time recursion to solve the variadic template
  /// parameter
  template <typename t, typename... ts>
  inline void format(std::ostringstream &p_stream, const t &p_t,
                     const ts &... p_ts) const {
    format(p_stream, p_t);
    format(p_stream, p_ts...);
  }

  /// \brief end of compile time recursion to solve the variadic template
  /// parameter
  template <typename t>
  inline void format(std::ostringstream &p_stream, const t &p_t) const {
    if constexpr (type::is_tuple<t>::value) {
      p_stream << to_str(p_t);
    } else {
      p_stream << p_t;
    }
  }

  inline void format(std::ostringstream &p_stream, const double &p_t) const {
    p_stream << std::setprecision(std::numeric_limits<double>::max_digits10)
             << p_t;
  }

  inline void format(std::ostringstream &p_stream, const float &p_t) const {
    p_stream << std::setprecision(std::numeric_limits<float>::max_digits10)
             << p_t;
  }

  inline void format(std::ostringstream &p_stream, const bool &p_t) const {
    p_stream << (p_t ? "true" : "false");
  }

  /// \brief copies a tuple fields into a string
  template <class TupType, size_t... I>
  std::string to_str(const TupType &_tup, std::index_sequence<I...>) const {
    std::stringstream _stream;
    _stream << "(";
    (..., (_stream << (I == 0 ? "" : ", ") << std::get<I>(_tup)));
    _stream << ")";
    return _stream.str();
  }

  /// \brief copies one tuple field into a string
  template <class... T> std::string to_str(const std::tuple<T...> &_tup) const {
    return to_str(_tup, std::make_index_sequence<sizeof...(T)>());
  }

private:
  std::string m_class = {"no-class"};

  /// \brief m_writer is the writer where the messages will be logged.
  ///
  /// Please, read the \p log class documentation above to be aware of the
  /// methods \p t_writer should implement
  writer m_writer = {[](std::string &&) {
    std::cerr << "NO WRITER CONFIGURED FOR LOGGING" << std::endl;
  }};

  /// \brief m_level is the current log level
  level m_level{logger::level::warn};

  /// \brief m_mutex allows a thread safe writing to the log writer
  std::mutex m_mutex;

  /// \brief m_separator is used to separate parts of the log message
  char m_separator{'|'};

  bool m_is_writer_set{false};

  static bool m_timestamp_as_number;
};

template <bool use> bool log<use>::m_timestamp_as_number{false};

/// \brief logs message to \p std::cerr
template <bool use = true> struct cerr : public log<use> {
  inline explicit cerr(std::string &&p_class)
      : log<use>(std::move(p_class),
                 [](std::string &&p_str) -> void { std::cerr << p_str; }) {}

  inline explicit cerr(const char *p_class)
      : log<use>(p_class,
                 [](std::string &&p_str) -> void { std::cerr << p_str; }) {}
};

/// \brief logs message to \p std::cout
template <bool use = true> struct cout : public log<use> {
  inline explicit cout(std::string &&p_class)
      : log<use>(std::move(p_class),
                 [](std::string &&p_str) -> void { std::cout << p_str; }) {}
  inline explicit cout(const char *p_class)
      : log<use>(p_class,
                 [](std::string &&p_str) -> void { std::cout << p_str; }) {}
};

/// \brief The log struct logs message to \p std::clog
template <bool use = true> struct clog : public log<use> {
  inline explicit clog(std::string &&p_class)
      : log<use>(std::move(p_class),
                 [](std::string &&p_str) -> void { std::clog << p_str; }) {}
  inline explicit clog(const char *p_class)
      : log<use>(p_class,
                 [](std::string &&p_str) -> void { std::clog << p_str; }) {}
};

/// \brief macros to make it easier to log

#ifdef TENACITAS_LOG

/// \brief Wraps to the debug log function
#define DEB(p_log, p_params...) p_log.debug(__LINE__, p_params)

/// \brief Wraps to the info log function
#define INF(p_log, p_params...) p_log.info(__LINE__, p_params)

/// \brief Wraps to the warn log function
#define WAR(p_log, p_params...) p_log.warn(__LINE__, p_params)

/// \brief Wraps to the error log function
#define ERR(p_log, p_params...) p_log.error(__LINE__, p_params)

/// \brief Wraps to the fatal log function
#define FAT(p_log, p_params...) p_log.fatal(__LINE__, p_params)

#else

/// \brief Wraps to the debug log function
#define DEB(p_log, p_params...)

/// \brief Wraps to the info log function
#define INF(p_log, p_params...)

/// \brief Wraps to the warn log function
#define WARN(p_log, p_params...)

/// \brief Wraps to the error log function
#define ERR(p_log, p_params...)

/// \brief Wraps to the fatal log function
#define FAT(p_log, p_params...)

#endif

} // namespace logger

// ############################################################################
//                                 concurrent
/// \brief support for concurrent (asynchronous) programming
namespace concurrent {

/// \brief Type of time used to define timeout
typedef std::chrono::microseconds timeout;

/// \brief Type of time used to define interval
typedef std::chrono::microseconds interval;

/// \brief Converts any type of time defined in std::chrono to \p timeout
template <typename t_time> inline timeout to_timeout(t_time p_time) {
  return std::chrono::duration_cast<timeout>(p_time);
}

/// \brief Converts any type of time defined in std::chrono to \p interval
template <typename t_time> inline interval to_interval(t_time p_time) {
  return std::chrono::duration_cast<interval>(p_time);
}

/// \brief The thread class is a wrapper for the std::thread, which joins in
/// destructor and move assignment
template <bool use = true> class thread {
public:
  /// \brief thread constructor
  ///
  /// \param p_func function that will run in a separated thread
  inline explicit thread(std::function<void()> &&p_func)
      : m_thread(std::move(p_func)) {}

  inline thread() = default;
  thread(const thread &) = delete;
  inline thread(thread &&) noexcept = default;
  thread &operator=(const thread &) = delete;

  ///
  /// \brief operator = move joins the \p this thread, if the thread is still
  /// running
  ///
  /// \param p_th thread to be move to \p this
  ///
  /// \return the new thread
  inline thread &operator=(thread &&p_th) noexcept {
    join();
    m_thread = std::move(p_th.m_thread);
    return *this;
  }

  inline friend std::ostream &operator<<(std::ostream &out,
                                         const thread &p_thread) {
    out << "(" << &p_thread << ", " << p_thread.m_thread.get_id() << ")";
    return out;
  }

  /// \brief join waits for the thread to finish
  inline void join() {
    if (m_thread.joinable()) {
      m_thread.join();
    }
  }

  inline void detach() { m_thread.detach(); }

  inline std::thread::id get_id() const { return m_thread.get_id(); }

  /// \brief ~thread joins if the thread is still running
  inline ~thread() { join(); }

  inline bool joinable() const { return m_thread.joinable(); }

private:
  /// \brief m_thread the wrappered std::thread
  std::thread m_thread;
};

/// \brief Type of function executed when a function times out
typedef std::function<void(std::thread::id)> timeout_callback;

/// \brief Base class to allow a function to be executed, with timeout control,
/// but not starting a thread for each execution
template <typename t_log> struct executer_base_t {

  virtual ~executer_base_t() {
    DEB(m_log, "entering destructor");
    stop();
    DEB(m_log, "leaving destructor");
  }

protected:
  template <typename t_time>
  executer_base_t(std::function<void()> p_function, t_time p_timeout,
                  timeout_callback p_timeout_callback)
      : m_function(p_function), m_timeout(to_timeout(p_timeout)),
        m_timeout_callback(p_timeout_callback) {
    DEB(m_log, "calling start from constructor");
    start();
  }

  /// \brief Stars the single thread that will execute the function
  /// asynchronously
  void start() {
    if (m_stopped) {
      DEB(m_log, "starting");
      m_stopped = false;
      DEB(m_log, "about to push a new 'loop'");
      m_loops.push_front({this});

      m_loop = m_loops.begin();
      m_loop->start();
      DEB(m_log, "new 'loop' pushed, ", &(*m_loop));
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  }

  /// \brief Stops the execution of the single created thread for the execution
  /// of the function asynchronously
  void stop() {
    if (!m_stopped) {
      DEB(this->m_log, "m_stopped = true");
      m_stopped = true;
      DEB(this->m_log, "m_cond_exec.notify_one()");
      m_cond_exec.notify_all();
      DEB(this->m_log, "m_cond_wait.notify_one()");
      m_cond_wait.notify_one();
    } else {
      DEB(this->m_log, "not stopping because it was already stopped");
    }
  }

  inline void set_log_debug_level() { m_log.set_debug_level(); }
  inline void set_log_info_level() { m_log.set_info_level(); }
  inline void set_log_level() { m_log.set_warn_level(); }

  template <typename t_result>
  t_result call(std::function<t_result()> p_ok,
                std::function<t_result()> p_not_ok,
                std::function<void()> p_save_params) {
    std::lock_guard<std::mutex> _lock_function(m_mutex_function);
    DEB(m_log, "operator()()");

    if (m_stopped) {
      ERR(m_log, "executer is stopped; call 'start()' first");
      return p_not_ok();
    }

    p_save_params();

    if (m_stopped) {
      DEB(m_log, "stopped");
      return p_not_ok();
    }

    DEB(m_log, "notifying there is work to be done");
    m_cond_exec.notify_all();

    DEB(m_log, "waiting ", m_timeout.count(), " microsecs for work");
    {
      std::unique_lock<std::mutex> _lock(m_mutex_wait);
      if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
        WAR(m_log, "timeout!");
        timeout_callback_thread(m_loop->id());
        m_loop->abandon();
        m_stopped = true;
        start();

        return p_not_ok();
      }
    }

    DEB(m_log, "work was done or 'executer' was stopped");
    if (m_stopped) {
      DEB(m_log, "stopped");
      return p_not_ok();
    }

    DEB(m_log, "function done");
    return p_ok();
  }

private:
  /// \brief Loop that will be executed asyncrhonously
  struct loop {
    loop() = default;
    loop(executer_base_t<t_log> *p_owner) : m_owner(p_owner) {
      //      m_log.set_DEB();
      DEB(m_log, "m_owner = ", m_owner);
    }
    loop(const loop &) = delete;
    loop(loop &&p_loop) noexcept : m_owner(p_loop.m_owner) {
      DEB(m_log, "move constructor m_owner = ", m_owner);
    }

    loop &operator=(const loop &) = delete;
    loop &operator=(loop &&p_loop) = delete;

    inline std::thread::id id() const { return m_thread.get_id(); }

    inline ~loop() {
      DEB(m_log, "destructor before, m_owner = ", m_owner);
      if (m_owner == nullptr) {
        DEB(m_log, "m_owner is null, leaving destructor");
        return;
      }

      if (m_thread.joinable()) {
        DEB(m_log, "joining");
        m_owner->m_cond_exec.notify_one();
        m_thread.join();
      }
      m_owner = nullptr;
      DEB(m_log, "destructor after, m_owner = ", m_owner);
    }

    inline void abandon() {
      m_thread.detach();
      DEB(m_log, "abandoning, m_owner = ", m_owner);
      m_owner = nullptr;
    }

    void start() {
      m_thread = concurrent::thread([this]() -> void { (*this)(); });
    }

    void operator()() {
      while (true) {

        if (!m_owner) {
          DEB(m_log, "owner is null");
          break;
        }

        if ((m_owner) && (m_owner->m_stopped)) {
          DEB(m_log, "stopped ");
          break;
        }

        if (m_owner) {
          std::unique_lock<std::mutex> _lock(m_owner->m_mutex_exec);
          if (m_owner) {
            DEB(m_log, "waiting for work");
            m_owner->m_cond_exec.wait(_lock);
            DEB(m_log, "not waiting for work anymore");
          }
        }

        if ((m_owner) && (m_owner->m_stopped)) {
          DEB(m_log, "stopped");
          break;
        }

        if (m_owner) {
          DEB(m_log, "executing");
          m_owner->m_function();
        }

        if ((m_owner) && (m_owner->m_stopped)) {
          DEB(m_log, "stopped");
          break;
        }

        if (m_owner) {
          DEB(m_log, "notifying");
          m_owner->m_cond_wait.notify_one();
        }
      }

      DEB(m_log, "leaving the loop");
    }

  private:
    executer_base_t<t_log> *m_owner{nullptr};

    concurrent::thread<> m_thread;

    t_log m_log{"concurrent::executer::loop"};
  };

private:
  /// \brief Thread to execute the function that is called if the \p function
  /// times out
  void timeout_callback_thread(std::thread::id p_id) {
    m_timeout_thread = concurrent::thread(
        [this, p_id]() -> void { m_timeout_callback(p_id); });
    m_timeout_thread.detach();
  }

private:
  std::function<void()> m_function;

  timeout m_timeout;

  timeout_callback m_timeout_callback;

  /// \brief Logger object
  t_log m_log{"concurrent::executer"};

  /// \brief The loop initiates stopped
  bool m_stopped{true};

  /// \brief Controls the execution of the execution loop
  std::mutex m_mutex_function;

  /// \brief Controls the execution of the execution loop
  std::condition_variable m_cond_exec;

  /// \brief Controls the waiting of the execution loop
  std::mutex m_mutex_wait;

  /// \brief Controls the waiting of the execution loop
  std::condition_variable m_cond_wait;

  /// \brief Controls the execution of the thread
  std::mutex m_mutex_exec;

  concurrent::thread<> m_timeout_thread;

  std::list<loop> m_loops;

  typename std::list<loop>::iterator m_loop;

  /// \brief Single thread used to execute the \p function over and over
  /// asynchronously
  concurrent::thread<> m_thread;
};

// ########## 1 ##########
template <typename t_log, typename t_result, typename... t_params>
struct executer_t;

// ########## 2 ##########
template <typename t_log, typename t_result>
struct executer_t<t_log, t_result, void>;

// ########## 3 ##########
template <typename t_log, typename... t_params>
struct executer_t<t_log, void, t_params...>;

// ########## 4 ##########
template <typename t_log> struct executer_t<t_log, void, void>;

// ########## 1 ##########
template <typename t_log, typename t_result, typename... t_params>
struct executer_t : public executer_base_t<t_log> {

  typedef std::function<t_result(t_params...)> worker;

  template <typename t_time>
  executer_t(worker p_worker, t_time p_timeout,
             timeout_callback p_timeout_callback)
      : executer_base_t<t_log>(
            [this, p_worker]() -> void {
              m_result = std::apply(p_worker, std::move(m_params));
            },
            p_timeout, p_timeout_callback) {
    this->start();
  }

  std::optional<t_result> operator()(t_params... p_params) {
    auto ok = [this]() -> std::optional<t_result> { return m_result; };
    auto not_ok = []() -> std::optional<t_result> { return {}; };
    auto save_params = [this, p_params...]() -> void {
      m_params = {p_params...};
    };

    return this->template call<std::optional<t_result>>(ok, not_ok,
                                                        save_params);
  }

private:
  std::tuple<t_params...> m_params;

  t_result m_result;
};

// ########## 2 ##########
template <typename t_log, typename t_result>
struct executer_t<t_log, t_result, void> : public executer_base_t<t_log> {

  typedef std::function<t_result()> worker;

  template <typename t_time>
  executer_t(worker p_worker, t_time p_timeout,
             timeout_callback p_timeout_callback)
      : executer_base_t<t_log>(
            [this, p_worker]() -> void { m_result = p_worker(); }, p_timeout,
            p_timeout_callback) {
    this->start();
  }

  std::optional<t_result> operator()() {

    auto ok = [this]() -> std::optional<t_result> { return {m_result}; };
    auto not_ok = []() -> std::optional<t_result> { return {}; };
    auto save_params = []() -> void {};

    return this->template call<std::optional<t_result>>(ok, not_ok,
                                                        save_params);
  }

private:
  t_result m_result;
};

// ########## 3 ##########
template <typename t_log, typename... t_params>
struct executer_t<t_log, void, t_params...> : public executer_base_t<t_log> {

  typedef std::function<void(t_params...)> worker;

  template <typename t_time>
  executer_t(worker p_worker, t_time p_timeout,
             timeout_callback p_timeout_callback)
      : executer_base_t<t_log>(
            [this, p_worker]() -> void {
              std::apply(p_worker, std::move(m_params));
            },
            p_timeout, p_timeout_callback) {
    this->start();
  }

  void operator()(t_params... p_params) {
    this->template call<void>(
        []() -> void { return; }, []() -> void { return; },
        [this, p_params...]() -> void { m_params = {p_params...}; });
  }

private:
  std::tuple<t_params...> m_params;
};

// ########## 4 ##########
template <typename t_log>
struct executer_t<t_log, void, void> : public executer_base_t<t_log> {

  typedef std::function<void()> worker;

  template <typename t_time>
  executer_t(worker p_worker, t_time p_timeout,
             timeout_callback p_timeout_callback)
      : executer_base_t<t_log>([this, p_worker]() -> void { p_worker(); },
                               p_timeout, p_timeout_callback) {
    this->start();
  }

  void operator()() {
    auto ok = []() -> void {};
    auto not_ok = []() -> void {};
    auto save_params = []() -> void {};

    this->template call<void>(ok, not_ok, save_params);
  }
};

/// \brief Type of function used to inform if a loop should stop
typedef std::function<bool()> breaker;

/// \brief base class for asynchronous loop
template <typename t_log> struct async_loop_base_t {

  async_loop_base_t(const async_loop_base_t &) = delete;
  async_loop_base_t(async_loop_base_t &&p_async_loop) = delete;
  async_loop_base_t &operator=(const async_loop_base_t &) = delete;
  async_loop_base_t &operator=(async_loop_base_t &&p_async_loop) = delete;

  virtual ~async_loop_base_t() { stop(); }

  void start() {
    if (!m_stopped) {
      return;
    }
    m_stopped = false;
    m_thread = concurrent::thread([this]() { loop(); });
  }

  void stop() {
    if (m_stopped) {
      return;
    }
    m_stopped = true;
    m_thread.join();
  }

  inline bool is_stopped() const { return m_stopped; }
  inline void set_log_debug() { m_log.set_debug_level(); }
  inline void set_log_info() { m_log.set_info_level(); }
  inline void set_log_warn() { m_log.set_warn_level(); }

protected:
  async_loop_base_t(breaker p_breaker = []() -> bool { return false; })
      : m_breaker(p_breaker, 500ms, [this](std::thread::id p_id) -> void {
          WAR(m_log, "thread ", p_id, " for breaker has timed out");
        }) {}

  virtual void loop() = 0;

protected:
  executer_t<t_log, bool, void> m_breaker;

  bool m_stopped{true};

  concurrent::thread<> m_thread;

  t_log m_log{"concurrent::async_loop"};
};

/// #### 1 ####
template <typename t_log, typename... t_params>
struct async_loop_t : public async_loop_base_t<t_log> {

  typedef std::function<void(t_params...)> worker;
  typedef std::function<std::tuple<t_params...>()> provider;

  template <typename t_time>
  async_loop_t(t_time p_timeout, timeout_callback p_timeout_callback,
               breaker p_breaker, worker p_worker, provider p_provider)
      : async_loop_base_t<t_log>(p_breaker),
        m_worker(p_worker, p_timeout, p_timeout_callback),
        m_provider(p_provider, 500ms, [this](std::thread::id p_id) -> void {
          WAR(this->m_log, "thread ", p_id, " for provider has timed out");
        }) {}

  template <typename t_time>
  async_loop_t(t_time p_timeout, timeout_callback p_timeout_callback,
               worker p_worker, provider p_provider)
      : async_loop_base_t<t_log>(),
        m_worker(p_worker, p_timeout, p_timeout_callback),
        m_provider(p_provider, 500ms, [this](std::thread::id p_id) -> void {
          WAR(this->m_log, "thread ", p_id, " for provider has timed out");
        }) {}

  inline worker get_worker() const { return m_worker; }

protected:
  void loop() override {
    while (true) {
      std::optional<std::tuple<t_params...>> _maybe_data = m_provider();

      if (this->m_stopped) {
        break;
      }

      std::optional<bool> _maybe_break = this->m_breaker();
      if ((_maybe_break) && (*_maybe_break)) {
        break;
      }

      if (_maybe_data) {
        std::tuple<t_params...> _params = std::move(*_maybe_data);

        std::apply(m_worker, _params);

        if (this->m_stopped) {
          break;
        }

        std::optional<bool> _maybe_break = this->m_breaker();
        if ((_maybe_break) && (*_maybe_break)) {
          break;
        }
      }
    }
  }

private:
  typedef executer_t<t_log, void, t_params...> worker_executer;
  typedef executer_t<t_log, std::tuple<t_params...>, void> provider_executer;

private:
  worker_executer m_worker;
  provider_executer m_provider;
};

/// #### 2 ####
template <typename t_log>
struct async_loop_t<t_log, void> : public async_loop_base_t<t_log> {

  typedef std::function<void()> worker;

  template <typename t_time>
  async_loop_t(t_time p_timeout, timeout_callback p_timeout_callback,
               breaker p_breaker, worker p_worker)
      : async_loop_base_t<t_log>(p_breaker),
        m_worker(p_worker, p_timeout, p_timeout_callback) {}

  template <typename t_time>
  async_loop_t(t_time p_timeout, timeout_callback p_timeout_callback,
               worker p_worker)
      : async_loop_base_t<t_log>(),
        m_worker(p_worker, p_timeout, p_timeout_callback) {}

  inline worker get_worker() const { return m_worker; }

protected:
  void loop() override {
    while (true) {

      m_worker();

      if (this->m_stopped) {
        break;
      }

      std::optional<bool> _maybe_break = this->m_breaker();
      if ((_maybe_break) && (*_maybe_break)) {
        break;
      }
    }
  }

private:
  typedef executer_t<t_log, void, void> worker_executer;

private:
  worker_executer m_worker;
};

template <typename t_log, typename t_async_loop> struct sleeping_loop_base_t {

  /// \brief used to notify about timeout of \p operation
  typedef std::function<void(std::thread::id)> timeout_callback;

  /// \brief default constructor not allowed
  sleeping_loop_base_t() = delete;

  /// \brief copy constructor not allowed
  sleeping_loop_base_t(const sleeping_loop_base_t &) = delete;

  /// \brief move constructor
  sleeping_loop_base_t(sleeping_loop_base_t &&p_sleep) = delete;

  /// \brief copy assignment not allowed
  sleeping_loop_base_t &operator=(const sleeping_loop_base_t &) = delete;

  /// \brief move assignment
  sleeping_loop_base_t &operator=(sleeping_loop_base_t &&p_sleep) = delete;

  /// \brief destructor interrupts the loop
  inline virtual ~sleeping_loop_base_t() {
    DEB(this->m_log, "destructor");
    stop();
  }

  /// \brief retrieves the interval defined for the loop to sleep between the
  /// execution of the operation
  inline interval get_interval() const { return m_interval; }

  /// \brief redefines the value of the execution interval
  ///
  /// \tparam t_interval is the type of time used to define the interval
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  template <typename t_interval>
  inline void set_interval(t_interval p_interval) {
    m_interval = to_interval(p_interval);
  }

  //  /// \brief Retrieves the operation defined to be executed
  //  inline typename t_async_loop::worker get_worker() const {
  //    return m_async.get_worker();
  //  }

  /// \brief Stops the loop, and starts it again
  void restart() {
    DEB(this->m_log, "restart");
    stop();
    start();
  }

  /// \brief run starts the loop
  void start() {
    if (!m_async.is_stopped()) {
      DEB(this->m_log, "not running async loop because it was not stopped");
      return;
    }
    DEB(this->m_log, "running async loop");
    m_stopped = false;
    m_async.start();
  }

  /// \brief stop stops the loop
  void stop() {

    if (m_async.is_stopped()) {
      DEB(this->m_log, "not stopping async loop because it was not running");
      return;
    }

    DEB(this->m_log, "stop");

    m_stopped = true;

    m_cond_var.notify_all();

    m_async.stop();
  }

  /// \brief retrieves if the loop is stopped
  inline bool is_stopped() const { return m_async.is_stopped(); }

  /// \brief retrieves the timeout for the operation
  inline timeout get_timeout() const { return m_async.get_timeout(); }

  /// \brief redefines the value of the timeout
  ///
  /// \tparam t_timeout is the type of time used to define timeout for the
  /// worker function
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  template <typename t_timeout> inline void set_timeout(t_timeout p_timeout) {
    m_async.set_timeout(p_timeout);
  }

  template <typename t_interval, typename... t_async_params>
  sleeping_loop_base_t(t_interval p_interval,
                       t_async_params &&... p_async_params)
      : m_interval(p_interval), m_async(p_async_params...) {}

  /// \brief breaker defines if the loop should stop
  ///
  /// \return \p true if the loop should break; \p false othewise
  bool breaker() {
    DEB(m_log, "interval = ", m_interval.count());

    std::unique_lock<std::mutex> _lock(m_mutex);
    if (m_cond_var.wait_for(_lock, m_interval) == std::cv_status::timeout) {
      // timeout, so do not stop
      DEB(m_log, "must not stop");
      return false;
    }
    // no timeout, which means the loop was stopped
    DEB(m_log, "must stop");
    return true;
  }

protected:
  interval m_interval;

  t_async_loop m_async;

  /// \brief controls asynchronous execution
  std::mutex m_mutex;

  /// \brief controls asynchronous execution
  std::condition_variable m_cond_var;

  bool m_stopped{true};

  /// \brief log for the class
  t_log m_log{"concurrent::sleeping_loop"};
};

/// \brief Base class for sleeping loops, which are loops that sleep during a
/// certain amount of time, then wake up and execute some work
///
/// \tparam t_log
///
/// \tparam t_params are the parameters that the work function
template <typename t_log, typename... t_params>
struct sleeping_loop_t
    : public sleeping_loop_base_t<t_log, async_loop_t<t_log, t_params...>> {

  /// \brief type of work executed in a loop in time intervals
  typedef std::function<void(t_params...)> worker;

  /// \brief Provider is the type of function that provides data to the
  /// work function
  ///
  /// \return \p an optional tuple of objects needed by the \p operation
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

  /// \brief Constructor
  ///
  /// \tparam t_timeout type of time used to define the timeout of the operation
  ///
  /// \tparam t_interval type of time used to define the execution interval of
  /// the operation
  ///
  /// \param p_timeout time used to define the timeout of the operation
  ///
  /// \param p_interval time used to define the execution interval of the
  /// operation
  ///
  /// \param p_worker the work function to be executed at \p p_interval
  ///
  /// \param p_timeout_callback function to be executed if \p p_worker times
  /// out
  ///
  /// \param p_provider function that provides the parameters to the \p
  /// p_worker
  template <typename t_timeout, typename t_interval>
  sleeping_loop_t(t_timeout p_timeout, t_interval p_interval, worker p_worker,
                  timeout_callback p_timeout_callback, provider p_provider)
      : sleeping_loop_base_t<t_log, async_loop_t<t_log, t_params...>>(
            p_interval, p_timeout, p_timeout_callback,
            [this]() -> bool { return this->breaker(); }, p_worker,
            p_provider) {
    DEB(this->m_log, "timeout = ", p_timeout.count(),
        ", interval = ", p_interval.count());
  }
};

/// \brief Base class for sleeping loops, which are loops that sleep during a
/// certain amount of time, then wake up and execute some work
///
/// \tparam t_log
///
/// \tparam t_params are the parameters that the work function
template <typename t_log>
struct sleeping_loop_t<t_log, void>
    : public sleeping_loop_base_t<t_log, async_loop_t<t_log, void>> {

  /// \brief type of work executed in a loop in time intervals
  typedef std::function<void()> worker;

  /// \brief Constructor
  ///
  /// \tparam t_interval type of time used to define the execution interval of
  /// the operation
  ///
  /// \param p_interval time used to define the execution interval of the
  /// operation
  ///
  /// \param p_operation the operation to be executed at \p p_interval
  template <typename t_timeout, typename t_interval>
  sleeping_loop_t(t_timeout p_timeout, t_interval p_interval, worker p_worker,
                  timeout_callback p_timeout_callback)
      : sleeping_loop_base_t<t_log, async_loop_t<t_log, void>>(
            p_interval, p_timeout, p_timeout_callback,
            [this]() -> bool { return this->breaker(); }, p_worker) {

    DEB(this->m_log, "timeout = ", p_timeout.count(),
        ", interval = ", p_interval.count());
  }
};
} // namespace concurrent

} // namespace tenacitas

#endif
