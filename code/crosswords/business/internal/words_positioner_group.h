#ifndef TENACITAS_CROSSWORDS_BUSINESS_WORDS_POSITIONER_GROUP_H
#define TENACITAS_CROSSWORDS_BUSINESS_WORDS_POSITIONER_GROUP_H

#include <chrono>
#include <condition_variable>
#include <functional>
#include <thread>
#include <vector>

#include <concurrent/business/thread_pool.h>
#include <crosswords/business/internal/words_positioner.h>
#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/words.h>

namespace tenacitas {
namespace crosswords {
namespace business {

/// \brief words_positioner_group_t manages threads of \p words_positioner
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
  typedef business::words_positioner_t<words_positioner_group_t, log>
    words_positioner;
  typedef concurrent::business::thread_pool_t<words, log> thread_pool;
  typedef typename thread_pool::worker worker;
  typedef entities::coordinate coordinate;
  typedef entities::coordinates coordinates;
  typedef entities::coordinate::x x;
  typedef entities::coordinate::y y;

  words_positioner_group_t(x p_x_limit, y p_y_limit)
    : m_x_limit(p_x_limit)
    , m_y_limit(p_y_limit)
  {
    m_thread_pool.add_work(8,
                           [this]() -> worker {
                             return words_positioner(
                               this, m_x_limit, m_y_limit);
                           },
                           std::chrono::milliseconds(10000));
    m_thread_pool.run();
  }

  //  words_positioner_group_t()=default;
  words_positioner_group_t(const words_positioner_group_t&) = delete;
  words_positioner_group_t(words_positioner_group_t&&) noexcept = default;
  words_positioner_group_t& operator=(const words_positioner_group_t&) = delete;
  words_positioner_group_t& operator=(
    words_positioner_group_t&& p_positioner) noexcept
  {
    if (this != &p_positioner) {
      m_x_limit = std::move(p_positioner.m_x_limit);
      m_y_limit = std::move(p_positioner.m_y_limit);
      m_thread_pool = std::move(p_positioner.m_thread_pool);
    }
    return *this;
  }
  ~words_positioner_group_t() = default;

  void set_result(words&& p_words)
  {
    crosswords_log_debug(log,
                         "defining result before: ",
                         print_words(p_words.begin(), p_words.end()));
    {
      std::lock_guard<std::mutex> _lock(m_mutex_result);
      if (m_result.get_size() == 0) {
        m_result = std::move(p_words);
        crosswords_log_debug(log,
                             "defining result after: ",
                             print_words(m_result.begin(), m_result.end()));
      } else {
        crosswords_log_debug(log, "ignoring results");
      }
    }
  }

  std::pair<bool, words> operator()(words::iterator p_begin,
                                    words::iterator p_end)
  {

    words _words(p_begin, p_end);
    std::pair<bool, words> _res = { false, words() };

    //    m_thread_pool.run();
    while (true) {
      crosswords_log_debug(
        log, "adding ", print_words(_words.begin(), _words.end()));

      m_thread_pool.handle(_words);
      {
        crosswords_log_debug(log, "waiting for result or timeout");
        std::unique_lock<std::mutex> _lock(m_mutex);
        if (m_cond.wait_for(_lock, std::chrono::milliseconds(100), [this]() {
              return m_result.get_size();
            })) {
          crosswords_log_debug(log,
                               "we have a result: ",
                               print_words(m_result.begin(), m_result.end()));
          _res = { true, m_result };
          break;
        }
      }
      if (!std::next_permutation(p_begin, p_end, words::cmp_words())) {
        crosswords_log_warn(log, "no more permutations");

        _res = { false, words() };
        break;
      }
      crosswords_log_debug(
        log, "new permutation before: ", print_words(p_begin, p_end));
      words _words(p_begin, p_end);
      crosswords_log_debug(log,
                           "new permutation after: ",
                           print_words(_words.begin(), _words.end()));
    }

    //    m_thread_pool.stop();
    return _res;
  }

private:
private:
  x m_x_limit;
  y m_y_limit;
  std::condition_variable m_cond;
  std::mutex m_mutex;
  std::mutex m_mutex_result;

  thread_pool m_thread_pool;

  words m_result;
};
} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // WORDS_POSITIONER_GROUP_H
