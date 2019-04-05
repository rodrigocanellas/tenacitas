#ifndef TENACITAS_INTERPRETER_BUSINESS_TYPE_H
#define TENACITAS_INTERPRETER_BUSINESS_TYPE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <set>
#include <string>

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

  private:
    ///
    /// \brief m_str the value of the @p type
    ///
    std::string m_str;
};

///
/// \brief types
///
struct types
{
    /// \brief
    typedef std::set<type>::const_iterator const_iterator;

    ///
    /// \brief value_t
    ///
    typedef type value_t;

    ///
    /// \brief operator <<
    /// \param p_out
    /// \param p_types
    /// \return
    ///
    inline friend std::ostream& operator<<(std::ostream& p_out,
                                           const types& p_types)
    {
        static const std::string _space(" ");
        for (const type& _type : p_types.m_set) {
            p_out << _type << _space;
        }
        return p_out;
    }

    /// \brief
    types() = delete;

    ///
    /// \brief types
    /// \param p_types_str is a string that contains space separated @p
    /// value objects, like "= == ;"
    ///
    explicit types(const std::string& p_types_str)
    {
        string::business::slipt_str(p_types_str,
                                    [this](std::string&& p_str) -> void {
                                        m_set.insert(type(std::move(p_str)));
                                    });
    }

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

    ///
    /// \brief add
    /// \param p_type
    ///
    inline void add(type&& p_type) { m_set.insert(std::move(p_type)); }

    /// \brief
    inline const_iterator begin() const { return m_set.begin(); }

    /// \brief
    inline const_iterator end() const { return m_set.end(); }

  private:
    ///
    /// \brief m_set the set of @p type objects
    ///
    std::set<type> m_set;
};

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
