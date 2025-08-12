/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_FORMAT_TST_FORMAT_NUMBER_TEST_H
#define TNCT_FORMAT_TST_FORMAT_NUMBER_TEST_H

#include <string>

#include "tnct/format/format_number.h"
#include "tnct/program/options.h"

namespace tnct::format::tst
{

struct format_number_000
{
  static std::string desc()
  {
    return "format a uint16_t number";
  }

  bool operator()(const program::options &)
  {
    const auto _str(format::format_max_number(static_cast<uint16_t>(34)));
    std::cerr << "str = " << _str << std::endl;
    return _str == "00034";
  }
};

struct format_number_001
{
  static std::string desc()
  {
    return "format a int16_t number";
  }

  bool operator()(const program::options &)
  {
    const auto _str(format::format_max_number(static_cast<int16_t>(-34), ' '));

    std::cerr << "str = '" << _str << '\'' << std::endl;
    return _str == "  -34";
  }
};
struct format_number_002
{
  static std::string desc()
  {
    return "format a uint8_t number";
  }

  bool operator()(const program::options &)
  {
    const auto _str(format::format_max_number(static_cast<uint8_t>(34)));

    std::cerr << "str = " << _str << std::endl;
    return _str == "034";
  }
};

struct format_number_003
{
  static std::string desc()
  {
    return "format a int8_t number";
  }

  bool operator()(const program::options &)
  {
    const auto _str(format::format_max_number(static_cast<int8_t>(-4), ' '));

    std::cerr << "str = '" << _str << '\'' << std::endl;
    return _str == " -4";
  }
};

struct format_number_004
{
  static std::string desc()
  {
    return "format a float number";
  }

  bool operator()(const program::options &)
  {
    const std::string _str(
        format::format_max_number(static_cast<float>(3.14), ' '));
    std::cerr << "str = '" << _str << '\'' << std::endl;
    return _str == "  3.140000";
  }
};

struct format_number_005
{
  static std::string desc()
  {
    return "format a double number";
  }

  bool operator()(const program::options &)
  {
    const std::string _str(
        format::format_max_number(static_cast<double>(3.14), ' '));
    std::cerr << "str = '" << _str << '\'' << std::endl;
    return _str == "            3.140000";
  }
};

struct format_number_006
{
  static std::string desc()
  {
    return "compiler error trying to format a string";
  }

  bool operator()(const program::options &)
  {
    // const std::string
    // _str(format::format_number<std::string>("-3"),'0');
    return true;
  }
};

struct format_number_007
{
  static std::string desc()
  {
    return "compiler error trying to format a class";
  }

  bool operator()(const program::options &)
  {
    // abc _abc;
    // const std::string _str(format::format_number<abc>(_abc),'0');
    return true;
  }

private:
  struct abc
  {
  };
};

struct format_number_008
{
  static std::string desc()
  {
    return "compiler error trying to format a enum of uint16_t";
  }

  bool operator()(const program::options &)
  {
    // abc _abc (abc::A);
    // const std::string _str(format::format_number<abc>(_abc),'0');

    return true;
  }

private:
  enum class abc : uint16_t
  {
    A = 0,
    B = 1,
    C = 2
  };
};

struct format_number_009
{
  static std::string desc()
  {
    return "format a uint32_t number";
  }

  bool operator()(const program::options &)
  {
    const auto _str(format::format_max_number(static_cast<uint32_t>(34)));
    std::cerr << "str = " << _str << std::endl;
    return _str == "0000000034";
  }
};

struct format_number_010
{
  static std::string desc()
  {
    return "format a int32_t number";
  }

  bool operator()(const program::options &)
  {
    const auto _str(format::format_max_number(static_cast<int32_t>(-34), ' '));
    std::cerr << "str = '" << _str << '\'' << std::endl;
    return _str == "       -34";
  }
};

struct format_number_011
{
  static std::string desc()
  {
    return "format a uint64_t number";
  }

  bool operator()(const program::options &)
  {
    const auto _str(format::format_max_number(static_cast<uint64_t>(34)));
    std::cerr << "str = '" << _str << '\'' << std::endl;
    return _str == "00000000000000000034";
  }
};

struct format_number_012
{
  static std::string desc()
  {
    return "format a int64_t number";
  }

  bool operator()(const program::options &)
  {
    const auto _str(format::format_max_number(static_cast<int64_t>(-34), ' '));
    std::cerr << "str = '" << _str << '\'' << std::endl;
    return _str == "                 -34";
  }
};

struct format_number_014
{
  static std::string desc()
  {
    return "format of 34 as uint64_t number in a 6 chars long string";
  }

  bool operator()(const program::options &)
  {
    const auto _str(format::format_fix_number(static_cast<uint64_t>(34), 6));
    std::cerr << "str = '" << _str << '\'' << std::endl;
    return _str == "000034";
  }
};

struct format_number_015
{
  static std::string desc()
  {
    return "format of 73290 as uint32_t number in a 4 chars long string";
  }

  bool operator()(const program::options &)
  {
    const auto _str(format::format_fix_number(static_cast<uint32_t>(73290), 4));
    std::cerr << "str = '" << _str << '\'' << std::endl;
    return _str == "73290";
  }
};

} // namespace tnct::format::tst

#endif
