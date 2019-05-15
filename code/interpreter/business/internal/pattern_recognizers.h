#ifndef TENACITAS_INTERPRETER_BUSINESS_PATTERN_RECOGNIZERS_H
#define TENACITAS_INTERPRETER_BUSINESS_PATTERN_RECOGNIZERS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <iostream>
#include <list>
#include <string>

#include <interpreter/business/internal/pattern_recognizer.h>
#include <interpreter/business/internal/type.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the business
namespace business {

///
/// \brief pattern_recognizers
///
struct pattern_recognizers
{
    /// \brief
    typedef std::set<type>::const_iterator const_iterator;

    /// \brief
    pattern_recognizers() = default;

    /// \brief not allowed
    pattern_recognizers(const pattern_recognizers&) = delete;

    /// \brief not allowed
    pattern_recognizers(pattern_recognizers&&) = delete;

    /// \brief Destructor
    ~pattern_recognizers() = default;

    /// \brief not allowed
    pattern_recognizers& operator=(const pattern_recognizers&) = delete;

    /// \brief not allowed
    pattern_recognizers& operator=(pattern_recognizers&&) = delete;

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
    inline void add(pattern_recognizer&& p_recognizer)
    {
        m_list.push_back(std::move(p_recognizer));
    }

    ///
    /// \brief recognize
    /// \param p_str
    /// \return if not recognized, \p type::undefined; otherwise a valid \p type
    ///
    inline type recognize(const std::string& p_str)
    {
        for (pattern_recognizer& _recognize : m_list) {
            type _type(_recognize(p_str));
            if (_type != type::undefined) {
                return _type;
            }
        }
        return type::undefined;
    }

  private:
    ///
    /// \brief map_t
    ///
    typedef std::list<pattern_recognizer> list;

  private:
    ///
    /// \brief m_set the set of @p type objects
    ///
    list m_list;
};

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
