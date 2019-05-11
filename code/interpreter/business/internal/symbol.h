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
///
struct symbol
{

  public:
    ///
    /// \brief operator <<
    /// \param p_out
    /// \param p_symbol
    /// \return
    ///
    friend std::ostream& operator<<(std::ostream& p_out,
                                    const symbol& p_symbol);

    /// \brief default ctor not allowed
    inline symbol() = delete;

    /// \brief
    inline explicit symbol(lexeme&& p_lexeme, type&& p_type)
      : m_lexeme(std::move(p_lexeme))
      , m_type(std::move(p_type))
    {}

    /// \brief not allowed
    symbol(const symbol&) = delete;

    /// \brief not allowed
    symbol(symbol&&) = default;

    /// \brief Destructor
    ~symbol() = default;

    /// \brief not allowed
    symbol& operator=(const symbol&) = delete;

    /// \brief not allowed
    symbol& operator=(symbol&&) = delete;

    /// \brief equal-to
    inline bool operator==(const symbol& p_symbol) const
    {
        return ((m_lexeme == p_symbol.m_lexeme) && (m_type == p_symbol.m_type));
    }

    /// \brief not-equal-to
    inline bool operator!=(const symbol& p_symbol) const
    {
        return ((m_lexeme != p_symbol.m_lexeme) || (m_type != p_symbol.m_type));
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
        if (m_type < p_symbol.m_type) {
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
    inline type get_type() const { return m_type; }

  private:
    /// \brief
    lexeme m_lexeme;

    /// \brief
    type m_type;
};

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
