#ifndef TENACITAS_INTERPRETER_BUSINESS_SYMBOL_H
#define TENACITAS_INTERPRETER_BUSINESS_SYMBOL_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

#include <interpreter/business/internal/lexeme.h>
#include <interpreter/business/internal/type.h>
#include <interpreter/business/internal/types.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the business
namespace business {

/// \brief symbol Represents a symbol read from the text being interpreted
///
///  A symbol contains a lexeme, which is the string read from the text being
///  analysed.
///
///  A symbol has a type, like "integer", "variable_identifier" or
///  "reserved_word". The type of a symbol is determined by a
///  tenacitas::interpreter::scanner object.
struct symbol
{

  public:
    ///
    /// \brief operator <<
    /// \param p_out
    /// \param p_symbol
    /// \return
    ///
    inline friend std::ostream& operator<<(std::ostream& p_out,
                                           const symbol& p_symbol)
    {
        static const char* _space = " ";
        static const char* _open = "[";
        static const char* _close = "]";
        p_out << _open << p_symbol.m_lexeme << _space << *(p_symbol.m_type_ite)
              << _close << _space;
        return p_out;
    }

    /// \brief
    inline symbol() = delete;

    /// \brief
    inline explicit symbol(const lexeme& p_lexeme, types::const_iterator p_type)
      : m_lexeme(p_lexeme)
      , m_type_ite(p_type)
    {}

    /// \brief not allowed
    symbol(const symbol&) = default;

    /// \brief not allowed
    symbol(symbol&&) = default;

    /// \brief Destructor
    ~symbol() = default;

    /// \brief not allowed
    symbol& operator=(const symbol&) = default;

    /// \brief not allowed
    symbol& operator=(symbol&&) = default;

    /// \brief equal-to
    inline bool operator==(const symbol& p_symbol) const
    {
        return ((m_lexeme == p_symbol.m_lexeme) &&
                (*m_type_ite == *(p_symbol.m_type_ite)));
    }

    /// \brief not-equal-to
    inline bool operator!=(const symbol& p_symbol) const
    {
        return ((m_lexeme != p_symbol.m_lexeme) ||
                (*m_type_ite != *(p_symbol.m_type_ite)));
    }

    /// \brief less-than
    inline bool operator<(const symbol& p_symbol) const
    {
        if (m_lexeme < p_symbol.m_lexeme) {
            return true;
        }
        if (m_lexeme > p_symbol.m_lexeme) {
            return false;
        }
        if ((*m_type_ite) < *(p_symbol.m_type_ite)) {
            return true;
        }
        return false;
    }

    /// \brief not allowed
    void* operator new[](size_t) = delete;

    /// \brief not allowed
    void operator delete[](void*) = delete;

    /// \brief not allowed
    void operator delete(void* p) = delete;

    /// \brief not allowed
    void* operator new(size_t) = delete;

    ///
    /// \brief get_lexeme
    /// \return the @p lexeme associated to the @p symbol
    ///
    inline lexeme get_lexeme() const { return m_lexeme; }

    ///
    /// \brief get_type
    /// \return the @p type associated to the @p symbol
    ///
    inline type get_type() const { return *m_type_ite; }

  private:
    /// \brief
    lexeme m_lexeme;

    /// \brief
    types::const_iterator m_type_ite;
};

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
