#ifndef TENACITAS_INTERPRETER_BUSINESS_SYMBOL_H
#define TENACITAS_INTERPRETER_BUSINESS_SYMBOL_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <list>
#include <string>

#include <interpreter/business/type.h>
#include <interpreter/business/value.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the business
namespace business {

/// \brief symbol Represents a symbol read from the text being interpreted
///
///  A symbol contains a value, usualy called lexema, which is the string
///  read from the text being analysed.
///
///  A symbol has a type, like "integer", "variable_identifier" or
///  "reserved_word". The type of a symbol is determined by a
///  tenacitas::interpreter::scanner object.
struct symbol
{

  public:
    /// \brief
    inline symbol();

    /// \brief
    inline explicit symbol(const value& p_value, const type& p_type);

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
    inline bool operator==(const symbol& p_symbol) const;

    /// \brief not-equal-to
    inline bool operator!=(const symbol& p_symbol) const;

    /// \brief less-than
    inline bool operator<(const symbol& p_symbol) const;

    /// \brief not allowed
    void* operator new[](size_t) = delete;

    /// \brief not allowed
    void operator delete[](void*) = delete;

    /// \brief not allowed
    void operator delete(void* p) = delete;

    /// \brief not allowed
    void* operator new(size_t) = delete;

    /// \brief Very special, indicating that all the input text was analysed
    static const value eot;

  private:
    /// \brief
    static const value m_dummy_value;

    /// \brief
    static const type m_dummy_type;

    /// \brief
    value m_value = m_dummy_value;

    /// \brief
    type m_type = m_dummy_type;
};

///
/// \brief symbols
///
struct symbols
{
    /// \brief
    typedef std::list<value>::const_iterator const_iterator;

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
    void push_back(symbol&& p_symbol);

    /// \brief
    const_iterator begin() const;

    /// \brief
    const_iterator end() const;

  private:
    std::list<symbol> m_symbols;
};

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
