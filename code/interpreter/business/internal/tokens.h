#ifndef TENACITAS_INTERPRETER_BUSINESS_SPECIAL_H
#define TENACITAS_INTERPRETER_BUSINESS_SPECIAL_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE
/// file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>

#include <interpreter/business/type.h>
#include <string/business/split_str.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the group
namespace business {

/// \brief specials allows to define symbols that will be recognized by value,
/// like '=' '<=' or '.'
struct tokens
{
    friend std::ostream& operator<<(std::ostream& p_out,
                                    const tokens& p_specials);

  public:
    /// \brief default constructor
    tokens() = default;

    /// \brief not allowed
    tokens(const tokens&) = delete;

    /// \brief not allowed
    tokens(tokens&&) = default;

    /// \brief destructor
    ~tokens() = default;

    /// \brief add adds a group of value strings encoded in a string
    /// \param p_type identifier of the group of specials, like
    /// 'relational-operator'
    /// \param p_specials the specials, like '> < <= >='
    void add(const type& p_type, const std::string& p_specials);

    /// \brief recognize tries to recognize a \p type associated to a string
    /// \param p_str string which \p type will try to be recognized, like '<='
    /// \return \p type::undefined, if no \p type was found associated to \p
    /// p_str, or a valid \p type otherwise
    type operator()(const std::string& p_str) const;

    /// \brief indicates is the set o specials is empty
    bool empty() const { return m_special_set.empty(); }

    /// \brief not allowed
    tokens& operator=(const tokens&) = delete;

    /// \brief not allowed
    tokens& operator=(tokens&&) = delete;

    /// \brief not allowed
    void* operator new[](size_t) = delete;

    /// \brief not allowed
    void operator delete[](void*) = delete;

    /// \brief not allowed
    void operator delete(void* p) = delete;

    /// \brief not allowed
    void* operator new(size_t) = delete;

  private:
    /// \brief string_set set of string values
    typedef std::set<std::string> string_set;

    /// \brief type_set set of string values associated to a type
    typedef std::map<type, string_set> type_set;

    /// \brief special_set set of types associated to a size
    typedef std::map<uint8_t, type_set> special_set;

  private:
    /// \brief m_special_set set of specials
    special_set m_special_set;
};
} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
