#ifndef TENACITAS_INTERPRETER_BUSINESS_TYPE_H
#define TENACITAS_INTERPRETER_BUSINESS_TYPE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <functional>
#include <set>
#include <string>

#include <interpreter/business/internal/lexeme.h>
#include <string/business/split_str.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the business
namespace business {

///
/// \brief The type struct
///
struct type
{

    ///
    /// \brief recognize function that recognizes, or not, if a \p lexeme is
    /// of this \p type
    ///
    typedef std::function<bool(const lexeme&)> recognizer;

    ///
    /// \brief operator <<
    /// \param p_out
    /// \param p_type
    /// \return
    ///
    inline friend std::ostream& operator<<(std::ostream& p_out,
                                           const type& p_type)
    {
        p_out << p_type.m_str;
        return p_out;
    }

    /// \brief
    type()
      : m_str("--- INVALID TYPE ---")
      , m_recognize([](const lexeme&) -> bool { return false; })
    {}

    /// \brief constructor by copy
    ///
    /// \param p_str is the identifier of the type
    ///
    inline explicit type(const std::string& p_str, recognizer p_recognize)
      : m_str(p_str)
      , m_recognize(p_recognize)
    {}

    /// \brief constructor by move
    ///
    /// \param p_str is the identifier of the type
    ///
    inline explicit type(std::string&& p_str, recognizer p_recognize)
      : m_str(std::move(p_str))
      , m_recognize(p_recognize)
    {}

    /// \brief
    type(const type&) = default;

    /// \brief
    type(type&&) = default;

    /// \brief Destructor
    ~type() = default;

    /// \brief
    type& operator=(const type&) = default;

    /// \brief
    type& operator=(type&&) = default;

    /// \brief equal-to
    inline bool operator==(const type& p_type) const
    {
        return m_str == p_type.m_str;
    }

    /// \brief not-equal-to
    inline bool operator!=(const type& p_type) const
    {
        return m_str != p_type.m_str;
    }

    /// \brief less-than
    inline bool operator<(const type& p_type) const
    {
        return m_str < p_type.m_str;
    }

    /// \brief greater-than
    inline bool operator>(const type& p_type) const
    {
        return m_str > p_type.m_str;
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
    /// \brief recognize
    /// \param p_lex
    /// \return
    ///
    bool recognize(const lexeme& p_lexeme) const
    {
        return m_recognize(p_lexeme);
    }

  private:
    ///
    /// \brief m_str the value of the \p type
    ///
    std::string m_str;

    ///
    /// \brief m_recognize reports if a \p lexeme is of this type
    ///
    recognizer m_recognize;
};

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
