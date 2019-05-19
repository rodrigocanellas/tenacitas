#ifndef TENACITAS_INTERPRETER_BUSINESS_PATTERN_RECOGNIZERS_H
#define TENACITAS_INTERPRETER_BUSINESS_PATTERN_RECOGNIZERS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <functional>
#include <iostream>
#include <list>
#include <string>

#include <interpreter/business/recognizer.h>
#include <interpreter/business/type.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the business
namespace business {

/// \brief recognizers group of \p recognizer objects, which try to recognize a
/// \p type based on a string
struct recognizers
{
    /// \brief
    typedef std::set<type>::const_iterator const_iterator;

    /// \brief
    recognizers() = default;

    /// \brief not allowed
    recognizers(const recognizers&) = delete;

    /// \brief not allowed
    recognizers(recognizers&&) = default;

    /// \brief Destructor
    ~recognizers() = default;

    /// \brief not allowed
    recognizers& operator=(const recognizers&) = delete;

    /// \brief not allowed
    recognizers& operator=(recognizers&&) = delete;

    /// \brief not allowed
    void* operator new[](size_t) = delete;

    /// \brief not allowed
    void operator delete[](void*) = delete;

    /// \brief not allowed
    void operator delete(void* p) = delete;

    /// \brief not allowed
    void* operator new(size_t) = delete;

    /// \brief add adds a \p recognizer to the set
    /// \param p_recognizer is a new recognizer
    inline void add(recognizer&& p_recognizer)
    {
        m_list.push_back(std::move(p_recognizer));
    }

    /// \brief recognize tries to recognize a \p type among the \p recognizer
    /// objects, based on a string \param p_str the value to be recognized
    /// \return if not recognized, \p type::undefined; otherwise a valid \p type
    inline type recognize(const std::string& p_str)
    {
        for (recognizer& _recognize : m_list) {
            type _type(_recognize(p_str));
            if (_type != type::undefined) {
                return _type;
            }
        }
        return type::undefined;
    }

    /// \brief identifies if the list of recognizers is empty
    inline bool empty() const { return m_list.empty(); }

  private:
    /// \brief type for the list of \p recognizer objects
    typedef std::list<recognizer> list;

  private:
    /// \brief m_list is the list of \p recognizer objects
    list m_list;
};

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
