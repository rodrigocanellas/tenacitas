/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_CONTAINER_CIRCULAR_QUEUE_H
#define TENACITAS_LIB_CONTAINER_CIRCULAR_QUEUE_H

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

#include "tenacitas.lib/container/circular_queue_on_list.h"
#include "tenacitas.lib/generic/fmt.h"
#include "tenacitas.lib/generic/program_options.h"
#include "tenacitas.lib/log/cerr.h"
#include "tenacitas.lib/traits/queue.h"

using namespace tenacitas::lib;

struct circular_queue_test_base {
public:
  using queue = src::container::circular_queue_on_list<log::cerr, int16_t>;

protected:
  bool check(const queue &p_queue, size_t p_occupied, size_t p_capacity,
             log::cerr &p_log) {
    if (p_queue.capacity() != p_capacity) {
      p_log.err(src::generic::fmt("capacity should be ", p_capacity, " but it is ",
                             p_queue.capacity()));
      return false;
    }

    if (p_queue.occupied() != p_occupied) {
      p_log.err(src::generic::fmt("there should be ", p_occupied,
                             " slots occupied, but there are ",
                             p_queue.occupied()));
      return false;
    }
    return true;
  }

  void traverse(queue &p_queue, log::cerr &p_log) {
    // p_queue.traverse([&](const queue::data &p_data) -> void {
    //   p_log.tst(src::generic::fmt("--> ", p_data));
    // });
    p_log.tst(p_queue.full_report());
  }

  bool get(queue &p_queue, queue::data p_value, size_t p_occupied,
           size_t p_capacity, log::cerr &p_log) {
    queue::data _data;
    {
      std::optional<queue::data> _maybe{p_queue.pop()};
      if (!_maybe.has_value()) {
        p_log.err(
            src::generic::fmt("no data read, when it should have been ", p_value));
        return false;
      }
      _data = std::move(*_maybe);
    }
    if (_data != p_value) {
      p_log.err(
          src::generic::fmt("data should be ", p_value, ", but it is ", _data));
      return false;
    }

    p_log.tst(src::generic::fmt("getting ", _data));

    if (!check(p_queue, p_occupied, p_capacity, p_log)) {
      return false;
    }

    traverse(p_queue, p_log);
    return true;
  }

  bool add(queue &p_queue, queue::data p_value, size_t p_occupied,
           size_t p_capacity, log::cerr &p_log) {
    p_log.tst(src::generic::fmt("adding ", p_value));

    p_queue.push(p_value);

    if (!check(p_queue, p_occupied, p_capacity, p_log)) {
      return false;
    }
    // bool _full{p_occupied == p_capacity};
    // if (p_queue.full() != _full) {
    //   p_log.err(src::generic::fmt("It should be", (_full ? "" : " not"),
    //                          " full, but it is ", (!_full ? "" : "not ")));
    //   return false;
    // }

    traverse(p_queue, p_log);

    return true;
  }
};

struct circular_queue_000 : public circular_queue_test_base {
  static std::string desc() {
    return "Tests if the queue is correctly created";
  }

  bool operator()(const src::generic::program_options &) {
    log::cerr _log;

    queue _queue("q000", _log, 3);

    if (!check(_queue, 0, 3, _log)) {
      return false;
    }
    traverse(_queue, _log);

    return true;
  }
};

struct circular_queue_001 : public circular_queue_test_base {
  static std::string desc() { return "Adds -9 and gets it"; }

  bool operator()(const src::generic::program_options &) {
    log::cerr _log;
    queue _queue("q001", _log, 3);

    if (!check(_queue, 0, 3, _log)) {
      return false;
    }
    traverse(_queue, _log);

    if (!add(_queue, -9, 1, 3, _log)) {
      return false;
    }

    if (!get(_queue, -9, 0, 3, _log)) {
      return false;
    }

    return true;
  }
};

struct circular_queue_002 : public circular_queue_test_base {
  static std::string desc() { return "Adds -9 and tries to get 38"; }

  bool operator()(const src::generic::program_options &) {
    log::cerr _log;
    queue _queue("q002", _log, 3);

    if (!check(_queue, 0, 3, _log)) {
      return false;
    }
    traverse(_queue, _log);

    if (!add(_queue, -9, 1, 3, _log)) {
      return false;
    }

    if (get(_queue, 38, 0, 3, _log)) {
      return false;
    }

    return true;
  }
};

struct circular_queue_003 : public circular_queue_test_base {
  static std::string desc() { return "Adds -9 and 38 and gets -9"; }

  bool operator()(const src::generic::program_options &) {
    log::cerr _log;
    queue _queue("q003", _log, 3);

    if (!check(_queue, 0, 3, _log)) {
      return false;
    }
    traverse(_queue, _log);

    if (!add(_queue, -9, 1, 3, _log)) {
      return false;
    }

    if (!add(_queue, 38, 2, 3, _log)) {
      return false;
    }

    if (!get(_queue, -9, 1, 3, _log)) {
      return false;
    }

    return true;
  }
};

struct circular_queue_004 : public circular_queue_test_base {
  static std::string desc() { return "Adds -9 and 38 and gets both"; }

  bool operator()(const src::generic::program_options &) {
    log::cerr _log;
    queue _queue("q004", _log, 3);

    if (!check(_queue, 0, 3, _log)) {
      return false;
    }
    traverse(_queue, _log);

    if (!add(_queue, -9, 1, 3, _log)) {
      return false;
    }

    if (!add(_queue, 38, 2, 3, _log)) {
      return false;
    }

    if (!get(_queue, -9, 1, 3, _log)) {
      return false;
    }

    if (!get(_queue, 38, 0, 3, _log)) {
      return false;
    }

    return true;
  }
};

struct circular_queue_005 : public circular_queue_test_base {
  static std::string desc() { return "Adds -9, 38, 4, gets -9 and adds 5"; }

  bool operator()(const src::generic::program_options &) {
    log::cerr _log;
    queue _queue("q005", _log, 3);

    if (!check(_queue, 0, 3, _log)) {
      return false;
    }
    traverse(_queue, _log);

    if (!add(_queue, -9, 1, 3, _log)) {
      return false;
    }

    if (!add(_queue, 38, 2, 3, _log)) {
      return false;
    }

    if (!add(_queue, 4, 3, 3, _log)) {
      return false;
    }

    if (!get(_queue, -9, 2, 3, _log)) {
      return false;
    }

    if (!add(_queue, 5, 3, 3, _log)) {
      return false;
    }

    return true;
  }
};

struct circular_queue_006 : public circular_queue_test_base {
  static std::string desc() {
    return "Adds -9, 38, 4, gets -9 and 38, and adds 5";
  }

  bool operator()(const src::generic::program_options &) {
    log::cerr _log;
    queue _queue("q006", _log, 3);

    if (!check(_queue, 0, 3, _log)) {
      return false;
    }
    traverse(_queue, _log);

    if (!add(_queue, -9, 1, 3, _log)) {
      return false;
    }

    if (!add(_queue, 38, 2, 3, _log)) {
      return false;
    }

    if (!add(_queue, 4, 3, 3, _log)) {
      return false;
    }

    if (!get(_queue, -9, 2, 3, _log)) {
      return false;
    }

    if (!get(_queue, 38, 1, 3, _log)) {
      return false;
    }

    if (!add(_queue, 5, 2, 3, _log)) {
      return false;
    }

    return true;
  }
};

struct circular_queue_007 : public circular_queue_test_base {
  static std::string desc() { return "Adds -9, 38, 4, gets all three"; }

  bool operator()(const src::generic::program_options &) {
    log::cerr _log;
    queue _queue("q007", _log, 3);

    if (!check(_queue, 0, 3, _log)) {
      return false;
    }
    traverse(_queue, _log);

    if (!add(_queue, -9, 1, 3, _log)) {
      return false;
    }

    if (!add(_queue, 38, 2, 3, _log)) {
      return false;
    }

    if (!add(_queue, 4, 3, 3, _log)) {
      return false;
    }

    if (!get(_queue, -9, 2, 3, _log)) {
      return false;
    }

    if (!get(_queue, 38, 1, 3, _log)) {
      return false;
    }

    if (!get(_queue, 4, 0, 3, _log)) {
      return false;
    }

    return true;
  }
};

struct circular_queue_008 : public circular_queue_test_base {
  static std::string desc() {
    return "Adds -9, 38, 4, gets all three and adds 5";
  }

  bool operator()(const src::generic::program_options &) {
    log::cerr _log;
    queue _queue("q008", _log, 3);

    if (!check(_queue, 0, 3, _log)) {
      return false;
    }
    traverse(_queue, _log);

    if (!add(_queue, -9, 1, 3, _log)) {
      return false;
    }

    if (!add(_queue, 38, 2, 3, _log)) {
      return false;
    }

    if (!add(_queue, 4, 3, 3, _log)) {
      return false;
    }

    if (!get(_queue, -9, 2, 3, _log)) {
      return false;
    }

    if (!get(_queue, 38, 1, 3, _log)) {
      return false;
    }

    if (!get(_queue, 4, 0, 3, _log)) {
      return false;
    }

    if (!add(_queue, 5, 1, 3, _log)) {
      return false;
    }

    return true;
  }
};

struct circular_queue_009 : public circular_queue_test_base {
  static std::string desc() {
    return "Adds " + std::to_string(m_amount) + " strings of 4k";
  }
  bool operator()(const src::generic::program_options &) {
    // using namespace tenacitas::test::src::container::circular_queue;
    log::cerr _log;

    try {

      src::container::circular_queue_on_list<log::cerr, std::string> queue("q009", _log,
                                                              100000);
      std::string data(4 * 1024, 'z');

      for (uint32_t i = 0; i < m_amount; ++i) {
        queue.push(data);
      }

      uint32_t j{0};
      while (!queue.empty()) {
        std::optional<std::string> maybe{queue.pop()};
        if (!maybe) {
          _log.err(src::generic::fmt("error getting data # ", j));
          return false;
        }
        _log.tst(src::generic::fmt("capacity = ", queue.capacity(), ", occupied = ",
                              queue.occupied(), ", data # ", j++));
      }
    } catch (std::exception &ex) {
      _log.err(ex.what());
      return false;
    } catch (...) {
      _log.err("unknown error");
      return false;
    }

    return true;
  }

private:
  static constexpr uint32_t m_amount{1000000};
};

struct circular_queue_010 : public circular_queue_test_base {
  static std::string desc() { return "Adds -9, 38, 4, 11, -301"; }

  bool operator()(const src::generic::program_options &) {
    log::cerr _log;
    queue _queue("q010", _log, 3);

    if (!check(_queue, 0, 3, _log)) {
      return false;
    }
    traverse(_queue, _log);

    if (!add(_queue, -9, 1, 3, _log)) {
      return false;
    }

    if (!add(_queue, 38, 2, 3, _log)) {
      return false;
    }

    if (!add(_queue, 4, 3, 3, _log)) {
      return false;
    }

    if (!add(_queue, 11, 4, 4, _log)) {
      return false;
    }

    if (!add(_queue, -301, 5, 5, _log)) {
      return false;
    }

    return true;
  }
};

struct circular_queue_011 : public circular_queue_test_base {
  static std::string desc() {
    return "Adds -9, 38, 4, 11, -301, 82, gets -9 and 38, adds 26, 91 and -22";
  }

  bool operator()(const src::generic::program_options &) {
    log::cerr _log;
    _log.set_tra();
    queue _queue("q011", _log, 3);

    if (!check(_queue, 0, 3, _log)) {
      return false;
    }
    traverse(_queue, _log);

    if (!add(_queue, -9, 1, 3, _log)) {
      return false;
    }

    if (!add(_queue, 38, 2, 3, _log)) {
      return false;
    }

    if (!add(_queue, 4, 3, 3, _log)) {
      return false;
    }

    if (!add(_queue, 11, 4, 4, _log)) {
      return false;
    }

    if (!add(_queue, -301, 5, 5, _log)) {
      return false;
    }

    if (!add(_queue, 82, 6, 6, _log)) {
      return false;
    }

    if (!get(_queue, -9, 5, 6, _log)) {
      return false;
    }

    if (!get(_queue, 38, 4, 6, _log)) {
      return false;
    }

    if (!add(_queue, 26, 5, 6, _log)) {
      return false;
    }

    if (!add(_queue, 91, 6, 6, _log)) {
      return false;
    }

    if (!add(_queue, -22, 7, 7, _log)) {
      return false;
    }

    return true;
  }
};

struct circular_queue_012 : public circular_queue_test_base {
  static std::string desc() {
    return "Tests if 'src::container::circular_queue' conforms to 'traits::queue";
  }

  bool operator()(const src::generic::program_options &) {
    log::cerr _log;

    queue _queue("q000", _log, 3);

    conforms(_log, _queue);

    return true;
  }

private:
  template <traits::queue<log::cerr, int16_t> t_queue>
  void conforms(log::cerr &p_logger, t_queue &p_queue) {
    p_logger.tst(p_queue.id());
  }
};

#endif
