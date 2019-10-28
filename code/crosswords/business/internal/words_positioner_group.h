#ifndef TENACITAS_CROSSWORDS_BUSINESS_WORDS_POSITIONER_GROUP_H
#define TENACITAS_CROSSWORDS_BUSINESS_WORDS_POSITIONER_GROUP_H

#include <chrono>
#include <condition_variable>
#include <functional>
#include <thread>
#include <vector>

#include <crosswords/messages/not_positioned.h>
#include <crosswords/messages/to_position.h>
#include <crosswords/messages/positioned.h>
#include <concurrent/business/dispatcher.h>
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
  typedef business::words_positioner_t<log> words_positioner;
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
//    m_thread_pool.add_work(8,
//                           [this]() -> worker {
//                             return words_positioner(
//                             this, m_x_limit, m_y_limit);
//                           },
//                           std::chrono::milliseconds(10000));
    dispatcher_positioned::subscribe([this](messages::positioned &&p_positioned)->bool{
      return this->handle_positioned(std::move(p_positioned));
    },
    std::chrono::milliseconds(1000));

    dispatcher_not_positioned::subscribe(
          [this](messages::not_positioned &&p_not_positioned)->bool{
      return this->handle_not_positioned(std::move(p_not_positioned));
    },
    std::chrono::milliseconds(1000));
  }


  //  words_positioner_group_t()=default;
  words_positioner_group_t(const words_positioner_group_t&) = delete;
  words_positioner_group_t(words_positioner_group_t&&) noexcept = default;
  words_positioner_group_t& operator=(const words_positioner_group_t&) = delete;
  words_positioner_group_t& operator=(
      words_positioner_group_t&& p_positioner) noexcept {
    using namespace std;
    if (this != &p_positioner) {
      m_x_limit = std::move(p_positioner.m_x_limit);
      m_y_limit = std::move(p_positioner.m_y_limit);

//      m_mutex_positioned = std::move(p_positioner.m_mutex_positioned);
      m_positioned = p_positioner.m_positioned;

      m_result = std::move(p_positioner.m_result);

      m_num_perms = p_positioner.m_num_perms;

//      m_mutex_not_positioned =std::move(p_positioner.m_mutex_not_positioned);

      m_num_not_positioned= p_positioner.m_num_not_positioned;

      m_no_more_permutations=p_positioner.m_no_more_permutations;

//      m_cond_wait = std::move(p_positioner.m_cond_wait);
//      m_mutex_wait = std::move(p_positioner.m_mutex_wait);
    }
    return *this;
  }
  ~words_positioner_group_t() = default;


  bool handle_positioned(messages::positioned && p_positioned) {
    std::lock_guard<std::mutex> _lock(m_mutex_positioned);
    if (m_positioned!=true) {
      m_positioned=true;
      m_result = p_positioned.get_words();
    }
    return true;
  }

  bool handle_not_positioned(messages::not_positioned&&) {
    std::lock_guard<std::mutex> _lock(m_mutex_not_positioned);
    ++m_num_not_positioned;
    return true;
  }

  std::pair<bool, words> operator()(words::iterator p_begin,
                                    words::iterator p_end)
  {

    std::pair<bool, words> _res = { false, words() };

    {
        std::lock_guard<std::mutex> _lock(m_mutex_positioned);
        m_positioned=false;
        m_result.clear();
        m_num_perms = 1;
        m_num_not_positioned = 0;
        m_no_more_permutations=false;
    }

    while (true) {
      if (m_positioned) {
        _res = {true, m_result};
        break;
      }

      crosswords_log_debug(log, "dispatching ", print_words(p_begin, p_end));
      dispatcher_to_position::publish(
            messages::to_position(words(p_begin, p_end)));

      if (m_positioned) {
        _res = {true, m_result};
        break;
      }

      if (m_no_more_permutations) {
        crosswords_log_info(log, "waiting for ", m_num_perms,
                            " permutations to be tried");
        std::unique_lock<std::mutex> _lock(m_mutex_wait);
        m_cond_wait.wait(_lock, [this]()->bool{
          return ((m_positioned) || (m_num_not_positioned == m_num_perms));
        });
        if (m_positioned) {
          _res = {true, m_result};
        }
        else {
           _res = {false, words()};
        }
        break;
      }
      else if (!std::next_permutation(p_begin, p_end, words::cmp_words())) {
          m_no_more_permutations=true;
          crosswords_log_warn(log, "no more permutations; total = ",
                              m_num_perms);
        }
        else {
          crosswords_log_debug(log, "permutation #: ", m_num_perms++, ": ",
                               print_words(p_begin, p_end));
        }
    }

    return _res;
  }

private:

  typedef concurrent::business::dispatcher_t<messages::to_position,
  log> dispatcher_to_position;

  typedef concurrent::business::dispatcher_t<messages::positioned,
  log> dispatcher_positioned;

  typedef concurrent::business::dispatcher_t<messages::not_positioned,
  log> dispatcher_not_positioned;

private:
  x m_x_limit;
  y m_y_limit;

  std::mutex m_mutex_positioned;
  bool m_positioned = false;

  words m_result;

  uint64_t m_num_perms = 1;

  std::mutex m_mutex_not_positioned;
  uint64_t m_num_not_positioned= 0;

  bool m_no_more_permutations=false;

  std::condition_variable m_cond_wait;
  std::mutex m_mutex_wait;
};
} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // WORDS_POSITIONER_GROUP_H
