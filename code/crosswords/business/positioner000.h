#ifndef TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_000_H
#define TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_000_H

#include <algorithm>
#include <cstdint>
#include <ctime>
#include <limits>
#include <list>
#include <sstream>
#include <string>
#include <utility>

#include <crosswords/business/internal/log.h>
#include <crosswords/entities/description.h>
#include <crosswords/entities/limit.h>
#include <crosswords/entities/position.h>
#include <crosswords/entities/positions.h>
#include <crosswords/entities/word.h>

namespace tenacitas {
namespace crosswords {
namespace business {

/// \brief positioner000_t positions words
///
/// \param t_data is the type of the data to be handled. If it is not \p void,
/// it must be:
///    - default constructible
///    - move constructible
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
struct positioner000_t
{

    typedef entities::positions positions;
    typedef entities::position position;
    typedef position::direction direction;
    typedef position::orientation orientation;
    typedef entities::word word;
    typedef entities::description description;
    typedef entities::limit limit;
    typedef t_log log;

    enum class result : int16_t
    {
        ok = 0,
        word_overflows_horizontal = 1,
        word_overflows_vertical = 2
    };

    positioner000_t(limit p_vertical_limit, limit p_horizontal_limit)
      : m_vertical(p_vertical_limit)
      , m_horizontal(p_horizontal_limit)
    {
        crosswords_log_debug(
          log, "vertical = ", m_vertical, ", horizontal = ", m_horizontal);
    }

    result add(const word& p_word, const description& p_description)
    {
        limit _word_size = static_cast<limit>(p_word.size());
        if (_word_size > m_vertical) {
            crosswords_log_error(
              log,
              p_word,
              "'s size is ",
              _word_size,
              ", which is bigger than the vertical limit of ",
              m_vertical);
            return result::word_overflows_vertical;
        }
        if (_word_size > m_horizontal) {
            crosswords_log_error(
              log,
              p_word,
              "'s size is ",
              _word_size,
              ", which is bigger than the horizontal limit of ",
              m_horizontal);
            return result::word_overflows_horizontal;
        }
        crosswords_log_debug(log,
                             "'",
                             p_word,
                             "', with description '",
                             p_description,
                             "' was added");
        m_positions.push_back(
          position(static_cast<position::id>(m_positions.size() + 1),
                   p_word,
                   p_description));
        return result::ok;
    }

    positions operator()()
    {
        log_positions();
        order_positions_by_size();
        create_first_history();
        log_current_history();
        position_words();
        optimize_positions();
        return m_positions;
    }

  private:
    typedef std::list<typename positions::const_iterator> positions_iterators;

    typedef std::list<positions_iterators> positions_history;

  private:
    inline void order_positions_by_size()
    {
        crosswords_log_debug(log,
                             "ordering positions by word size, descending");
        m_positions.sort(
          [](const position& p_pos1, const position& p_pos2) -> bool {
              return (p_pos1.get_word().size() > p_pos2.get_word().size());
          });
    }

    inline void create_first_history()
    {
        positions_iterators _first;
        crosswords_log_debug(log, "creating first positions history");
        for (positions::const_iterator _ite = m_positions.begin();
             _ite != m_positions.end();
             ++_ite) {
            _first.push_back(_ite);
        }
        m_positions_history.push_back(std::move(_first));
    }

    void log_current_history()
    {
        define_current_history();
        crosswords_log_debug(log, "log_current_history");
        positions_iterators::const_iterator _end = m_current_history->end();
        positions_iterators::const_iterator _ite = m_current_history->begin();

        int16_t _i = 0;
        for (; _ite != _end; ++_ite) {
            positions::const_iterator _pos = *_ite;
            crosswords_log_debug(log, "\t", _i, ":", &(*_pos));
            ++_i;
        }
    }

    std::string positions_ite2str(
      positions_iterators::const_iterator p_ite) const
    {
        std::stringstream _stream;
        positions::const_iterator _pos = *p_ite;
        _stream << "(" << &(*_pos) << "," << (*p_ite)->get_id() << ","
                << (*p_ite)->get_word() << ")";
        return _stream.str();
    }
    void log_positions() const
    {
        crosswords_log_debug(log, "log_positions");
        positions::const_iterator _end = m_positions.end();
        positions::const_iterator _ite = m_positions.begin();
        int16_t _i = 0;
        for (; _ite != _end; ++_ite) {
            crosswords_log_debug(log,
                                 "\t",
                                 _i,
                                 "=(",
                                 &(*_ite),
                                 ",",
                                 _ite->get_id(),
                                 ",",
                                 _ite->get_word(),
                                 ")");

            ++_i;
        }
    }

    bool position_words()
    {
        crosswords_log_debug(log, "positon_words");

        bool _is_positioned = true;

        define_current_history();
        positions_iterators::const_iterator _current_history_ite =
          m_current_history->begin();

        while (true) {
            crosswords_log_debug(log,
                                 "starting loop, trying to position ",
                                 positions_ite2str(_current_history_ite));
            if (positioned(_current_history_ite)) {
                crosswords_log_debug(log,
                                     positions_ite2str(_current_history_ite),
                                     " was positioned ");
                if (m_positioned.size() == m_current_history->size()) {
                    crosswords_log_debug(log, "all positioned");
                    _is_positioned = true;
                    break;
                }
                ++_current_history_ite;
                crosswords_log_debug(log,
                                     "new current_history_ite = ",
                                     positions_ite2str(_current_history_ite));
            } else {
                crosswords_log_debug(log,
                                     positions_ite2str(_current_history_ite),
                                     " was not positioned");
                if ((*_current_history_ite)->get_id() ==
                    m_current_history->back()->get_id()) {
                    crosswords_log_debug(
                      log, "and it was the last, so we clear positioned list");
                    m_positioned.clear();
                }
                crosswords_log_debug(log, "updating history");
                update_history(_current_history_ite);
                crosswords_log_debug(log, "checking for cycle");
                if (cycle()) {
                    crosswords_log_debug(log, "CYCLE!!");
                    _is_positioned = false;
                    break;
                }
                define_current_history();
                _current_history_ite = m_current_history->begin();
                crosswords_log_debug(log,
                                     "new current history defined, ",
                                     positions_ite2str(_current_history_ite),
                                     " and is the new current");
                positions_iterators::size_type _size = m_positioned.size();
                crosswords_log_debug(
                  log, "moving to the right position in the history");
                for (positions_iterators::size_type _counter = 0;
                     _counter < _size;
                     ++_counter) {
                    crosswords_log_debug(
                      log, "\t", positions_ite2str(_current_history_ite));
                    ++_current_history_ite;
                }
            }
        }
        return _is_positioned;
    }

    void update_history(positions_iterators::const_iterator p_ite)
    {
        //        crosswords_log_debug(log, "updating history");

        positions_iterators _positions_iterators(*m_current_history);

        _positions_iterators.remove(*p_ite);

        _positions_iterators.push_back(*p_ite);

        m_positions_history.push_back(std::move(_positions_iterators));
    }

    bool positioned(positions_iterators::const_iterator /*p_positon_iterator*/)
    {
        if (m_positioned.empty()) {
            position_first();
            return true;
        }
        return false;
    }

    inline void define_current_history()
    {
        m_current_history = m_positions_history.end();
        --m_current_history;
    }

    bool cycle()
    {
        if (m_positions_history.size() <= 1) {
            return false;
        }

        positions_history::const_iterator _last = m_positions_history.end();
        --_last;
        positions_history::const_iterator _ite = m_positions_history.begin();
        while (true) {
            if (_ite == _last) {
                break;
            }
            if (!equals(*_ite, *_last)) {
                return false;
            }
            ++_ite;
        }
        return true;
    }

    bool equals(const positions_iterators& p_ite1,
                const positions_iterators& p_ite2)
    {
        positions_iterators::const_iterator _end1 = p_ite1.end();
        positions_iterators::const_iterator _ite1 = p_ite1.begin();

        positions_iterators::const_iterator _end2 = p_ite2.end();
        positions_iterators::const_iterator _ite2 = p_ite2.begin();

        while (true) {
            if ((_ite1 == _end1) || (_ite2 == _end2)) {
                break;
            }

            if ((*_ite1)->get_id() != (*_ite2)->get_id()) {
                return false;
            }
            ++_ite1;
            ++_ite2;
        }
        return true;
    }

    //    bool equals(const positons_iterators& p_ids1,
    //                const positons_iterators& p_ids2)
    //    {
    //        positons_ids::const_iterator _end1 = p_ids1.end();
    //        positons_ids::const_iterator _ite1 = p_ids1.begin();
    //        positons_ids::const_iterator _end2 = p_ids2.end();
    //        positons_ids::const_iterator _ite2 = p_ids2.begin();
    //        while (true) {
    //            if ((_ite1 == _end1) || (_ite2 != _end2)) {
    //                break;
    //            }

    //            if (*_ite1 != *_ite2) {
    //                return false;
    //            }
    //        }
    //        return true;
    //    }

    void position_first()
    {
        typename positions::iterator _begin = m_positions.begin();
        if (m_vertical < m_horizontal) {
            limit x = static_cast<limit>(m_horizontal / 2);
            limit y = static_cast<limit>(m_vertical / 2) -
                      static_cast<limit>(_begin->get_word().size() / 2);
            _begin->define(x, y, direction::vertical, orientation::forward);
        } else {
            limit x = static_cast<limit>(m_horizontal / 2) -
                      static_cast<limit>(_begin->get_word().size() / 2);
            limit y = static_cast<limit>(m_vertical / 2);
            _begin->define(x, y, direction::horizontal, orientation::forward);
        }
        m_positioned.push_back(_begin);
    }

    void optimize_positions()
    {
        limit _min_x = std::numeric_limits<limit>::max();
        limit _min_y = _min_x;

        {
            positions::const_iterator _end = m_positions.end();
            for (positions::const_iterator _ite = m_positions.begin();
                 _ite != _end;
                 ++_ite) {
                if (_ite->get_x() < _min_x) {
                    _min_x = _ite->get_x();
                }
                if (_ite->get_y() < _min_y) {
                    _min_y = _ite->get_y();
                }
            }
        }
        positions::iterator _end = m_positions.end();
        if ((_min_x > 0) || (_min_y > 0)) {
            for (positions::iterator _ite = m_positions.begin(); _ite != _end;
                 ++_ite) {
                _ite->define(_ite->get_x() - _min_x,
                             _ite->get_y() - _min_y,
                             _ite->get_direction(),
                             _ite->get_orientation());
            }
        }
    }

  private:
    limit m_vertical;
    limit m_horizontal;

    positions m_positions;
    positions_history m_positions_history;
    positions_iterators m_positioned;
    positions_history::const_iterator m_current_history;
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // POSITIONER_H
