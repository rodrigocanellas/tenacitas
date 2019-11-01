#ifndef TENACITAS_BUSINESS_CROSSWORDS_POSITIONER004_H
#define TENACITAS_BUSINESS_CROSSWORDS_POSITIONER004_H

#include <algorithm>
#include <ctime>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>
#include <utility>

#include <business/crosswords/internal/log.h>
#include <business/crosswords/internal/word_positioner.h>
#include <entities/crosswords/coordinate.h>
#include <entities/crosswords/description.h>
#include <entities/crosswords/lexeme.h>
#include <entities/crosswords/word.h>
#include <entities/crosswords/words.h>

namespace tenacitas {
namespace business {
namespace crosswords {

/// \brief positioner003_t position \p words
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
struct positioner004_t
{
  typedef t_log log;

  typedef entities::crosswords::words words;
  typedef entities::crosswords::word word;
  typedef entities::crosswords::lexeme lexeme;
  typedef entities::crosswords::description description;
  typedef entities::crosswords::coordinate::x x;
  typedef entities::crosswords::coordinate::y y;

  typedef word_positioner_t<log> word_positioner;

  explicit positioner004_t(x p_x_limit = x(13), y p_y_limit = y(13))
    : m_x_limit(p_x_limit)
    , m_y_limit(p_y_limit)
  {}

  void add(lexeme&& p_lexeme, description&& p_description)
  {
    lexeme::size_type _word_size = p_lexeme.size();
    if ((m_y_limit < y(_word_size)) && (m_x_limit < x(_word_size))) {

      if (m_y_limit < y(_word_size)) {
        std::stringstream _stream;
        _stream << p_lexeme << "'s size is " << _word_size
                << ", which is bigger than the vertical limit of "
                << m_y_limit;
        std::string _msg = _stream.str();

        crosswords_log_error(log, _stream.str());
        throw std::runtime_error(_msg);
      }
      if (m_x_limit < x(_word_size)) {
        std::string _msg(
              p_lexeme + "'s size is " + std::to_string(_word_size) +
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
    m_words.add(word(static_cast<word::id>(m_words.get_size() + 1),
                     p_lexeme,
                     p_description));
  }

  words operator()()
  {
    m_words.sort();
    crosswords_log_debug(log, "words after ordered ",
                         print_words(m_words.begin(), m_words.end()));

    //        words _original(m_words);

    //        uint32_t _counter = 0;
    crosswords_log_info(log, "STARTING ", std::time(nullptr));

    words::iterator _begin = m_words.begin();
    words::iterator _end = m_words.end();
    words::iterator _ite = _begin;

    while (true) {

      if (_ite == _end) {
        break;
      }

      //      crosswords_log_info(log, "           COUNTER = ", m_counter++);
      //      if (m_counter > 100000) {
      //        return m_words;
      //      }

      if (position(_begin, std::next(_ite))){
        ++_ite;
      }
      else {
        crosswords_log_warn(log, "failed to position ", print_words(_begin, std::next(_ite)));
        add_failure(_begin, std::next(_ite));
        uint8_t _shifter = 0;
        while(true) {

          words::iterator _aux = std::next(_ite, ++_shifter);
          crosswords_log_debug(log, "before swapping ", print_words(_begin, std::next(_ite)));
          std::iter_swap(_ite, _aux);
          crosswords_log_debug(log, "after swapping ", print_words(_begin, std::next(_ite)));

          if (_ite == _end) {
            break;
          }

          if (!already_tried(_begin, std::next(_ite))) {
            crosswords_log_debug(log, "new set ",
                                 print_words(_begin, std::next(_ite)));
            break;
          }
          crosswords_log_debug(log, "new set ", print_words(_begin, std::next(_ite)),
                               " was already tried");

          crosswords_log_debug(log, "before swapping back ", print_words(_begin, std::next(_ite)));
          std::iter_swap(_aux, _ite);
          crosswords_log_debug(log, "after swapping ", print_words(_begin, std::next(_ite)));
        }
      }
    }
    crosswords_log_info(log, "FINISHING ", std::time(nullptr));
    return m_words;
  }

private:
  typedef std::list<word::id> words_ids;
  typedef std::list<words_ids> failures;

private:
  bool position(words::iterator p_first, words::iterator p_end)
  {
    words::iterator _ite = p_first;
    uint32_t _counter = 0;


    crosswords_log_info(log, "############ ", _counter++, " - ",
                        print_words(p_first, p_end));

    while (true) {

      if (m_counter > 100000) {
        crosswords_log_fatal(log, "Maximum number of permutations in set ", print_words(p_first, p_end));
        return false;
      }

      if (_ite == p_end) {
        break;
      }


      crosswords_log_debug(log, "trying to position ", _ite->get_lexeme());
      if (_ite->positioned()) {
        crosswords_log_debug(log, _ite->get_lexeme(), " was previously positioned");
        ++_ite;
      } else if (m_word_positioner(p_first, _ite, m_x_limit, m_y_limit)) {
        crosswords_log_debug(log, _ite->get_lexeme(), " was positioned");
        //m_words.print_positioned(m_x_limit, m_y_limit);
        ++_ite;
      } else {
        crosswords_log_debug(log, _ite->get_lexeme(), " was NOT positioned");
        if (!std::next_permutation(p_first, p_end, words::cmp_words())) {
          crosswords_log_warn(log, "no more permutations!");
          return false;
        }
        crosswords_log_info(log, "############ ", _counter++, " - ",
                            print_words(p_first, p_end));

        _ite = p_first;
      }
    }
    return true;
  }

  bool already_tried(words::const_iterator p_begin,
                     words::const_iterator p_end)
  {
    auto _num_words = std::distance(p_begin, p_end);

    for (const words_ids& _words_ids : m_failures) {
      bool _match = true;
      if (_words_ids.size() == static_cast<words_ids::size_type>(_num_words)) {
        words::const_iterator _word_ite = p_begin;
        for (const word::id _word_id : _words_ids) {
          if (_word_id != _word_ite->get_id()) {
            _match = false;
            break;
          }
          ++_word_ite;
        }
        if (_match) {
          return true;
        }
      }
    }
    return false;
  }

  void add_failure(words::const_iterator p_begin, words::const_iterator p_end)
  {
    words_ids _words_ids;
    for (words::const_iterator _ite = p_begin; _ite != p_end; ++_ite) {
      _words_ids.push_back(_ite->get_id());
    }
    m_failures.push_back(std::move(_words_ids));
  }
  void unposition(words::iterator p_begin, words::iterator p_end)
  {
    for (words::iterator _ite = p_begin; _ite != p_end; ++_ite) {
      _ite->unposition();
    }
  }

  std::string print_words(words::const_iterator p_begin,
                          words::const_iterator p_end)
  {
    words::const_iterator _last = std::prev(p_end);
    std::stringstream _stream;
    _stream << "{";
    for (words::const_iterator _ite = p_begin; _ite != p_end; ++_ite) {
      _stream << _ite->get_lexeme();
      if (_ite != _last) {
        _stream << " ";
      }
    }
    _stream << "}";
    return _stream.str();
  }

private:
  uint32_t m_counter = { 0 };
  x m_x_limit;
  y m_y_limit;
  words m_words;
  failures m_failures;
  word_positioner m_word_positioner;
};
} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // POSITIONER003_H
