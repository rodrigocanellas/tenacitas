#ifndef TENACITAS_CROSSWORDS_BUSINESS_WORD_POSITIONER_H
#define TENACITAS_CROSSWORDS_BUSINESS_WORD_POSITIONER_H

#include <algorithm>
#include <ctime>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>

#include <crosswords/business/internal/log.h>
#include <crosswords/business/internal/positions_occupied.h>
#include <crosswords/business/internal/validate_position.h>
#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/description.h>
#include <crosswords/entities/lexeme.h>
#include <crosswords/entities/word.h>
#include <crosswords/entities/words.h>

namespace tenacitas {
namespace crosswords {
namespace business {

/// \brief positioner002_t position \p words
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
struct word_positioner_t {
    typedef entities::words words;
    typedef entities::word word;
    typedef entities::coordinate::x x;
    typedef entities::coordinate::y y;

    bool operator()(words::const_iterator /*p_begin*/,
                    words::const_iterator /*p_to_position*/,
                    x /*p_x_limit*/,
                    y /*p_y_limit*/) {

        return false;
    }

private:
    void unposition(words::iterator p_begin, words::iterator p_end) {
        for(words::iterator _ite = p_begin; _ite != p_end; ++_ite) {
            _ite->unposition();
        }
    }

};

} // namespace business
} // namespace crosswords
} // namespace tenacitas


#endif // WORD_POSITIONER_H
