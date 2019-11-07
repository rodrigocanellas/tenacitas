#ifndef TENACITAS_CROSSWORDS_BUSINESS_WORDS_POSITIONER_H
#define TENACITAS_CROSSWORDS_BUSINESS_WORDS_POSITIONER_H

#include <algorithm>
#include <ctime>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <thread>

#include <concurrent/business/dispatcher.h>
#include <concurrent/business/traits.h>
#include <crosswords/business/internal/log.h>
#include <crosswords/business/internal/positions_occupied.h>
#include <crosswords/business/internal/validate_position.h>
#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/description.h>
#include <crosswords/entities/lexeme.h>
#include <crosswords/entities/word.h>
#include <crosswords/entities/words.h>
#include <crosswords/messages/not_positioned.h>
#include <crosswords/messages/positioned.h>
#include <crosswords/messages/stop_positioning.h>
#include <crosswords/messages/to_position.h>

namespace tenacitas {
namespace crosswords {
namespace business {

/// \brief words_positioner_t tries to position a set of words
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
struct words_positioner_t
{
  typedef t_log log;

  typedef entities::words words;
  typedef entities::word word;
  typedef entities::lexeme lexeme;
  typedef entities::coordinate coordinate;
  typedef entities::coordinates coordinates;
  typedef entities::coordinate::x x;
  typedef entities::coordinate::y y;

  typedef positions_occupied_t<log> positions_occupied;
  typedef validate_position_t<log> validate_position;

  typedef concurrent::business::work_status work_status;
  typedef concurrent::business::dispatcher_t<messages::positioned<log>, log>
    dispatcher_positioned;

  typedef concurrent::business::dispatcher_t<messages::not_positioned, log>
    dispatcher_not_positioned;

  words_positioner_t(x p_x_limit, y p_y_limit)
    : m_x_limit(p_x_limit)
    , m_y_limit(p_y_limit)
  {}

  words_positioner_t() = default;
  words_positioner_t(const words_positioner_t&) = default;
  words_positioner_t(words_positioner_t&&) noexcept = default;
  words_positioner_t& operator=(const words_positioner_t&) = default;
  words_positioner_t& operator=(words_positioner_t&&) noexcept = default;
  ~words_positioner_t() = default;

  work_status operator()(messages::stop_positioning p_stop_positioning)
  {
    crosswords_log_debug(log,
                         "received stop positioning (",
                         p_stop_positioning.get_words().get_size(),
                         ",",
                         m_words.get_size(),
                         ")");
    if (p_stop_positioning.get_words().get_size() == m_words.get_size()) {
      crosswords_log_debug(log, "flaging to stop");
      m_stop_positioning = true;
    }
    return work_status::dont_stop;
  }

  work_status operator()(messages::to_position<log>&& p_to_position)
  {
    m_words = p_to_position.get_words();
    //    crosswords_log_debug(log, "not copying positions_occupied, and
    //    unpositiong");
    m_positions_occupied = p_to_position.get_positions_occupied();
    //    m_words.unposition();
    words::iterator _begin = m_words.begin();
    words::iterator _end = m_words.end();
    words::iterator _ite = _begin;
    m_last_first_position_horizontal.reset();
    m_last_first_position_vertical.reset();

    crosswords_log_info(log, "####### ", print_words(_begin, _end));

    //    crosswords_log_debug(log, "configuration: ",
    //                         crosswords::entities::print_positioned(_begin,
    //                         _end,
    //                                                    m_x_limit,
    //                                                    m_y_limit));
    crosswords_log_debug(
      log, "configuration: ", words(_begin, _end).print_words());

    m_stop_positioning = false;

    while (true) {
      if (m_stop_positioning) {
        return work_status::dont_stop;
      }
      if (m_position_first_status == position_first_status::done) {
        m_positions_occupied.clear();
        unposition(_begin, _end);
        m_position_first_status = position_first_status::horizontal;
        m_last_first_position_horizontal.reset();
        m_last_first_position_vertical.reset();

        crosswords_log_debug(log,
                             print_words(m_words.begin(), m_words.end()),
                             " was not positioned");
        // give up this set of words, in this order
        // returns true to indicate that it will receive other messages
        dispatcher_not_positioned::publish(m_words);
        return work_status::dont_stop;
      }
      bool _hope_to_position = true;
      while (true) {
        if (m_stop_positioning) {
          return work_status::dont_stop;
        }

        if (_ite == _end) {
          crosswords_log_debug(log,
                               "defining words: ",
                               print_words(m_words.begin(), m_words.end()));
          dispatcher_positioned::publish(m_words, m_positions_occupied);
          break;
        }

        crosswords_log_debug(log, "trying to position ", _ite->get_lexeme());
        if (m_stop_positioning) {
          return work_status::dont_stop;
        }
        if (_ite->positioned()) {
          crosswords_log_debug(log, _ite->get_lexeme(), " already positioned");
          ++_ite;
        } else {
          if (m_stop_positioning) {
            return work_status::dont_stop;
          }
          positioning _positioning = position(_begin, _end, _ite);
          if (_positioning == positioning::ok) {
            crosswords_log_debug(log, _ite->get_lexeme(), " positioned!");
            if (m_stop_positioning) {
              return work_status::dont_stop;
            }
            //            crosswords_log_debug(
            //              log, print_positioned(_begin, _end, m_x_limit,
            //              m_y_limit));
            crosswords_log_debug(log, words(_begin, _end).print_words());
            ++_ite;
          } else {
            if (m_stop_positioning) {
              return work_status::dont_stop;
            }
            if (_positioning == positioning::some_not_positioned) {
              m_positions_occupied.clear();
              unposition(_begin, _end);
              crosswords_log_debug(log, _ite->get_lexeme(), " not positioned");
              //                m_last_first_position_horizontal.reset();
              //                m_last_first_position_vertical.reset();
              _ite = _begin;
            } else { // positioning::first_not_positioned
              crosswords_log_debug(
                log, "first word ", _ite->get_lexeme(), " not positioned");
              _hope_to_position = false;
              break;
            }
            //            m_positions_occupied.clear();
            //            unposition(_begin, _ite);
            //            crosswords_log_debug(log, _ite->get_lexeme(), " not
            //            positioned"); _hope_to_position = false; break;
          }
        }
      }
      if (m_stop_positioning) {
        return work_status::dont_stop;
      }

      if (_hope_to_position) {
        break;
      }

      if (m_position_first_status == position_first_status::horizontal) {
        m_position_first_status = position_first_status::vertical;
      } else if (m_position_first_status == position_first_status::vertical) {
        m_position_first_status = position_first_status::done;
      }

      if (m_stop_positioning) {
        return work_status::dont_stop;
      }
      m_positions_occupied.clear();
      unposition(_begin, _end);
      m_last_first_position_horizontal.reset();
      m_last_first_position_vertical.reset();
      _ite = _begin;
    }
    // false to stop the async_loop where this function is running
    return work_status::dont_stop;
  }

private:
  enum class positioning : char
  {
    ok = 'o',
    first_not_positioned = 'f',
    some_not_positioned = 'n'
  };

  enum class position_first_status : char
  {
    horizontal = 'h',
    vertical = 'v',
    done = 'd'
  };

private:
  positioning position(words::iterator p_first_positioned,
                       words::const_iterator p_end,
                       words::iterator p_to_position)
  {
    crosswords_log_debug(log, "positioning ", *p_to_position);
    if (no_word_positioned(p_first_positioned, p_end)) {
      if (!position_first(p_to_position)) {
        return positioning::first_not_positioned;
      }
      return positioning::ok;
    }

    //    positioning _positioning = positioning::ok;
    //    if (m_position_from_first) {
    //      _positioning = position_from_first(p_first_positioned,
    //      p_to_position);

    //      if (_positioning == positioning::ok) {
    //        return positioning::ok;
    //      }

    //      m_positions_occupied.clear();
    //      unposition(p_first_positioned, p_to_position);

    //      return _positioning;
    //    }

    //    _positioning = position_from_last(p_first_positioned, p_to_position);

    //    if (_positioning == positioning::ok) {
    //      return positioning::ok;
    //    }
    //    return _positioning;

    return position_from_last(p_first_positioned, p_end, p_to_position);
    //    return position_from_first(p_first_positioned, p_to_position);
  }

  positioning position_from_last(words::iterator p_first_positioned,
                                 words::const_iterator p_end,
                                 words::iterator p_to_position)
  {

    words::const_iterator _last_positioned = std::prev(p_end);
    while (true) {
      if (_last_positioned == p_to_position) {
        crosswords_log_debug(log,
                             "avoiding to position ",
                             p_to_position->get_lexeme(),
                             " againt ",
                             _last_positioned->get_lexeme());
        if (_last_positioned != p_first_positioned) {
          --_last_positioned;
          continue;
        } else {
          return positioning::some_not_positioned;
        }
      }
      if (_last_positioned == p_first_positioned) {
        bool _is_positioned =
          position_x(p_first_positioned, _last_positioned, p_to_position);

        if (_is_positioned) {
          m_positions_occupied.add(*p_to_position);
          return positioning::ok;
        }

        crosswords_log_debug(log,
                             "unable to position ",
                             p_to_position->get_lexeme(),
                             " against ",
                             _last_positioned->get_lexeme());

        crosswords_log_debug(log, "trying to reposition first word");

        return positioning::some_not_positioned;
      }

      bool _is_positioned =
        position_x(p_first_positioned, _last_positioned, p_to_position);

      if (_is_positioned) {
        m_positions_occupied.add(*p_to_position);
        return positioning::ok;
      }

      crosswords_log_debug(log,
                           "unable to position ",
                           p_to_position->get_lexeme(),
                           " against ",
                           _last_positioned->get_lexeme());

      --_last_positioned;
    }
  }

  positioning position_from_first(words::iterator p_first_positioned,
                                  words::iterator p_to_position)
  {
    words::const_iterator _last_positioned = p_first_positioned;
    while (true) {
      if (_last_positioned == p_to_position) {
        break;
      }
      bool _is_positioned =
        position_x(p_first_positioned, _last_positioned, p_to_position);

      if (_is_positioned) {
        m_positions_occupied.add(*p_to_position);
        return positioning::ok;
      }

      crosswords_log_debug(log,
                           "unable to position ",
                           p_to_position->get_lexeme(),
                           " against ",
                           _last_positioned->get_lexeme());

      ++_last_positioned;
    }
    return positioning::some_not_positioned;
  }

  bool no_word_positioned(words::const_iterator p_begin,
                          words::const_iterator p_end) const
  {
    for (words::const_iterator _ite = p_begin; _ite != p_end; ++_ite) {
      if (_ite->positioned()) {
        return false;
      }
    }
    return true;
  }

  bool position_x(words::const_iterator p_first_positioned,
                  words::const_iterator p_last_positioned,
                  words::iterator p_to_position)
  {
    //        crosswords_log_debug(log,
    //                             "trying to position ",
    //                             p_to_position->get_lexeme(),
    //                             " against ",
    //                             *p_last_positioned);

    validate_position _validate_position;

    const lexeme& _positioned = p_last_positioned->get_lexeme();
    lexeme::size_type _positioned_size = _positioned.size();

    const lexeme& _to_position = p_to_position->get_lexeme();
    lexeme::size_type _to_position_size = _to_position.size();

    for (lexeme::size_type _i_positioned = 0; _i_positioned < _positioned_size;
         ++_i_positioned) {
      for (lexeme::size_type _i_to_position = 0;
           _i_to_position < _to_position_size;
           ++_i_to_position) {
        if (_to_position[_i_to_position] == _positioned[_i_positioned]) {
          //                    crosswords_log_debug(log,
          //                                         "intesect at ",
          //                                         _i_positioned,
          //                                         " in positioned
          //                                         ", _positioned,
          //                                         " and in ",
          //                                         _i_to_position,
          //                                         " in ",
          //                                         _to_position);
          if (p_last_positioned->get_direction() == word::direction::vertical) {

            p_to_position->position(
              p_last_positioned->get_coordinates()[_i_positioned].get_x() -
                x(_i_to_position),
              p_last_positioned->get_coordinates()[_i_positioned].get_y(),

              word::direction::horizontal,
              word::orientation::forward);
          } else {
            p_to_position->position(
              p_last_positioned->get_coordinates()[_i_positioned].get_x(),
              p_last_positioned->get_coordinates()[_i_positioned].get_y() -
                y(_i_to_position),

              word::direction::vertical,
              word::orientation::forward);
          }
          //                    crosswords_log_debug(
          //                      log, "it seems ", *p_to_position, "
          //                      was positioned");
          if (_validate_position(p_first_positioned,
                                 p_to_position,
                                 _i_to_position,
                                 m_positions_occupied,
                                 m_x_limit,
                                 m_y_limit)) {
            return true;
          }
          //                    crosswords_log_debug(log, "invalid
          //                    position");
          p_to_position->unposition();
        }
      }
    }
    return false;
  }

  bool position_first_horizontal(words::iterator p_to_position,
                                 lexeme::size_type p_lexeme_size)
  {
    if (x(p_lexeme_size) > m_x_limit) {
      return false;
    }
    if (m_last_first_position_horizontal == coordinate(x(-1), y(-1))) {
      m_last_first_position_horizontal = coordinate(x(0), y(0));
    } else {
      if ((m_last_first_position_horizontal.get_x() + x(1) +
           x(p_lexeme_size)) >= m_x_limit) {
        if (m_last_first_position_horizontal.get_y() == (m_y_limit - y(1))) {
          return false;
        }
        m_last_first_position_horizontal =
          coordinate(x(0), m_last_first_position_horizontal.get_y() + y(1));
      } else {
        m_last_first_position_horizontal =
          coordinate(m_last_first_position_horizontal.get_x() + x(1),
                     m_last_first_position_horizontal.get_y());
      }
    }

    p_to_position->position(m_last_first_position_horizontal.get_x(),
                            m_last_first_position_horizontal.get_y(),
                            word::direction::horizontal,
                            word::orientation::forward);
    return true;
  }

  bool position_first_verical(words::iterator p_to_position,
                              lexeme::size_type p_lexeme_size)
  {
    if (y(p_lexeme_size) > m_y_limit) {
      return false;
    }
    if (m_last_first_position_vertical == coordinate(x(-1), y(-1))) {
      m_last_first_position_vertical = coordinate(x(0), y(0));
    } else {
      if ((m_last_first_position_vertical.get_y() + y(1) + y(p_lexeme_size)) >=
          m_y_limit) {
        if (m_last_first_position_vertical.get_x() == (m_x_limit - x(1))) {
          return false;
        }
        m_last_first_position_vertical =
          coordinate(m_last_first_position_vertical.get_x() + x(1), y(0));
      } else {
        m_last_first_position_vertical =
          coordinate(m_last_first_position_vertical.get_x(),
                     m_last_first_position_vertical.get_y() + y(1));
      }
    }
    p_to_position->position(m_last_first_position_vertical.get_x(),
                            m_last_first_position_vertical.get_y(),
                            word::direction::vertical,
                            word::orientation::forward);

    return true;
  }

  bool position_first(words::iterator p_to_position)
  {

    lexeme::size_type _lexeme_size = p_to_position->get_lexeme().size();

    if (m_position_first_status == position_first_status::horizontal) {

      if (position_first_horizontal(p_to_position, _lexeme_size)) {
        m_positions_occupied.add(*p_to_position);
        return true;
      }
      return false;
    }

    if (position_first_verical(p_to_position, _lexeme_size)) {
      m_positions_occupied.add(*p_to_position);
      return true;
    }
    return false;
  }

  void unposition(words::iterator p_begin, words::iterator p_end)
  {
    for (words::iterator _ite = p_begin; _ite != p_end; ++_ite) {
      _ite->unposition();
    }
  }

private:
private:
  positions_occupied m_positions_occupied;
  coordinate m_last_first_position_horizontal = { x(-1), y(-1) };
  coordinate m_last_first_position_vertical = { x(-1), y(-1) };
  words m_words;
  x m_x_limit;
  y m_y_limit;

  position_first_status m_position_first_status =
    position_first_status::horizontal;

  bool m_stop_positioning = false;
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // WORD_POSITIONER_H
