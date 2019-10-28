#ifndef TENACITAS_CROSSWORDS_BUSINESS_POSITIONER006_H
#define TENACITAS_CROSSWORDS_BUSINESS_POSITIONER006_H

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>
#include <utility>

#include <crosswords/business/internal/log.h>
#include <crosswords/business/internal/words_positioner_group.h>
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
struct positioner006_t
{
  typedef t_log log;

  typedef entities::words words;
  typedef entities::word word;
  typedef entities::lexeme lexeme;
  typedef entities::description description;
  typedef entities::coordinate::x x;
  typedef entities::coordinate::y y;

  typedef words_positioner_group_t<log> words_positioner_group;

  explicit positioner006_t(x p_x_limit = x(13), y p_y_limit = y(13))
    : m_x_limit(p_x_limit)
    , m_y_limit(p_y_limit)
      , m_words_positioner_group(p_x_limit, p_y_limit)
  {}

  positioner006_t(const positioner006_t&) = delete;
  positioner006_t(positioner006_t&&) noexcept = default;
  positioner006_t& operator=(const positioner006_t&) = delete;
  positioner006_t& operator=(positioner006_t&&) noexcept = default;
  ~positioner006_t() = default;

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
      static_cast<word::id>(m_words.get_size() + 1), p_lexeme, p_description));
  }

  inline x get_x_limit() const { return m_x_limit; }
  inline y get_y_limit() const { return m_y_limit; }

  words operator()()
  {
    using namespace std;
    m_words.sort();
    crosswords_log_debug(
      log, "words after ordered ",
          print_words(m_words.begin(), m_words.end()));
    // save original set
    words _original(m_words);

    auto _start_time = chrono::system_clock::now();

    words::iterator _begin = m_words.begin();
    words::iterator _end = m_words.end();
    words::iterator _ite = _begin;

    while (true) {

      if (_ite == _end) {
        break;
      }

      std::pair<bool, words> _res =
          m_words_positioner_group(_begin, next(_ite));
      if (_res.first) {
        crosswords_log_info(
          log, "old order: ",
              print_words(m_words.begin(), m_words.end()));
        crosswords_log_info(
          log, "result: ",
              print_words(_res.second.begin(), _res.second.end()));
        //        std::copy(_res.second.begin(), _res.second.end(),
        //        m_words.begin());
        words::size _size = _res.second.get_size();
        crosswords_log_info(log, "size = ", _size);

        words::iterator _old = std::next(m_words.begin(), _size);
        for (; _old != m_words.end(); ++_old) {
          _res.second.add(std::move(*_old));
        }

        m_words = std::move(_res.second);
        crosswords_log_info(
          log, "new order: ",
              print_words(m_words.begin(), m_words.end()));
        _ite = std::next(m_words.begin(), _size);
        crosswords_log_info(
          log, "new set: ",
              print_words(m_words.begin(), std::next(_ite)));
        _end = m_words.end();
        _begin = m_words.begin();
        print_positioned(m_words.begin(), m_words.end(), m_x_limit, m_y_limit);

      } else {
        crosswords_log_error(
          log, "unable to position ", print_words(_begin, next(_ite)));

        if (!already_tried(_begin, next(_ite))) {
          add_failure(_begin, next(_ite));
        }

        if (next(_ite) == _end) {
          crosswords_log_error(
            log, "no solution for ", print_words(_begin, next(_ite)));
          _ite = _end;
          break;
        }

        // changing [_begin, _ite] for [_begin, next(_ite, _shifter)]

        uint8_t _shifter = 0;

        while (true) {
          words::iterator _aux = next(_ite, ++_shifter);

          if (_aux == _end) {
            crosswords_log_error(
              log, "no solution for ", print_words(_begin, _end));
            _ite = _end;
            break;
          }

          iter_swap(_ite, _aux);

          if (!already_tried(_begin, next(_ite))) {
            crosswords_log_info(
              log, "new set ", print_words(_begin, next(_ite)));
            _ite = _begin;
            break;
          }

          crosswords_log_info(log,
                              print_words(_begin, std::next(_ite)),
                              " have already been tried");
          iter_swap(_aux, _ite);
        } // while
      }
    } // while

    auto _end_time = chrono::system_clock::now();
    chrono::duration<double> _diff = _end_time - _start_time;
    crosswords_log_info(log, "TIME ", _diff.count());
    return m_words;
  }

private:
  typedef std::list<word::id> words_ids;
  typedef std::list<words_ids> failures;

private:
  bool already_tried(words::const_iterator p_begin, words::const_iterator p_end)
  {
    auto _num_words = std::distance(p_begin, p_end);

    for (const words_ids& _words_ids : m_failures) {
      bool _match = true;
      if (_words_ids.size() <= static_cast<words_ids::size_type>(_num_words)) {
        words::const_iterator _word_ite = p_begin;
        for (const word::id _word_id : _words_ids) {
          word::id _fail_id = _word_ite->get_id();
          if (_word_id != _fail_id) {
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
  words_positioner_group m_words_positioner_group;
};
} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // POSITIONER003_H
