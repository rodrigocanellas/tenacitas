#ifndef TENACITAS_INTERPRETER_BUSINESS_LEXEME_H
#define TENACITAS_INTERPRETER_BUSINESS_LEXEME_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <cstdint>
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
/// \brief The lexeme struct represents a word read from input text
///
struct lexeme
{

    ///
    /// \brief operator <<
    /// \param p_out
    /// \param p_lexeme
    /// \return
    ///
    inline friend std::ostream& operator<<(std::ostream& p_out,
                                           const lexeme& p_lexeme)
    {
        p_out << p_lexeme.m_str;
        return p_out;
    }

    /// \brief
    lexeme() = delete;

    /// \brief constructor by copy
    ///
    /// \param p_str is the identifier of the lexeme
    ///
    inline explicit lexeme(const std::string& p_str)
      : m_str(p_str)
    {}

    /// \brief constructor by move
    ///
    /// \param p_str is the identifier of the lexeme
    ///
    inline explicit lexeme(std::string&& p_str)
      : m_str(std::move(p_str))
    {}

    /// \brief not allowed
    lexeme(const lexeme&) = default;

    /// \brief not allowed
    lexeme(lexeme&&) = default;

    /// \brief Destructor
    ~lexeme() = default;

    /// \brief not allowed
    lexeme& operator=(const lexeme&) = default;

    /// \brief not allowed
    lexeme& operator=(lexeme&&) = default;

    /// \brief equal-to
    inline bool operator==(const lexeme& p_lexeme) const
    {
        return m_str == p_lexeme.m_str;
    }

    /// \brief not-equal-to
    inline bool operator!=(const lexeme& p_lexeme) const
    {
        return m_str != p_lexeme.m_str;
    }

    /// \brief less-than
    inline bool operator<(const lexeme& p_lexeme) const
    {
        return m_str < p_lexeme.m_str;
    }

    /// \brief greater-than
    inline bool operator>(const lexeme& p_lexeme) const
    {
        return m_str > p_lexeme.m_str;
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
    /// \brief m_str the value of the @p lexeme
    ///
    std::string m_str;
};

///
/// \brief lexemes represents the set of reserved @p lexeme
///
struct lexemes
{
    /// \brief
    typedef std::set<lexeme>::const_iterator const_iterator;

    ///
    /// \brief operator <<
    /// \param p_out
    /// \param p_lexemes
    /// \return
    ///
    inline friend std::ostream& operator<<(std::ostream& p_out,
                                           const lexemes& p_lexemes)
    {
        static const std::string _space(" ");
        for (const lexeme& _lexeme : p_lexemes.m_set) {
            p_out << _lexeme << _space;
        }
        return p_out;
    }

    /// \brief
    lexemes() = delete;

    ///
    /// \brief lexemes
    /// \param p_lexemes_str is a string that contains space separated @p
    /// lexeme objects, like "= == ;"
    ///
    explicit lexemes(const std::string& p_lexemes_str)
    {
        string::business::slipt_str(p_lexemes_str,
                                    [this](std::string&& p_str) -> void {
                                        m_set.insert(lexeme(std::move(p_str)));
                                    });
    }

    /// \brief not allowed
    lexemes(const lexemes&) = delete;

    /// \brief not allowed
    lexemes(lexemes&&) = delete;

    /// \brief Destructor
    ~lexemes() = default;

    /// \brief not allowed
    lexemes& operator=(const lexemes&) = delete;

    /// \brief not allowed
    lexemes& operator=(lexemes&&) = delete;

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
    /// \param p_lexeme
    ///
    inline void add(lexeme&& p_lexeme) { m_set.insert(std::move(p_lexeme)); }

    /// \brief
    inline const_iterator begin() const { return m_set.begin(); }

    /// \brief
    inline const_iterator end() const { return m_set.end(); }

    /// \brief
    inline bool empty() const { return m_set.empty(); }

    /// \brief
    inline int16_t size() const { return static_cast<int16_t>(m_set.size()); }

  private:
    ///
    /// \brief m_set the set of @p lexeme object
    ///
    std::set<lexeme> m_set;
};

} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
