#ifndef TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_001_H
#define TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_001_H

#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

#include <crosswords/business/internal/log.h>
#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/description.h>
#include <crosswords/entities/lexeme.h>
#include <crosswords/entities/word.h>
#include <crosswords/entities/words.h>

namespace tenacitas {
namespace crosswords {
namespace business {

struct matrix
{
    typedef entities::coordinate::x x;
    typedef entities::coordinate::y y;

    matrix() = delete;
    matrix(x p_x, y p_y)
      : m_x(p_x)
      , m_y(p_y)
      , m_letters(m_x.get_value<size>() * m_y.get_value<size>(), ' ')
    {}
    matrix(const matrix&) = default;
    matrix(matrix&&) noexcept = default;

    matrix& operator=(const matrix&) = default;
    matrix& operator=(matrix&&) noexcept = default;

    ~matrix() = default;

    void set(x p_x, y p_y, char p_c)
    {
        m_letters[p_x.get_value<size>() * p_y.get_value<size>()] = p_c;
    }

    char get(x p_x, y p_y) const
    {
        return m_letters[p_x.get_value<size>() * p_y.get_value<size>()];
    }

    void print() const
    {
        using namespace std;

        size _x_size = m_x.get_value<size>();
        size _y_size = m_y.get_value<size>();

        cerr << "    ";

        for (size _x = 0; _x < _x_size; ++_x) {
            for (size _y = 0; _y < _y_size; ++_y) {
                cerr << setw(2) << setfill('0') << m_letters[_x * _y] << " ";
            }
            cerr << endl;
        }
    }

  private:
    typedef std::string letters;
    typedef letters::size_type size;

  private:
    x m_x;
    y m_y;
    letters m_letters;
};

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
struct positioner001_t
{};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // POSITIONER001_H
