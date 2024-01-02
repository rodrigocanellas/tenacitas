#ifndef TENACITAS_H
#define TENACITAS_H

#include <algorithm>
#include <chrono>
#include <cmath>
#include <concepts>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>

using namespace std::chrono_literals;

/// \brief Runs a test
///
/// \param tester_a is an instance of tenacitas::tester_a defined
/// below
///
/// \param test is the name of a class that implements
///
/// \code
/// bool operator()(const tenacitas::lib::program_options_a &)
///
/// static std::string desc()
/// \endcode
///
#define run_test(tester, test) tester.run<test>(#test)
namespace tenacitas::lib {

/// \brief Program options parser
struct program_options_a {
  /// \brief name of an option
  typedef std::string name;

  /// \brief value of an option
  typedef std::string value;

  program_options_a() = default;
  ~program_options_a() = default;
  program_options_a(const program_options_a &) = delete;
  program_options_a(program_options_a &&) = delete;
  program_options_a &operator=(const program_options_a &) = delete;
  program_options_a &operator=(program_options_a &&) = delete;
  void *operator new(size_t) = delete;

  /// \brief parses the options passed to a program
  ///
  /// \param p_argc number of options
  /// \param p_argv vector of strings with the options
  /// \param p_mandatory set of options that are mandatory
  ///
  /// \details an option must be preceded with '--', like '--log_file-name
  ///  abc.txt'.
  ///
  /// There are 3 types of parameter:
  /// \p boolean, where the parameter has no value, like '--print-log'
  ///
  /// \p single, where the parameter has a single value, like '--log_file-name
  /// abc.txt'
  ///
  /// \p set, where the parameter has a set of values, like '--tests { test1
  /// test2 test3 }
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
                                  const program_options_a &p_options) {
    for (const program_options_a::name &_boolean : p_options.m_booleans) {
      p_out << "[" << _boolean << "] ";
    }

    for (program_options_a::singles ::const_iterator _ite =
             p_options.m_singles.begin();
         _ite != p_options.m_singles.end(); ++_ite) {
      p_out << "[" << _ite->first << "," << _ite->second << "] ";
    }

    for (program_options_a::sets::const_iterator _ite =
             p_options.m_sets.begin();
         _ite != p_options.m_sets.end(); ++_ite) {
      p_out << "[" << _ite->first << " { ";
      for (const program_options_a::value &_value : _ite->second) {
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

/// \brief The test struct executes tests implemented in classes
///
/// \tparam use makes tenacitas::tester to be compiled only if
/// actually used
///
/// \code
/// #include <iostream>
/// #include <string>
///
/// #include <tester/test.h>
///
/// using namespace tenacitas::lib::src;
///
/// struct test_ok {
///   bool operator()(const program_options &) { return true; }
///
///   static std::string desc() { return "an ok test"; }
/// };
///
/// struct test_fail {
///   bool operator()(const program_options &) { return true; }
///   static std::string desc() { return "a fail test"; }
/// };
///
/// struct test_error {
///  bool operator()(const program_options &) {
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
///  int main(int argc, char **argv) {
///  try {
///    test::tester _tester(argc, argv);
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
template <bool use = true> struct tester_a {
  /// \brief Constructor
  /// If '--desc' is passed, \p operator() will print a description of the
  /// tests.
  /// If '--exec' is passed, \p operator() will execute the tests
  /// If '--exec { <test-name-1> <test-name-2> ... }' is passed, \p operator()
  /// will execute the tests between '{' and '}'
  ///
  /// \param argc number of strings in \p argv
  ///
  /// \param argv parameters passed to the program
  tester_a(int argc, char **argv,
           std::initializer_list<program_options_a::name> &&p_mandatory =
               {}) noexcept
      : m_argc(argc), m_argv(argv) {
    m_pgm_name = m_argv[0];

    try {
      m_options.parse(m_argc, m_argv, std::move(p_mandatory));

      if (m_options.get_bool_param("exec")) {
        m_execute_tests = true;
      } else if (m_options.get_bool_param("desc")) {
        m_print_desc = true;
      } else {
        std::optional<std::list<program_options_a::value>> _maybe =
            m_options.get_set_param("exec");
        if (_maybe) {
          m_execute_tests = true;
          std::list<program_options_a::value> _tests_to_exec =
              std::move(*_maybe);
          m_tests_to_exec.insert(_tests_to_exec.begin(), _tests_to_exec.end());
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

  /// \brief Default constructor not allowed
  tester_a() = delete;

  /// \brief Copy constructor not allowed
  tester_a(const tester_a &) = delete;

  /// \brief Copy constructor not allowed
  tester_a(tester_a &&) = delete;

  /// \brief Copy assignment not allowed
  tester_a &operator=(const tester_a &) = delete;

  /// \brief Move assignment not allowed
  tester_a &operator=(tester_a &&) = delete;

  /// \brief Executes the test
  ///  If the test passes, the message "SUCCESS for <name>" will be
  /// printed; if the test does not pass, the message "FAIL for <name>" will
  /// be printed; if an error occurr while executing the test , the messae
  /// "ERROR for <name> <desc>" will be printed
  ///
  /// \tparam t_test_class must implement:
  /// \code
  /// bool operator()(const program_options &)
  ///
  /// static std::string desc()
  /// \endcode
  ///
  /// \details You can use the macro 'run_test' defined above, instead of
  /// calling this method
  template <typename t_test_class>
  void run(const std::string &p_test_name) noexcept {
    using namespace std;
    try {
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
    } catch (std::exception &_ex) {
      std::cout << "EXCEPTION '" << _ex.what() << "'" << std::endl;
      return;
    }
  }

private:
  /// \brief Executes the test
  /// \tparam t_test_class must implement:
  /// \code
  /// bool operator()(const program_options &)
  ///
  /// static std::string desc()
  /// \endcode
  template <typename t_test_class> void exec(const std::string p_test_name) {
    using namespace std;
    bool result = false;
    try {
      t_test_class _test_obj;
      cerr << "\n############ -> " << p_test_name << " - "
           << t_test_class::desc() << endl;
      result = _test_obj(m_options);
      //            cout << (result ? "SUCCESS" : "FAIL") << " for " <<
      //            p_test_name
      //                 << endl;
      cout << p_test_name << (result ? " SUCCESS" : " FAIL") << endl;
    } catch (exception &_ex) {
      cout << "ERROR for " << p_test_name << " '" << _ex.what() << "'" << endl;
    }
    cerr << "############ <- " << p_test_name << endl;
  }

  /// \brief print_mini_howto prints a mini how-to for using the \p test class
  void print_mini_howto() {
    using namespace std;
    cout << "Syntax:\n"
         << "\t'" << m_pgm_name
         << " --desc' will display a description of the test\n"
         << "\t'" << m_pgm_name << " --exec' will execute the all the tests\n"
         << "\t'" << m_pgm_name
         << " --exec { <test-name-1> <test-name-2> ...}' will execute tests "
            "defined between '{' and '}'\n"
         << "\t'" << m_pgm_name << "' displays this message\n\n"
         << "For the programmers: \n"
         << "\t1 - Programmers should use 'std::cerr' to print messages\n"
         << "\t2 - If do not want your 'std::cerr' messages to be "
            "displayed, "
            "use\n"
         << "\t'" << m_pgm_name
         << " --exec 2> /dev/null' to execute the tests\n\n"
         << "Output:\n"
         << "\tIf the test passes, the message \"SUCCESS for <name>\" will "
            "be "
            "printed\n"
         << "\tIf the test does not pass, the message \"FAIL for <name>\" "
            "will "
            "be "
            "printed\n"
         << "\tIf an error occurr while executing the test , the message "
            "\"ERROR "
            "for <name> <desc>\" will be printed\n"
         << "\tIf an exception occurrs, the message \"EXCEPTION "
            "<description>\" "
            "will be printed"
         << endl;
  }

private:
  /// \brief Name of the test program
  std::string m_pgm_name;

  /// \brief Indicates if the tests should actually be executed, or if theirs
  /// description should be printed
  bool m_execute_tests = {false};

  /// \brief Prints test decription to \p cout
  bool m_print_desc = {false};

  /// \brief Number of parameters passed to the \p test object
  int m_argc = {-1};

  /// \brief Parameters passed to the \p test object
  char **m_argv = {nullptr};

  /// \brief Set of tests to execute
  std::set<std::string> m_tests_to_exec;

  program_options_a m_options;
};

template <typename t>
concept id_c =
    std::unsigned_integral<std::remove_const_t<std::remove_reference_t<t>>>;

/// \brief creation of a unique identifier
struct id_t {
  /// \brief Identifier based on a number
  template <typename t_number>
  inline id_t(t_number p_value) : m_value(p_value) {}

  /// \brief Identifier based on a pointer value
  template <typename t_this>
  id_t(t_this *p_this) : m_value(reinterpret_cast<decltype(m_value)>(p_this)) {}

  /// \brief Identifier self generated
  id_t() {
    std::this_thread::sleep_for(5ns);
    m_value = static_cast<decltype(m_value)>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count());
  }

  id_t(const id_t &) = default;
  id_t(id_t &&) = default;
  id_t &operator=(const id_t &) = default;
  id_t &operator=(id_t &&) = default;

  /// \brief
  inline constexpr bool operator==(const id_t &p_uuid) const {
    return m_value == p_uuid.m_value;
  }

  /// \brief
  inline constexpr bool operator!=(const id_t &p_uuid) const {
    return m_value != p_uuid.m_value;
  }

  /// \brief
  inline constexpr bool operator>(const id_t &p_uuid) const {
    return m_value > p_uuid.m_value;
  }

  /// \brief
  inline constexpr bool operator<(const id_t &p_uuid) const {
    return m_value < p_uuid.m_value;
  }

  /// \brief
  inline constexpr bool operator>=(const id_t &p_uuid) const {
    return m_value >= p_uuid.m_value;
  }

  /// \brief
  inline constexpr bool operator<=(const id_t &p_uuid) const {
    return m_value <= p_uuid.m_value;
  }

  /// \brief Outputs the last 4 digits of the identifier
  friend std::ostream &operator<<(std::ostream &p_out, const id_t &p_uuid) {
    p_out << p_uuid.string();
    return p_out;
  }

  /// \brief Retrieves the last 4 digits of the identifier
  inline operator std::string() const { return string(); }

  inline constexpr operator size_t() const { return m_value; }

  /// \brief Returns the value of the identifier as a string
  inline std::string string() const { return {std::to_string(m_value)}; }

private:
  size_t m_value{0};
};

struct uuid {
  uuid() = default;
  uuid(const uuid &) = default;
  uuid(uuid &) = default;

  uuid(std::array<uint8_t, 16> &&p_value) : m_value(std::move(p_value)) {}
  uuid(std::initializer_list<uint8_t> &&p_values) {
    uint8_t _counter{0};

    for (uint8_t _value : p_values) {
      if (_counter > 15) {
        return;
      }
      m_value[_counter++] = _value;
    }
  }

  uuid &operator=(const uuid &) = default;
  uuid &operator=(uuid &) = default;

  bool operator==(const uuid &p_uuid) const {
    return m_value == p_uuid.m_value;
  }
  bool operator!=(const uuid &p_uuid) const {
    return m_value != p_uuid.m_value;
  }
  bool operator<(const uuid &p_uuid) const { return m_value < p_uuid.m_value; }
  bool operator<=(const uuid &p_uuid) const {
    return m_value <= p_uuid.m_value;
  }
  bool operator>(const uuid &p_uuid) const { return m_value < p_uuid.m_value; }
  bool operator>=(const uuid &p_uuid) const {
    return m_value <= p_uuid.m_value;
  }

  friend std::ostream &operator<<(std::ostream &p_out, const uuid &p_uuid) {
    for (value ::value_type _v : p_uuid.m_value) {
      p_out << std::hex << std::uppercase << static_cast<uint16_t>(_v);
    }
    return p_out;
  }

private:
  using value = std::array<uint8_t, 16>;

private:
  value m_value;
};

/// \brief Type of alingnment when formating numbers
enum class align_t : char { left = 'l', right = 'd' };

namespace internal {
/// \brief Maximum number of characters needed to represent a type of number
///
/// <tt>internal::max_str_length_a<uint16_t>()</tt> is 5, while
/// <tt>internal::max_str_length_a<uint32_t>()</tt> is 10
///
/// \tparam t_num_type is the type of number
template <typename t_num_type> inline uint16_t max_str_length_a() {
  return (
      static_cast<uint16_t>(std::log10(std::pow(2, sizeof(t_num_type) * 8))) +
      1);
}

} // namespace internal

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
/// \param p_align defines if \p p_num will be aligned at left, right, or
/// center of the string
template <typename t_num_type>
inline std::string format_number_a(t_num_type p_num, char p_fill = '0',
                                   align_t p_align = align_t::right) {
  std::stringstream _stream;
  _stream << (p_align == align_t::right ? std::right : std::left) << std::fixed
          << std::setfill(p_fill)
          << std::setw(internal::max_str_length_a<t_num_type>()) << p_num;
  return _stream.str();
}

/// \brief Specialization for uint8_t of \p
/// tenacitas::lib::format_number
///
/// \param p_num is the value of the number to be formated
///
/// \param p_fill is the char used to fill the parts of the string not filled
/// with \p p_num digits
///
/// \param p_align defines if \p p_num will be aligned at left, right, or
/// center of the string
inline std::string format_number(uint8_t p_num, char p_fill = '0',
                                 align_t p_align = align_t::right) {
  std::stringstream _stream;
  _stream << (p_align == align_t::right ? std::right : std::left) << std::fixed
          << std::setfill(p_fill)
          << std::setw(internal::max_str_length_a<uint8_t>())
          << static_cast<uint16_t>(p_num);
  return _stream.str();
}

/// \brief Specialization for int8_t of \p
/// tenacitas::lib::format_number
///
/// \param p_num is the value of the number to be formated
///
/// \param p_fill is the char used to fill the parts of the string not filled
/// with \p p_num digits
///
/// \param p_align defines if \p p_num will be aligned at left, right, or
/// center of the string
inline std::string format_number(int8_t p_num, char p_fill = '0',
                                 align_t p_align = align_t::right) {
  std::stringstream _stream;
  _stream << (p_align == align_t::right ? std::right : std::left) << std::fixed
          << std::setfill(p_fill)
          << std::setw(internal::max_str_length_a<uint8_t>())
          << static_cast<int16_t>(p_num);
  return _stream.str();
}

/// \brief Specialization for uint8_t of \p
/// tenacitas::lib::format_number
///
/// \param p_num is the value of the number to be formated
///
/// \param p_fill is the char used to fill the parts of the string not filled
/// with \p p_num digits
///
/// \param p_align defines if \p p_num will be aligned at left, right, or
/// center of the string
template <typename t_num_type>
inline std::string format_number(t_num_type p_num, uint8_t p_size,
                                 char p_fill = '0',
                                 align_t p_align = align_t::right) {
  std::stringstream _stream;
  _stream << (p_align == align_t::right ? std::right : std::left) << std::fixed
          << std::setfill(p_fill)
          << std::setw(p_size)
          //            << std::setprecision(internal::max_str_length_a<long
          //            double>())
          << p_num;
  return _stream.str();
}

namespace internal {

template <typename t_tuple, std::size_t t_idx>
concept has_tuple_element_c = requires(t_tuple t) {
  typename std::tuple_element_t<t_idx, std::remove_const_t<t_tuple>>;
  {
    get<t_idx>(t)
    } -> std::convertible_to<const std::tuple_element_t<t_idx, t_tuple> &>;
};

} // namespace internal

// \log_file Solution based on
// https://stackoverflow.com/questions/68443804/c20-concept-to-check-tuple-like-types
template <typename t_tuple>
concept tuple_like_c = !std::is_reference_v<t_tuple> && requires(t_tuple t) {
  typename std::tuple_size<t_tuple>::type;
  requires std::derived_from<
      std::tuple_size<t_tuple>,
      std::integral_constant<std::size_t, std::tuple_size_v<t_tuple>>>;
}
&&[]<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
  return (internal::has_tuple_element_c<t_tuple, t_idx> && ...);
}
(std::make_index_sequence<std::tuple_size_v<t_tuple>>());

namespace internal {

template <size_t t_idx, typename t_type, tuple_like_c t_tuple>
constexpr ssize_t idx_a() {
  if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
    return -1;
  } else if constexpr (std::is_same_v<t_type,
                                      std::tuple_element_t<t_idx, t_tuple>>) {
    return static_cast<ssize_t>(t_idx);
  } else {
    return idx_a<t_idx + 1, t_type, t_tuple>();
  }
}

} // namespace internal

/// \brief Type trait that identifies if an object is not a tuple
template <typename> struct is_tuple_r : std::false_type {};

/// \brief Type trait that identifies if an object is a tuple
template <typename... t>
struct is_tuple_r<std::tuple<t...>> : std::true_type {};

template <typename t_type, tuple_like_c t_types> struct index_r {
  static constexpr ssize_t value = internal::idx_a<0, t_type, t_types>();
};

/// \brief Type trait that identifies if a type is not a \p pair
template <typename> struct is_pair_r : std::false_type {};

/// \brief Type trait that identifies if a type is a tuple
template <typename... t> struct is_pair_r<std::pair<t...>> : std::true_type {};

/// \brief Determines is a type is not a shared pointer
template <typename t> struct is_shared_ptr_r : std::false_type {};

template <typename t>
struct is_shared_ptr_r<std::shared_ptr<t>> : std::true_type {};

/// \brief Determines is a type is not a unique pointer
template <typename t> struct is_unique_ptr_r : std::false_type {};

/// \brief Determines is a type is a unique pointer
template <typename t>
struct is_unique_ptr_r<std::unique_ptr<t>> : std::true_type {};

/// \brief Determines if a type is a shared or unique pointer
template <typename t>
using is_smart_ptr_r =
    std::conditional_t<is_shared_ptr_r<t>::value, std::true_type,
                       std::conditional_t<is_unique_ptr_r<t>::value,
                                          std::true_type, std::false_type>>;

template <size_t t_idx, typename... t_function, typename... t_data>
requires(... and std::is_invocable_r_v<
         void, t_function,
         t_data &>) void traverse_a(std::tuple<t_function...> p_function,
                                    std::tuple<t_data...> &p_data) {
  std::get<t_idx>(p_function)(std::get<t_idx>(p_data));
}

template <size_t... t_idxs, typename... t_function, typename... t_data>
requires(... and std::is_invocable_r_v<
         void, t_function,
         t_data &>) void traverse_a(std::index_sequence<t_idxs...>,
                                    std::tuple<t_function...> p_function,
                                    std::tuple<t_data...> &p_data) {
  (traverse_a<t_idxs>(p_function, p_data), ...);
}

template <typename... t_function, typename... t_data>
requires(... and std::is_invocable_r_v<
         void, t_function,
         t_data &>) void traverse_a(std::tuple<t_function...> p_function,
                                    std::tuple<t_data...> &p_data) {
  traverse_a(std::make_index_sequence<sizeof...(t_data)>{}, p_function, p_data);
}

template <size_t t_idx, typename t_function, typename... t_data>
requires(... and std::is_invocable_r_v<
         void, t_function, t_data &>) void traverse_a(t_function p_function,
                                                      std::tuple<t_data...>
                                                          &p_data) {
  p_function(std::get<t_idx>(p_data));
}

template <size_t... t_idxs, typename t_function, typename... t_data>
requires(... and std::is_invocable_r_v<
         void, t_function,
         t_data &>) void traverse_a(std::index_sequence<t_idxs...>,
                                    t_function p_function,
                                    std::tuple<t_data...> &p_data) {
  (traverse_a<t_idxs>(p_function, p_data), ...);
}

template <typename t_function, typename... t_data>
requires(... and std::is_invocable_r_v<
         void, t_function, t_data &>) void traverse_a(t_function p_function,
                                                      std::tuple<t_data...>
                                                          &p_data) {
  traverse_a(std::make_index_sequence<sizeof...(t_data)>{}, p_function, p_data);
}

#ifdef TENACITAS_LOG

enum class log_level_t : int8_t {
  test = -2,
  no_log = -1,
  trace = 0,
  debug = 1,
  info = 2,
  warn = 3,
  error = 4,
  fatal = 5
};

/// \brief Output operator for \p level
///
/// \param p_out is the output stream
/// \param p_level is the level to be written to p_out
inline std::ostream &operator<<(std::ostream &p_out, log_level_t p_level) {
  switch (p_level) {
  case log_level_t::test:
    p_out << "TST";
    break;
  case log_level_t::trace:
    p_out << "TRA";
    break;
  case log_level_t::debug:
    p_out << "DEB";
    break;
  case log_level_t::info:
    p_out << "INF";
    break;
  case log_level_t::warn:
    p_out << "WAR";
    break;
  case log_level_t::error:
    p_out << "ERR";
    break;
  case log_level_t::fatal:
    p_out << "FAT";
    break;
  default:
    p_out << "***";
  }

  return p_out;
}

namespace internal {

static uint8_t max_log_file_name_lenght{15};

/// \brief Thread safe writing of log messages to console (cerr/cout/clog)
/// A log message has the format:
/// log-level|timestamp|log_file-name|thread-id|line-number|contents
///
/// \details \p log-level is TNCT_LOG_TRA, TNCT_LOG_DEB, TNCT_LOG_INF,
/// TNCT_LOG_WAR, TNCT_LOG_ERR or TNCT_LOG_FAT; \p timestamp is a EPOCH number
/// in microsecs; \p log_file-name is the name of the log_file where the message
/// is being writen; \p thread-id is the id of the thread that is logging; \p
/// line-number is the number of the line that is logging; contents are the log
/// message specific contents
template <bool use = true> class log_wrapper_a {
public:
  /// \brief Responsible for actually writing the log message
  typedef std::function<void(std::string &&)> writer;

public:
  /// \brief Default contructor not allowed
  log_wrapper_a() = default;

  /// \brief Copy constructor not allowed
  log_wrapper_a(const log_wrapper_a &) = delete;

  /// \brief Move constructor not allowed
  log_wrapper_a(log_wrapper_a &&) = delete;

  /// \brief Copy assignment not allowed
  log_wrapper_a &operator=(const log_wrapper_a &) = delete;

  /// \brief Move assignment not allowed
  log_wrapper_a &operator=(log_wrapper_a &&) = delete;

  /// \brief New operator not allowed
  void *operator new(size_t) = delete;

  /// \brief Delete operator not allowed
  void operator delete(void *) = delete;

  /// \brief Defines the function responsible for actually writing the message
  ///
  /// \tparam p_writer the function responsible for actually writing the
  /// message
  inline static void set_writer(writer p_writer) { m_writer = p_writer; }

  /// \brief Defines the separator to be used in the log messages
  /// Default is '|'
  ///
  /// \param p_separator the value of the separator
  inline static void set_separator(char p_separator) {
    m_separator = p_separator;
  }

  /// \brief Retrieves the separator used in the log messages
  ///
  /// \return the value of the separator
  inline char get_separator() const { return m_separator; }

  /// \brief Logs message with \p test severity
  /// Each parameter must implement the output operator.
  /// The log message will only be allways, and it should only be used in
  /// tests
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_log_file is the name of the log_file that is logging
  ///
  /// \param p_line is the line where the logging is done
  ///
  /// \param p_params are the contens of the log message
  template <typename... t_params>
  inline static void test(const char *p_log_file, uint16_t p_line,
                          t_params &&...p_params) {
    write(log_level_t::test, p_log_file, p_line,
          std::forward<t_params>(p_params)...);
  }

  /// \brief Logs message with \p trace severity
  /// Each parameter must implement the output operator.
  /// The log message will only be printed if the current log level is \p
  /// level::trace
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_log_file is the name of the log_file that is logging
  ///
  /// \param p_line is the line where the logging is done
  ///
  /// \param p_params are the contens of the log message
  template <typename... t_params>
  inline static void trace(const char *p_log_file, uint16_t p_line,
                           t_params &&...p_params) {
    if (log_level_t::trace >= g_level) {
      write(log_level_t::trace, p_log_file, p_line,
            std::forward<t_params>(p_params)...);
    }
  }

  /// \brief Logs message with \p debug severity
  /// Each parameter must implement the output operator.
  /// The log message will only be printed if the current log level is \p
  /// level::debug or \p level::trace
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_log_file is the name of the log_file that is logging
  ///
  /// \param p_line is the line where the logging is done
  ///
  /// \param p_params are the contens of the log message
  template <typename... t_params>
  inline static void debug(const char *p_log_file, uint16_t p_line,
                           t_params &&...p_params) {
    if (log_level_t::debug >= g_level) {
      write(log_level_t::debug, p_log_file, p_line,
            std::forward<t_params>(p_params)...);
    }
  }

  /// \brief Logs message with \p info severity
  /// Each parameter must implement the output operator.
  /// The log message will only be printed if the current log level is \p
  /// level::info, \p level::debug or \p level::trace
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_log_file is the name of the log_file that is logging
  ///
  /// \param p_line is the line where the logging is done
  ///
  /// \param p_params are the contens of the log message
  template <typename... t_params>
  inline static void info(const char *p_log_file, uint16_t p_line,
                          t_params &&...p_params) {
    if (log_level_t::info >= g_level) {
      write(log_level_t::info, p_log_file, p_line,
            std::forward<t_params>(p_params)...);
    }
  }

  /// \brief Logs message with \p warn severity
  /// Each parameter must implement the output operator.
  /// The log message will only be printed if the current log level is \p
  /// level::warn, \p level::info, \p level::debug or \p
  /// level::trace
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_log_file is the name of the log_file that is logging
  ///
  /// \param p_line is the line where the logging is done
  ///
  /// \param p_params are the contens of the log message
  template <typename... t_params>
  inline static void warn(const char *p_log_file, uint16_t p_line,
                          t_params &&...p_params) {
    if (log_level_t::warn >= g_level) {
      write(log_level_t::warn, p_log_file, p_line,
            std::forward<t_params>(p_params)...);
    }
  }

  /// \brief Logs message with \p error severity
  /// Each parameter must implement the output operator.
  /// \p level::error messages are allways logged
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_log_file is the name of the log_file that is logging
  ///
  /// \param p_line is the line where the logging is done
  ///
  /// \param p_params are the contens of the log message
  template <typename... t_params>
  inline static void error(const char *p_log_file, uint16_t p_line,
                           t_params &&...p_params) {
    write(log_level_t::error, p_log_file, p_line,
          std::forward<t_params>(p_params)...);
  }

  /// \brief Logs message with \p fatal severity
  /// Each parameter must implement the output operator.
  /// \p level::fatal messages are allways logged
  ///
  /// \tparam t_params are the types of the values to be logged.
  ///
  /// \param p_log_file is the name of the log_file that is logging
  ///
  /// \param p_line is the line where the logging is done
  ///
  /// \param p_params are the contens of the log message
  template <typename... t_params>
  inline static void fatal(const char *p_log_file, uint16_t p_line,
                           t_params &&...p_params) {
    write(log_level_t::fatal, p_log_file, p_line,
          std::forward<t_params>(p_params)...);
  }

  inline static void set_trace() { g_level = log_level_t::trace; }
  inline static void set_debug() { g_level = log_level_t::debug; }
  inline static void set_info() { g_level = log_level_t::info; }
  inline static void set_warn() { g_level = log_level_t::warn; }

private:
  /// \brief Actually writes the message
  ///
  /// \tparam t_params are the types of the values to be logged, and each
  /// parameter must implement the output operator
  ///
  /// \param p_level is the severity level of the message
  ///
  /// \param p_log_file is the name of the log_file that is logging
  ///
  /// \param p_line is the line where the logging is done
  ///
  /// \param p_params are the contens of the log message
  template <typename... t_params>
  static void write(log_level_t p_level, const char *p_log_file,
                    uint16_t p_line, t_params &&...p_params) {
    std::stringstream _stream;

    const double _now_us =
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count();
    const time_t _now_s{static_cast<time_t>(_now_us / 1000000)};

    // YYYY-MM-DD HH:MM:SS
    char _time_str[4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 1];
    strftime(_time_str, sizeof _time_str, "%Y-%m-%d %H:%M:%S",
             localtime(&_now_s));

    _stream << p_level << m_separator << _time_str << ','
            << format_number(
                   static_cast<uint32_t>(_now_us - (_now_s * 1000000)), 6, '0',
                   align_t::right)
            << m_separator
            //            << _now_us - (_now_s * 1000000) << m_separator
            << std::this_thread::get_id() << m_separator << std::setfill(' ')
            << std::left << std::setw(max_log_file_name_lenght)
            << std::filesystem::path(p_log_file).filename().string()
            << m_separator << format_number(p_line, uint8_t{5}) << m_separator;

    format(_stream, std::forward<t_params>(p_params)...);
    _stream << '\n';

    std::lock_guard<std::mutex> _lock(m_mutex);
    m_writer(_stream.str());
  }

  /// \brief Compile time recursion to solve the variadic template parameter
  template <typename t, typename... ts>
  inline static void format(std::ostream &p_stream, const t &p_t,
                            const ts &...p_ts) {
    format(p_stream, p_t);
    format(p_stream, p_ts...);
  }

  inline static void format(std::ostream &) {}

  inline static void format(std::ostream &p_stream, char p_c) {
    p_stream << p_c;
  }

  /// \brief End of compile time recursion to solve the variadic template
  /// parameter
  template <typename t>
  static void format(std::ostream &p_stream, const t &p_t) {
    if constexpr (is_tuple_r<std::decay_t<t>>::value) {
      format(p_stream, '(');
      format<0>(p_stream, p_t);
      format(p_stream, ')');
    } else {
      p_stream << p_t;
    }
  }

  /// \brief Appends a double to the buffer
  ///
  /// \param p_t is the double to be appended to the buffer
  static void format(std::ostream &p_stream, double p_t) {
    p_stream << std::setprecision(std::numeric_limits<double>::max_digits10)
             << p_t;
  }

  /// \brief Appends a float to the buffer
  ///
  /// \param p_t is the float to be appended to the buffer
  static void format(std::ostream &p_stream, float p_t) {
    p_stream << std::setprecision(std::numeric_limits<double>::max_digits10)
             << p_t;
  }

  /// \brief Appends a bool to the buffer
  ///
  /// \param p_t is the bool to be appended to the buffer
  static void format(std::ostream &p_stream, bool p_t) {
    constexpr char _true[]{"true"};
    constexpr char _false[]{"false"};
    p_stream << (p_t ? _true : _false);
  }

  /// \brief Appends a pair to the buffer
  ///
  /// \param t_first is the type of the first field of the pair
  ///
  /// \param t_second is the type of the second field of the pair
  ///
  /// \param p_pair is the pair to be appended to the buffer
  template <typename t_first, typename t_second>
  static void format(std::ostream &p_stream,
                     const std::pair<t_first, t_second> &p_pair) {
    p_stream << '(';
    format(p_stream, std::get<0>(p_pair));
    p_stream << ',';
    format(p_stream, std::get<1>(p_pair));
    p_stream << ')';
  }

  static void format(std::ostream &p_stream, std::chrono::minutes p_time) {
    p_stream << p_time.count();
  }

  static void format(std::ostream &p_stream, std::chrono::seconds p_time) {
    p_stream << p_time.count();
  }

  static void format(std::ostream &p_stream, std::chrono::milliseconds p_time) {
    p_stream << p_time.count();
  }

  static void format(std::ostream &p_stream, std::chrono::microseconds p_time) {
    p_stream << p_time.count();
  }

  static void format(std::ostream &p_stream, std::chrono::nanoseconds p_time) {
    p_stream << p_time.count();
  }

  static void format(std::ostream &p_stream, std::byte p_value) {
    p_stream << std::hex << std::uppercase << static_cast<uint16_t>(p_value);
  }

  //  /// \brief Copies a tuple fields into a string
  //  template <class TupType, size_t... I>
  //  static void to_str(std::ostream &p_stream, const TupType &_tup,
  //                     std::index_sequence<I...>) {
  //    //    std::stringstream _stream;
  //    p_stream << "(";
  //    (..., (p_stream << (I == 0 ? "" : ", ") << std::get<I>(_tup)));
  //    p_stream << ")";
  //    //    return _stream.str();
  //  }

  //  /// \brief Copies one tuple field into a string
  //  template <class... T>
  //  static void to_str(std::ostream &p_stream, const std::tuple<T...> &_tup) {
  //    to_str(p_stream, _tup, std::make_index_sequence<sizeof...(T)>());
  //  }

  template <size_t index = 0, typename... t_fields>
  static void format(std::ostream &p_stream,
                     const std::tuple<t_fields...> &p_fields) {
    format(p_stream, std::get<index>(p_fields));
    if constexpr (index + 1 != sizeof...(t_fields)) {
      format(p_stream, ',');
      format<index + 1>(p_stream, p_fields);
    }
  }

private:
  /// \brief Allows a thread safe writing to the log writer
  static std::mutex m_mutex;

  /// \brief Used to separate parts of the log message
  static char m_separator;

  /// \brief Function that actually writes the log message
  ///
  /// The default implementation writes the \p std::cerr
  static writer m_writer;

  /// \brief Current log level
  static log_level_t g_level;
};

template <bool use> std::mutex log_wrapper_a<use>::m_mutex;

/// \brief Used to separate parts of the log message
template <bool use> char log_wrapper_a<use>::m_separator{'|'};

/// \brief Function that actually writes the log message
///
/// The default implementation writes the \p std::cerr
template <bool use>
typename log_wrapper_a<use>::writer log_wrapper_a<use>::m_writer{
    [](std::string &&p_str) -> void { std::cerr << p_str; }};

/// \brief Current log level
template <bool use> log_level_t log_wrapper_a<use>::g_level{log_level_t::warn};

/// \brief Writes log messages to a log_file
///
/// A maximum log_file size is defined, and when this size is exceeded, a new
/// log_file is created
struct log_file_a {
  /// \brief Constructor
  ///
  /// \param p_log_file_name base log_file_a name, the actual log_file_a name is
  /// this name appended with a timestamp with microsecs precision
  ///
  /// In this case, the maximum log_file_a size is 5MB
  log_file_a(const std::string &p_log_file_name)
      : m_base_name(p_log_file_name) {
    open_log_file();
  }

  /// \brief Constructor
  ///
  /// \param p_log_file_name base log_file_a name, the actual log_file_a name is
  /// this name appended with a timestamp with microsecs precision
  ///
  /// \param p_size_in_bytes is the maximum log_file_a size
  log_file_a(const std::string &p_log_file_name, uint32_t p_size_in_bytes)
      : m_base_name(p_log_file_name), m_max_size(p_size_in_bytes) {
    open_log_file();
  }

  /// \brief Writes a string to the log_file_a
  void operator()(std::string &&p_str) {
    auto _size = static_cast<decltype(m_max_size)>(p_str.size());
    if ((m_current_size + _size) > m_max_size) {
      open_log_file();
      m_current_size = 0;
    }
    m_current_size += _size;
    m_stream << p_str;
    m_stream.flush();
  }

private:
  void open_log_file() {
    if (m_stream.is_open()) {
      m_stream.close();
    }

    m_current_name = m_base_name + "_" +
                     std::to_string(std::chrono::high_resolution_clock::now()
                                        .time_since_epoch()
                                        .count()) +
                     ".log";
    m_stream.open(m_current_name, std::ios::out);
    if (!m_stream.is_open()) {
      throw std::runtime_error("could not open '" + m_current_name + "'");
    }
  }

private:
  std::string m_base_name;
  std::string m_current_name;
  std::ofstream m_stream;
  uint32_t m_max_size{5 * 1024 * 1024};
  uint32_t m_current_size{0};
};
} // namespace internal

///// \brief Sets the global log level as 'trace'
inline void set_trace_level() { internal::log_wrapper_a<>::set_trace(); }

/// \brief Sets the global log level as 'debug'
inline void set_debug_level() { internal::log_wrapper_a<>::set_debug(); }

/// \brief Sets the global log level as 'info'
inline void set_info_level() { internal::log_wrapper_a<>::set_info(); }

/// \brief Sets the global log level as 'warn'
inline void set_warn_level() { internal::log_wrapper_a<>::set_warn(); }

/// \brief Defines that log messages will be written to \p std::cerr
inline void set_writer_cerr() {
  internal::log_wrapper_a<>::set_writer(
      [](std::string &&p_str) -> void { std::cerr << p_str; });
}

/// \brief Defines that log messages will be written to \p std::cout
inline void set_writer_cout() {
  internal::log_wrapper_a<>::set_writer(
      [](std::string &&p_str) -> void { std::cout << p_str; });
}

/// \brief Defines that log messages will be written to \p std::clog
inline void set_writer_clog() {
  internal::log_wrapper_a<>::set_writer(
      [](std::string &&p_str) -> void { std::clog << p_str; });
}

/// \brief Defines the function used to write the log messages
inline void set_writer(std::function<void(std::string &&)> p_writer) {
  internal::log_wrapper_a<>::set_writer(p_writer);
}

inline void set_log_file_writer(const std::string &p_base_log_file_name,
                                uint32_t p_max_log_file_size = 5 * 1024 *
                                                               1024) {
  auto _log_file{std::make_shared<internal::log_file_a>(p_base_log_file_name,
                                                        p_max_log_file_size)};

  internal::log_wrapper_a<>::set_writer(
      [_log_file](std::string &&p_str) -> void {
        (*_log_file)(std::move(p_str));
      });
}

inline void set_max_log_file_name_lenght(uint8_t p_value) {
  internal::max_log_file_name_lenght = p_value;
}

/// \brief log_wrapper to the test log function
#define TNCT_LOG_TST(p_params...)                                              \
  tenacitas::lib::internal::log_wrapper_a<>::test(__FILE__, __LINE__, p_params)

/// \brief log_wrapper_a to the trace log function
#define TNCT_LOG_TRA(p_params...)                                              \
  tenacitas::lib::internal::log_wrapper_a<>::trace(__FILE__, __LINE__, p_params)

/// \brief log_wrapper_a to the debug log function
#define TNCT_LOG_DEB(p_params...)                                              \
  tenacitas::lib::internal::log_wrapper_a<>::debug(__FILE__, __LINE__, p_params)

/// \brief log_wrapper_a to the info log function
#define TNCT_LOG_INF(p_params...)                                              \
  tenacitas::lib::internal::log_wrapper_a<>::info(__FILE__, __LINE__, p_params)

/// \brief log_wrapper_a to the warn log function
#define TNCT_LOG_WAR(p_params...)                                              \
  tenacitas::lib::internal::log_wrapper_a<>::warn(__FILE__, __LINE__, p_params)

/// \brief log_wrapper_a to the error log function
#define TNCT_LOG_ERR(p_params...)                                              \
  tenacitas::lib::internal::log_wrapper_a<>::error(__FILE__, __LINE__, p_params)

/// \brief log_wrapper_a to the fatal log function
#define TNCT_LOG_FAT(p_params...)                                              \
  tenacitas::lib::internal::log_wrapper_a<>::fatal(__FILE__, __LINE__, p_params)

/// \brief Defines the character to separate the fields in a log message
#define TNCT_LOG_SEP(separator)                                                \
  tenacitas::lib::internal::log_wrapper_a<>::set_separator(separator)

#endif // TENACITAS_LOG

/// \brief Implements a circular queue which size is increased if it becomes
/// full
///
/// The purpose is to avoid unnecessary memory allocations to create nodes in
/// the queue by reusing nodes which data have been read
///
/// \tparam t_data defines the types of the data contained in the queue
template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
struct circular_queue_t {
  using data = t_data;

  circular_queue_t() = default;

  circular_queue_t(const circular_queue_t &p_queue);

  circular_queue_t(circular_queue_t &&) = default;

  circular_queue_t &operator=(const circular_queue_t &p_queue);

  circular_queue_t &operator=(circular_queue_t &&p_queue) = default;

  ~circular_queue_t() = default;

  void traverse(std::function<void(const t_data &)> p_visitor) const;

  void add(t_data &&p_data);

  void add(const t_data &p_data);

  std::optional<t_data> get();

  constexpr bool full() const { return (m_amount == m_list.size()); }

  constexpr bool empty() const { return m_amount == 0; }

  /// \brief amount of nodes in the queue
  constexpr size_t capacity() const { return m_list.size(); }

  /// \brief amount of nodes that that have t_data objects
  constexpr size_t occupied() const { return m_amount; }

  /// \brief returns an identifier used for debug purposes
  size_t get_id() const { return m_id; }

  void clear() {
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_head = m_tail;
    m_amount = 0;
  }

private:
  using list = std::list<t_data>;
  using iterator = typename list::iterator;
  using const_iterator = typename list::const_iterator;

private:
  // Amount of nodes actually used
  size_t m_amount{0};

  list m_list;
  iterator m_head;
  iterator m_tail;

  id_t m_id;

  std::mutex m_mutex;
};

// #############################################################################
// IMPLEMENTATIION

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
circular_queue_t<t_data>::circular_queue_t(const circular_queue_t &p_queue)
    : m_amount(p_queue.m_amount), m_list(p_queue.m_list),
      m_head(p_queue.m_head), m_tail(p_queue.m_tail) {}

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
    circular_queue_t<t_data>
&circular_queue_t<t_data>::operator=(const circular_queue_t &p_queue) {
  if (this != &p_queue) {
    m_amount = p_queue.m_amount;
    m_list = p_queue.m_list;
    m_head = p_queue.m_head;
    m_tail = p_queue.m_tail;
  }
  return *this;
}

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
void circular_queue_t<t_data>::traverse(
    std::function<void(const t_data &)> p_visitor) const {
  if (m_amount == 0) {
    return;
  }

  if (m_head == m_tail) {
    p_visitor(*m_tail);
    return;
  }

  const_iterator _ite{m_tail};
  while (_ite != m_head) {
    if (_ite == m_list.end()) {
      _ite = m_list.begin();
      p_visitor(*_ite);
    } else {
      p_visitor(*_ite);
      ++_ite;
    }
  }
}

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
void circular_queue_t<t_data>::add(t_data &&p_data) {
  std::lock_guard<std::mutex> _lock(m_mutex);

  if (m_list.empty()) {
    m_list.emplace_back(std::move(p_data));
    m_tail = m_head = m_list.begin();
    ++m_amount;
    return;
  }

  if (full()) {
    m_head = m_list.emplace(std::next(m_head), std::move(p_data));
  } else {
    ++m_head;
    if (m_head == m_list.end()) {
      m_head = m_list.begin();
    }
  }

  *m_head = std::move(p_data);
  ++m_amount;
}

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
void circular_queue_t<t_data>::add(const t_data &p_data) {
  std::lock_guard<std::mutex> _lock(m_mutex);

  if (m_list.empty()) {
    m_list.emplace_back(p_data);
    m_tail = m_head = m_list.begin();
    ++m_amount;
    return;
  }

  if (full()) {
    m_head = m_list.emplace(std::next(m_head), p_data);
  } else {
    ++m_head;
    if (m_head == m_list.end()) {
      m_head = m_list.begin();
    }
  }

  *m_head = p_data;
  ++m_amount;
}

template <typename t_data>
requires std::move_constructible<t_data> && std::copy_constructible<t_data>
    std::optional<t_data> circular_queue_t<t_data>::get() {
  std::lock_guard<std::mutex> _lock(m_mutex);

  if (m_list.empty() || (occupied() == 0)) {
    return {};
  }

  t_data _data = *m_tail;
  ++m_tail;
  --m_amount;
  if (m_tail == m_list.end()) {
    m_tail = m_list.begin();
  }
  return _data;
}

template <typename t>
concept event_c = std::default_initializable<t> && std::copy_constructible<t>;

template <typename t>
concept publisher_c = requires(t p_t) {
  typename t::events_published;

  requires tuple_like_c<typename t::events_published> &&

      []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return ((event_c<typename std::tuple_element_t<
                 t_idx, typename t::events_published>>)&&...);
  }
  (std::make_index_sequence<std::tuple_size_v<typename t::events_published>>());
};

template <typename t>
concept subscriber_c = requires(t p_t) {
  typename t::events_subscribed;

  requires tuple_like_c<typename t::events_subscribed> &&

      []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return ((event_c<typename std::tuple_element_t<
                 t_idx, typename t::events_subscribed>>)&&...);
  }
  (std::make_index_sequence<
      std::tuple_size_v<typename t::events_subscribed>>());
};

template <typename t, typename... t_events>
concept dispatcher_c = requires(t p_t) {
  typename t::events;

  typename t::ptr;

  requires !std::copy_constructible<t> && !std::move_constructible<t> &&
      !std::default_initializable<t>;

  requires(is_shared_ptr_r<typename t::ptr>::value == true);

  tuple_like_c<typename t::events>

      && // every type in t::events is a event_c

          []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return (event_c<std::tuple_element<t_idx, typename t::events>> && ...);
  }
  (std::make_index_sequence<std::tuple_size_v<typename t::events>>())

      && // every type in t_events is a event_c

          []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return (event_c<std::tuple_element<t_idx, std::tuple<t_events...>>> && ...);
  }
  (std::make_index_sequence<std::tuple_size_v<std::tuple<t_events...>>>())

      && // every event in t_events exists in t::events

          []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return (index_r<std::tuple_element_t<t_idx, std::tuple<t_events...>>,
                    typename t::events>::value &&
            ...);
  }
  (std::make_index_sequence<std::tuple_size_v<std::tuple<t_events...>>>());
};

/// \brief Type of function that subscriber an event
template <event_c t_event> using subscriber_a = std::function<void(t_event &&)>;

/// \brief Defines the priority of an event publishing among other publishings
/// for that event. The smallest the value, the lowest the priority
enum class priority : uint8_t {
  lowest = 1,
  very_low,
  low,
  medium,
  high,
  very_high,
  highest
};

/// \brief Output operator for \p priority
inline std::ostream &operator<<(std::ostream &p_out, priority p_priority) {
  switch (p_priority) {
  case priority::lowest:
    p_out << "lowest";
    break;
  case priority::very_low:
    p_out << "very low";
    break;
  case priority::low:
    p_out << "low";
    break;
  case priority::medium:
    p_out << "medium";
    break;
  case priority::high:
    p_out << "high";
    break;
  case priority::very_high:
    p_out << "very high";
    break;
  case priority::highest:
    p_out << "highest";
    break;
  default:
    p_out << "UNDEFINED";
    break;
  }
  return p_out;
}
using queue_id_t = size_t;

namespace internal {

// A publishing for an event
template <event_c t_event> class event_queue_t {
public:
  event_queue_t() = delete;

  event_queue_t(priority p_priority = priority::medium);

  event_queue_t(const event_queue_t &) = delete;

  event_queue_t(event_queue_t &&p_queue) {
    m_id = std::move(p_queue.m_id);
    m_priority = std::move(p_queue.m_priority);
    m_loops = std::move(p_queue.m_loops);
    m_stopped = p_queue.m_stopped.load();
    m_queued_data = std::move(p_queue.m_queued_data);
    m_circular_queue = std::move(p_queue.m_circular_queue);
  }

  event_queue_t &operator=(const event_queue_t &) = delete;
  event_queue_t &operator=(event_queue_t &&) = delete;

  // Adds an event to the queue of events
  bool add_event(const t_event &p_event);

  // Adds a subscriber that will compete with the other existing subscribers for
  // an event in the queue
  void add_subscriber(subscriber_a<t_event> p_subscriber);

  // Adds a bunch of subscribers
  void add_subscriber(std::unsigned_integral auto p_num_subscribers,
                      std::function<subscriber_a<t_event>()> p_factory);

  ~event_queue_t();

  // \brief Stops this publishing
  void stop() {
    trace(__LINE__, "entering stop()");

    if (m_stopped) {
      trace(__LINE__, "not stopping because it is stopped");
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_cond.notify_all();
      return;
    }

    trace(__LINE__, "notifying loops to stop");

    {
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_stopped = true;
      m_cond.notify_all();
    }

    for (std::thread &_thread : m_loops) {
      if (_thread.joinable()) {
        trace(__LINE__, "is joinable", _thread.get_id());

        _thread.join();

        trace(__LINE__, "joined", _thread.get_id());
      } else {
        trace(__LINE__, "is not joinable", _thread.get_id());
      }
    }
    trace(__LINE__, "leaving stop()");
  }

  // Amount of events added
  size_t amount_added() const;

  // Informs if the publishing is stopped
  bool is_stopped() const;

  queue_id_t get_id() const;

  constexpr priority get_priority() const;

  void set_priority(priority p_priority);

  // \return Returns the size of the queue of \p t_event
  size_t get_size() const;

  // \return Returns the amount of \p t_event objects in the queue
  size_t get_occupied() const;

  void clear() {
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_paused = true;
    m_circular_queue.clear();
    m_paused = false;
    m_cond.notify_all();
  }

  // publishing is ordered by tenacitas::lib::async::priority
  constexpr bool operator<(const event_queue_t &p_publishing) const;

  // publishing is ordered by tenacitas::lib::async::priority
  constexpr bool operator>(const event_queue_t &p_publishing) const;

  // publishing is compared by tenacitas::lib::publishing_id
  constexpr bool operator!=(const event_queue_t &p_publishing) const;

  // publishing is compared by tenacitas::lib::publishing_id
  constexpr bool operator==(const event_queue_t &p_publishing) const;

  void report(std::ostringstream &p_out) const;

  //  void clear() {
  //    std::lock_guard<std::mutex> _lock(m_mutex);
  //    stop();
  //    m_circular_queue.clear();
  //  }

  void trace(uint16_t p_line, std::string_view p_text,
             std::thread::id p_thread = std::this_thread::get_id()) {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA(
        format_number(p_line, uint8_t{5}), " - E ", typeid(t_event).name(),
        ", Q {", this, ',', m_id, "}, C {", m_circular_queue.get_id(), ",",
        m_circular_queue.occupied(), "}, # L ", m_loops.size(), ", S? ",
        (m_stopped.load() ? 'T' : 'F'), ", T ", p_thread, " - ", p_text);
#endif
  }

protected:
  // Group of loops that asynchronously call the subscribers
  using loops = std::vector<std::thread>;

  // Queue used to store the events to be handled
  using circular_queue = circular_queue_t<t_event>;

protected:
  void subscriber_loop(subscriber_a<t_event> p_subscriber);

  void empty_queue(const std::thread::id &p_loop_id,
                   subscriber_a<t_event> p_subscriber);

protected:
  // Identifier of this publishing
  size_t m_id;

  // Priority of this publishing
  priority m_priority;

  // Asynchronous loops, where events are retrieved from queue, and
  // subscribers are called
  loops m_loops;

  // Indicates if the dispatcher should continue to run
  std::atomic_bool m_stopped{false};

  std::atomic_bool m_paused{false};

  // Amount of queued data
  size_t m_queued_data{0};

  // Controls access to the \p m_loops while inserting a new subscriber
  std::mutex m_add_subscriber;

  // Controls access to inserting data
  std::mutex m_mutex;

  // Controls access to the data produced
  std::condition_variable m_cond;

  circular_queue m_circular_queue;
};

template <event_c t_event> inline event_queue_t<t_event>::~event_queue_t() {
  trace(__LINE__, "entering destructor");
  stop();
  trace(__LINE__, "leaving destructor");
}

template <event_c t_event>
inline bool event_queue_t<t_event>::is_stopped() const {
  return m_stopped;
}

template <event_c t_event>
inline queue_id_t event_queue_t<t_event>::get_id() const {
  return m_id;
}

template <event_c t_event>
inline constexpr priority event_queue_t<t_event>::get_priority() const {
  return m_priority;
}

template <event_c t_event>
inline void event_queue_t<t_event>::set_priority(priority p_priority) {
  m_priority = p_priority;
}

template <event_c t_event>
inline constexpr bool
event_queue_t<t_event>::operator<(const event_queue_t &p_publishing) const {
  return m_priority < p_publishing.m_priority;
}

template <event_c t_event>
inline constexpr bool
event_queue_t<t_event>::operator>(const event_queue_t &p_publishing) const {
  return m_priority > p_publishing.m_priority;
}

template <event_c t_event>
inline constexpr bool
event_queue_t<t_event>::operator!=(const event_queue_t &p_publishing) const {
  return m_id != p_publishing.m_id;
}

template <event_c t_event>
inline constexpr bool
event_queue_t<t_event>::operator==(const event_queue_t &p_publishing) const {
  return m_id == p_publishing.m_id;
}

template <event_c t_event>
inline event_queue_t<t_event>::event_queue_t(priority p_priority)
    : m_id(id_t()), m_priority(p_priority) {
  trace(__LINE__, "creating queue");
}

template <event_c t_event>
bool event_queue_t<t_event>::add_event(const t_event &p_event) {
  try {
    trace(__LINE__, "adding event");

    m_circular_queue.add(p_event);

    std::lock_guard<std::mutex> _lock(m_mutex);
    m_cond.notify_all();

    ++m_queued_data;

    return true;

  } catch (std::exception &_ex) {
    trace(__LINE__, "error adding event: " + std::string(_ex.what()));
  }
  return false;
}

template <event_c t_event>
void event_queue_t<t_event>::add_subscriber(
    std::unsigned_integral auto p_num_subscribers,
    std::function<subscriber_a<t_event>()> p_factory) {
  trace(__LINE__, "adding many subscribers");
  for (decltype(p_num_subscribers) _i = 0; _i < p_num_subscribers; ++_i) {
    add_subscriber(p_factory());
  }
}

template <event_c t_event>
inline size_t event_queue_t<t_event>::amount_added() const {
  return m_circular_queue.capacity();
}

template <event_c t_event>
inline size_t event_queue_t<t_event>::get_size() const {
  return m_circular_queue.capacity();
}

template <event_c t_event>
inline size_t event_queue_t<t_event>::get_occupied() const {
  return m_circular_queue.occupied();
}

template <event_c t_event>
inline void event_queue_t<t_event>::report(std::ostringstream &p_out) const {
  p_out << "\tpublishing: " << m_id << '\n'
        << "\t\tnum subscribers: " << m_loops.size()
        << ", priority: " << m_priority << '\n';
}

template <event_c t_event>
void event_queue_t<t_event>::add_subscriber(
    subscriber_a<t_event> p_subscriber) {
  if (m_stopped) {
    trace(__LINE__, "not adding subscriber because stopped");
    return;
  }

  trace(__LINE__, "adding subscriber");

  std::lock_guard<std::mutex> _lock(m_add_subscriber);

  m_loops.push_back(std::thread(
      [this, p_subscriber]() -> void { subscriber_loop(p_subscriber); }));
}

template <event_c t_event>
void event_queue_t<t_event>::subscriber_loop(
    subscriber_a<t_event> p_subscriber) {
  auto _queue_id{m_circular_queue.get_id()};
  auto _loop_id{std::this_thread::get_id()};

  trace(__LINE__, "starting subscriber's loop", _loop_id);

  if (!p_subscriber) {
    std::stringstream _stream;
    _stream << "event " << typeid(t_event).name() << "<< queue {" << this
            << "<<" << m_id << "}<< circular " << m_circular_queue.get_id()
            << "<< queued_data = " << m_queued_data << "<< priority "
            << m_priority << "<< loops = " << &m_loops
            << "<< stopped = " << m_stopped.load() << "<< thread "
            << std::this_thread::get_id() << " - "
            << "invalid event subscriber";
    const std::string _str{_stream.str()};
#ifdef TENACITAS_LOG
    TNCT_LOG_FAT(_str);
#endif
    throw std::runtime_error(_str);
  }

  auto _subscriber_id{&p_subscriber};

  if (m_stopped) {
    trace(__LINE__, "leaving because it was already stopped", _loop_id);
    return;
  }

  trace(__LINE__, "entering loop");

  while (true) {
    {
      trace(__LINE__, "locking", _loop_id);

      std::unique_lock<std::mutex> _lock(m_mutex);
      m_cond.wait(_lock, [this, _subscriber_id, _loop_id, _queue_id]() -> bool {
        trace(__LINE__, "entering condition", _loop_id);
        if (m_paused) {
          trace(__LINE__, "stopped", _loop_id);
          return false;
        }
        if (m_stopped) {
          trace(__LINE__, "stopped", _loop_id);
          return true;
        }
        if (!m_circular_queue.empty()) {
          trace(__LINE__, "there is data", _loop_id);
          return true;
        }
        trace(__LINE__, "waiting", _loop_id);
        return false;
      });
    }

    trace(__LINE__, "lock released", _loop_id);

    if (m_stopped) {
      trace(__LINE__, "stopped due to explicit stop", _loop_id);
      break;
    }

    trace(__LINE__, "about to get the event from the queue", _loop_id);

    std::optional<t_event> _maybe{m_circular_queue.get()};
    if (!_maybe.has_value()) {
      trace(__LINE__, "no event in queue", _loop_id);
      continue;
    }
    t_event _event{std::move(*_maybe)};

    trace(__LINE__, "about to call subscriber", _loop_id);
    p_subscriber(std::move(_event));

    trace(__LINE__, "event handled", _loop_id);
  }
  trace(__LINE__, "leaving subscriber's loop", _loop_id);
}

template <event_c t_event>
void event_queue_t<t_event>::empty_queue(const std::thread::id &p_loop_id,
                                         subscriber_a<t_event> p_subscriber) {
  trace(__LINE__, "entering empty_queue", p_loop_id);
  while (true) {
    std::optional<t_event> _maybe{m_circular_queue.get()};
    if (!_maybe.has_value()) {
      break;
    }

    if (m_stopped) {
      trace(__LINE__, "stop", p_loop_id);
      break;
    }

    t_event _event{std::move(*_maybe)};

    if (this->m_stopped) {
      trace(__LINE__, "stop", p_loop_id);
      break;
    }

    trace(__LINE__, "event to be passed to a subscriber", p_loop_id);
    p_subscriber(std::move(_event));
    trace(__LINE__, "event handled", p_loop_id);
  }
}
} // namespace internal

template <typename t_to, typename t_from>
concept chrono_convertible_c = requires(t_from p_time) {
  std::chrono::duration_cast<t_to>(p_time);
};

template <typename t_from>
concept convertible_to_milli_c =
    chrono_convertible_c<std::chrono::milliseconds, t_from>;

template <typename t_from>
concept convertible_to_sec_c =
    chrono_convertible_c<std::chrono::seconds, t_from>;

template <typename t_from>
concept convertible_to_nano_c =
    chrono_convertible_c<std::chrono::nanoseconds, t_from>;

/// \brief Periodically executes a function
struct sleeping_loop_a {
  /// \brief Signature of the function that will be called in each round of
  /// the loop
  using function = std::function<void()>;

  /// \brief Constructor
  ///
  /// \tparam t_interval is the type of time used to define the amount of time
  /// that the function will be called
  ///
  /// \param p_function to be executed at a certain interval
  ///
  /// \param p_interval interval that \p p_function will be executed
  template <convertible_to_nano_c t_interval>
  sleeping_loop_a(function p_function, t_interval p_interval);

  sleeping_loop_a() = delete;
  sleeping_loop_a(const sleeping_loop_a &) = delete;
  sleeping_loop_a &operator=(const sleeping_loop_a &) = delete;

  /// \brief Destructor
  /// The loops stops calling the function
  ~sleeping_loop_a() {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id, " - destructor");
#endif
    stop();
    if (m_thread.get_id() == std::thread::id()) {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("sleeping loop ", m_id,
                   " - not joining because m_thread.get_id() == "
                   "std::thread::id()");
#endif
      return;
    }
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id,
                 " - m_thread.get_id() != std::thread::id()");
#endif
    if (!m_thread.joinable()) {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("sleeping loop ", m_id,
                   " - not joining because m_thread is not joinable");
#endif
      return;
    }
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id, " - m_thread.joinable()");
#endif
    {
      std::lock_guard<std::mutex> _lock(m_mutex_join);
      m_thread.join();
    }
  }

  /// \brief Move constructor
  sleeping_loop_a(sleeping_loop_a &&p_loop);

  /// \brief Move assignment
  sleeping_loop_a &operator=(sleeping_loop_a &&p_loop) {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id, " move assignment");
#endif
    if (this != &p_loop) {
      move(std::move(p_loop));
    }
    return *this;
  }

  /// \brief Starts calling the function periodically
  void start() {
    if (!m_stopped) {
      return;
    }
    m_stopped = false;

#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id, " - starting loop thread");
#endif
    m_thread = std::thread([this]() { loop(); });
  }

  /// \brief Stops the loop, if it was started
  void stop() {
    if (m_stopped) {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("sleeping loop ", m_id,
                   " - not stopping because it is stopped");
#endif
      return;
    }
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id, " - stopping");
#endif
    m_stopped = true;
    m_cond.notify_one();

#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id, " - leaving stop");
#endif
  }

  /// \brief Retrieves if the loop was stopped
  bool is_stopped() const;

private:
  /// \brief Helper move function
  void move(sleeping_loop_a &&p_loop) {
    bool _stopped = p_loop.is_stopped();
    id_t _other = p_loop.m_id;
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id, " - moving ", _other, " which was",
                 (_stopped ? " " : " not "), "stopped");
#endif

    p_loop.stop();

    m_function = p_loop.m_function;
    m_interval = std::move(p_loop.m_interval);
    m_stopped = true;

    if (!_stopped) {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("sleeping loop ", m_id, " - starting it because ", _other,
                   " was running");
#endif
      start();
    }
#ifdef TENACITAS_LOG
    else {
      TNCT_LOG_TRA("sleeping loop ", m_id, " - NOT starting it because ",
                   _other, " was NOT running");
    }
#endif
  }

  /// \brief Loop where the function will be called
  void loop() {
    while (true) {
      if (m_stopped) {
#ifdef TENACITAS_LOG
        TNCT_LOG_TRA("sleeping loop ", m_id, " - stop");
#endif
        break;
      }

#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("sleeping loop ", m_id, " - calling subscriber ",
                   &m_function);
#endif

      m_function();

      if (m_stopped) {
#ifdef TENACITAS_LOG
        TNCT_LOG_TRA("sleeping loop ", m_id, " - stop");
#endif
        break;
      }
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("sleeping loop ", m_id, " - waiting for ",
                   m_interval.count(), " microsecs to elaps, or a stop order");
#endif
      {
        std::unique_lock<std::mutex> _lock(m_mutex_interval);
        if ((m_cond.wait_for(_lock, m_interval) ==
             std::cv_status::no_timeout) ||
            (m_stopped)) {
#ifdef TENACITAS_LOG
          TNCT_LOG_TRA("sleeping loop ", m_id, " - ordered to stop");
#endif
          break;
        }
      }
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("sleeping loop ", m_id, " - ", m_interval.count(),
                   " ms elapsed");
#endif
    }
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("sleeping loop ", m_id, " - leaving loop");
#endif
  }

private:
  /// \brief Function that will be called in each round of the loop
  function m_function;

  /// \brief Interval for calling m_function
  std::chrono::nanoseconds m_interval;

  /// \brief Indicates that the loop must stop
  std::atomic<bool> m_stopped{true};

  /// \brief Thread where the \p loop method will run
  std::thread m_thread;

  /// \brief Protects joining the thread
  std::mutex m_mutex_join;

  /// \brief Controls the interval execution
  std::mutex m_mutex_interval;

  /// \brief Controls the interval execution
  std::condition_variable m_cond;

  /// \brief Identifier of the slepping_loop, to help debugging
  id_t m_id;
};

template <convertible_to_nano_c t_interval>
inline sleeping_loop_a::sleeping_loop_a(function p_function,
                                        t_interval p_interval)
    : m_function(p_function),
      m_interval(std::chrono::duration_cast<decltype(m_interval)>(p_interval)) {
#ifdef TENACITAS_LOG
  TNCT_LOG_TRA("sleeping loop ", m_id, " - creating with function ",
               &m_function);
#endif
}

inline sleeping_loop_a::sleeping_loop_a(sleeping_loop_a &&p_loop) {
#ifdef TENACITAS_LOG
  TNCT_LOG_TRA("sleeping loop ", m_id, " move constructor");
#endif
  move(std::move(p_loop));
}

inline bool sleeping_loop_a::is_stopped() const { return m_stopped; }

/// \brief Executes a function synchronoulsy with timeout control
/// The function may or may not return, and may or may not receive parameters
///
/// \tparam t_function the function to be executed. It may not return, as
///
/// <tt>void t_function(std::function<bool()> p_timeout, t_params...
/// p_params)</tt> or it may return, as in
///
/// <tt>some-ret t_function(std::function<bool()> p_timeout, t_params...
/// p_params)</tt>
///
/// where <tt>p_timeout</tt> informs that the function has exceeded the maximum
/// time of execution, and can stop executing
///
/// So, \p t_function should eventually call for <tt>p_timeout()</tt> to decide
/// if it must continue to execute_a
///
/// \tparam t_params possible parameters of t_function
///
/// \param p_max_time is the maximum amount of time that \p p_function should
/// take to execute_a
///
/// \param p_function is a function with one of the signatures described above
///
/// \param p_params are the possible arguments for \p p_function
///
/// \attention Please, take a look at
/// <tt>tenacitas.lib.async/exp/executer_000/main.cpp</tt> for examples
template <typename t_function, typename... t_params>
inline std::conditional_t<
    // if 't_function' return type is 'void'
    std::is_void_v<
        std::invoke_result_t<t_function, std::function<bool()>, t_params...>>,
    // the 'execute' wrapper will return 'bool', which will be 'true' if the
    // 'p_function' executes in less 'p_max_time', or 'false' otherwise
    bool,
    // else it will result a 'std::optional' with the return type of
    // 't_function', which will contain a value of that type, if the
    // 'p_function' executes in less 'p_max_time', or empty otherwise
    std::optional<
        std::invoke_result_t<t_function, std::function<bool()>, t_params...>>>
execute_a(convertible_to_nano_c auto p_max_time, t_function &p_function,
          t_params &&...p_params) {
  std::mutex _mutex;
  std::condition_variable _cond;
  bool _timeout{false};

  auto _is_timeout = [&_timeout]() { return _timeout; };

  using t_ret = typename std::invoke_result_t<t_function, std::function<bool()>,
                                              t_params...>;

  auto _ns{std::chrono::duration_cast<std::chrono::nanoseconds>(p_max_time)};

  if constexpr (std::is_void_v<t_ret>) {
    std::thread _th([&]() -> void {
      p_function(_is_timeout, std::forward<t_params>(p_params)...);
      _cond.notify_one();
    });

    std::unique_lock<std::mutex> _lock{_mutex};
    if (_cond.wait_for(_lock, _ns) != std::cv_status::timeout) {
      //      _th.join();
      _th.detach();
      return true;
    }
    _timeout = true;
    _th.join();
    return false;
  } else {
    t_ret _ret;

    std::thread _th([&]() -> void {
      _ret = p_function(_is_timeout, std::forward<t_params>(p_params)...);
      _cond.notify_one();
    });

    std::unique_lock<std::mutex> _lock{_mutex};
    if (_cond.wait_for(_lock, _ns) != std::cv_status::timeout) {
      //      _th.join();
      _th.detach();
      return {std::move(_ret)};
    }
    _timeout = true;
    _th.join();
    std::optional<t_ret> _aux{};
    return _aux;
  }
};

/** \brief Class that allows creation of queues for an event, creation of
subscribers for an event, and publising of events


The UML below, written in www.plantuml.com, shows the conceptual structure of
the \p dispatcher_a and other associated structures

@startuml
hide empty members
allow_mixing
skinparam linetype ortho


class dispatcher_a<t_event...>
class queue<t_event>
class subscriber
class event

dispatcher_a -[#green]->  event
dispatcher_a -[#blue]->  event
queue *-- "*" event
subscriber .right.>  "handles" event
queue *-- priority
dispatcher_a *-- "*" queue

note "<color green>publishes\n<color blue>subscribes" as legend
@enduml

The central concept is an \p event, a struct that contains data about an
interesting event in the application. It can be, for example, a incoming message
from a network connection, a user menu choice or a temperature coming from a
sensor.

A \p tenacitas::lib::subscriber is a function object that will
handle a \p t_event object.

A \p queue is where \p event objects will be queued for subscibers objects to
access them. For example, a user menu choice can generate a log message, a
message sent to a remote program, and to change the color of widget in the user
interface.

Besides, it is possible to define, for example, one single subscriber for the
logging queue, five subscribers for the network message queue, and
two for the widget color changing. When the user makes its menu choice, the five
\p tenacitas::lib::subscriber objects in the message network queue
will "fight each other" to get the event to handle; the same for the two \p
tenacitas::lib::subscriber in the color changing queue.

It is possible to define a priority between the queues, so that an event
is delivered to the highest priority queue before the others.

The \p dispatcher_a class is responsible for managing the creation of
queue, adding \p tenacitas::lib::subscriber to the queues, and
publish event objects to the queues.

Please, look at the \p Examples section for examples on how to use these
functions and classes.
*/
template <event_c... t_events> struct dispatcher_a {

  using ptr = std::shared_ptr<dispatcher_a>;

  using events = std::tuple<t_events...>;

  static ptr create() { return ptr(new dispatcher_a()); }

  dispatcher_a(const dispatcher_a &) = delete;
  dispatcher_a(dispatcher_a &&) = delete;
  dispatcher_a &operator=(const dispatcher_a &) = delete;
  dispatcher_a &operator=(dispatcher_a &&) = delete;

  ~dispatcher_a() {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("destructor");
#endif
    // stop();
  }

  template <event_c t_event>
  queue_id_t add_queue(priority p_priority = priority ::medium) {

    constexpr size_t _event_index{
        assert_event_defined_in_dispatcher<t_event>()};

    std::lock_guard<std::mutex> _lock(m_event_queue_list_mutex[_event_index]);

    event_queue_list<t_event> &_event_queue_list{
        std::get<_event_index>(m_event_queue_list_tuple)};

    event_queue_list_iterator<t_event> _ite{
        add_event_queue<t_event>(_event_queue_list, p_priority)};

    queue_id_t _queue_id{_ite->get_id()};

    sort_event_queue_list(_event_queue_list);

    return _queue_id;
  }

  template <subscriber_c t_subscriber, event_c t_event>
  queue_id_t subscribe(subscriber_a<t_event> p_subscriber,
                       priority p_priority = priority ::medium) {

    assert_event_defined_in_subscriber<t_subscriber, t_event>();

    constexpr auto _event_index{assert_event_defined_in_dispatcher<t_event>()};

    std::lock_guard<std::mutex> _lock(m_event_queue_list_mutex[_event_index]);

    event_queue_list<t_event> &_event_queue_list{
        std::get<_event_index>(m_event_queue_list_tuple)};

    event_queue_list_iterator<t_event> _ite{
        add_event_queue<t_event>(_event_queue_list, p_priority)};

    queue_id_t _queue_id{_ite->get_id()};

    _ite->add_subscriber(p_subscriber);

    sort_event_queue_list<t_event>(_event_queue_list);

    return _queue_id;
  }

  template <subscriber_c t_subscriber, event_c t_event>
  void subscribe(const queue_id_t &p_queue_id,
                 subscriber_a<t_event> p_subscriber) {
    assert_event_defined_in_subscriber<t_subscriber, t_event>();

    constexpr auto _event_index{assert_event_defined_in_dispatcher<t_event>()};

    std::lock_guard<std::mutex> _lock(m_event_queue_list_mutex[_event_index]);

    event_queue_list_iterator<t_event> _ite{
        find_event_queue<_event_index, t_event>(p_queue_id)};

    _ite->add_subscriber(p_subscriber);
  }

  /// \brief Adds a bunch of subscribers to an existing queue
  ///
  /// \tparam t_event is the type of event that will be handled by the \p
  /// p_subscriber in the existing queue
  ///
  /// \param p_id is the identifier of the queue
  ///
  /// \param p_num_workers defines the number of subscribers to be added
  ///
  /// \param p_factory is a function that creates subscribers
  template <subscriber_c t_subscriber, event_c t_event>
  void subscribe(const queue_id_t &p_queue_id,
                 std::unsigned_integral auto p_num_workers,
                 std::function<subscriber_a<t_event>()> p_factory) {

    assert_event_defined_in_subscriber<t_subscriber, t_event>();

    constexpr auto _event_index{assert_event_defined_in_dispatcher<t_event>()};

    std::lock_guard<std::mutex> _lock(m_event_queue_list_mutex[_event_index]);

    event_queue_list_iterator<t_event> _ite{
        find_event_queue<_event_index, t_event>(p_queue_id)};

    _ite->add_subscriber(p_num_workers, p_factory);
  }

  /// \brief Removes all queues of all events
  void clear() {
    //    typename event_queue_list_mutex::size_type _mutex_index{0};
    //    auto _function{[&](auto &p_event_queue_list) {
    //      for (auto &_event_queue : p_event_queue_list) {
    //        std::lock_guard<std::mutex> _lock(
    //            m_event_queue_list_mutex[_mutex_index++]);
    //        _event_queue.clear();
    //      }
    //    }};
    //    traverse(_function, m_event_queue_list_tuple);
    clear<0>();
  }

  template <event_c t_event> void clear() {
    constexpr auto _event_index{assert_event_defined_in_dispatcher<t_event>()};

    std::lock_guard<std::mutex> _lock(m_event_queue_list_mutex[_event_index]);

    event_queue_list<t_event> &_event_queue_list{
        std::get<_event_index>(m_event_queue_list_tuple)};

    for (auto &_event_queue : _event_queue_list) {
      _event_queue.clear();
    }
  }

  template <event_c t_event> void clear(queue_id_t p_queue_id) {

    constexpr auto _event_index{index_r<t_event, events>::value};

    event_queue_list<t_event> &_event_queue_list{
        std::get<_event_index>(m_event_queue_list_tuple)};

    for (auto &_event_queue : _event_queue_list) {
      if (_event_queue.get_id() == p_queue_id) {
        _event_queue.clear();
        break;
      }
    }
  }

  template <publisher_c t_publisher, event_c t_event>
  bool publish(const t_event &p_event) {
    assert_event_defined_in_publisher<t_publisher, t_event>();

    constexpr auto _events_queues_list_index{
        assert_event_defined_in_dispatcher<t_event>()};

    try {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("queue ", typeid(t_event).name());
#endif
      return internal_publish<_events_queues_list_index, t_event>(p_event);
    } catch (std::exception &_ex) {
#ifdef TENACITAS_LOG
      TNCT_LOG_ERR("error queue ", typeid(t_event).name(), ": '", _ex.what(),
                   '\'');
#endif
      return false;
    }
  }

  template <publisher_c t_publisher, event_c t_event>
  void publish_to_queue(queue_id_t p_queue_id, const t_event &p_event) {

    assert_event_defined_in_publisher<t_publisher, t_event>();

    constexpr auto _event_queues_list_index{
        assert_event_defined_in_dispatcher<t_event>()};

    event_queue_list_iterator<t_event> _ite{
        find_event_queue<_event_queues_list_index, t_event>(p_queue_id)};

    _ite->add_event(p_event);
  }

  template <publisher_c t_publisher, event_c t_event, typename... t_params>
  bool publish(t_params &&...p_params) {
    assert_event_defined_in_publisher<t_publisher, t_event>();

    constexpr auto _event_queues_list_index{
        assert_event_defined_in_dispatcher<t_event>()};

    try {
      t_event _event{std::forward<t_params>(p_params)...};
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("event to publish: ", typeid(t_event).name());
#endif
      return internal_publish<_event_queues_list_index, t_event>(_event);
    } catch (std::exception &_ex) {
#ifdef TENACITAS_LOG
      TNCT_LOG_ERR("error queue event '", _ex.what(), '\'');
#endif
      return false;
    }
  }

  template <event_c t_event>
  void set_priority(const queue_id_t &p_queue_id, priority p_priority) {
    constexpr auto _event_index{assert_event_defined_in_dispatcher<t_event>()};

    std::lock_guard<std::mutex> _lock(m_event_queue_list_mutex[_event_index]);

    event_queue_list_iterator<t_event> _ite{
        find_event_queue<_event_index, t_event>(p_queue_id)};

    _ite->set_priority(p_priority);
  }

  template <event_c t_event>
  priority get_priority(const queue_id_t &p_queue_id) {
    constexpr auto _event_queues_list_index{
        assert_event_defined_in_dispatcher<t_event>()};

    event_queue_list_iterator<t_event> _ite{
        find_event_queue<_event_queues_list_index, t_event>(p_queue_id)};

    return _ite->get_priority();
  }

  /// \brief Retrieves the size of the a queue of events
  ///
  /// \param p_id is the identifier of the queue
  ///
  /// \return the size of the event queue or 0 if no queue was found
  template <event_c t_event> size_t queue_size(const queue_id_t &p_queue_id) {

    constexpr auto _event_queues_list_index{
        assert_event_defined_in_dispatcher<t_event>()};

    event_queue_list_iterator<t_event> _ite{
        find_event_queue<_event_queues_list_index, t_event>(p_queue_id)};

    return _ite->get_size();
  }

  /// \brief Retrieves how many positions in the queue of events for a
  /// queue are occupied
  ///
  /// \param p_id is the identifier of the queue
  ///
  /// \return the number of occupied positions or 0 if no queue was found
  template <event_c t_event>
  size_t occupied_in_queue(const queue_id_t &p_queue_id) {
    constexpr auto _event_queues_list_index{
        assert_event_defined_in_dispatcher<t_event>()};

    event_queue_list_iterator<t_event> _ite{
        find_event_queue<_event_queues_list_index, t_event>(p_queue_id)};

    return _ite->get_occupied();
  }

  template <event_c t_event> size_t amount_of_queues() const {
    constexpr auto _event_queues_list_index{
        assert_event_defined_in_dispatcher<t_event>()};

    const auto &_event_queues{
        std::get<_event_queues_list_index>(m_event_queue_list_tuple)};

    return _event_queues.size();
  }

  //  /// \brief Removes all events in all the queues of \p t_event
  //  template <event_c t_event> void empty_queue();

  //  /// \brief Removes all events in all the queues of \p t_event
  //  template <event_c t_event> void empty_queue(queue_id p_queue_id);

  // \brief Waits for all the events in all the groups of  subscribers to be
  // handled
  //
  // TODO test it
  //  void wait() {
  //    for (const auto &_value_type : m_events_queues) {
  //      for (const queue_ptr &_queue_ptr : _value_type.second) {
  //                    _queue_ptr->empty_queue();
  //      }
  //    }
  //        TNCT_LOG_TRA("dispatcher_a ", get_id(), " - finished waiting");
  //  }

private:
  dispatcher_a() = default;

private:
  template <typename t_event>
  using event_queue = internal::event_queue_t<t_event>;

  template <typename t_event>
  using event_queue_list = std::list<event_queue<t_event>>;

  using event_queue_list_tuple =
      std::tuple<std::list<internal::event_queue_t<t_events>>...>;

  template <typename t_event>
  using event_queue_list_iterator =
      typename event_queue_list<t_event>::iterator;

  template <event_c t_event> struct event_mutex { std::mutex mutex; };

  using event_queue_list_mutex =
      std::array<std::mutex, std::tuple_size_v<events>>;

private:
  template <size_t t_idx = 0> void clear() {
    if constexpr (t_idx >= std::tuple_size_v<events>) {
      return;
    } else {
      std::lock_guard<std::mutex> _lock(m_event_queue_list_mutex[t_idx]);

      std::get<t_idx>(m_event_queue_list_tuple).clear();

      return clear<t_idx + 1>();
    }
  }

  /// \brief Stops all the queues
  // void stop() { stop_all(std::make_index_sequence<sizeof...(t_events)>{}); }

  template <size_t t_event_index, event_c t_event>
  event_queue_list_iterator<t_event> find_event_queue(queue_id_t p_queue_id) {

    event_queue_list<t_event> &_event_queue_list{
        std::get<t_event_index>(m_event_queue_list_tuple)};

    return find_event_queue(_event_queue_list, p_queue_id);
  }

  template <size_t t_event_queues_index, typename t_event>
  bool internal_publish(const t_event &p_event) {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("internal queue ", typeid(t_event).name());
#endif

    event_queue_list<t_event> &_event_queues{
        std::get<t_event_queues_index>(m_event_queue_list_tuple)};
    for (event_queue<t_event> &_event_queue : _event_queues) {
      _event_queue.add_event(p_event);
    }
    return true;
  }

  template <event_c t_event>
  event_queue_list_iterator<t_event>
  add_event_queue(event_queue_list<t_event> &p_event_queue_list,
                  priority p_priority) {

    event_queue<t_event> _event_queue{p_priority};

    p_event_queue_list.push_back(std::move(_event_queue));

    return std::prev(p_event_queue_list.end());
  }

  template <event_c t_event>
  static constexpr size_t assert_event_defined_in_dispatcher() {
    constexpr auto _index{index_r<t_event, events>::value};
    static_assert(_index != -1,
                  "trying to subscribe to an event not defined in dispatcher");
    return static_cast<size_t>(_index);
  }

  template <subscriber_c t_subscriber, event_c t_event>
  static constexpr void assert_event_defined_in_subscriber() {
    using events_subscribed = typename t_subscriber::events_subscribed;

    static_assert(
        index_r<t_event, events_subscribed>::value != -1,
        "trying to subscribe to an event not in 'events_subscribed' of the "
        "class that wants to subscribe for it");
  }

  template <publisher_c t_publisher, event_c t_event>
  constexpr void assert_event_defined_in_publisher() const {
    using events_published = typename t_publisher::events_published;

    static_assert(index_r<t_event, events_published>::value != -1,
                  "trying to publish an event not in 'events_published' of the "
                  "class that wants to publish it");
  }

  template <event_c t_event>
  void sort_event_queue_list(event_queue_list<t_event> &p_event_queue_list) {
    //    std::sort(p_event_queue_list.begin(), p_event_queue_list.end(),
    //              [](const event_queue<t_event> &p_event_queue_left,
    //                 const event_queue<t_event> &p_event_queue_right) {
    //                return p_event_queue_left.get_priority() <
    //                       p_event_queue_right.get_priority();
    //              });
    p_event_queue_list.sort(
        [](const event_queue<t_event> &p_event_queue_left,
           const event_queue<t_event> &p_event_queue_right) {
          return p_event_queue_left.get_priority() <
                 p_event_queue_right.get_priority();
        });
  }

  template <event_c t_event>
  event_queue_list_iterator<t_event>
  find_event_queue(event_queue_list<t_event> &p_event_queue_list,
                   queue_id_t p_queue_id) {

    event_queue_list_iterator<t_event> _ite{std::find_if(
        p_event_queue_list.begin(), p_event_queue_list.end(),
        [&](const auto &p_queue) { return p_queue.get_id() == p_queue_id; })};

    if (_ite == p_event_queue_list.end()) {
      std::stringstream _stream;
      _stream << "event '" << typeid(t_event).name()
              << "' - no queue found for queue id " << p_queue_id;
      const std::string _str{_stream.str()};
#ifdef TENACITAS_LOG
      TNCT_LOG_FAT(_str);
#endif
      throw std::runtime_error(_str);
    }
    return _ite;
  }

private:
  event_queue_list_tuple m_event_queue_list_tuple;

  event_queue_list_mutex m_event_queue_list_mutex;
};

template <typename t_ret, typename t_val>
std::optional<t_ret> factorial(t_val p_val) {
  t_ret ret = 1;

  for (t_val _i = 1; _i <= p_val; ++_i) {
    ret *= _i;
  }

  return {ret};
}

template <typename t_int, typename t_data> struct matrix_a {
  using index = t_int;
  using data = t_data;

  matrix_a() = default;
  matrix_a(const matrix_a &p_matrix)
      : m_num_rows(p_matrix.m_num_rows), m_num_cols(p_matrix.m_num_cols),
        m_initial(p_matrix.m_initial),
        m_vec(new t_data[m_num_cols * m_num_rows]) {
    std::memcpy(m_vec.get(), p_matrix.m_vec.get(), m_num_rows * m_num_cols);
    //        std::copy(&p_matrix.m_vec[0], &p_matrix.m_vec[m_num_rows *
    //        m_num_cols], &m_vec[0]);
  }

  matrix_a(matrix_a &&p_matrix)
      : m_num_rows(p_matrix.m_num_rows), m_num_cols(p_matrix.m_num_cols),
        m_initial(p_matrix.m_initial), m_vec(std::move(p_matrix.m_vec)) {}

  matrix_a(t_int p_num_rows, t_int p_num_cols, t_data p_initial)
      : m_num_rows(p_num_rows), m_num_cols(p_num_cols), m_initial(p_initial),
        m_vec(new t_data[m_num_cols * m_num_rows]) {
    reset();
  }

  ~matrix_a() = default;

  matrix_a &operator=(const matrix_a &p_matrix) {
    if (this != &p_matrix) {
      m_initial = p_matrix.m_initial;
      m_num_rows = p_matrix.m_num_rows;
      m_num_cols = p_matrix.m_num_cols;
      m_vec = std::unique_ptr<t_data>(new t_data[m_num_cols * m_num_rows]);
      std::memcpy(m_vec.get(), p_matrix.m_vec.get(), m_num_rows * m_num_cols);
    }
    return *this;
  }

  matrix_a &operator=(matrix_a &&p_matrix) {
    if (this != &p_matrix) {
      m_vec = std::move(p_matrix.m_vec);
      m_initial = p_matrix.m_initial;
      m_num_rows = p_matrix.m_num_rows;
      m_num_cols = p_matrix.m_num_cols;
    }
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const matrix_a &p_matrix) {
    if (!p_matrix.m_vec) {
      return p_out;
    }

    p_out << '\n';
    for (t_int _row = 0; _row < p_matrix.get_num_rows(); ++_row) {
      for (t_int _col = 0; _col < p_matrix.get_num_cols(); ++_col) {
        p_out << p_matrix(_row, _col) << ' ';
      }
      p_out << '\n';
    }
    return p_out;
  }

  inline t_data &operator()(t_int p_row, t_int p_col) {
    if (!m_vec) {
      throw std::runtime_error("matrix without rows or cols");
    }
    return m_vec.get()[(p_row * m_num_cols) + p_col];
  }

  inline const t_data &operator()(t_int p_row, t_int p_col) const {
    if (!m_vec) {
      throw std::runtime_error("matrix without rows or cols");
    }
    return m_vec.get()[(p_row * m_num_cols) + p_col];
  }

  inline t_int get_num_rows() const { return m_num_rows; }
  inline t_int get_num_cols() const { return m_num_cols; }

  void reset() {
    for (t_int _row = 0; _row < get_num_rows(); ++_row) {
      for (t_int _col = 0; _col < get_num_cols(); ++_col) {
        (*this)(_row, _col) = m_initial;
      }
    }
  }

private:
  t_int m_num_rows{0};
  t_int m_num_cols{0};

  t_data m_initial;
  std::unique_ptr<t_data> m_vec;
};

using translation_entry_t = uint32_t;

using dictionary_t = std::map<translation_entry_t, std::string>;

inline std::ostream &operator<<(std::ostream &p_out,
                                const dictionary_t &p_dictionary) {
  for (const dictionary_t::value_type &p_value : p_dictionary) {
    p_out << '(' << p_value.first << ',' << p_value.second << ')';
  }
  return p_out;
}

using translate_a = std::function<std::string(
    const translation_entry_t &p_entry, std::string_view p_default)>;

struct translator_from_file_a {
  translator_from_file_a() = default;
  translator_from_file_a(const translator_from_file_a &) = delete;
  translator_from_file_a(translator_from_file_a &&) = delete;
  ~translator_from_file_a() = default;

  translator_from_file_a &operator=(const translator_from_file_a &) = delete;
  translator_from_file_a &operator=(translator_from_file_a &&) = delete;

  uint32_t inline size() const { return m_dictionary.size(); }

  std::string inline get(translation_entry_t p_word_id) {
    return m_dictionary[p_word_id];
  }

  bool load(const std::string &p_file_name) {
    const std::regex _regex{R"(^\s*(\d*)\s*\|(.*)$)"};

    dictionary_t _dictionary;

    try {
      std::ifstream _file(p_file_name);
      if (!_file.good()) {
#ifdef TENACITAS_LOG
        TNCT_LOG_ERR("translation file '", p_file_name, "' not found");
#endif
        return false;
      }

      std::string _line;

      std::getline(_file, _line);
      while (!_file.eof()) {
        if (_line.empty()) {
          continue;
        }

        std::smatch _matches;
        std::regex_search(_line, _matches, _regex);
        if (_matches.size() != 3) {
          continue;
        }
        _dictionary[std::stoul(_matches[1].str())] = _matches[2].str();
        std::getline(_file, _line);
      }
      m_dictionary = std::move(_dictionary);
      return true;

    } catch (std::exception &_ex) {
#ifdef TENACITAS_LOG
      TNCT_LOG_ERR("Loading translation file '", p_file_name, "': '",
                   _ex.what(), '\'');
#endif
    }
    return false;
  }

  friend inline std::ostream &
  operator<<(std::ostream &p_out, const translator_from_file_a &p_translator) {
    for (const dictionary_t::value_type &p_value : p_translator.m_dictionary) {
      p_out << '(' << p_value.first << ',' << p_value.second << ')';
    }

    return p_out;
  }

private:
  dictionary_t m_dictionary;
};

struct translator_in_memory_a {
  translator_in_memory_a() = default;
  translator_in_memory_a(dictionary_t &&p_dictionary)
      : m_dictionary(std::move(p_dictionary)) {}
  translator_in_memory_a(const translator_in_memory_a &) = delete;
  translator_in_memory_a(translator_in_memory_a &&) = delete;
  ~translator_in_memory_a() = default;

  translator_in_memory_a &operator=(const translator_in_memory_a &) = delete;
  translator_in_memory_a &operator=(translator_in_memory_a &&) = delete;

  uint32_t inline size() const { return m_dictionary.size(); }

  inline std::string get(translation_entry_t p_word_id) {
    return m_dictionary[p_word_id];
  }

  void inline add(translation_entry_t p_entry, std::string_view p_str) {
    m_dictionary[p_entry] = p_str;
  }

  void inline add(dictionary_t &&p_dictionary) {
    for (const dictionary_t::value_type &_value : p_dictionary) {
      m_dictionary[_value.first] = _value.second;
    }
  }

  friend inline std::ostream &
  operator<<(std::ostream &p_out, const translator_in_memory_a &p_translator) {
    for (const dictionary_t::value_type &p_value : p_translator.m_dictionary) {
      p_out << '(' << p_value.first << ',' << p_value.second << ')';
    }

    return p_out;
  }

private:
  dictionary_t m_dictionary;
};

} // namespace tenacitas::lib

namespace tnctl = tenacitas::lib;

#endif
