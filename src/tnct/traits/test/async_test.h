/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_TRAITS_TEST_ASYNC_TEST_H
#define TNCT_TRAITS_TEST_ASYNC_TEST_H

#include <tuple>

#include "tnct/log/cerr.h"
#include "tnct/program/options.h"
#include "tnct/traits/async/dispatcher.h"
#include "tnct/traits/async/event.h"
#include "tnct/traits/async/publish_method.h"

using namespace tnct;

namespace tnct::traits::test {

struct dispatcher_000 {

  static std::string desc() { return "Basic traitrs::dispatcher test"; }

  bool operator()(const program::options &) {
    tnct::log::cerr _logger;

    my_dispatcher _dispatcher;

    f(_dispatcher);

    return true;
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

  template <traits::async::event... t_events> struct dispatcher {
    using events = std::tuple<t_events...>;

    dispatcher() = default;
    dispatcher(const dispatcher &) = delete;
    dispatcher(dispatcher &&) = delete;

    dispatcher &operator=(const dispatcher &) = delete;
    dispatcher &operator=(dispatcher &&) = delete;

    void *operator new(size_t) = delete;
  };

  using my_dispatcher = dispatcher<event_a, event_b>;

  template <traits::async::dispatcher<event_a, event_b> t_dispatcher>
  void f(t_dispatcher &) {}
};

struct has_publish_method_000 {
  static std::string desc() { return "A valid has_publish_method"; }

  bool operator()(const program::options &) {
    using my_publisher = publisher<event_a>;
    my_publisher _publisher;
    event_a _event_a;

    result _result{f(_publisher, _event_a)};

    std::cerr << "result = "
              << static_cast<std::underlying_type_t<result>>(_result) << '\n';

    return true;
  }

private:
  enum class result : int { r0 = 1, r1 = -1 };
  struct event_a {
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &) {
      return p_out;
    }
  };

  template <traits::async::event... t_events> struct publisher {
    template <traits::async::event t_event> result publish(const t_event &) {
      return result::r0;
    }
  };

  template <async::publish_method_like<result, event_a> t_has_publish_method>
  result f(t_has_publish_method &p_publisher, const event_a &p_event_a) {
    return p_publisher.publish(p_event_a);
  };
};

struct has_publish_method_001 {
  static std::string desc() { return "A invalid has_publish_method"; }

  bool operator()(const program::options &) {

    // The code below does not compile because 'publisher' has no 'publish'
    // method

    // using my_publisher = publisher<event_a>;
    // my_publisher _publisher;
    // event_a _event_a;

    // result _result{f(_publisher, _event_a)};

    // std::cerr << "result = "
    //           << static_cast<std::underlying_type_t<result>>(_result) <<
    //           '\n';

    return true;
  }

private:
  enum class result : int { r0 = 1, r1 = -1 };
  struct event_a {
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &) {
      return p_out;
    }
  };
  template <traits::async::event... t_events> struct publisher {

    // template <traits::async::event t_event> result publish(const t_event &) {
    //   return result::r0;
    // }
  };

  template <async::publish_method_like<result, event_a> t_has_publish_method>
  result f(t_has_publish_method &p_publisher, const event_a &p_event_a) {
    return p_publisher.publish(p_event_a);
  };
};

struct has_publish_method_002 {
  static std::string desc() { return "Checks publihing with multiples events"; }

  bool operator()(const program::options &) {
    static_assert(
        traits::async::publish_method_like<publisher, result, event_a>,
        "publisher should match publish_method_like for event_a!");

    static_assert(
        traits::async::publish_method_like<publisher, result, event_b>,
        "publisher should match publish_method_like for event_b!");

    static_assert(
        traits::async::publish_method_like<publisher, result, event_c>,
        "publisher should match publish_method_like for event_c!");

    static_assert(traits::async::event<event_a>, "event_a is not an event");

    static_assert(traits::async::event<event_b>, "event_b is not an event");

    static_assert(traits::async::event<event_c>, "event_c is not an event");

    publisher _publisher;

    event_b _event_b;
    auto _result{publish(_publisher, _event_b)};
    if (_result != result::r1) {
      return false;
    }

    event_a _event_a;
    _result = publish(_publisher, _event_a);
    if (_result != result::r0) {
      return false;
    }

    event_c _event_c;

    _result = publish(_publisher, std::move(_event_c));
    if (_result != result::r2) {
      return false;
    }

    _result = publish(_publisher, event_b{});
    if (_result != result::r1) {
      return false;
    }

    return true;
  }

private:
  enum class result : int { r0 = 1, r1 = 2, r2 = 3, r3 = 4 };
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

  // struct publisher {
  //   template <traits::async::event t_event> result publish(const t_event &) {
  //     return result::r0;
  //   } // ✅ Only const version

  //   // Uncomment this to fix:
  //   template <traits::async::event t_event> result publish(t_event &&) {
  //     return result::r1;
  //   }
  // };

  // Should FAIL because publisher lacks `publish(T&&)`
  // static_assert(async::has_rvalue_publish<publisher, result, event_a>);

  // static_assert(async::has_lvalue_publish<publisher, result, event_a>);

  struct publisher {
    template <traits::async::event t_event> result publish(const t_event &) {
      if constexpr (std::is_same_v<t_event, event_a>) {
        return result::r0;
      } else if constexpr (std::is_same_v<t_event, event_b>) {
        return result::r1;
      } else if constexpr (std::is_same_v<t_event, event_c>) {
        return result::r2;
      } else
        return result::r3;
    }

    template <traits::async::event t_event> result publish(t_event &&) {
      if constexpr (std::is_same_v<t_event, event_a>) {
        return result::r0;
      } else if constexpr (std::is_same_v<t_event, event_b>) {
        return result::r1;
      } else if constexpr (std::is_same_v<t_event, event_c>) {
        return result::r2;
      } else
        return result::r3;
    }
  };

  template <async::publishers_methods_like<result, event_a, event_b, event_c>
                t_publisher>
  result publish(t_publisher &p_publisher, const event_a &p_event_a) {
    return p_publisher.template publish<event_a>(p_event_a);
  }

  template <async::publishers_methods_like<result, event_a, event_b, event_c>
                t_publisher>
  result publish(t_publisher &p_publisher, const event_b &p_event_b) {
    return p_publisher.template publish<event_b>(p_event_b);
  }

  template <async::publishers_methods_like<result, event_a, event_b, event_c>
                t_publisher>
  result publish(t_publisher &p_publisher, const event_c &p_event_c) {
    return p_publisher.template publish<event_c>(p_event_c);
  }
};

} // namespace tnct::traits::test
#endif
