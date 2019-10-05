#ifndef TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_001_H
#define TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_001_H

#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

#include <crosswords/business/internal/log.h>
#include <crosswords/entities/coordinate.h>

namespace tenacitas {
namespace crosswords {
namespace business {

/// \brief positioner000_t position \p words
///
/// \tparam t_log provides log funcionality:
/// static void debug(const std::string & p_file, int p_line, const
/// t_params&... p_params)
/// static void info(const std::string & p_file, int p_line, const t_params&...
/// p_params)
/// static void warn(const std::string & p_file, int p_line, const t_params&...
/// p_params)
/// static void error(const std::string & p_file, int p_line, const
/// t_params&... p_params)
/// static void fatal(const std::string & p_file, int p_line, const
/// t_params&... p_params)
///
template<typename t_log>
struct matrix
{
    typedef entities::coordinate::x x;
    typedef entities::coordinate::y y;

    matrix() = delete;
    matrix(x p_x_size, y p_y_size)
      : m_x_size(p_x_size)
      , m_y_size(p_y_size)
      , m_letters(m_x_size.get_value<size>() * m_y_size.get_value<size>(), '-')
    {}
    matrix(const matrix&) = default;
    matrix(matrix&&) noexcept = default;

    matrix& operator=(const matrix&) = default;
    matrix& operator=(matrix&&) noexcept = default;

    ~matrix() = default;

    void set(x p_x, y p_y, char p_c)
    {
        if (p_x > m_x_size) {
            throw std::runtime_error(
              std::to_string(p_x.get_value<int16_t>()) + " is bigger than " +
              std::to_string(m_x_size.get_value<int16_t>()));
        }

        if (p_y > m_y_size) {
            throw std::runtime_error(
              std::to_string(p_y.get_value<int16_t>()) + " is bigger than " +
              std::to_string(m_y_size.get_value<int16_t>()));
        }

        m_letters[(p_x.get_value<size>() * m_x_size.get_value<size>()) +
                  p_y.get_value<size>()] = p_c;
    }

    char get(x p_x, y p_y) const
    {
        if (p_x > m_x_size) {
            throw std::runtime_error(
              std::to_string(p_x.get_value<int16_t>()) + " is bigger than " +
              std::to_string(m_x_size.get_value<int16_t>()));
        }

        if (p_y > m_y_size) {
            throw std::runtime_error(
              std::to_string(p_y.get_value<int16_t>()) + " is bigger than " +
              std::to_string(m_y_size.get_value<int16_t>()));
        }

        return m_letters[(p_x.get_value<size>() * m_x_size.get_value<size>()) +
                         p_y.get_value<size>()];
    }

    void print() const
    {
        using namespace std;

        size _x_size = m_x_size.get_value<size>();
        size _y_size = m_y_size.get_value<size>();

        cerr << "\n   ";

        for (size _x = 0; _x < _x_size; ++_x) {
            cerr << setw(2) << setfill('0') << _x << " ";
        }
        cerr << endl;
        for (size _y = 0; _y < _y_size; ++_y) {
            cerr << setw(2) << setfill('0') << _y << " ";
            for (size _x = 0; _x < _x_size; ++_x) {
                cerr << " " << m_letters[(_x * _x_size) + _y] << " ";
            }
            cerr << endl;
        }
    }

    x x_size() const { return m_x_size; }
    y y_size() const { return m_y_size; }

  private:
    typedef std::string letters;
    typedef letters::size_type size;

  private:
    x m_x_size;
    y m_y_size;
    letters m_letters;
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // MATRIX_H
