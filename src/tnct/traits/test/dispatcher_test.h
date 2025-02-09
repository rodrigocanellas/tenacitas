/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_TRAITS_TEST_DISPATCHER_TEST_H
#define TNCT_TRAITS_TEST_DISPATCHER_TEST_H

#include <tuple>

#include "tnct/log/cerr.h"
#include "tnct/traits/dispatcher.h"
#include "tnct/traits/event.h"

using namespace tnct;

namespace tnct::traits::test {

struct dispatcher_000 {

  bool operator()() {
    log::cerr _logger;

    my_dispatcher _dispatcher;

    f(_dispatcher);

    return false;
  }

private:
  struct event_a {
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &) {
      return p_out;
    }
  };

  struct event_b {
    friend std::ostream &operator<<(std::ostream &p_out, const event_b &) {
      return p_out;
    }
  };

  struct event_c {
    friend std::ostream &operator<<(std::ostream &p_out, const event_c &) {
      return p_out;
    }
  };

  template <traits::event... t_events> struct dispatcher {
    using events = std::tuple<t_events...>;

    dispatcher() = default;
    dispatcher(const dispatcher &) = delete;
    dispatcher(dispatcher &&) = delete;

    dispatcher &operator=(const dispatcher &) = delete;
    dispatcher &operator=(dispatcher &&) = delete;

    void *operator new(size_t) = delete;
  };

  using my_dispatcher = dispatcher<event_a, event_b>;

  template <traits::dispatcher<event_a, event_b> t_dispatcher>
  void f(t_dispatcher &) {}
};

} // namespace tnct::traits::test
#endif
