#ifndef TENACITAS_INTERPRETER_BUSINESS_SYMBOLS_H
#define TENACITAS_INTERPRETER_BUSINESS_SYMBOLS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <set>
#include <string>

#include <interpreter/business/internal/lexeme.h>
#include <interpreter/business/internal/symbol.h>
#include <interpreter/business/internal/type.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the business
namespace business {

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

    /// \brief
    inline bool empty() const { return m_set.empty(); }

    /// \brief
    inline int16_t size() const { return static_cast<int16_t>(m_set.size()); }

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
