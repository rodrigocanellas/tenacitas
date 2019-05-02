#ifndef TENACITAS_INTERPRETER_BUSINESS_TYPES_H
#define TENACITAS_INTERPRETER_BUSINESS_TYPES_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <set>
#include <string>

#include <interpreter/business/internal/type.h>
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

    /// \brief
    inline int16_t size() const { return static_cast<int16_t>(m_set.size()); }

    ///
    /// \brief recognize checks if \p lexeme is of any known \p type
    /// \param p_les the string to be verified
    /// \return (true, a const_iterator to the \p type), if \p p_lex is of kown
    /// a \p type; (false, end()) otherwise
    ///
    inline std::pair<bool, const_iterator> recognize(const lexeme& p_lex) const
    {
        const_iterator _end = m_set.end();
        for (const_iterator _ite = m_set.begin(); _ite != _end; ++_ite) {
            if (_ite->recognize(p_lex)) {
                return { true, _ite };
            }
        }
        return { false, _end };
    }

  private:
    ///
    /// \brief find
    /// \param p_type
    /// \return a valid \p const_iterator if found, or \p end() if not found
    ///
    inline const_iterator find(const type& p_type) const
    {
        return m_set.find(p_type);
    }

  private:
    ///
    /// \brief map_t
    ///
    typedef std::set<type> set_t;

    ///
    /// \brief m_set the set of @p type objects
    ///
    set_t m_set;
};

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
