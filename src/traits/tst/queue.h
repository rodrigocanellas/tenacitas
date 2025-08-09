/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstdint>
#include <optional>
#include <source_location>
#include <string>
#include <string_view>

#include "tenacitas.lib/container/circular_queue.h"
#include "tenacitas.lib/generic/tester.h"
#include "tenacitas.lib/log/cout.h"
#include "tenacitas.lib/traits/queue.h"

using namespace tenacitas::lib;

using queue = src::container::circular_queue<uint16_t, supplier>;

template <traits::queue t_queue> bool f1(t_queue &p_queue) {
  uint16_t _i0(9);
  uint16_t _i1(21);
  p_queue.add(_i0);
  p_queue.add(std::move(_i1));
  p_queue.add(uint16_t(3));

  for (uint8_t _i = 0; _i < 5; ++_i) {
    std::optional<uint16_t> _maybe_v(p_queue.get());
    if (_maybe_v) {
      TNCT_LOG_TST("value # ", _i, " = ", _maybe_v.value());
    } else {
      return false;
    }
  }
  return true;
}

template <traits::queue t_queue> bool f2(t_queue &p_queue) {
  {
    p_queue.add(std::move('H'));

    std::optional<char> _maybe_v(p_queue.get());
    if (_maybe_v) {
      char _value = _maybe_v.value();
      TNCT_LOG_TST("value = ", _value);
      if (_value != 'H') {
        return false;
      }

    } else {
      return false;
    }
  }

  {
    char _c('n');
    p_queue.add(_c);
    std::optional<char> _maybe_v(p_queue.get());
    if (_maybe_v) {
      char _value = _maybe_v.value();
      TNCT_LOG_TST("value = ", _value);
      if (_value != 'n') {
        return false;
      }
    } else {
      return false;
    }
  }

  return true;
}

struct test_000 {
  static std::string desc() { return ""; }

  bool operator()(const src::generic::program_options &) {
    queue _queue;

    return f1(_queue);
  }
};

// struct test_001
// {
//     static std::string desc() { return ""; }

//     bool operator()(const src::generic::program_options &)
//     {
//         invalid_queue<char> _queue;

//         return f2(_queue);
//     }

// private:
//     // ATTENTION!!

//     // comment one of the methods at a time to see the compiler error due to
//     the concept 'traits::queue' is not satisfied
//   //   template<typename t_data>
//   //   struct invalid_queue
//   //   {
//   //       invalid_queue() = default;

//   //       void add(t_data &&) {}

//   //       void add(const t_data &) {}

//   //       std::optional<t_data> get() { return {}; }
//   // };
// }
// ;

struct test_002 {
  static std::string desc() { return ""; }

  bool operator()(const src::generic::program_options &) {
    valid_queue<char> _queue;

    return f2(_queue);
  }

private:
  template <typename t_data> struct valid_queue {
    using data = t_data;
    void add(t_data &&p_data) { m_data = std::move(p_data); }

    std::optional<t_data> get() { return {m_data}; }

    void add(const t_data &p_data) { m_data = p_data; }

  private:
    t_data m_data;
  };
};

int main(int argc, char **argv) {

  log::set_trace_level();

  src::generic::tester _tester(argc, argv);
  run_test(_tester, test_000);
  run_test(_tester, test_002);
}
