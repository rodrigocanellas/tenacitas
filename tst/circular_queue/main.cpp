/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

#include <tnct/lib/alg/log.h>
#include <tnct/lib/alg/tester.h>
#include <tnct/lib/dat/circular_queue.h>

using namespace tnct::lib;

using queue = tld::circular_queue<int16_t>;

bool check(const queue &p_queue, size_t p_occupied, size_t p_capacity) {
  if (p_queue.capacity() != p_capacity) {
    TNCT_LOG_ERR("capacity should be ", p_capacity, " but it is ",
                 p_queue.capacity());
    return false;
  }

  if (p_queue.occupied() != p_occupied) {
    TNCT_LOG_ERR("there should be ", p_occupied,
                 " slots occupied, but there are ", p_queue.occupied());
    return false;
  }
  return true;
}

void traverse(const queue &p_queue, size_t p_occupied, size_t p_capacity) {
  TNCT_LOG_TST("traversing queue with ", p_capacity, " slots, and ", p_occupied,
               " occupied");
  p_queue.traverse(
      [](const queue::data &p_data) -> void { TNCT_LOG_TST("* ", p_data); });
}

bool get(queue &p_queue, queue::data p_value, size_t p_occupied,
         size_t p_capacity) {
  queue::data _data;
  {
    std::optional<queue::data> _maybe{p_queue.get()};
    if (!_maybe.has_value()) {
      TNCT_LOG_ERR("no data read, when it should have been ", p_value);
      return false;
    }
    _data = std::move(*_maybe);
  }
  if (_data != p_value) {
    TNCT_LOG_ERR("data should be ", p_value, ", but it is ", _data);
    return false;
  }

  TNCT_LOG_TST("getting ", _data);

  if (!check(p_queue, p_occupied, p_capacity)) {
    return false;
  }

  traverse(p_queue, p_occupied, p_capacity);
  return true;
}

bool add(queue &p_queue, queue::data p_value, size_t p_occupied,
         size_t p_capacity) {

  TNCT_LOG_TST("adding ", p_value);

  p_queue.add(p_value);

  if (!check(p_queue, p_occupied, p_capacity)) {
    return false;
  }
  bool _full{p_occupied == p_capacity};
  if (p_queue.full() != _full) {
    TNCT_LOG_ERR("It should be", (_full ? "" : " not"), " full, but it is ",
                 (!_full ? "" : "not "));
    return false;
  }

  traverse(p_queue, p_occupied, p_capacity);

  return true;
}

struct test_000 {
  static std::string desc() {
    return "Tests if the queue is correctly created";
  }

  bool operator()(const tla::program_options &) {
    queue _queue;

    if (!check(_queue, 0, 0)) {
      return false;
    }
    traverse(_queue, 0, 0);

    return true;
  }
};

struct test_001 {
  static std::string desc() { return "Adds -9 and gets it"; }

  bool operator()(const tla::program_options &) {
    queue _queue;

    if (!check(_queue, 0, 0)) {
      return false;
    }
    traverse(_queue, 0, 0);

    if (!add(_queue, -9, 1, 1)) {
      return false;
    }

    if (!get(_queue, -9, 0, 1)) {
      return false;
    }

    return true;
  }
};

struct test_002 {
  static std::string desc() { return "Adds -9 and tries to get 38"; }

  bool operator()(const tla::program_options &) {
    queue _queue;

    if (!check(_queue, 0, 0)) {
      return false;
    }
    traverse(_queue, 0, 0);

    if (!add(_queue, -9, 1, 1)) {
      return false;
    }

    if (get(_queue, 38, 0, 1)) {
      return false;
    }

    return true;
  }
};

struct test_003 {
  static std::string desc() { return "Adds -9 and 38 and gets one"; }

  bool operator()(const tla::program_options &) {
    queue _queue;

    if (!check(_queue, 0, 0)) {
      return false;
    }
    traverse(_queue, 0, 0);

    if (!add(_queue, -9, 1, 1)) {
      return false;
    }

    if (!add(_queue, 38, 2, 2)) {
      return false;
    }

    if (!get(_queue, -9, 1, 2)) {
      return false;
    }

    return true;
  }
};

struct test_004 {
  static std::string desc() { return "Adds -9 and 38 and gets two"; }

  bool operator()(const tla::program_options &) {
    queue _queue;

    if (!check(_queue, 0, 0)) {
      return false;
    }
    traverse(_queue, 0, 0);

    if (!add(_queue, -9, 1, 1)) {
      return false;
    }

    if (!add(_queue, 38, 2, 2)) {
      return false;
    }

    if (!get(_queue, -9, 1, 2)) {
      return false;
    }

    if (!get(_queue, 38, 0, 2)) {
      return false;
    }

    return true;
  }
};

struct test_005 {
  static std::string desc() { return "Adds -9, 38, 4, gets one and adds 5"; }

  bool operator()(const tla::program_options &) {
    queue _queue;

    if (!check(_queue, 0, 0)) {
      return false;
    }
    traverse(_queue, 0, 0);

    if (!add(_queue, -9, 1, 1)) {
      return false;
    }

    if (!add(_queue, 38, 2, 2)) {
      return false;
    }

    if (!add(_queue, 4, 3, 3)) {
      return false;
    }

    if (!get(_queue, -9, 2, 3)) {
      return false;
    }

    if (!add(_queue, 5, 3, 3)) {
      return false;
    }

    return true;
  }
};

struct test_006 {
  static std::string desc() { return "Adds -9, 38, 4, gets two and adds 5"; }

  bool operator()(const tla::program_options &) {
    queue _queue;

    if (!check(_queue, 0, 0)) {
      return false;
    }
    traverse(_queue, 0, 0);

    if (!add(_queue, -9, 1, 1)) {
      return false;
    }

    if (!add(_queue, 38, 2, 2)) {
      return false;
    }

    if (!add(_queue, 4, 3, 3)) {
      return false;
    }

    if (!get(_queue, -9, 2, 3)) {
      return false;
    }

    if (!get(_queue, 38, 1, 3)) {
      return false;
    }

    if (!add(_queue, 5, 2, 3)) {
      return false;
    }

    return true;
  }
};

struct test_007 {
  static std::string desc() { return "Adds -9, 38, 4, gets three"; }

  bool operator()(const tla::program_options &) {
    queue _queue;

    if (!check(_queue, 0, 0)) {
      return false;
    }
    traverse(_queue, 0, 0);

    if (!add(_queue, -9, 1, 1)) {
      return false;
    }

    if (!add(_queue, 38, 2, 2)) {
      return false;
    }

    if (!add(_queue, 4, 3, 3)) {
      return false;
    }

    if (!get(_queue, -9, 2, 3)) {
      return false;
    }

    if (!get(_queue, 38, 1, 3)) {
      return false;
    }

    if (!get(_queue, 4, 0, 3)) {
      return false;
    }

    return true;
  }
};

struct test_008 {
  static std::string desc() { return "Adds -9, 38, 4, gets three, adds 5"; }

  bool operator()(const tla::program_options &) {
    queue _queue;

    if (!check(_queue, 0, 0)) {
      return false;
    }
    traverse(_queue, 0, 0);

    if (!add(_queue, -9, 1, 1)) {
      return false;
    }

    if (!add(_queue, 38, 2, 2)) {
      return false;
    }

    if (!add(_queue, 4, 3, 3)) {
      return false;
    }

    if (!get(_queue, -9, 2, 3)) {
      return false;
    }

    if (!get(_queue, 38, 1, 3)) {
      return false;
    }

    if (!get(_queue, 4, 0, 3)) {
      return false;
    }

    if (!add(_queue, 5, 1, 3)) {
      return false;
    }

    return true;
  }
};

struct test_009 {
  static std::string desc() {
    return "Adds 1,000,000 strings of 4k to a circular-queue";
  }
  bool operator()(const tla::program_options &) {
    try {
      tld::circular_queue<std::string> queue;
      std::string data(4 * 1024, 'z');

      for (uint32_t i = 0; i < 1000000; ++i) {
        queue.add(data);
      }

      uint32_t j{0};
      while (!queue.empty()) {
        std::optional<std::string> maybe{queue.get()};
        if (!maybe) {
          TNCT_LOG_ERR("error getting data # ", j);
          return false;
        }
        TNCT_LOG_TST("data # ", j++, ": ", maybe.value());
      }
    } catch (std::exception &ex) {
      TNCT_LOG_ERR(ex.what());
      return false;
    } catch (...) {
      TNCT_LOG_ERR("unknown error");
      return false;
    }

    return true;
  }
};

int main(int argc, char **argv) {

  tla::set_debug_level();
  tla::set_max_log_file_name_lenght(8);

  tla::tester _tester(argc, argv);
  run_test(_tester, test_000);
  run_test(_tester, test_001);
  run_test(_tester, test_002);
  run_test(_tester, test_003);
  run_test(_tester, test_004);
  run_test(_tester, test_005);
  run_test(_tester, test_006);
  run_test(_tester, test_007);
  run_test(_tester, test_008);
  run_test(_tester, test_009);
}
