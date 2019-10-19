#ifndef TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_002_H
#define TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_002_H

//#include <algorithm>
//#include <ctime>
//#include <iostream>
//#include <list>
//#include <set>
//#include <sstream>
//#include <string>

//#include <crosswords/business/internal/log.h>
//#include <crosswords/business/internal/position_word.h>
//#include <crosswords/business/internal/positions_occupied.h>
//#include <crosswords/entities/coordinate.h>
//#include <crosswords/entities/description.h>
//#include <crosswords/entities/lexeme.h>
//#include <crosswords/entities/word.h>
//#include <crosswords/entities/words.h>

// namespace tenacitas {
// namespace crosswords {
// namespace business {

///// \brief positioner002_t position \p words
/////
///// \tparam t_log provides log funcionality:
///// static void debug(const std::string & p_file, int p_line, const
///// t_params&... p_params)
///// static void info(const std::string & p_file, int p_line, const
///t_params&...
///// p_params)
///// static void warn(const std::string & p_file, int p_line, const
///t_params&...
///// p_params)
///// static void error(const std::string & p_file, int p_line, const
///// t_params&... p_params)
///// static void fatal(const std::string & p_file, int p_line, const
///// t_params&... p_params)
/////
// template<typename t_log>
// struct positioner002_t
//{
//    typedef t_log log;

//    typedef entities::words words;
//    typedef entities::word word;
//    typedef entities::lexeme lexeme;
//    typedef entities::description description;
//    typedef entities::coordinate::x x;
//    typedef entities::coordinate::y y;

//    typedef positions_occupied_t<log> positions_occupied;
//    typedef position_word_t<log> position_word;

//    explicit positioner002_t(x p_x_limit = x(13), y p_y_limit = y(13))
//      : m_x_limit(p_x_limit)
//      , m_y_limit(p_y_limit)
//    {}

//    void add(lexeme&& p_lexeme, description&& p_description)
//    {
//        lexeme::size_type _word_size = p_lexeme.size();
//        if ((m_y_limit < y(_word_size)) && (m_x_limit < x(_word_size))) {

//            if (m_y_limit < y(_word_size)) {
//                std::stringstream _stream;
//                _stream << p_lexeme << "'s size is " << _word_size
//                        << ", which is bigger than the vertical limit of "
//                        << m_y_limit;
//                std::string _msg = _stream.str();

//                crosswords_log_error(log, _stream.str());
//                throw std::runtime_error(_msg);
//            }
//            if (m_x_limit < x(_word_size)) {
//                std::string _msg(
//                  p_lexeme + "'s size is " + std::to_string(_word_size) +
//                  ", which is bigger than the horizontal "
//                  "limit of " +
//                  std::to_string(m_x_limit.get_value<int16_t>()));
//                crosswords_log_error(log, _msg);
//                throw std::runtime_error(_msg);
//            }
//        }
//        crosswords_log_debug(log,
//                             "'",
//                             p_lexeme,
//                             "', with description '",
//                             p_description,
//                             "' was added");
//        m_words.push_back(word(
//          static_cast<word::id>(m_words.size() + 1), p_lexeme,
//          p_description));
//    }

//    words operator()()
//    {
//        std::sort(m_words.begin(), m_words.end(), cmp_words());
//        position_word _position_word;
//        uint32_t _counter = 0;
//        crosswords_log_info(log, "STARTING ", std::time(nullptr));
//        while (true) {
//            std::stringstream _stream;
//            for (const word& _w : m_words) {
//                _stream << _w.get_lexeme() << " ";
//            }
//            std::cerr << _counter++ << " - " << _stream.str() << std::endl;

//            bool _positioned = true;
//            words::iterator _end = m_words.end();
//            words::iterator _begin = m_words.begin();
//            for (words::iterator _ite = _begin; _ite != _end; ++_ite) {
//                if (!_position_word(m_words,
//                                    _ite,
//                                    m_x_limit,
//                                    m_y_limit,
//                                    m_positions_occupied)) {
//                    _positioned = false;
//                    break;
//                }
//            }
//            if (_positioned) {
//                break;
//            }

//            m_positions_occupied.clear();
//            for (word& _w : m_words) {
//                _w.unposition();
//            }

//            // find next permutation
//            if (!std::next_permutation(_begin, _end, cmp_words())) {
//                break;
//            }
//        }
//        crosswords_log_info(log, "FINISHING ", std::time(nullptr));
//        return m_words;
//    }

//  private:
//    struct cmp_words
//    {
//        bool operator()(const word& p_w1, const word& p_w2)
//        {
//            if (p_w1.get_size() > p_w2.get_size()) {
//                return true;
//            }
//            if (p_w1.get_size() == p_w2.get_size()) {
//                return p_w1.get_lexeme() > p_w2.get_lexeme();
//            }
//            return false;
//        }
//    };

//  private:
//    x m_x_limit;
//    y m_y_limit;
//    words m_words;
//    positions_occupied m_positions_occupied;
//};

//} // namespace business
//} // namespace crosswords
//} // namespace tenacitas

#endif // POSITIONER002_H
