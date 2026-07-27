/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_INTERPRETER_DAT_FAULT_H
#define TNCT_INTERPRETER_DAT_FAULT_H

#include <sstream>
#include <string>
#include <string_view>

#include "tnct/interpreter/dat/column_number.h"
#include "tnct/interpreter/dat/error_code.h"
#include "tnct/interpreter/dat/line_number.h"

namespace tnct::interpreter::dat {

/// \brief A fault in the interpreter
class fault final {

public:
  fault(error_code p_error_code, std::string_view p_file_name,
        line_number p_line_number, column_number p_column_number)
      : m_error_code(p_error_code), m_line_number(p_line_number),
        m_column_number(p_column_number), m_file_name(p_file_name) {

    std::stringstream _stream;
    _stream << '{' << m_error_code << ", " << m_file_name << ", "
            << m_line_number << ", " << m_column_number << '}';
    m_what = _stream.str();
  }

  fault() = delete;
  fault(const fault &) = default;
  fault(fault &&) = default;

  /** @brief Destructor */
  ~fault() = default;

  fault &operator=(const fault &) = default;
  fault &operator=(fault &&) = default;

  error_code get_error_code() const { return m_error_code; }

  /** @brief Retrieves the line number where the fault occurred */
  line_number get_line() const { return m_line_number; };

  /** @brief Retrieves the column number where the fault occurred */
  column_number get_column() const { return m_column_number; };

  const std::string &get_file_name() const { return m_file_name; };

  const std::string what() const { return m_what; }

  friend std::ostream &operator<<(std::ostream &p_out, const fault &p_fault) {
    p_out << p_fault.what();
    return p_out;
  }

private:
  error_code m_error_code;

  line_number m_line_number;

  column_number m_column_number;

  std::string m_file_name;

  std::string m_what{"{}"};
};

} // namespace tnct::interpreter::dat

#endif
