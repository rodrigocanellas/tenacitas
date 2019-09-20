#ifndef TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_000_H
#define TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_000_H

#include <cstdint>
#include <list>
#include <string>
#include <utility>
#include <limits>

#include <crosswords/entities/positions.h>
#include <crosswords/entities/position.h>
#include <crosswords/entities/word.h>
#include <crosswords/entities/description.h>
#include <crosswords/entities/limit.h>
#include <crosswords/business/internal/log.h>

namespace tenacitas {
namespace crosswords {
namespace business {

template <typename t_log>
struct positioner000_t {


    typedef entities::positions positions;
    typedef entities::position position;
    typedef position::direction direction;
    typedef position::orientation orientation;
    typedef entities::word word;
    typedef entities::description description;
    typedef entities::limit limit;
    typedef t_log log;

    enum class result : int16_t {
        ok = 0
        , word_overflows_horizontal = 1
        , word_overflows_vertical = 2
    };

    positioner000_t(limit p_vertical_limit, limit p_horizontal_limit)
        : m_vertical(p_vertical_limit)
        , m_horizontal(p_horizontal_limit) {

        crosswords_log_debug(log, "vertical = ", m_vertical, ", horizontal = ", m_horizontal);
    }

    result add(const word & p_word, const description & p_description) {
        limit _word_size = static_cast<limit>(p_word.size());
        if ( _word_size > m_vertical)  {
            return result::word_overflows_vertical;
        }
        if (_word_size  > m_horizontal)  {
            return result::word_overflows_horizontal;
        }
        m_positions.push_back(position(static_cast<position::id>(m_positions.size()+1)
                                       , p_word
                                       , p_description));
        return result::ok;
    }

    positions operator()() {
        position_all();
        optimize_positions();
        return m_positions;
    }

private:
    typedef std::list<typename positions::iterator> positions_iterators;

private:
    void position_all() {
        return position_first();
        //        positions_iterators::iterator _pos = m_positions.begin();
        //        positions_iterators::iterator _end = m_positions.end();
        //        ++_pos;
        ////        for (_pos; _pos != _end; ++_pos) {

        ////        }



    }

    void position_first() {
        typename positions::iterator _begin = m_positions.begin();
        if (m_vertical < m_horizontal) {
            limit x = static_cast<limit>(m_horizontal/2);
            limit y = static_cast<limit>(m_vertical/2) -
                    static_cast<limit>(_begin->get_word().size()/2);
            _begin->define(x, y, direction::vertical, orientation::forward);
            m_positioned.push_back(_begin); }
        else {
            limit x = static_cast<limit>(m_horizontal/2)  -
                    static_cast<limit>(_begin->get_word().size()/2);
            limit y = static_cast<limit>(m_vertical/2);
            _begin->define(x, y, direction::horizontal, orientation::forward);

        }
        m_positioned.push_back(_begin);
    }

    void optimize_positions() {
        limit _min_x = std::numeric_limits<limit>::max();
        limit _min_y = _min_x;

        {
            positions::const_iterator _end = m_positions.end();
            for(positions::const_iterator _ite = m_positions.begin();
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
        if ( (_min_x > 0) || (_min_y > 0) ) {
            for(positions::iterator _ite = m_positions.begin();
                _ite != _end;
                ++_ite) {
                _ite->define(_ite->get_x() -_min_x
                             , _ite->get_y() - _min_y
                             , _ite->get_direction()
                             , _ite->get_orientation());
            }
        }
    }
private:

    positions m_positions;

    limit m_vertical;
    limit m_horizontal;

    positions_iterators m_positioned;

};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // POSITIONER_H
