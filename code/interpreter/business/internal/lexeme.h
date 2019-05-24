#ifndef TENACITAS_INTERPRETER_BUSINESS_LEXEME_H
#define TENACITAS_INTERPRETER_BUSINESS_LEXEME_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <cstdint>
#include <set>
#include <string>

#include <string/business/split_str.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the group
namespace business {

///
/// \brief The lexeme struct represents a word read from input text
///
struct lexeme
{

    ///
    /// \brief const_iterator
    ///
    typedef std::string::const_iterator const_iterator;

    ///
    /// \brief operator <<
    /// \param p_out
    /// \param p_lexeme
    /// \return
    ///
    inline friend std::ostream& operator<<(std::ostream& p_out,
                                           const lexeme& p_lexeme)
    {
        p_out << p_lexeme.m_str;
        return p_out;
    }

    /// \brief
    lexeme() = delete;

    /// \brief constructor by copy
    ///
    /// \param p_str is the identifier of the lexeme
    ///
    inline explicit lexeme(const std::string& p_str)
      : m_str(p_str)
    {}

    /// \brief constructor by move
    ///
    /// \param p_str is the identifier of the lexeme
    ///
    inline explicit lexeme(std::string&& p_str)
      : m_str(std::move(p_str))
    {}

    /// \brief copy ctor
    lexeme(const lexeme&) = default;

    /// \brief move ctor
    lexeme(lexeme&&) = default;

    /// \brief Destructor
    ~lexeme() = default;

    /// \brief copy operator
    lexeme& operator=(const lexeme&) = default;

    /// \brief move operator
    lexeme& operator=(lexeme&&) = default;

    /// \brief equal-to
    inline bool operator==(const lexeme& p_lexeme) const
    {
        return m_str == p_lexeme.m_str;
    }

    /// \brief not-equal-to
    inline bool operator!=(const lexeme& p_lexeme) const
    {
        return m_str != p_lexeme.m_str;
    }

    /// \brief less-than
    inline bool operator<(const lexeme& p_lexeme) const
    {
        return m_str < p_lexeme.m_str;
    }

    /// \brief greater-than
    inline bool operator>(const lexeme& p_lexeme) const
    {
        return m_str > p_lexeme.m_str;
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
    /// \brief begin
    /// \return
    ///
    inline const_iterator begin() const { return m_str.begin(); }

    ///
    /// \brief end
    /// \return
    ///
    inline const_iterator end() const { return m_str.end(); }

  private:
    ///
    /// \brief m_str the value of the @p lexeme
    ///
    std::string m_str;
};

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
