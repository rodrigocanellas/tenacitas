#ifndef TENACITAS_STATUS_RESULT_H
#define TENACITAS_STATUS_RESULT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory"

/// \author Rodrigo Canellas - rodrigo.canellas@gmail.com
/// \date 06/2020

#include <cstdint>
#include <iostream>
#include <string>
#include <utility>

/// \brief namespace of the organization
namespace tenacitas {

/// \brief namespace of the owner
namespace status {

/// \brief owner is the owner of a set of results
typedef uint32_t owner;

/// \brief code associated to a result
typedef int32_t code;

/// \brief general is a owner that owns general result values
static const owner project{1592002363};

/// \brief result associates a \p owner to a \p code
struct result {

  /// \brief constructor is set to a \p status::id owner, and code 0, if none
  /// is defined
  inline result() : m_owner(project), m_code(0) {}

  /// \brief constructor is set to a \p general owner, and \p ok code, if none
  /// is defined
  inline result(owner p_owner, code p_code)
      : m_owner(p_owner), m_code(p_code) {}

  inline result(result &&p_result) = default;

  inline result(const result &p_result) = default;

  result &operator=(result &&) = default;

  inline result &operator=(const result &) = default;

  ~result() = default;

  /// \brief retrieves the code of this result
  inline code get_code() { return m_code; }

  /// \brief retrieves the owner associated to this result
  inline code get_owner() { return m_owner; }

  inline bool operator==(const result &p_result) const {
    return (m_owner == p_result.m_owner) && (m_code == p_result.m_code);
  }

  inline bool operator!=(const result &p_result) const {
    return (m_owner != p_result.m_owner) || (m_code != p_result.m_code);
  }

  friend std::ostream &operator<<(std::ostream &p_out, const result &p_result) {
    p_out << "[" << p_result.m_owner << "," << p_result.m_code << "]";
    return p_out;
  }

private:
  owner m_owner;
  code m_code;
};

/// \brief code for results with no error
static const result ok{project, 0};

/// \brief code for results with some error
static const result error{project, -1};

} // namespace status
} // namespace tenacitas

#endif
