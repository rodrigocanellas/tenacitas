#ifndef TENACITAS_INTERPRETER_BUSINESS_TOKENS_H
#define TENACITAS_INTERPRETER_BUSINESS_TOKENS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE
/// file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <cstddef>
#include <cstdint>
#include <set>
#include <stdexcept>
#include <string>

#include <interpreter/business/internal/recognizer.h>
#include <interpreter/business/internal/type.h>
#include <string/business/split_str.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the interpreter
namespace interpreter {
/// \brief namespace of the business
namespace business {

/// \brief tokens allows to define symbols that will be recognized by value,
/// like '=' or '<='
struct tokens
{

  public:
    /// \brief default constructor
    tokens() = default;

    /// \brief not allowed
    tokens(const tokens&) = delete;

    /// \brief not allowed
    tokens(tokens&&) = delete;

    /// \brief destructor
    ~tokens() = default;

    ///
    /// \brief add adds a group of tokens \b of the same size encoded in a
    /// string \param p_id identifier (name) of the token \param p_tokens
    /// \return
    ///
    void add(type::id&& p_id, std::string&& p_tokens);

    ///
    /// \brief recognize
    /// \param p_str
    /// \return
    ///
    type recognize(const std::string& p_str) const;

    /// \brief not allowed
    tokens& operator=(const tokens&) = delete;

    /// \brief not allowed
    tokens& operator=(tokens&&) = delete;

    /// \brief not allowed
    void* operator new[](size_t) = delete;

    /// \brief not allowed
    void operator delete[](void*) = delete;

    /// \brief not allowed
    void operator delete(void* p) = delete;

    /// \brief not allowed
    void* operator new(size_t) = delete;

  private:
    //    struct token_recognizer
    //    {
    //        token_recognizer() = default;

    //        ~token_recognizer() = default;

    //        token_recognizer(token_recognizer&&) noexcept = default;
    //        token_recognizer(const token_recognizer&) = default;

    //        token_recognizer& operator=(token_recognizer&&) noexcept =
    //        default; token_recognizer& operator=(const token_recognizer&) =
    //        default;

    //        bool operator()(const std::string& p_str)
    //        {
    //            //            return m_tokens.find(p_str) !=
    //            std::string::npos; return (m_set.find(p_str) != m_set.end());
    //        }

    //        void add(std::string&& p_str) { m_set.emplace(std::move(p_str)); }
    //        void add(const std::string& p_str) { m_set.emplace(p_str); }

    //      private:
    //        typedef std::set<std::string> set;

    //      private:
    //        set m_set;
    //    };

    ///
    /// \brief The token_size struct
    ///
    struct token
    {

        /// \brief default ctor not allowed
        token() = delete;

        ///
        /// \brief token
        /// \param p_size
        /// \param p_type
        ///
        inline token(uint8_t p_size, type&& p_type)
          : m_size(p_size)
          , m_type(std::move(p_type))
        {}

        /// \brief destructor
        ~token() = default;

        /// \brief token copy ctor not allowed
        token(const token&) = delete;

        ///
        /// \brief token move ctor
        /// \param p_token_size
        ///
        token(token&& p_token_size) noexcept
          : m_size(p_token_size.m_size)
          , m_type(std::move(p_token_size.m_type))
        {}

        ///
        /// \brief size
        /// \return
        ///
        inline uint8_t get_size() const { return m_size; }

        ///
        /// \brief get_type
        /// \return
        ///
        inline type get_type() const { return m_type; }

        ///
        /// \brief recognize
        /// \param p_str
        /// \return
        ///
        inline bool recognize(const std::string& p_str) const
        {
            return m_type.recognize(p_str);
        }

        inline void add(const std::string& p_str) { m_strings.emplace(p_str); }

        /// \brief token_size copy operator not allowed
        token& operator=(const token&) = delete;

        ///
        /// \brief operator = move operator
        /// \param p_token_size
        /// \return
        ///
        token& operator=(token&& p_token_size) noexcept
        {
            if (this != &p_token_size) {
                m_size = p_token_size.m_size;
                m_type = std::move(p_token_size.m_type);
            }
            return *this;
        }

        ///
        /// \brief operator ==
        /// \param p_token_size
        /// \return
        ///
        inline bool operator==(const token& p_token_size) const
        {
            return m_size == p_token_size.m_size;
        }

        ///
        /// \brief operator !=
        /// \param p_token_size
        /// \return
        ///
        inline bool operator!=(const token& p_token_size) const
        {
            return m_size != p_token_size.m_size;
        }

        ///
        /// \brief operator <
        /// \param p_token_size
        /// \return
        ///
        inline bool operator<(const token& p_token_size) const
        {
            return m_size < p_token_size.m_size;
        }

      private:
        typedef std::set<std::string> strings;

      private:
        uint8_t m_size;
        type m_type;
        strings m_strings;
    };

    ///
    /// \brief set_t
    ///
    typedef std::set<token> set_t;

  private:
    ///
    /// \brief m_set
    ///
    set_t m_set;
};
} // namespace business
} // namespace interpreter
} // namespace tenacitas

#endif
