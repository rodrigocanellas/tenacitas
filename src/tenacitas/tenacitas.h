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
#include <cstdint>
#include <cstring>
#include <functional>
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
#include <type_traits>
#include <utility>

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
  template <typename... t_params>
  inline void debug(uint32_t p_line, const t_params &... p_params) {
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
  template <typename... t_params>
  inline void info(uint32_t p_line, const t_params &... p_params) {
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
  template <typename... t_params>
  inline void warn(uint32_t p_line, const t_params &... p_params) {
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
  template <typename... t_params>
  inline void error(uint32_t p_line, const t_params &... p_params) {
    write(level::error, p_line, p_params...);
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
              << m_class << m_separator << p_line;
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

/// \brief The log struct logs message to \p std::cerr
template <bool use = true> struct cerr : public log<use> {
  inline explicit cerr(std::string &&p_class)
      : log<use>(std::move(p_class),
                 [](std::string &&p_str) -> void { std::cerr << p_str; }) {}

  inline explicit cerr(const char *p_class)
      : log<use>(p_class,
                 [](std::string &&p_str) -> void { std::cerr << p_str; }) {}
};
} // namespace logger

} // namespace tenacitas

#endif
