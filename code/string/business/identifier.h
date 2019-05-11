#ifndef TENACITAS_STRING_BUSINESS_IDENTIFIER_H
#define TENACITAS_STRING_BUSINESS_IDENTIFIER_H

#include <cstddef>
#include <stdexcept>
#include <string>

namespace tenacitas {
namespace string {
namespace business {

///
/// \brief The identifier creates an indentifier based on string for a specific
/// class
///
/// \tparam t_owner is the class that will have a string based identifier
///
template<typename t_owner>
struct identifier
{

    ///
    /// \brief const_iterator
    ///
    typedef std::string::const_iterator const_iterator;

    ///
    /// \brief size
    ///
    typedef uint8_t size;

    ///
    /// \brief operator <<
    /// \param p_out
    /// \param p_identifier
    /// \return
    ///
    inline friend std::ostream& operator<<(std::ostream& p_out,
                                           const identifier& p_identifier)
    {
        p_out << p_identifier.m_str;
        return p_out;
    }

    /// \brief not-allowed
    identifier() = delete;

    /// \brief constructor by copy
    ///
    /// \param p_str is the identifier of the identifier
    ///
    inline explicit identifier(const std::string& p_str)
      : m_str(p_str)
    {}

    /// \brief constructor by move
    ///
    /// \param p_str is the identifier of the identifier
    ///
    inline explicit identifier(std::string&& p_str)
      : m_str(std::move(p_str))
    {}

    /// \brief copy ctor
    identifier(const identifier&) = default;

    /// \brief move ctor
    identifier(identifier&&) noexcept = default;

    /// \brief Destructor
    ~identifier() = default;

    /// \brief copy operator
    identifier& operator=(const identifier&) = default;

    /// \brief move operator
    identifier& operator=(identifier&&) noexcept = default;

    /// \brief equal-to
    inline bool operator==(const identifier& p_identifier) const
    {
        return m_str == p_identifier.m_str;
    }

    /// \brief not-equal-to
    inline bool operator!=(const identifier& p_identifier) const
    {
        return m_str != p_identifier.m_str;
    }

    /// \brief less-than
    inline bool operator<(const identifier& p_identifier) const
    {
        return m_str < p_identifier.m_str;
    }

    /// \brief greater-than
    inline bool operator>(const identifier& p_identifier) const
    {
        return m_str > p_identifier.m_str;
    }

    ///
    /// \brief size
    /// \return
    ///
    inline size get_size() const { return static_cast<size>(m_str); }

    /// \brief not allowed
    void* operator new[](size_t) = delete;

    /// \brief not allowed
    void operator delete[](void*) = delete;

    /// \brief not allowed
    void operator delete(void* p) = delete;

    /// \brief not allowed
    void* operator new(size_t) = delete;

    ///
    /// \brief begin
    /// \return
    ///
    inline const_iterator begin() const { return m_str.begin(); }

    ///
    /// \brief end
    /// \return
    ///
    inline const_iterator end() const { return m_str.end(); }

    inline char operator[](uint16_t p_index) const
    {
        if (p_index > static_cast<uint16_t>(m_str.size())) {
            throw std::out_of_range(std::to_string(p_index) +
                                    " is greater than " +
                                    std::to_string(m_str.size()));
        }
        return m_str[p_index];
    }

  private:
    ///
    /// \brief m_str the value of the @p identifier
    ///
    std::string m_str;
};

} // namespace business
} // namespace string
} // namespace tenacitas

#endif // TENACITAS_STRING_BUSINESS_IDENTIFIER_H
