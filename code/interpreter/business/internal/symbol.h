#ifndef TENACITAS_INTERPRETER_BUSINESS_SYMBOL_H
#define TENACITAS_INTERPRETER_BUSINESS_SYMBOL_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <set>
#include <string>

#include <interpreter/business/internal/lexeme.h>
#include <interpreter/business/internal/type.h>

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
        static const std::string _space = " ";
        static const std::string _open = "[";
        static const std::string _close = "]";
        p_out << _open << p_symbol.m_lexeme << _space << p_symbol.m_type
              << _close;
        return p_out;
    }

    /// \brief
    inline symbol() = default;

    /// \brief
    inline explicit symbol(const lexeme& p_lexeme, const type& p_type)
      : m_lexeme(p_lexeme)
      , m_type(p_type)
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

    /// \brief Very special, indicating that all the input text was analysed
    static const lexeme eot;

  private:
    /// \brief
    static const lexeme m_dummy_lexeme;

    /// \brief
    static const type m_dummy_type;

    /// \brief
    lexeme m_lexeme = m_dummy_lexeme;

    /// \brief
    type m_type = m_dummy_type;
};

///
/// \brief symbols
///
struct symbols
{
    /// \brief
    typedef std::set<symbol>::const_iterator const_iterator;

    ///
    /// \brief operator <<
    /// \param p_out
    /// \param p_symbols
    /// \return
    ///
    inline friend std::ostream& operator<<(std::ostream& p_out,
                                           const symbols& p_symbols)
    {
        static const std::string _space(" ");
        for (const symbol& _symbol : p_symbols.m_set) {
            p_out << _symbol << _space;
        }
        return p_out;
    }

    /// \brief
    symbols() = default;

    /// \brief not allowed
    symbols(const symbols&) = default;

    /// \brief not allowed
    symbols(symbols&&) = default;

    /// \brief Destructor
    ~symbols() = default;

    /// \brief not allowed
    symbols& operator=(const symbols&) = default;

    /// \brief not allowed
    symbols& operator=(symbols&&) = default;

    /// \brief not allowed
    void* operator new[](size_t) = delete;

    /// \brief not allowed
    void operator delete[](void*) = delete;

    /// \brief not allowed
    void operator delete(void* p) = delete;

    /// \brief not allowed
    void* operator new(size_t) = delete;

    /// \brief
    inline void add(symbol&& p_symbol) { m_set.insert(std::move(p_symbol)); }

    /// \brief
    const_iterator begin() const { return m_set.begin(); }

    /// \brief
    const_iterator end() const { return m_set.end(); }

  private:
    ///
    /// \brief m_set
    ///
    std::set<symbol> m_set;
};

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
