#ifndef TENACITAS_INTERPRETER_BUSINESS_VALUE_H
#define TENACITAS_INTERPRETER_BUSINESS_VALUE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <list>
#include <string>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the business
namespace business {

///
/// \brief The value struct
///
struct value
{

    /// \brief
    value() = delete;

    /// \brief
    explicit value(const std::string& p_str);

    /// \brief not allowed
    value(const value&) = default;

    /// \brief not allowed
    value(value&&) = default;

    /// \brief Destructor
    ~value() = default;

    /// \brief not allowed
    value& operator=(const value&) = default;

    /// \brief not allowed
    value& operator=(value&&) = default;

    /// \brief equal-to
    inline bool operator==(const value& p_value) const;

    /// \brief not-equal-to
    inline bool operator!=(const value& p_value) const;

    /// \brief less-than
    inline bool operator<(const value& p_value) const;

    /// \brief not allowed
    void* operator new[](size_t) = delete;

    /// \brief not allowed
    void operator delete[](void*) = delete;

    /// \brief not allowed
    void operator delete(void* p) = delete;

    /// \brief not allowed
    void* operator new(size_t) = delete;

  private:
    std::string m_str;
};

///
/// \brief values
///
struct values
{
    /// \brief
    typedef std::list<value>::const_iterator const_iterator;

    /// \brief
    values() = delete;

    ///
    /// \brief values
    /// \param p_values_str is a string that contains space separated @p
    /// value objects, like "= == ;"
    ///
    explicit values(const std::string& p_values_str);

    /// \brief not allowed
    values(const values&) = delete;

    /// \brief not allowed
    values(values&&) = delete;

    /// \brief Destructor
    ~values() = delete;

    /// \brief not allowed
    values& operator=(const values&) = delete;

    /// \brief not allowed
    values& operator=(values&&) = delete;

    /// \brief not allowed
    void* operator new[](size_t) = delete;

    /// \brief not allowed
    void operator delete[](void*) = delete;

    /// \brief not allowed
    void operator delete(void* p) = delete;

    /// \brief not allowed
    void* operator new(size_t) = delete;

    /// \brief
    const_iterator begin() const;

    /// \brief
    const_iterator end() const;

  private:
    std::list<value> m_values;
};

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
