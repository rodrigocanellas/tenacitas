/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_TRAITS_TEST_ASYNC_TEST_H
#define TNCT_TRAITS_TEST_ASYNC_TEST_H

#include <tuple>

#include "tnct/container/circular_queue.h"
#include "tnct/log/cerr.h"
#include "tnct/log/no_logger.h"
#include "tnct/program/options.h"
#include "tnct/string/fixed_size_string.h"
#include "tnct/traits/async/add_handling_method.h"
#include "tnct/traits/async/dispatcher.h"
#include "tnct/traits/async/event.h"
#include "tnct/traits/async/publish_method.h"
#include "tnct/traits/async/result.h"

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

  template <async::has_const_lvalue_publish_method<result, event_a>
                t_has_publish_method>
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

  template <async::has_const_lvalue_publish_method<result, event_a>
                t_has_publish_method>
  result f(t_has_publish_method &p_publisher, const event_a &p_event_a) {
    return p_publisher.publish(p_event_a);
  };
};

struct has_publish_method_002 {
  static std::string desc() { return "Checks publihing with multiples events"; }

  bool operator()(const program::options &) {
    static_assert(
        traits::async::has_const_lvalue_publish_method<publisher, result,
                                                       event_a>,
        "publisher should match has_const_lvalue_publish for event_a!");

    publisher _publisher;

    event_b _event_b;
    auto _result{f(_publisher, _event_b)};
    if (_result != result::b) {
      return false;
    }

    event_a _event_a;
    _result = f(_publisher, _event_a);
    if (_result != result::a) {
      return false;
    }

    event_c _event_c;
    _result = f(_publisher, _event_c);
    if (_result != result::c) {
      return false;
    }

    int16_t _i{-9};
    const std::string &_s{"hi"};
    _result = f<publisher>(_publisher, _i, _s);
    if (_result != result::d) {
      return false;
    }

    return true;
  }

private:
  enum class result : int { unknown = -1, a = 1, b = 2, c = 3, d = 4 };
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

  struct event_d {
    event_d() = default;
    event_d(int16_t p_i, const std::string &p_s) : m_i(p_i), m_s(p_s) {}

    friend std::ostream &operator<<(std::ostream &p_out, const event_d &) {
      return p_out;
    }

    int16_t m_i;
    const std::string m_s;
  };

  template <async::has_const_lvalue_publish_method<result, event_a>
                t_has_publish_method>
  result f(t_has_publish_method &p_publisher, const event_a &p_event_a) {
    return p_publisher.publish(p_event_a);
  };

  template <async::has_const_lvalue_publish_method<result, event_a>
                t_has_publish_method>
  result f(t_has_publish_method &p_publisher, const event_b &p_event_b) {
    return p_publisher.publish(p_event_b);
  };

  template <async::has_const_lvalue_publish_method<result, event_a>
                t_has_publish_method>
  result f(t_has_publish_method &p_publisher, const event_c &p_event_c) {
    return p_publisher.publish(p_event_c);
  };

  template <async::has_variadic_params_publish_method<result, event_d>
                t_has_publish_method>
  result f(t_has_publish_method &p_publisher, int16_t p_i,
           const std::string &p_s) {
    return p_publisher.template publish<event_d>(p_i, p_s);
  };

  struct publisher {
    template <traits::async::event t_event> result publish(const t_event &) {
      if constexpr (std::is_same_v<t_event, event_a>) {
        return result::a;
      } else if constexpr (std::is_same_v<t_event, event_b>) {
        return result::b;
      } else if constexpr (std::is_same_v<t_event, event_c>) {
        return result::c;
      } else
        return result::d;
    }

    template <async::event t_event, typename... t_params>
    result publish(t_params &&...) {
      if constexpr (std::is_same_v<t_event, event_d>) {
        return result::d;
      }
      return result::unknown;
    }
  };
};

enum class result_master : int { unknown = -1, a = 1, b = 2, c = 3, d = 4 };
std::ostream &operator<<(std::ostream &p_out, const result_master &p_result) {
  p_out << static_cast<std::underlying_type_t<result_master>>(p_result);
  return p_out;
}

enum class priority_master : uint16_t { l = 0, m = 1, h = 2 };
std::ostream &operator<<(std::ostream &p_out,
                         const priority_master &p_priority_master) {
  p_out << static_cast<std::underlying_type_t<priority_master>>(
      p_priority_master);
  return p_out;
}

struct has_add_handling_method_000 {
  static std::string desc() {
    return "Check if tnct::traits::async::has_add_handling_method captures in "
           "compile time if a class has the 'add_handling' method correctly "
           "declared";
  }

  bool operator()(const program::options &) {
    dummy _dummy;
    foo(_dummy);

    return true;
  }

private:
  using logger = tnct::log::no_logger;

  struct handling_a {
    struct event {
      friend std::ostream &operator<<(std::ostream &p_out, const event &) {
        return p_out;
      }
    };

    using priority = priority_master;

    struct handler {
      void operator()(event &&) {}
    };

    using queue = tnct::container::circular_queue<logger, event, 10>;

    using id = tnct::string::fixed_size_string<5>;

    const id &get_id() const { return m_id; }
    handler get_handler() const { return handler{}; }
    queue get_queue() { return queue{m_logger}; }
    std::size_t get_num_handlers() const { return 3; }
    priority get_priority() const { return priority::h; }

  private:
    logger m_logger;
    id m_id{"01234"};
  };

  template <traits::async::has_add_handling_method<result_master, handling_a>
                t_add_handling_method>
  void foo(t_add_handling_method &p_add_handling_method) {
    p_add_handling_method.template add_handling<handling_a>(
        std::move(handling_a{}));
  }

  struct dummy {
    template <traits::async::handling t_handling>
    result_master add_handling(t_handling &&) {
      return result_master::a;
    }
  };
};

struct has_add_handling_method_001 {
  static std::string desc() {
    return "Check if tnct::traits::async::has_add_handling_method captures in "
           "compile time an error if a class does not have the 'add_handling' "
           "method correctly declared because the the handling_id does not "
           "conform to tnct::string::fixed_size_string";
  }

  bool operator()(const program::options &) {
    static_assert(!tnct::traits::string::fixed_size_string<handling_a::id>);
    static_assert(!tnct::traits::async::handling<handling_a>);
    static_assert(
        !tnct::traits::async::has_add_handling_method<dummy, result_master,
                                                      handling_a>);

    // the following lines cause a compiler error, as expected, because
    // handling_a::id does not conform to
    // 'tnct::traits::string::fixed_size_string'
    // dummy _dummy;
    // foo(_dummy);

    return true;
  }

private:
  using logger = tnct::log::no_logger;

  struct handling_a {
    struct event {
      friend std::ostream &operator<<(std::ostream &p_out, const event &) {
        return p_out;
      }
    };

    struct handler {
      void operator()(event &&) {}
    };

    using queue = tnct::container::circular_queue<logger, event, 10>;
    using priority = priority_master;
    using id = int *;

    const id &get_id() const { return m_id; }
    handler get_handler() const { return handler{}; }
    queue get_queue() { return queue{m_logger}; }
    std::size_t get_num_handlers() const { return 3; }
    priority get_priority() const { return priority::h; }

  private:
    logger m_logger;
    id m_id{nullptr};
  };

  // template <traits::async::has_add_handling_method<result_master, handling_a>
  //               t_add_handling_method>
  // void foo(t_add_handling_method p_add_handling_method) {
  //   static_assert(traits::async::handling<handling_a>,
  //                 "handling_a does not satisfy traits::async::handling");
  //   p_add_handling_method.template add_handling<handling_a>(
  //       std::move(handling_a{}));
  // }

  struct dummy {
    template <typename t_event> result_master add_handling(handling_a &&) {
      return result_master::a;
    }
  };
};

struct has_add_handling_method_002 {
  static std::string desc() {
    return "Check if tnct::traits::async::has_add_handling_method captures in "
           "compile time an error if a class does not have the 'add_handling' "
           "method handler conform to tnct::async::handler";
  }

  bool operator()(const program::options &) {

    static_assert(
        !tnct::traits::async::handler<handling_a::handler, handling_a::event>);
    static_assert(!tnct::traits::async::handling<handling_a>);

    // the following lines cause a compiler error, as expected, because
    // 'handling_a::handler' declared below is not compatible with
    // 'tnct::traits::async::handler'
    // dummy _dummy;
    // foo(_dummy);

    return true;
  }

private:
  using logger = tnct::log::no_logger;

  struct handling_a {
    struct event {
      friend std::ostream &operator<<(std::ostream &p_out, const event &) {
        return p_out;
      }
    };

    struct handler {
      void operator()(event) {}
    };

    using queue = tnct::container::circular_queue<logger, event, 10>;
    using priority = priority_master;
    using id = tnct::string::fixed_size_string<5>;

    const id &get_id() const { return m_id; }
    handler get_handler() const { return handler{}; }
    queue get_queue() { return queue{m_logger}; }
    std::size_t get_num_handlers() const { return 3; }
    priority get_priority() const { return priority::h; }

  private:
    logger m_logger;
    id m_id{"01234"};
  };

  template <traits::async::has_add_handling_method<result_master, handling_a>
                t_add_handling_method>
  void foo(t_add_handling_method p_add_handling_method) {
    p_add_handling_method.template add_handling<handling_a>(
        std::move(handling_a{}));
  }

  struct dummy {
    template <typename t_event> result_master add_handling(handling_a &&) {
      return result_master::a;
    }
  };
};

struct has_add_handling_method_003 {
  static std::string desc() {
    return "Check if tnct::traits::async::has_add_handling_method captures in "
           "compile time an error if a class does not have the 'add_handling' "
           "method correctly declared because the event informed does not "
           "conform to 'tnct::traits::async::event";
  }

  bool operator()(const program::options &) {

    static_assert(!tnct::traits::async::event<handling_a::event>);
    static_assert(!tnct::traits::async::handling<handling_a>);

    // the following lines cause a compiler error, as expected, because
    // 'handling_a::event' declared below is not compatible with
    // 'tnct::traits::async::event'
    // dummy _dummy;
    // foo(_dummy);

    return true;
  }

private:
  using logger = tnct::log::no_logger;

  struct handling_a {
    struct event {};

    struct handler {
      void operator()(event &&) {}
    };

    // the following lines cause a compiler error, as expected, because
    // 'handling_a::event' declared below is not compatible with
    // 'tnct::traits::async::event'

    // using queue = tnct::container::circular_queue<logger, event, 10>;
    using priority = priority_master;
    using id = tnct::string::fixed_size_string<5>;

    const id &get_id() const { return m_id; }
    handler get_handler() const { return handler{}; }
    //     queue get_queue() { return queue{m_logger}; }
    std::size_t get_num_handlers() const { return 3; }
    priority get_priority() const { return priority::h; }

  private:
    // logger m_logger;
    id m_id{"01234"};
  };

  template <traits::async::has_add_handling_method<result_master, handling_a>
                t_add_handling_method>
  void foo(t_add_handling_method p_add_handling_method) {
    p_add_handling_method.template add_handling<handling_a>(
        std::move(handling_a{}));
  }

  struct dummy {
    template <typename t_event> result_master add_handling(handling_a &&) {
      return result_master::a;
    }
  };
};

struct has_add_handling_method_004 {
  static std::string desc() {
    return "Check if tnct::traits::async::has_add_handling_method captures in "
           "compile time an error if a class does not have the 'add_handling' "
           "method correctly declared because the queue informed does not "
           "conform to 'tnct::traits::container::queue";
  }

  bool operator()(const program::options &) {
    static_assert(
        !tnct::traits::container::queue<handling_a::queue, handling_a::event>);
    static_assert(!tnct::traits::async::handling<handling_a>);
    // dummy _dummy;
    // foo(_dummy);

    return true;
  }

private:
  using logger = tnct::log::no_logger;

  struct handling_a {
    struct event {
      friend std::ostream &operator<<(std::ostream &p_out, const event &) {
        return p_out;
      }
    };

    struct handler {
      void operator()(event &&) {}
    };

    using queue = int;

    enum class priority : char { l = 'l', m = 'm', h = 'h' };
    using id = tnct::string::fixed_size_string<5>;

    const id &get_id() const { return m_id; }
    handler get_handler() const { return handler{}; }
    queue get_queue() { return queue{}; }
    std::size_t get_num_handlers() const { return 3; }
    priority get_priority() const { return priority::h; }

  private:
    // logger m_logger;
    id m_id{"01234"};
  };

  template <traits::async::has_add_handling_method<result_master, handling_a>
                t_add_handling_method>
  void foo(t_add_handling_method p_add_handling_method) {
    p_add_handling_method.template add_handling<handling_a>(
        std::move(handling_a{}));
  }

  struct dummy {
    template <typename t_event> result_master add_handling(handling_a &&) {
      return result_master::a;
    }
  };
};

struct has_add_handling_method_006 {
  static std::string desc() {
    return "Check if tnct::traits::async::has_add_handling_method captures in "
           "compile time an error if a class does not have the 'add_handling' "
           "method correctly declared because the result informed "
           "does not conform to 'tnct::traits::async::result";
  }

  bool operator()(const program::options &) {

    static_assert(!tnct::traits::async::result<result>);
    static_assert(!tnct::traits::async::has_add_handling_method<dummy, result,
                                                                handling_a>);

    // the following lines cause a compiler error, as expected, because
    // 'result' declared below is not compatible with
    // 'tnct::traits::async::result'

    // dummy _dummy;
    // foo(_dummy);

    return true;
  }

private:
  using logger = tnct::log::no_logger;

  using result = int;

  struct handling_a {
    struct event {
      friend std::ostream &operator<<(std::ostream &p_out, const event &) {
        return p_out;
      }
    };

    struct handler {
      void operator()(event &&) {}
    };

    using queue = tnct::container::circular_queue<logger, event, 10>;

    using priority = priority_master;
    using id = tnct::string::fixed_size_string<5>;

    const id &get_id() const { return m_id; }
    handler get_handler() const { return handler{}; }
    queue get_queue() { return queue{m_logger}; }
    std::size_t get_num_handlers() const { return 3; }
    priority get_priority() const { return priority::h; }

  private:
    logger m_logger;
    id m_id{"01234"};
  };

  template <traits::async::has_add_handling_method<result, handling_a>
                t_add_handling_method>
  void foo(t_add_handling_method p_add_handling_method) {
    p_add_handling_method.template add_handling<handling_a>(
        std::move(handling_a{}));
  }

  struct dummy {
    template <typename t_event> result_master add_handling(handling_a &&) {
      return result_master::a;
    }
  };
};

struct handling_priority_000 {
  static std::string desc() {
    return "a 'handling_priority' which is not conformance to "
           "tnct::traits::async::handling_priority because it does not "
           "conformance to tnct::traits::has_output_operator";
  }

  bool operator()(const program::options &) {
    // enum class priority_000 : int { a = -1, b = 0, c = 1 };
    // static_assert(tnct::traits::async::handling_priority<priority_000>,
    //               "priority_000 is not tnct::traits::async::handling_priority
    //               " "conformance");

    /*
async_test.h:660:19: Static assertion failed: priority_000 is not
tnct::traits::async::handling_priority conformance

async_test.h:660:19: because 'priority_000' does not satisfy 'handling_priority'

handling_priority.h:15:45: because 'priority_000' does not satisfy
'has_output_operator'

has_output_operator.h:18:8: because 'os << value' would be invalid: invalid
operands to binary expression ('std::basic_ostream<char, char_traits<char>>' and
'const priority_000')
*/
    return true;
  }
};

struct handling_priority_001 {
  static std::string desc() {
    return "a 'handling_priority' which is not conformance to "
           "tnct::traits::async::handling_priority because it does not "
           "conformance to tnct::traits::enum_like";
  }

  bool operator()(const program::options &) {
    // using priority_001 = uint16_t;
    // static_assert(tnct::traits::async::handling_priority<priority_001>,
    //               "priority_000 is not tnct::traits::async::handling_priority
    //               " "conformance");
    /*
async_test.h:692:19: Static assertion failed: priority_000 is not
tnct::traits::async::handling_priority conformance

async_test.h:692:19: because 'priority_001' (aka 'unsigned short') does not
satisfy 'handling_priority'

handling_priority.h:15:29: because 'unsigned short' does not satisfy 'enum_like'

enum.h:14:21: because 'std::is_enum_v<unsigned short>' evaluated to false
*/

    return true;
  }
};

enum class priority_002 : int { a = -1, b = 0, c = 1 };
std::ostream &operator<<(std::ostream &p_out, const priority_002 &) {
  return p_out;
}

struct handling_priority_002 {
  static std::string desc() {
    return "a 'handling_priority' which is conformance to "
           "tnct::traits::async::handling_priority";
  }

  bool operator()(const program::options &) {
    static_assert(tnct::traits::async::handling_priority<priority_002>,
                  "priority_000 is not tnct::traits::async::handling_priority "
                  "conformance");

    return true;
  }
};

struct handler_000 {
  static std::string desc() {
    return "A lambda function that is tnct::traits::async::handler conformance";
  }
  bool operator()(const program::options &) {

    static_assert(tnct::traits::async::handler<handler, event>,
                  "'auto _handle = [](event &&) {}' is conformance to "
                  "tnct::traits::async::handler");

    return true;
  }

private:
  struct event {
    friend std::ostream &operator<<(std::ostream &p_out, const event &) {
      return p_out;
    }
  };
  struct handler {
    void operator()(event &&) {}
  };
};

struct handler_001 {
  static std::string desc() {
    return "A lambda function that is not tnct::traits::async::handler "
           "conformance";
  }
  bool operator()(const program::options &) {
    auto _handle = []() {};

    static_assert(!tnct::traits::async::handler<decltype(_handle), event>,
                  "'auto _handle = [](event &&) {}' is not conformance to "
                  "tnct::traits::async::handler");

    return true;
  }

private:
  struct event {
    friend std::ostream &operator<<(std::ostream &p_out, const event &) {
      return p_out;
    }
  };
};

struct handler_002 {
  static std::string desc() {
    return "A 'handle' class that is tnct::traits::async::handler conformance";
  }
  bool operator()(const program::options &) {

    static_assert(
        tnct::traits::async::handler<handle, event>,
        "'handle class is conformance to tnct::traits::async::handler");

    return true;
  }

private:
  struct event {
    friend std::ostream &operator<<(std::ostream &p_out, const event &) {
      return p_out;
    }
  };

  struct handle {
    void operator()(event &&) {}
  };
};

struct handler_003 {
  static std::string desc() {
    return "A 'handle' class that is not conformant to "
           "tnct::traits::async::handler ";
  }
  bool operator()(const program::options &) {

    static_assert(
        !tnct::traits::async::handler<handle, event>,
        "'handle class is not conformant to tnct::traits::async::handler");

    return true;
  }

private:
  struct event {
    friend std::ostream &operator<<(std::ostream &p_out, const event &) {
      return p_out;
    }
  };

  struct handle {
    void operator()() {}
  };
};

struct handler_004 {
  static std::string desc() {
    return "Another 'handle' class that is not conformant to "
           "tnct::traits::async::handler";
  }

  bool operator()(const program::options &) {
    static_assert(
        !tnct::traits::async::handler<handle, event>,
        "'handle' class is not conformant to tnct::traits::async::handler");
    return true;
  }

private:
  struct event {
    friend std::ostream &operator<<(std::ostream &p_out, const event &) {
      return p_out;
    }
  };

  struct handle {
    void operator()(event) {}
  };
};

struct handler_005 {
  static std::string desc() {
    return "Yet another 'handle' class that is not conformant to "
           "tnct::traits::async::handler";
  }

  bool operator()(const program::options &) {
    static_assert(
        !tnct::traits::async::handler<handle, event>,
        "'handle' class is not conformant to tnct::traits::async::handler");
    return true;
  }

private:
  struct event {
    friend std::ostream &operator<<(std::ostream &p_out, const event &) {
      return p_out;
    }
  };

  struct handle {
    void operator()(const event &) {}
  };
};

struct event_000 {
  static std::string desc() {
    return "A 'event' class is not conformant to 'tnct::traits::async::event' "
           "because it does not have output operator";
  }

  bool operator()(const program::options &) {
    static_assert(
        !tnct::traits::async::event<event>,
        "'event' should not be  conformant to 'tnct::traits::async::event' "
        "because it does not output operator");

    return true;
  }

private:
  struct event {};
};

struct event_001 {
  static std::string desc() {
    return "A 'event' class is not conformant to 'tnct::traits::async::event' "
           "because its default constructor is deleted";
  }

  bool operator()(const program::options &) {
    static_assert(
        !tnct::traits::async::event<event>,
        "'event' should not be  conformant to 'tnct::traits::async::event' "
        "because its default constructor is deleted");

    return true;
  }

private:
  struct event {
    event() = delete;
  };
};

struct event_002 {
  static std::string desc() {
    return "A 'event' class is not conformant to 'tnct::traits::async::event' "
           "because its copy constructor is deleted";
  }

  bool operator()(const program::options &) {
    static_assert(
        !tnct::traits::async::event<event>,
        "'event' should not be  conformant to 'tnct::traits::async::event' "
        "because its copy constructor is deleted");

    return true;
  }

private:
  struct event {
    event(const event &) = delete;
  };
};

struct event_003 {
  static std::string desc() {
    return "A 'event' class is not conformant to 'tnct::traits::async::event' "
           "because its move constructor is deleted";
  }

  bool operator()(const program::options &) {
    static_assert(
        !tnct::traits::async::event<event>,
        "'event' should not be  conformant to 'tnct::traits::async::event' "
        "because its move constructor is deleted");

    return true;
  }

private:
  struct event {
    event(event &&) = delete;
  };
};

struct event_004 {
  static std::string desc() {
    return "A 'event' class is conformant to 'tnct::traits::async::event'";
  }

  bool operator()(const program::options &) {
    static_assert(
        tnct::traits::async::event<event>,
        "'event' should be  conformant to 'tnct::traits::async::event' ");

    return true;
  }

private:
  struct event {
    event() = default;
    event(event &&) = default;
    event(const event &) = default;
    friend std::ostream &operator<<(std::ostream &p_out, const event &) {
      return p_out;
    }
  };
};

} // namespace tnct::traits::test
#endif
