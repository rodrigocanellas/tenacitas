#ifndef TENACITAS_INTERPRETER_BUSINESS_TYPES_H
#define TENACITAS_INTERPRETER_BUSINESS_TYPES_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <iostream>
#include <set>
#include <string>

#include <interpreter/business/type.h>
#include <string/business/split_str.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the business
namespace business {

///
/// \brief types
///
struct types
{
    /// \brief
    typedef std::set<type>::const_iterator const_iterator;

    ///
    /// \brief operator <<
    /// \param p_out
    /// \param p_types
    /// \return
    ///
    friend std::ostream& operator<<(std::ostream& p_out, const types& p_types);

    /// \brief
    types() = default;

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
    const_iterator add(type&& p_type);

    //    /// \brief
    //    inline const_iterator begin() const { return m_set.begin(); }

    /// \brief
    inline const_iterator end() const { return m_set.end(); }

    /// \brief
    inline bool empty() const { return m_set.empty(); }

    //    /// \brief
    //    inline int16_t size() const { return
    //    static_cast<int16_t>(m_set.size()); }

    //    ///
    //    /// \brief recognize checks if \p p_str is of any known \p type
    //    /// \param p_les the string to be verified
    //    /// \return a valid const_iterator to the \p type, if \p p_lex is of a
    //    known
    //    /// \p type; \p end() otherwise
    //    ///
    //    const_iterator recognize(const std::string& p_str) const;

    inline const_iterator find(const type& p_type) const
    {
        return m_set.find(p_type);
    }

  private:
    ///
    /// \brief map_t
    ///
    typedef std::set<type> set;

  private:
    ///
    /// \brief m_set the set of @p type objects
    ///
    set m_set;
};

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
