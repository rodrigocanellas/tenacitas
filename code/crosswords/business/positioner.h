#ifndef TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_H
#define TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_H

#include <algorithm>
#include <ctime>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>
#include <utility>

#include <crosswords/business/internal/log.h>
#include <crosswords/business/internal/positioner007.h>
#include <crosswords/business/internal/words_positioner_group.h>
#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/description.h>
#include <crosswords/entities/lexeme.h>
#include <crosswords/entities/words.h>

namespace tenacitas {
namespace crosswords {
namespace business {

/// \brief positioner_t position \p words
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
struct positioner_t
{
  typedef t_log log;

  typedef entities::words words;
  typedef entities::lexeme lexeme;
  typedef entities::description description;
  typedef entities::coordinate::x x;
  typedef entities::coordinate::y y;

  inline explicit positioner_t(x p_x_limit = x(13), y p_y_limit = y(13))
    : m_positioner(p_x_limit, p_y_limit)
  {}

  positioner_t(const positioner_t&) = delete;
  positioner_t(positioner_t&& p_positioner) noexcept = default;
  positioner_t& operator=(const positioner_t&) = delete;
  positioner_t& operator=(positioner_t&& p_positioner) noexcept = default;
  ~positioner_t() {}

  inline void add(lexeme&& p_lexeme, description&& p_description)
  {
    m_positioner.add(std::move(p_lexeme), std::move(p_description));
  }

  inline words operator()() { return m_positioner(); }

  inline x get_x_limit() const { return m_positioner.get_x_limit(); }
  inline y get_y_limit() const { return m_positioner.get_y_limit(); }

private:
  typedef positioner007_t<log> positioner007;

private:
  positioner007 m_positioner;
};
} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // POSITIONER003_H
