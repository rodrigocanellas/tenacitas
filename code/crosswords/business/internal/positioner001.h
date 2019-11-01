#ifndef TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_001_H
#define TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_001_H

//#include <algorithm>
//#include <ctime>
//#include <iostream>
//#include <list>
//#include <set>
//#include <sstream>
//#include <string>

//#include <crosswords/business/internal/log.h>
//#include <crosswords/business/internal/positions_occupied.h>
//#include <crosswords/business/internal/words_by_size.h>
//#include <crosswords/entities/coordinate.h>
//#include <crosswords/entities/description.h>
//#include <crosswords/entities/lexeme.h>
//#include <crosswords/entities/word.h>
//#include <crosswords/entities/words.h>

// namespace tenacitas {
// namespace crosswords {
// namespace business {

///// \brief positioner001_t position \p words
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
// struct positioner001_t
//{
//    typedef t_log log;

//    typedef entities::words words;
//    typedef entities::word word;
//    typedef entities::lexeme lexeme;
//    typedef entities::description description;
//    typedef entities::coordinate::x x;
//    typedef entities::coordinate::y y;

//    typedef words_by_size_t<log> words_by_size;
//    typedef std::list<words_by_size> words_by_size_list;
//    typedef positions_occupied_t<log> positions_occupied;

//    explicit positioner001_t(x p_x_limit = x(13), y p_y_limit = y(13))
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
//        std::sort(m_words.begin(), m_words.end(), crosswords::entities::cmp_words());
//        group_words();
//        typedef typename words_by_size_list::iterator iterator;
//        iterator _end = m_words_by_size_list.end();
//        iterator _begin = m_words_by_size_list.begin();
//        iterator _ite = _begin;
//        uint32_t _counter = 0;
//        crosswords_log_info(log, "STARTING ", std::time(nullptr));

//        while (true) {
//            crosswords_log_debug(log, "===========> ", _counter++);
//            if (_ite == _end) {
//                return m_words;
//            }

//            bool _positioned = _ite->position(
//              m_words, m_x_limit, m_y_limit, m_positions_occupied);
//            if (!_positioned) {
//                if (_ite == _begin) {
//                    crosswords_log_error(log, "fail to position");
//                    return m_words;
//                }
//                crosswords_log_warn(log,
//                                    "could not position words of"
//                                    "size ",
//                                    _ite->get_size());
//                --_ite;
//            } else {
//                ++_ite;
//            }
//            //            std::cerr << "type to continue...";
//            //            char _go;
//            //            std::cin >> _go;
//        }
//        crosswords_log_info(log, "FINISHING ", std::time(nullptr));
//        return m_words;
//    }

//  private:
//    void group_words()
//    {
//        lexeme::size_type _size = 0;
//        for (const word& _word : m_words) {
//            lexeme::size_type _word_size = _word.get_size();
//            if (_word_size != _size) {
//                _size = _word_size;
//                m_words_by_size_list.push_back(words_by_size(_size));
//            }
//        }
//        m_words_by_size_list.sort(
//          [](const words_by_size& p_wbs1, const words_by_size& p_wbs2) -> bool
//          {
//              return p_wbs1.get_size() > p_wbs2.get_size();
//          });
//    }

//  private:
//    x m_x_limit;
//    y m_y_limit;
//    words m_words;
//    words_by_size_list m_words_by_size_list;
//    positions_occupied m_positions_occupied;
//};

//} // namespace business
//} // namespace crosswords
//} // namespace tenacitas

#endif // POSITIONER001_H
