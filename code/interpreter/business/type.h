#ifndef TENACITAS_INTERPRETER_BUSINESS_TYPE_H
#define TENACITAS_INTERPRETER_BUSINESS_TYPE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <set>
#include <string>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the group
namespace business {

///
/// \brief The type struct represents a word read from input text
///
struct type
{

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
    type() = delete;

    /// \brief constructor by copy
    ///
    /// \param p_str is the identifier of the type
    ///
    inline explicit type(const std::string& p_str)
      : m_str(p_str)
    {}

    /// \brief constructor by move
    ///
    /// \param p_str is the identifier of the type
    ///
    inline explicit type(std::string&& p_str)
      : m_str(std::move(p_str))
    {}

    /// \brief copy ctor
    type(const type&) = default;

    /// \brief move ctor
    type(type&&) = default;

    /// \brief Destructor
    ~type() = default;

    /// \brief copy operator
    type& operator=(const type&) = default;

    /// \brief move operator
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
    /// \brief undefined type representing a fail in recognizing a type
    ///
    static const type undefined;

    ///
    /// \brief reserved type of a reserved word, like 'while'
    ///
    static const type reserved;

  private:
    ///
    /// \brief m_str the value of the @p type
    ///
    std::string m_str;
};

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
