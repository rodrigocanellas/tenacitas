#ifndef TENACITAS_CROSSWORDS_BUSINESS_POSITIONER007_H
#define TENACITAS_CROSSWORDS_BUSINESS_POSITIONER007_H

#include <crosswords/business/internal/words_positioner_1.h>
#include <crosswords/business/internal/log.h>
#include <crosswords/business/internal/letters_distributions.h>
#include <crosswords/business/internal/positions_occupied.h>
#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/description.h>
#include <crosswords/entities/lexeme.h>
#include <crosswords/entities/word.h>
#include <crosswords/entities/words.h>


namespace tenacitas {
namespace crosswords {
namespace business {

/// \brief positioner006_t position \p words
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
struct positioner007_t
{
  typedef t_log log;

  typedef entities::description description;
  typedef entities::words words;
  typedef entities::word word;
  typedef entities::lexeme lexeme;
  typedef entities::coordinate::x x;
  typedef entities::coordinate::y y;

  typedef positions_occupied_t<log> positions_occupied;
  typedef words_positioner_1_t<log>  words_positioner;

  explicit positioner007_t(x p_x_limit = x(13), y p_y_limit = y(13))
    : m_x_limit(p_x_limit)
    , m_y_limit(p_y_limit)
    , m_words_positioner(m_x_limit, m_y_limit)
  {}

  positioner007_t(const positioner007_t&) = delete;
  positioner007_t(positioner007_t&&) noexcept = default;
  positioner007_t& operator=(const positioner007_t&) = delete;
  positioner007_t& operator=(positioner007_t&&) noexcept = default;
  ~positioner007_t() = default;

  void add(lexeme&& p_lexeme, description&& p_description)
  {
    lexeme::size_type _word_size = p_lexeme.size();
    if ((m_y_limit < y(_word_size)) && (m_x_limit < x(_word_size))) {

      if (m_y_limit < y(_word_size)) {
        std::stringstream _stream;
        _stream << p_lexeme << "'s size is " << _word_size
                << ", which is bigger than the vertical limit of " << m_y_limit;
        std::string _msg = _stream.str();

        crosswords_log_error(log, _stream.str());
        throw std::runtime_error(_msg);
      }
      if (m_x_limit < x(_word_size)) {
        std::string _msg(p_lexeme + "'s size is " + std::to_string(_word_size) +
                         ", which is bigger than the horizontal "
                         "limit of " +
                         std::to_string(m_x_limit.get_value<int16_t>()));
        crosswords_log_error(log, _msg);
        throw std::runtime_error(_msg);
      }
    }
    crosswords_log_debug(log,
                         "'",
                         p_lexeme,
                         "', with description '",
                         p_description,
                         "' was added");
    m_words.add(word(
                  static_cast<word::id>(m_words.get_size() + 1),
                  p_lexeme,
                  p_description)
                );
  }

  inline x get_x_limit() const { return m_x_limit; }
  inline y get_y_limit() const { return m_y_limit; }

  words operator()() {
    letters_distributions _letters_distributions(m_words);
    _letters_distributions.sort_by_amount();

    words_iterators _words_iterators;

    while (true) {
      words::iterator _word_ite = _letters_distributions.next();
      if (_word_ite == m_words.end()) {
        break;
      }

      if (!m_words_positioner(m_words.begin(),
                              std::next(_word_ite),
                              m_positions_occupied)) {
        break;
      }
    }


    return m_words;
  }

private:


private:
  x m_x_limit;
  y m_y_limit;
  words m_words;
  positions_occupied m_positions_occupied;
  words_positioner m_words_positioner;
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // POSITIONER007_H
