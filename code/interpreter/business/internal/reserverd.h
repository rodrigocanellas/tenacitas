#ifndef TENACITAS_INTERPRETER_BUSINESS_reserved_H
#define TENACITAS_INTERPRETER_BUSINESS_reserved_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <set>
#include <string>

#include <interpreter/business/type.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the group
namespace business {

/// \brief reserved allows to define reserved words, like 'while', 'if' or
/// 'break'. The strings recognized by \p reserved will have \p type::reserved
struct reserved
{

  public:
    /// \brief default ctor not allowed
    inline reserved() = default;

    /// \brief copy ctor not allowed
    reserved(const reserved&) = delete;

    /// \brief move ctor not allowed
    reserved(reserved&&) = delete;

    // \brief destructor
    ~reserved() = default;

    /// \brief copy operator not allowed
    reserved& operator=(const reserved&) = delete;

    /// \brief move operator not allowed
    reserved& operator=(reserved&&) = delete;

    /// \brief dymanic array allocation not allowed
    void* operator new[](size_t) = delete;

    /// \brief dymanic array deallocation allowed
    void operator delete[](void*) = delete;

    /// \brief dymanic allocation not allowed
    void operator delete(void* p) = delete;

    /// \brief dymanic deallocation not allowed
    void* operator new(size_t) = delete;

    /// \brief inserts a string in the set of reserved words
    inline void add(const std::string& p_reserved)
    {
        m_reserved_set.insert(p_reserved);
    }

    /// \brief operator () tries to recognize a string as a reserved word
    /// \param p_str the string to be recognized
    /// \return \p type::undefined if \p p_str was not recognized; \p
    /// type::reserved otherwise
    inline type operator()(const std::string& p_str)
    {
        return (m_reserved_set.find(p_str) == m_reserved_set.end()
                  ? type::undefined
                  : type::reserved);
    }

  private:
    /// \brief reserved_set type of the set of reserved words
    typedef std::set<std::string> reserved_set;

  private:
    /// \brief m_reserved_set the set of reserved words
    reserved_set m_reserved_set;
};
} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
