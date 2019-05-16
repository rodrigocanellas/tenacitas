#ifndef TENACITAS_INTERPRETER_BUSINESS_TOKEN_H
#define TENACITAS_INTERPRETER_BUSINESS_TOKEN_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE
/// file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <cstdint>
#include <set>
#include <stdexcept>
#include <string>

#include <interpreter/business/internal/type.h>
#include <string/business/split_str.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the business
namespace business {

///
/// \brief The token struct associates a \p type to a set of string values, all
/// with the same size
///
/// \example a token with \p type 'relational-operator', and string values 2
/// bytes long: <= >=
///
/// \example a token with \p type 'relational-operator', and string values 1
/// bytes long: < >
///
/// \example a token with \p type 'reserved-word', and string values 3 bytes
/// long: for
///
/// \example a token with \p type 'reserved-word', and string values 2 bytes
/// long: if
///
struct token
{

    /// \brief default ctor not allowed
    token() = delete;

    ///
    /// \brief token
    /// \param p_size size of the token
    /// \param p_type type of the token, like 'relational-operator'
    ///
    inline token(uint8_t p_size, const type& p_type)
      : m_size(p_size)
      , m_type(p_type)
      , m_strings()
    {}

    /// \brief destructor
    ~token() = default;

    /// \brief token copy ctor not allowed
    token(const token&) = delete;

    /// \brief token move ctor
    token(token&& p_token_size) noexcept = default;

    /// \brief size returns the size of the string values recognized by this \p
    /// token
    inline uint8_t get_size() const { return m_size; }

    /// \brief get_type returns the \p type associated to the string values
    /// recognized by this \p token \return
    inline const type& get_type() const { return m_type; }

    /// \brief recognize checks if a string value is recognized by this \p token
    /// \param p_str the string value to be checked
    /// \return \p type::undefined if \p p_str is not recognized, otherwise a
    /// valid \p type
    inline type recognize(const std::string& p_str) const
    {
        return (m_strings.find(p_str) != m_strings.end() ? m_type
                                                         : type::undefined);
    }

    /// \brief add adds a value to the ser of values that are recognized by this
    /// \p token
    inline void add(const std::string& p_str)
    {
        if (static_cast<uint8_t>(p_str.size()) == m_size) {
            m_strings.emplace(p_str);
        }
    }

    /// \brief token_size copy operator not allowed
    token& operator=(const token&) = delete;

    /// \brief operator = move operator
    token& operator=(token&& p_token) noexcept = default;

    /// \brief operator == two \p token are equal if theirs size and type are
    /// equal
    inline bool operator==(const token& p_token) const
    {
        return ((m_size == p_token.m_size) && (m_type == p_token.m_type));
    }

    /// \brief operator != two \p token objects are not equal if theirs size or
    /// type are not
    inline bool operator!=(const token& p_token) const
    {
        return ((m_size != p_token.m_size) || (m_type != p_token.m_type));
    }

    /// \brief operator <
    inline bool operator<(const token& p_token) const
    {
        if (m_size > p_token.m_size) {
            return false;
        }
        if (m_size < p_token.m_size) {
            return true;
        }
        if (m_type < p_token.m_type) {
            return true;
        }
        return false;
    }

  private:
    /// \brief strings type for the set of strings
    typedef std::set<std::string> strings;

  private:
    /// \brief m_size size of the token
    uint8_t m_size;

    /// \brief m_type type of the token
    type m_type;

    /// \brief m_strings set of values that are recognized by this token
    strings m_strings;
};

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
