#ifndef TENACITAS_TYPE_ENTITIES_NUMBER_H
#define TENACITAS_TYPE_ENTITIES_NUMBER_H

#include <cstdint>
#include <iostream>

namespace tenacitas {
namespace type {
namespace entities {

template<typename t_value, typename t_class, uint8_t unmatch = 0>
class number_t
{
    friend std::ostream& operator<<(std::ostream& p_out,
                                    const number_t& p_number)
    {
        p_out << p_number.m_value;
        return p_out;
    }

  public:
    typedef t_value value;

    number_t()
      : m_value(0)
    {}

    explicit number_t(const t_value& p_value)
      : m_value(p_value)
    {}

    template<typename t_other_value>
    explicit number_t(t_other_value p_other_value)
      : m_value(static_cast<t_value>(p_other_value))
    {}

    ~number_t() = default;

    number_t(const number_t&) = default;

    number_t(number_t&&) noexcept = default;

    explicit operator value() const { return m_value; }

    bool operator==(const number_t& p_number) const
    {
        return (m_value == p_number.m_value);
    }

    bool operator!=(const number_t& p_number) const
    {
        return (m_value != p_number.m_value);
    }

    bool operator>(const number_t& p_number) const
    {
        return (m_value > p_number.m_value);
    }

    bool operator>=(const number_t& p_number) const
    {
        return (m_value >= p_number.m_value);
    }

    bool operator<(const number_t& p_number) const
    {
        return (m_value < p_number.m_value);
    }

    bool operator<=(const number_t& p_number) const
    {
        return (m_value <= p_number.m_value);
    }

    number_t& operator=(const number_t&) = default;

    number_t& operator=(number_t&&) noexcept = default;

    number_t operator/(number_t p_number) const
    {
        number_t _copy(*this);
        _copy.m_value /= p_number.m_value;
        return _copy;
    }

    number_t& operator/=(number_t p_number)
    {
        m_value /= p_number.m_value;
        return *this;
    }

    number_t operator*(number_t p_number) const
    {
        number_t _copy(*this);
        _copy.m_value *= p_number.m_value;
        return _copy;
    }

    number_t& operator*=(number_t p_number)
    {
        m_value *= p_number.m_value;
        return *this;
    }

    number_t operator+(number_t p_number) const
    {
        number_t _copy(*this);
        _copy.m_value += p_number.m_value;
        return _copy;
    }

    number_t& operator+=(number_t p_number)
    {
        m_value += p_number.m_value;
        return *this;
    }

    number_t operator-(number_t p_number) const
    {
        number_t _copy(*this);
        _copy.m_value -= p_number.m_value;
        return _copy;
    }

    number_t& operator-=(number_t p_number)
    {
        m_value -= p_number.m_value;
        return *this;
    }

    number_t& operator++()
    {
        ++m_value;
        return *this;
    }

    number_t& operator--()
    {
        --m_value;
        return *this;
    }

    const number_t operator++(int) const
    {
        const number_t _copy(*this);
        ++(*this);
        return _copy;
        //        m_value++;
        //        return *this;
    }

    const number_t operator--(int) const
    {
        const number_t _copy(*this);
        --(*this);
        return _copy;
    }

  private:
    value m_value;
};

} // namespace entities
} // namespace type
} // namespace tenacitas

#endif
