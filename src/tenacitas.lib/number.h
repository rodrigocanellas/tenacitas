#ifndef TENACITAS_NUMBER_H
#define TENACITAS_NUMBER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <thread>

#include <tenacitas.lib/calendar.h>

using namespace std::chrono_literals;

/// \brief master namespace
namespace tenacitas {

/// \brief manipulates numbers, like formating, and creation of a, hopefully,
/// unique identifier
namespace number {

/// \brief Maximum number of characters needed to represent a type of number
///
/// <tt>max_str_length<uint16_t>()</tt> is 5, while
/// <tt>max_str_length<uint32_t>()</tt> is 10
///
/// \tparam t_num_type is the type of number
template <typename t_num_type>
inline uint16_t max_str_length() {
    return (
        static_cast<uint16_t>(std::log10(std::pow(2, sizeof(t_num_type) * 8))) +
        1);
}

/// \brief Type of alingnment when formating numbers
enum class align : char { left = 'l', right = 'd' };

/// \brief Formats a number with the maximum characters necessary to represent
/// the number to string, filled with \p p_fill and aligned with \p p_align
///
/// \tparam t_num_type is the type of the number to be formated
///
/// \param p_num is the value of the number to be formated
///
/// \param p_fill is the char used to fill the parts of the string not filled
/// with \p p_num digits
///
/// \param p_align defines if \p p_num will be aligned at left, right, or
/// center of the string
template <typename t_num_type>
inline std::string
format(t_num_type p_num, char p_fill = '0', align p_align = align::right) {
    std::stringstream _stream;
    _stream << (p_align == align::right ? std::right : std::left)
            << std::setfill(p_fill) << std::setw(max_str_length<t_num_type>())
            << p_num;
    return _stream.str();
}

/// \brief Specialization for uint8_t of \p tenacitas::number::format
///
/// \param p_num is the value of the number to be formated
///
/// \param p_fill is the char used to fill the parts of the string not filled
/// with \p p_num digits
///
/// \param p_align defines if \p p_num will be aligned at left, right, or
/// center of the string
inline std::string
format(uint8_t p_num, char p_fill = '0', align p_align = align::right) {
    std::stringstream _stream;
    _stream << (p_align == align::right ? std::right : std::left)
            << std::setfill(p_fill) << std::setw(max_str_length<uint8_t>())
            << static_cast<uint16_t>(p_num);
    return _stream.str();
}

/// \brief Specialization for int8_t of \p tenacitas::number::format
///
/// \param p_num is the value of the number to be formated
///
/// \param p_fill is the char used to fill the parts of the string not filled
/// with \p p_num digits
///
/// \param p_align defines if \p p_num will be aligned at left, right, or
/// center of the string
inline std::string
format(int8_t p_num, char p_fill = '0', align p_align = align::right) {
    std::stringstream _stream;
    _stream << (p_align == align::right ? std::right : std::left)
            << std::setfill(p_fill) << std::setw(max_str_length<uint8_t>())
            << static_cast<int16_t>(p_num);
    return _stream.str();
}

/// \brief Specialization for uint8_t of \p tenacitas::number::format
///
/// \param p_num is the value of the number to be formated
///
/// \param p_fill is the char used to fill the parts of the string not filled
/// with \p p_num digits
///
/// \param p_align defines if \p p_num will be aligned at left, right, or
/// center of the string
template <typename t_num_type>
inline std::string format(t_num_type p_num,
                          uint8_t p_size,
                          char p_fill = '0',
                          align p_align = align::right) {
    std::stringstream _stream;
    _stream << (p_align == align::right ? std::right : std::left)
            << std::setfill(p_fill) << std::setw(p_size) << p_num;
    return _stream.str();
}

/// \brief Generates a unique (sort of) identifier
struct id {

    /// \brief
    id() {
        std::this_thread::sleep_for(50ms);
        m_value = calendar::now<>::microsecs();
    }

    id(const id &) = default;
    id(id &&) = default;
    id &operator=(const id &) = default;
    id &operator=(id &&) = default;

    /// \brief
    inline bool operator==(const id &p_uuid) const {
        return m_value == p_uuid.m_value;
    }

    /// \brief
    inline bool operator!=(const id &p_uuid) const {
        return m_value != p_uuid.m_value;
    }

    /// \brief
    inline bool operator>(const id &p_uuid) const {
        return m_value > p_uuid.m_value;
    }

    /// \brief
    inline bool operator<(const id &p_uuid) const {
        return m_value < p_uuid.m_value;
    }

    /// \brief
    inline bool operator>=(const id &p_uuid) const {
        return m_value >= p_uuid.m_value;
    }

    /// \brief
    inline bool operator<=(const id &p_uuid) const {
        return m_value <= p_uuid.m_value;
    }

    /// \brief
    friend std::ostream &operator<<(std::ostream &p_out, const id &p_uuid) {
        p_out << p_uuid.str();
        return p_out;
    }

    /// \brief
    inline operator std::string() const { return str(); }

    /// \brief
    inline std::string str() const {
        std::string _str {std::to_string(m_value)};
        return _str.substr(_str.size() - 4);
    }

private:
    uint64_t m_value {0};
};

} // namespace number

} // namespace tenacitas

#endif
