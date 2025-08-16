#ifndef TNCT_CROSSWORDS_BUS_INTERNAL_ASSEMBLER_H
#define TNCT_CROSSWORDS_BUS_INTERNAL_ASSEMBLER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <atomic>
#include <cmath>
#include <condition_variable>
#include <memory>
#include <sys/sysinfo.h>
#include <tuple>

// #include "tnct/crosswords/bus/internal/dispatcher.h"

#include "tnct/async/cpt/is_dispatcher.h"
#include "tnct/async/handling_priority.h"
#include "tnct/crosswords/bus/internal/organizer.h"
#include "tnct/crosswords/dat/grid.h"
#include "tnct/crosswords/evt/grid_attempt_configuration.h"
#include "tnct/crosswords/evt/grid_permutations_tried.h"
#include "tnct/crosswords/evt/grid_to_organize.h"
#include "tnct/format/fmt.h"
#include "tnct/log/cpt/logger.h"
#include "tnct/math/factorial.h"
#include "tnct/time/cpt/chrono_convertible.h"

using namespace std::chrono_literals;

namespace tnct::crosswords::bus::internal
{

/// \brief Tries to assemble a grid
template <log::cpt::logger t_logger, async::cpt::is_dispatcher t_dispatcher>
struct assembler
{
  using events_published =
      std::tuple<evt::grid_attempt_configuration, evt::grid_permutations_tried,
                 evt::grid_to_organize>;

  using events_subscribed =
      std::tuple<evt::grid_create_stop, evt::grid_to_organize>;

  assembler(t_logger &p_logger, t_dispatcher &p_dispatcher)
      : m_logger(p_logger), m_dispatcher(p_dispatcher),
        m_organizer(p_logger, m_dispatcher)
  {
    configure_dispatcher(
        static_cast<size_t>(std::thread::hardware_concurrency()));
  }
  assembler()                             = delete;
  assembler(const assembler &)            = delete;
  assembler(assembler &&)                 = delete;
  assembler &operator=(const assembler &) = delete;
  assembler &operator=(assembler &&)      = delete;
  ~assembler()
  {
    stop();
  }

  /// \brief Tries to assemble a tnct::crosswords::dat::grid
  ///
  /// \param p_entries entries used to assemble the grid
  ///

  template <time::cpt::convertible_to_sec t_duration>
  std::shared_ptr<dat::grid> start(const dat::entries &p_entries,
                                   dat::index p_num_rows, dat::index p_num_cols,
                                   t_duration p_wait_for)
  {

    TNCT_LOG_DEB(m_logger,
                 format::fmt("# queues for 'grid_to_organize': ",
                             m_dispatcher.template amount_of_queues<
                                 crosswords::evt::grid_to_organize>()));

    m_stop             = false;
    m_solved           = std::shared_ptr<dat::grid>();
    m_organizers_fails = 0;
    m_timeout          = false;
    m_dispatcher.template clear<crosswords::evt::grid_to_organize>(
        m_grid_to_organize);

    auto _entries{std::make_shared<dat::entries>(p_entries)};
    sort_entries(*_entries);

    auto _maybe{tnct::math::factorial<size_t>(_entries->get_num_entries())};
    if (!_maybe)
    {
      TNCT_LOG_WAR(m_logger, format::fmt("overflow when calculating factorial "
                                         "for '_entries->get_num_entries()' (",
                                         _entries->get_num_entries(), ")"));

      return nullptr;
    }
    size_t _max_permutations = _maybe.value();

    m_wait_for = std::chrono::duration_cast<std::chrono::seconds>(p_wait_for);

    auto _initial_free_memory{free_mem()};

    m_dispatcher.template publish<assembler, evt::grid_attempt_configuration>(
        p_num_rows, p_num_cols,
        static_cast<decltype(_initial_free_memory)>(_initial_free_memory
                                                    * m_perc_memory_to_be_used),
        _initial_free_memory, _max_permutations);

    TNCT_LOG_DEB(m_logger, format::fmt("amount of free memory = ",
                                       _initial_free_memory()));

    // first permutation
    dat::permutation _permutation;
    for (dat::entries::const_entry_ite _entry = _entries->begin();
         _entry != _entries->end(); ++_entry)
    {
      _permutation.push_back(_entry);
    }

    size_t _permutation_counter{0};

    auto _initial = std::chrono::high_resolution_clock::now();

    TNCT_LOG_DEB(m_logger, format::fmt("initial = ",
                                       _initial.time_since_epoch().count()));

    size_t     _slept{0};
    const auto _amount_to_sleep{100ms};

    while (true)
    {
      if (should_break(_initial, _permutation_counter, _max_permutations))
      {
        break;
      }

      dat::permutation _aux{_permutation.size()};
      std::reverse_copy(_permutation.begin(), _permutation.end(), _aux.begin());

      if (should_break(_initial, _permutation_counter, _max_permutations))
      {
        break;
      }

      auto _grid{std::make_shared<dat::grid>(
          _entries, _aux, p_num_rows, p_num_cols, ++_permutation_counter)};

      if (std::remainder(_permutation_counter, 5000) == 0)
      {
        TNCT_LOG_DEB(m_logger, format::fmt("permutation counter = ",
                                           _permutation_counter));
        m_dispatcher.template publish<assembler, evt::grid_permutations_tried>(
            _permutation_counter);
      }

      if (should_break(_initial, _permutation_counter, _max_permutations))
      {
        break;
      }

      m_dispatcher.template publish<assembler, evt::grid_to_organize>(_grid);

      if (should_break(_initial, _permutation_counter, _max_permutations))
      {
        break;
      }

      std::unique_lock<std::mutex> _lock_wait(m_mutex_wait);
      m_cond_wait.wait_for(
          _lock_wait, _amount_to_sleep,
          [&]()
          {
            if (!should_break(_initial, _permutation_counter, _max_permutations)
                && should_wait(_initial_free_memory))
            {
              ++_slept;
              return false;
            }
            return true;
          });

      if (should_break(_initial, _permutation_counter, _max_permutations))
      {
        break;
      }

      std::next_permutation(_permutation.begin(), _permutation.end(),
                            [this](dat::entries::const_entry_ite p_e1,
                                   dat::entries::const_entry_ite p_e2) -> bool
                            { return compare_entries(*p_e1, *p_e2); });
    }

    auto _current = std::chrono::high_resolution_clock::now();
    auto _elapsed =
        std::chrono::duration_cast<std::chrono::seconds>(_current - _initial);

    TNCT_LOG_DEB(
        m_logger,
        format::fmt("left the loop, solved = ", (m_solved ? "Y" : "N"),
                    ", timeout = ", (m_timeout ? "Y" : "N"),
                    ", stop = ", (m_stop ? "Y" : "N"), ", waiting more ",
                    (m_wait_for
                     + std::chrono::seconds(_slept * _amount_to_sleep.count()))
                        - _elapsed,
                    " secs"));

    std::unique_lock<std::mutex> _lock_finish(m_mutex_finish);
    m_cond_finish.wait_for(
        _lock_finish,
        (m_wait_for + std::chrono::seconds(_slept * _amount_to_sleep.count()))
            - _elapsed,
        [&]() { return (m_solved || m_timeout || m_stop); });

    TNCT_LOG_DEB(m_logger, "left the condition");

    if (m_solved)
    {

      TNCT_LOG_DEB(m_logger, "grid solved");

      m_dispatcher.template clear<evt::grid_to_organize>();
      return m_solved;
    }

    if (m_timeout)
    {

      TNCT_LOG_DEB(m_logger, "timeout");

      m_dispatcher.template clear<evt::grid_to_organize>();
    }

    return {};
  }

  /// \brief Stops assembling the grid
  inline void stop()
  {
    m_stop = true;
  }

private:
  bool should_wait(size_t p_initial_free_memory)
  {
#ifdef TENACITAS_LOG
    static size_t _counter{0};
#endif
    size_t _current_free_mem{free_mem()};

    if (m_max_mem_occupied)
    {

      if (m_dispatcher.template occupied_in_queue<evt::grid_to_organize>(
              m_grid_to_organize)
          < m_dispatcher.template queue_size<evt::grid_to_organize>(
              m_grid_to_organize))
      {
        TNCT_LOG_DEB(
            m_logger,
            format::fmt(
                m_dispatcher.template occupied_in_queue<evt::grid_to_organize>(
                    m_grid_to_organize),
                "/",
                m_dispatcher.template queue_size<evt::grid_to_organize>(
                    m_grid_to_organize),
                " - DONT WAIT"));
        return false;
      }
      return true;
    }

    if (_current_free_mem <= static_cast<decltype(_current_free_mem)>(
            m_perc_memory_to_be_used * p_initial_free_memory))
    {
#ifdef TENACITAS_LOG
      if (std::remainder(_counter++, 50000) == 0)
      {
        TNCT_LOG_DEB(m_logger,
                     format::fmt("current free mem = ", _current_free_mem));
      }
#endif
      m_max_mem_occupied = true;

      TNCT_LOG_DEB(m_logger,
                   format::fmt("allowed memory occupied at = ",
                               std::chrono::high_resolution_clock::now()
                                   .time_since_epoch()
                                   .count()));

      return true;
    }
    return false;
  }

  bool compare_entries(const dat::entry &p_e1, const dat::entry &p_e2)
  {
    if (p_e1.get_word().size() == p_e2.get_word().size())
    {
      return p_e1.get_word() < p_e2.get_word();
    }
    return p_e1.get_word().size() < p_e2.get_word().size();
  }

  void sort_entries(dat::entries &p_entries)
  {
    std::sort(p_entries.begin(), p_entries.end(),
              [&](const dat::entry &p_e1, const dat::entry &p_e2)
              { return compare_entries(p_e1, p_e2); });
  }

  void configure_dispatcher(size_t p_hw_num_threads)
  {

    TNCT_LOG_DEB(m_logger, "configuring queue for event evt::grid_create_stop");

    m_dispatcher.template subscribe<assembler, evt::grid_create_stop>(
        [&](auto) -> void { m_stop = true; },
        async::handling_priority::highest);

    m_grid_to_organize = m_dispatcher.template add_queue<evt::grid_to_organize>(
        async::handling_priority::highest);

    TNCT_LOG_DEB(m_logger,
                 format::fmt("p_hw_num_threads= ", p_hw_num_threads,
                             ", and queue id for evt::grid_to_organize = ",
                             m_grid_to_organize));

    for (decltype(p_hw_num_threads) _i = 0; _i < p_hw_num_threads; ++_i)
    {

      TNCT_LOG_DEB(m_logger, format::fmt("subscriber # ", _i,
                                         " for evt::grid_to_organize"));

      m_dispatcher.template subscribe<assembler, evt::grid_to_organize>(
          m_grid_to_organize,
          [&](auto p_event) -> void
          { on_new_grid_to_organize(std::move(p_event)); });
    }

    TNCT_LOG_DEB(m_logger,
                 "configuring queue for event evt::grid_create_solved");
  }

  bool
  should_break(const std::chrono::time_point<std::chrono::high_resolution_clock>
                     &p_initial,
               size_t p_permutation_counter, size_t p_max_permutations)
  {
    if (m_stop)
    {

      TNCT_LOG_DEB(m_logger, "stop requested");

      return true;
    }

    if (m_solved)
    {

      TNCT_LOG_DEB(m_logger, "grid assembled");

      return true;
    }

    if (p_permutation_counter > p_max_permutations)
    {

      TNCT_LOG_DEB(m_logger, "max permutations reached");

      return true;
    }

    auto _current = std::chrono::high_resolution_clock::now();
    auto _elapsed =
        std::chrono::duration_cast<std::chrono::seconds>(_current - p_initial);
    m_timeout = (_elapsed > m_wait_for);
    if (m_timeout)
    {

      TNCT_LOG_DEB(m_logger, "timeout");

      return true;
    }
    return false;
  }

  void on_new_grid_to_organize(evt::grid_to_organize &&p_event)
  {
    if (p_event.grid == nullptr)
    {
      TNCT_LOG_ERR(m_logger,
                   "could not call organizer because p_event.grid is null");
      return;
    }
    if (m_solved)
    {
      m_cond_wait.notify_all();
      return;
    }
    //    internal::organizer _organizer{m_dispatcher};

    TNCT_LOG_TRA(m_logger, format::fmt("calling organizer for permutation ",
                                       p_event.grid->get_permutation_number()));

    if (m_organizer(p_event.grid))
    {

      TNCT_LOG_DEB(m_logger,
                   format::fmt("grid was organized at permutation ",
                               p_event.grid->get_permutation_number()));

      // m_dispatcher. template clear<evt::grid_to_organize>();
      m_solved = p_event.grid;
      m_cond_wait.notify_all();
      m_cond_finish.notify_one();
      return;
    }

    // if assembler is waiting for an available slot, in case memory is full, we
    // notify that at one permutation was removed from the queues, but only if
    // the grid was not solved by another organizer
    if (!m_solved)
    {
      m_cond_wait.notify_all();
    }

#ifdef TENACITAS_LOG
    ++m_organizers_fails;
    if (std::remainder(m_organizers_fails, 5000) == 0)
    {

      TNCT_LOG_DEB(
          m_logger,
          format::fmt(
              m_organizers_fails, " attempts failed, with queue size = ",
              m_dispatcher
                  .template get_num_events<crosswords::evt::grid_to_organize>(
                      m_grid_to_organize),
              " and occupied = ",
              m_dispatcher.template occupied_in_queue<
                  crosswords::evt::grid_to_organize>(m_grid_to_organize)));
    }
#endif
  }

  size_t free_mem()
  {
    struct sysinfo sys_info;
    if (sysinfo(&sys_info) != 0)
    {
      return 0;
    }
    return static_cast<size_t>(sys_info.freeram * sys_info.mem_unit);
  }

private:
  t_logger                                   &m_logger;
  t_dispatcher                               &m_dispatcher;
  internal::organizer<t_logger, t_dispatcher> m_organizer;

  bool m_stop{false};

  std::atomic_bool     m_timeout{false};
  std::chrono::seconds m_wait_for;

  std::shared_ptr<dat::grid> m_solved;
  async::handling_name       m_grid_to_organize{"grid-to-organize"};
  size_t                     m_organizers_fails{0};

  std::mutex              m_mutex_finish;
  std::condition_variable m_cond_finish;

  std::mutex              m_mutex_wait;
  std::condition_variable m_cond_wait;

  std::atomic_bool m_max_mem_occupied{false};

  static constexpr float m_perc_memory_to_be_used{0.5};
};

} // namespace tnct::crosswords::bus::internal

#endif // ORGANIZER_H
