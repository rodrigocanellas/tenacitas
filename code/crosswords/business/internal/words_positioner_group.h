#ifndef TENACITAS_CROSSWORDS_BUSINESS_WORDS_POSITIONER_GROUP_H
#define TENACITAS_CROSSWORDS_BUSINESS_WORDS_POSITIONER_GROUP_H

#include <functional>
#include <thread>
#include <chrono>
#include <vector>

#include <concurrent/business/dispatcher.h>
#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/words.h>
#include <crosswords/business/internal/words_positioner.h>


namespace tenacitas {
namespace crosswords {
namespace business {

/// \brief words_positioner_group_t manages threads od \p words_positioner
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
struct words_positioner_group_t
{
  typedef t_log log;
  typedef entities::words words;
  typedef business::words_positioner_t<log> words_positioner;
  typedef concurrent::business::dispatcher_t<words, log> dispatcher;
  typedef typename dispatcher::worker worker;
  typedef entities::coordinate coordinate;
  typedef entities::coordinates coordinates;
  typedef entities::coordinate::x x;
  typedef entities::coordinate::y y;

  words_positioner_group_t(x p_x_limit, y p_y_limit)
    : m_x_limit(p_x_limit)
    , m_y_limit(p_y_limit)
  {
//    m_thread_pool.add_work(8,
//                           [this]() -> worker {
//                             words_positioner _words_positioner(m_x_limit,
//                                                                 m_y_limit);

//                             worker _work =
//                             [_words_positioner](words && p_words) -> bool {
//                               _words_positioner(p_words.begin(), p_words.end());
//                             };
//                           },
//                           std::chrono::milliseconds (10000)
//                           );

  }

  bool operator()(words::iterator p_begin, words::iterator p_end) {
    words _words(p_begin, p_end);
//    m_thread_pool.handle(_words);
  }

private:


private:
  x m_x_limit;
  y m_y_limit;

//  thread_pool m_thread_pool;


};
} // namespace business
} // namespace crosswords
} // namespace tenacitas


#endif // WORDS_POSITIONER_GROUP_H
