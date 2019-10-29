#ifndef TENACITAS_CROSSWORDS_BUSINESS_WORDS_POSITIONER_GROUP_H
#define TENACITAS_CROSSWORDS_BUSINESS_WORDS_POSITIONER_GROUP_H

#include <chrono>
#include <condition_variable>
#include <functional>
#include <thread>
#include <vector>
#include <memory>

#include <crosswords/business/internal/positions_occupied.h>
#include <concurrent/business/dispatcher.h>
#include <crosswords/business/internal/words_positioner.h>
#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/words.h>
#include <crosswords/messages/not_positioned.h>
#include <crosswords/messages/positioned.h>
#include <crosswords/messages/to_position.h>
#include <crosswords/messages/stop_positioning.h>

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
  typedef positions_occupied_t<log> positions_occupied;

  typedef concurrent::business::dispatcher_t<messages::to_position<log>, log>
  dispatcher_to_position;

  typedef typename std::shared_ptr<dispatcher_to_position> dispatcher_to_position_ptr;

  typedef concurrent::business::dispatcher_t<messages::positioned<log>, log>
  dispatcher_positioned;

  typedef typename std::shared_ptr<dispatcher_positioned> dispatcher_positioned_ptr;

  typedef concurrent::business::dispatcher_t<messages::not_positioned, log>
  dispatcher_not_positioned;

  typedef typename std::shared_ptr<dispatcher_not_positioned> dispatcher_not_positioned_ptr;

  typedef concurrent::business::dispatcher_t<messages::stop_positioning, log>
  dispatcher_stop_positioning;

  typedef typename std::shared_ptr<dispatcher_stop_positioning>
  dispatcher_stop_positioning_ptr;

  words_positioner_group_t(x p_x_limit, y p_y_limit,
                           dispatcher_positioned_ptr p_dispatcher_positioned_ptr,
                           dispatcher_not_positioned_ptr p_dispatcher_not_positioned_ptr  )
    : m_x_limit(p_x_limit)
    , m_y_limit(p_y_limit)
    , m_dispatcher_to_position_ptr(std::make_shared<dispatcher_to_position>())
    , m_dispatcher_positioned_ptr(p_dispatcher_positioned_ptr)
    , m_dispatcher_not_positioned_ptr(p_dispatcher_not_positioned_ptr)
    , m_dispatcher_stop_positioning_ptr(std::make_shared<dispatcher_stop_positioning>())
  {

    if (!m_dispatcher_created) {
      //      dispatcher_to_position::subscribe(words_positioner(m_x_limit,
      //      m_y_limit),
      //                                        std::chrono::milliseconds(5000));

//      std::function<words_positioner()> _factory = [this]() {
//        return words_positioner(m_x_limit, m_y_limit,
//                                m_dispatcher_positioned_ptr,
//                                m_dispatcher_not_positioned_ptr);
//      };
//      m_dispatcher_to_position_ptr->subscribe(
//            _factory, 8, std::chrono::milliseconds(5000));

      {
        std::shared_ptr<words_positioner>
            _words_positioner(std::make_shared<words_positioner>
                              (m_x_limit, m_y_limit,
                               m_dispatcher_positioned_ptr,
                               m_dispatcher_not_positioned_ptr));

        m_dispatcher_to_position_ptr->subscribe(
              [_words_positioner](messages::to_position<log> && p_to_position)->bool
        {
          return (*_words_positioner)(std::move(p_to_position));
        },
        std::chrono::milliseconds(5000));

        m_dispatcher_stop_positioning_ptr->subscribe(
              [_words_positioner](messages::stop_positioning && p_stop_positioning)->bool
        {
          return (*_words_positioner)(std::move(p_stop_positioning));
        },
        std::chrono::milliseconds(5000));
      }

      {
        std::shared_ptr<words_positioner>
            _words_positioner(std::make_shared<words_positioner>
                              (m_x_limit, m_y_limit,
                               m_dispatcher_positioned_ptr,
                               m_dispatcher_not_positioned_ptr));

        m_dispatcher_to_position_ptr->subscribe(
              [_words_positioner](messages::to_position<log> && p_to_position)->bool
        {
          return (*_words_positioner)(std::move(p_to_position));
        },
        std::chrono::milliseconds(5000));

        m_dispatcher_stop_positioning_ptr->subscribe(
              [_words_positioner](messages::stop_positioning && p_stop_positioning)->bool
        {
          return (*_words_positioner)(std::move(p_stop_positioning));
        },
        std::chrono::milliseconds(5000));
      }

//      {
//        std::shared_ptr<words_positioner>
//            _words_positioner(std::make_shared<words_positioner>
//                              (m_x_limit, m_y_limit,
//                               m_dispatcher_positioned_ptr,
//                               m_dispatcher_not_positioned_ptr));

//        m_dispatcher_to_position_ptr->subscribe(
//              [_words_positioner](messages::to_position<log> && p_to_position)->bool
//        {
//          return (*_words_positioner)(std::move(p_to_position));
//        },
//        std::chrono::milliseconds(5000));

//        m_dispatcher_stop_positioning_ptr->subscribe(
//              [_words_positioner](messages::stop_positioning && p_stop_positioning)->bool
//        {
//          return (*_words_positioner)(std::move(p_stop_positioning));
//        },
//        std::chrono::milliseconds(5000));
//      }

//      {
//        std::shared_ptr<words_positioner>
//            _words_positioner(std::make_shared<words_positioner>
//                              (m_x_limit, m_y_limit,
//                               m_dispatcher_positioned_ptr,
//                               m_dispatcher_not_positioned_ptr));

//        m_dispatcher_to_position_ptr->subscribe(
//              [_words_positioner](messages::to_position<log> && p_to_position)->bool
//        {
//          return (*_words_positioner)(std::move(p_to_position));
//        },
//        std::chrono::milliseconds(5000));

//        m_dispatcher_stop_positioning_ptr->subscribe(
//              [_words_positioner](messages::stop_positioning && p_stop_positioning)->bool
//        {
//          return (*_words_positioner)(std::move(p_stop_positioning));
//        },
//        std::chrono::milliseconds(5000));
//      }


      m_dispatcher_created = true;
    }
  }

  //  words_positioner_group_t()=default;
  words_positioner_group_t(const words_positioner_group_t&) = delete;
  words_positioner_group_t(words_positioner_group_t&&) noexcept = default;
  words_positioner_group_t& operator=(const words_positioner_group_t&) = delete;
  words_positioner_group_t& operator=(
      words_positioner_group_t&& p_positioner) noexcept
  {
    using namespace std;
    if (this != &p_positioner) {
      m_x_limit = std::move(p_positioner.m_x_limit);
      m_y_limit = std::move(p_positioner.m_y_limit);
      m_positioned = p_positioner.m_positioned;
      m_result = std::move(p_positioner.m_result);
      m_num_perms_not_positioned = p_positioner.m_num_perms_not_positioned;
      m_num_perms_generated = p_positioner.m_num_perms_generated;
      m_last_size = p_positioner.m_last_size;
    }
    return *this;
  }
  ~words_positioner_group_t() = default;

  bool operator()(messages::positioned<log>&& p_positioned)
  {
    if (p_positioned.get_words().get_size() == 1) {
      crosswords_log_debug(log, "size of positioned = ",
                           p_positioned.get_words().get_size());

      if (m_positioned) {
        crosswords_log_debug(log, "already positioned");
        return true;
      }
      crosswords_log_debug(log, "handling positioned");
      std::lock_guard<std::mutex> _lock(m_mutex_positioned);
      if (m_positioned != true) {
        crosswords_log_debug(log, "seting m_positoned");
        m_positioned = true;
        m_result = p_positioned.get_words();
        m_positions_occupied = p_positioned.get_positions_occupied();
        m_last_size = m_result.get_size();
        m_cond_wait.notify_one();
      } else {
        crosswords_log_debug(log, "m_positioned is already set");
      }
    }
    else {
      if (p_positioned.get_words().get_size() <= m_last_size) {
        crosswords_log_debug(log, "size of positioned (",
                             p_positioned.get_words().get_size(),
                             ") is <= to the last size (", m_last_size, ")");
        return true;
      }
      if (m_positioned) {
        crosswords_log_debug(log, "already positioned");
        return true;
      }
      crosswords_log_debug(log, "handling positioned");
      std::lock_guard<std::mutex> _lock(m_mutex_positioned);
      if (m_positioned != true) {
        crosswords_log_debug(log, "seting m_positoned");
        m_positioned = true;
        m_result = p_positioned.get_words();
        m_positions_occupied = p_positioned.get_positions_occupied();
        m_last_size = m_result.get_size();
        m_cond_wait.notify_one();
      } else {
        crosswords_log_debug(log, "m_positioned is already set");
      }
    }
    return true;
  }

  bool operator()(messages::not_positioned&& p_not_positioned)
  {
    crosswords_log_debug(log, "handling not positioned");

    if (p_not_positioned.get_words().get_size() <= m_last_size) {
      return true;
    }
    if (m_positioned) {
      return true;
    }
    std::lock_guard<std::mutex> _lock(m_mutex_positioned);
    ++m_num_perms_not_positioned;
    if (m_num_perms_not_positioned == m_num_perms_generated) {
      crosswords_log_debug(log,
                           "m_num_perms_not_positioned = ",
                           m_num_perms_not_positioned,
                           ", m_num_perms_generated = ",
                           m_num_perms_generated);
      m_cond_wait.notify_one();
    }
    return true;
  }

  std::pair<bool, words> operator()(words::iterator p_begin,
                                    words::iterator p_end)
  {

    std::pair<bool, words> _res = { false, words() };

    bool _no_more_permutations = false;

    {
      std::lock_guard<std::mutex> _lock(m_mutex_positioned);
      m_positioned = false;
      m_result.clear();
      m_num_perms_not_positioned = 0;
      m_num_perms_generated = 1;
//      m_positions_occupied.clear();
    }

//    crosswords_log_debug(log, "configuration: ",
//                         entities::print_positioned(p_begin, p_end,
//                                                    m_x_limit, m_y_limit));

   crosswords_log_debug(log, "configuration: ", words(p_begin, p_end).print_words());
    while (true) {
      if (m_positioned) {
        _res = { true, m_result };
        break;
      }

      if (std::distance(p_begin, p_end) == 1) {
        crosswords_log_debug(log, "publishing ", print_words(p_begin, p_end));
        m_dispatcher_to_position_ptr->publish(
              messages::to_position<log>(words(p_begin, p_end), m_positions_occupied));
        crosswords_log_debug(log, "only one word to be positioned");
        _no_more_permutations = true;
        m_last_size = 1;
      }

      if (_no_more_permutations) {
        std::unique_lock<std::mutex> _lock(m_mutex_wait);
        m_cond_wait.wait(_lock);

        if (m_positioned) {
          crosswords_log_debug(log, "+++++ positioned! ", m_result.print_words());

          m_dispatcher_stop_positioning_ptr->publish(messages::stop_positioning(m_result));
          _res = { true, m_result };
        } else if (m_num_perms_not_positioned == m_num_perms_generated) {
          crosswords_log_warn(log, "all permutations were tried, and no was positioned");
          _res = { false, words() };
          break;
        }
      } else {
        crosswords_log_debug(log, "publishing permutation # ",
                             m_num_perms_generated++, ": ",
                             print_words(p_begin, p_end));
        m_dispatcher_to_position_ptr->publish(
              messages::to_position<log>(words(p_begin, p_end), m_positions_occupied));

        if (m_positioned) {
          _res = { true, m_result };
          break;
        }

        if (!std::next_permutation(p_begin, p_end, words::cmp_words())) {
          _no_more_permutations = true;
          crosswords_log_warn(
                log, "no more permutations; total = ", --m_num_perms_generated);
        }
      }
    }

    return _res;
  }

private:

private:
  x m_x_limit;
  y m_y_limit;
  dispatcher_to_position_ptr m_dispatcher_to_position_ptr;
  dispatcher_positioned_ptr m_dispatcher_positioned_ptr;
  dispatcher_not_positioned_ptr m_dispatcher_not_positioned_ptr;
  dispatcher_stop_positioning_ptr m_dispatcher_stop_positioning_ptr;
  std::mutex m_mutex_positioned;
  bool m_positioned = false;

  words m_result;

  uint64_t m_num_perms_not_positioned = 0;
  uint64_t m_num_perms_generated = 1;

  std::condition_variable m_cond_wait;
  std::mutex m_mutex_wait;

  words::size m_last_size = 0;

  positions_occupied m_positions_occupied;

  static bool m_dispatcher_created;
};
template<typename t_log>
bool words_positioner_group_t<t_log>::m_dispatcher_created(false);
} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // WORDS_POSITIONER_GROUP_H
