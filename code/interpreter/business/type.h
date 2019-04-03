#ifndef TENACITAS_INTERPRETER_BUSINESS_TYPE_H
#define TENACITAS_INTERPRETER_BUSINESS_TYPE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <list>
#include <string>

#include <interpreter/business/value.h>

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

    /// \brief
    type() = delete;

    /// \brief
    explicit type(const std::string& p_str);

    /// \brief not allowed
    type(const type&) = default;

    /// \brief not allowed
    type(type&&) = default;

    /// \brief Destructor
    ~type() = default;

    /// \brief not allowed
    type& operator=(const type&) = default;

    /// \brief not allowed
    type& operator=(type&&) = default;

    /// \brief equal-to
    inline bool operator==(const type& p_type) const;

    /// \brief not-equal-to
    inline bool operator!=(const type& p_type) const;

    /// \brief less-than
    inline bool operator<(const type& p_type) const;

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
/// \brief types
///
struct types
{
    /// \brief
    typedef std::list<value>::const_iterator const_iterator;

    /// \brief
    types() = delete;

    ///
    /// \brief types
    /// \param p_types_str is a string that contains space separated @p
    /// value objects, like "= == ;"
    ///
    explicit types(const std::string& p_types_str);

    /// \brief not allowed
    types(const types&) = delete;

    /// \brief not allowed
    types(types&&) = delete;

    /// \brief Destructor
    ~types() = default;

    /// \brief not allowed
    types& operator=(const types&) = delete;

    /// \brief not allowed
    types& operator=(types&&) = delete;

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
    std::list<type> m_types;
};

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
