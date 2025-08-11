#ifndef TENACITAS_SRC_CROSSWORDS_BUS_INTERNAL_ASSEMBLER_H
#define TENACITAS_SRC_CROSSWORDS_BUS_INTERNAL_ASSEMBLER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <memory>
#include <sys/sysinfo.h>
#include <tuple>

#include "tenacitas/src/crosswords/asy/dispatcher.h"
// #include "tenacitas/src/crosswords/bus/internal/dispatcher.h"
#include "tenacitas/src/asy/event_priority.h"
#include "tenacitas/src/container/matrix.h"
#include "tenacitas/src/crosswords/asy/events.h"
#include "tenacitas/src/crosswords/dat/grid.h"
#include "tenacitas/src/crosswords/bus/internal/organizer.h"
#include "tenacitas/src/fmt/format_number.h"
#include "tenacitas/src/log/log.h"
#include "tenacitas/src/mat/factorial.h"
#include "tenacitas/src/sta/chrono_convertible.h"

namespace tenacitas::src::crosswords::bus::internal {

/// \brief Tries to assemble a grid
struct assembler {
  using events_published =
      std::tuple<asy::grid_attempt_configuration, asy::grid_permutations_tried,
                 asy::grid_to_organize>;

  using events_subscribed =
      std::tuple<asy::grid_create_stop, asy::grid_to_organize>;

  assembler(asy::dispatcher::ptr p_dispatcher)
      : m_dispatcher(p_dispatcher), m_organizer(m_dispatcher) {
    configure_dispatcher(
        static_cast<size_t>(std::thread::hardware_concurrency()));
  }
  assembler() = delete;
  assembler(const assembler &) = delete;
  assembler(assembler &&) = delete;
  assembler &operator=(const assembler &) = delete;
  assembler &operator=(assembler &&) = delete;
  ~assembler() { stop(); }

  /// \brief Tries to assemble a tenacitas::src::crosswords::dat::grid
  ///
  /// \param p_entries entries used to assemble the grid
  ///

  template <tenacitas::src::sta::convertible_to_sec t_duration>
  std::shared_ptr<dat::grid> start(const dat::entries &p_entries,
                                   dat::index p_num_rows, dat::index p_num_cols,
                                   t_duration p_wait_for) {

#ifdef TENACITAS_LOG
    TNCT_LOG_DEB(
        "# queues for 'grid_to_organize': ",
        m_dispatcher->amount_of_queues<crosswords::asy::grid_to_organize>());
#endif

    m_stop = false;
    m_solved = std::shared_ptr<dat::grid>();
    m_organizers_fails = 0;
    m_timeout = false;
    m_dispatcher->clear<crosswords::asy::grid_to_organize>(m_grid_organize);

    auto _entries{std::make_shared<dat::entries>(p_entries)};
    sort_entries(*_entries);

    auto _maybe{tenacitas::src::mat::factorial<size_t>(_entries->get_num_entries())};
    if (!_maybe) {
      return nullptr;
    }
    size_t _max_permutations = _maybe.value();

    m_wait_for = std::chrono::duration_cast<std::chrono::seconds>(p_wait_for);

    auto _initial_free_memory{free_mem()};

    m_dispatcher->publish<assembler, asy::grid_attempt_configuration>(
        p_num_rows, p_num_cols,
        static_cast<decltype(_initial_free_memory)>(_initial_free_memory *
                                                    m_perc_memory_to_be_used),
        _initial_free_memory, _max_permutations);

#ifdef TENACITAS_LOG
    TNCT_LOG_DEB("amount of free memory = ", _initial_free_memory);
#endif

    // first permutation
    dat::permutation _permutation;
    for (dat::entries::const_entry_ite _entry = _entries->begin();
         _entry != _entries->end(); ++_entry) {
      _permutation.push_back(_entry);
    }

    size_t _permutation_counter{0};

    auto _initial = std::chrono::high_resolution_clock::now();
#ifdef TENACITAS_LOG
    TNCT_LOG_DEB("initial = ", _initial.time_since_epoch().count());
#endif
    size_t _slept{0};
    const auto _amount_to_sleep{100ms};

    while (true) {
      if (should_break(_initial, _permutation_counter, _max_permutations)) {
        break;
      }

      dat::permutation _aux{_permutation.size()};
      std::reverse_copy(_permutation.begin(), _permutation.end(), _aux.begin());

      if (should_break(_initial, _permutation_counter, _max_permutations)) {
        break;
      }

      auto _grid{std::make_shared<dat::grid>(
          _entries, _aux, p_num_rows, p_num_cols, ++_permutation_counter)};
#ifdef TENACITAS_LOG
      if (std::remainder(_permutation_counter, 5000) == 0) {
        TNCT_LOG_DEB("permutation counter = ", _permutation_counter);
        m_dispatcher->publish<assembler, asy::grid_permutations_tried>(
            _permutation_counter);
      }
#endif
      if (should_break(_initial, _permutation_counter, _max_permutations)) {
        break;
      }

      m_dispatcher->publish<assembler, asy::grid_to_organize>(_grid);

      if (should_break(_initial, _permutation_counter, _max_permutations)) {
        break;
      }

      std::unique_lock<std::mutex> _lock_wait(m_mutex_wait);
      m_cond_wait.wait_for(_lock_wait, _amount_to_sleep, [&]() {
        if (!should_break(_initial, _permutation_counter, _max_permutations) &&
            should_wait(_initial_free_memory)) {
          ++_slept;
          return false;
        }
        return true;
      });

      if (should_break(_initial, _permutation_counter, _max_permutations)) {
        break;
      }

      std::next_permutation(_permutation.begin(), _permutation.end(),
                            [this](dat::entries::const_entry_ite p_e1,
                                   dat::entries::const_entry_ite p_e2) -> bool {
                              return compare_entries(*p_e1, *p_e2);
                            });
    }

    auto _current = std::chrono::high_resolution_clock::now();
    auto _elapsed =
        std::chrono::duration_cast<std::chrono::seconds>(_current - _initial);

#ifdef TENACITAS_LOG
    TNCT_LOG_DEB(
        "left the loop, solved = ", (m_solved ? "Y" : "N"),
        ", timeout = ", (m_timeout ? "Y" : "N"),
        ", stop = ", (m_stop ? "Y" : "N"), ", waiting more ",
        (m_wait_for + std::chrono::seconds(_slept * _amount_to_sleep.count())) -
            _elapsed,
        " secs");
#endif

    std::unique_lock<std::mutex> _lock_finish(m_mutex_finish);
    m_cond_finish.wait_for(
        _lock_finish,
        (m_wait_for + std::chrono::seconds(_slept * _amount_to_sleep.count())) -
            _elapsed,
        [&]() { return (m_solved || m_timeout || m_stop); });

#ifdef TENACITAS_LOG
    TNCT_LOG_DEB("left the condition");
#endif

    if (m_solved) {
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB("grid solved");
#endif
      m_dispatcher->clear<asy::grid_to_organize>();
#ifdef TENACITAS_LOG
      // TNCT_LOG_DEB("asy::grid_to_organize queue cleared");
#endif
      return m_solved;
    }

    if (m_timeout) {
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB("timeout");
#endif
      m_dispatcher->clear<asy::grid_to_organize>();
    }

    return {};
  }

  /// \brief Stops assembling the grid
  inline void stop() { m_stop = true; }

private:
  bool should_wait(size_t p_initial_free_memory) {
#ifdef TENACITAS_LOG
    static size_t _counter{0};
#endif
    size_t _current_free_mem{free_mem()};

    if (m_max_mem_occupied) {

      if (m_dispatcher->occupied_in_queue<asy::grid_to_organize>(
              m_grid_organize) <
          m_dispatcher->queue_size<asy::grid_to_organize>(m_grid_organize)) {
#ifdef TENACITAS_LOG
        TNCT_LOG_DEB(
            m_dispatcher->occupied_in_queue<asy::grid_to_organize>(
                m_grid_organize),
            "/",
            m_dispatcher->queue_size<asy::grid_to_organize>(m_grid_organize),
            " - DONT WAIT");
#endif
        return false;
      }
      return true;
    }

    if (_current_free_mem <=
        static_cast<decltype(_current_free_mem)>(m_perc_memory_to_be_used *
                                                 p_initial_free_memory)) {
#ifdef TENACITAS_LOG
      if (std::remainder(_counter++, 50000) == 0) {
        TNCT_LOG_DEB("current free mem = ", _current_free_mem);
      }
#endif
      m_max_mem_occupied = true;
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB(
          "allowed memory occupied at = ",
          std::chrono::high_resolution_clock::now().time_since_epoch().count());
#endif
      return true;
    }
    return false;
  }

  bool compare_entries(const dat::entry &p_e1, const dat::entry &p_e2) {
    if (p_e1.get_word().size() == p_e2.get_word().size()) {
      return p_e1.get_word() < p_e2.get_word();
    }
    return p_e1.get_word().size() < p_e2.get_word().size();
  }

  void sort_entries(dat::entries &p_entries) {
    std::sort(p_entries.begin(), p_entries.end(),
              [&](const dat::entry &p_e1, const dat::entry &p_e2) {
                return compare_entries(p_e1, p_e2);
              });
  }

  void configure_dispatcher(size_t p_hw_num_threads) {
#ifdef TENACITAS_LOG
    TNCT_LOG_DEB("configuring queue for event asy::grid_create_stop");
#endif
    m_dispatcher->subscribe<assembler, asy::grid_create_stop>(
        [&](auto) -> void { m_stop = true; },
        tenacitas::src::asy::event_priority::highest);

    m_grid_organize = m_dispatcher->add_queue<asy::grid_to_organize>(
        tenacitas::src::asy::event_priority::highest);

#ifdef TENACITAS_LOG
    TNCT_LOG_DEB(
        "p_hw_num_threads= ", p_hw_num_threads,
        ", and queue id for asy::grid_to_organize = ", m_grid_organize);
#endif

    for (decltype(p_hw_num_threads) _i = 0; _i < p_hw_num_threads; ++_i) {
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB("subscriber # ", _i, " for asy::grid_to_organize");
#endif
      m_dispatcher->subscribe<assembler, asy::grid_to_organize>(
          m_grid_organize, [&](auto p_event) -> void {
            on_new_grid_to_organize(std::move(p_event));
          });
    }

#ifdef TENACITAS_LOG
    TNCT_LOG_DEB("configuring queue for event asy::grid_create_solved");
#endif
  }

  bool
  should_break(const std::chrono::time_point<std::chrono::high_resolution_clock>
                   &p_initial,
               size_t p_permutation_counter, size_t p_max_permutations) {
    if (m_stop) {
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB("stop requested");
#endif
      return true;
    }

    if (m_solved) {
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB("grid assembled");
#endif
      return true;
    }

    if (p_permutation_counter > p_max_permutations) {
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB("max permutations reached");
#endif
      return true;
    }

    auto _current = std::chrono::high_resolution_clock::now();
    auto _elapsed =
        std::chrono::duration_cast<std::chrono::seconds>(_current - p_initial);
    m_timeout = (_elapsed > m_wait_for);
    if (m_timeout) {
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB("timeout");
#endif
      return true;
    }
    return false;
  }

  void on_new_grid_to_organize(asy::grid_to_organize &&p_event) {
    if (p_event.grid == nullptr) {
      TNCT_LOG_ERR("could not call organizer because p_event.grid is null");
      return;
    }
    if (m_solved) {
      m_cond_wait.notify_all();
      return;
    }
//    internal::organizer _organizer{m_dispatcher};
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("calling organizer for permutation ",
                 p_event.grid->get_permutation_number());
#endif
    if (m_organizer(p_event.grid)) {
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB("grid was organized at permutation ",
                   p_event.grid->get_permutation_number());
#endif

      // m_dispatcher->clear<asy::grid_to_organize>();
      m_solved = p_event.grid;
      m_cond_wait.notify_all();
      m_cond_finish.notify_one();
      return;
    }

    // if assembler is waiting for an available slot, in case memory is full, we
    // notify that at one permutation was removed from the queues, but only if
    // the grid was not solved by another organizer
    if (!m_solved) {
      m_cond_wait.notify_all();
    }

#ifdef TENACITAS_LOG
    ++m_organizers_fails;
    if (std::remainder(m_organizers_fails, 5000) == 0) {
      std::stringstream _stream;
      _stream
          << m_organizers_fails << " attempts failed, with queue size = "
          << m_dispatcher->queue_size<crosswords::asy::grid_to_organize>(
                 m_grid_organize)
          << " and occupied = "
          << m_dispatcher->occupied_in_queue<crosswords::asy::grid_to_organize>(
                 m_grid_organize);
      std::string _str{_stream.str()};
      TNCT_LOG_DEB(_str);
    }
#endif
  }

  size_t free_mem() {
    struct sysinfo sys_info;
    if (sysinfo(&sys_info) != 0) {
      return 0;
    }
    return static_cast<size_t>(sys_info.freeram * sys_info.mem_unit);
  }

private:
  asy::dispatcher::ptr m_dispatcher;
  internal::organizer m_organizer;

  bool m_stop{false};

  std::atomic_bool m_timeout{false};
  std::chrono::seconds m_wait_for;

  std::shared_ptr<dat::grid> m_solved;
  tenacitas::src::asy::queue_id m_grid_organize;
  size_t m_organizers_fails{0};

  std::mutex m_mutex_finish;
  std::condition_variable m_cond_finish;

  std::mutex m_mutex_wait;
  std::condition_variable m_cond_wait;

  std::atomic_bool m_max_mem_occupied{false};

  static constexpr float m_perc_memory_to_be_used{0.5};
};

} // namespace tenacitas::src::crosswords::bus::internal

#endif // ORGANIZER_H
