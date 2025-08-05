/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TENACITAS_TST_ASYNC_TRAITS_TEST_H
#define TENACITAS_TST_ASYNC_TRAITS_TEST_H

#include <string>

#include "tenacitas/src/async/dispatcher.h"
#include "tenacitas/src/async/handling_priority.h"
#include "tenacitas/src/async/result.h"
#include "tenacitas/src/async/traits/has_add_handling_method.h"
#include "tenacitas/src/async/traits/has_publish_method.h"
#include "tenacitas/src/async/traits/is_dispatcher.h"
#include "tenacitas/src/async/traits/is_event.h"
#include "tenacitas/src/async/traits/is_handler.h"
#include "tenacitas/src/container/circular_queue.h"
#include "tenacitas/src/format/fmt.h"
#include "tenacitas/src/log/cerr.h"
#include "tenacitas/src/program/options.h"

using namespace tenacitas;

namespace tenacitas::tst::async::traits
{

struct event_000
{
  static std::string desc()
  {
    return "Verifies that a class that is not default contructible is not "
           "compatible with 'event";
  }

  bool operator()(const src::program::options &)
  {
    static_assert(
        !src::async::traits::is_event<event_a>,
        "'event_a' is not compatible with src::async::traits::event<event_a>");

    return true;
  }

private:
  struct event_a
  {
    event_a() = delete;
  };
};

struct event_001
{
  static std::string desc()
  {
    return "Verifies that a class that is not copy contructible is not "
           "compatible with 'event";
  }

  bool operator()(const src::program::options &)
  {
    static_assert(
        !src::async::traits::is_event<event_a>,
        "'event_a' is not compatible with src::async::traits::event<event_a>");

    return true;
  }

private:
  struct event_a
  {
    event_a()                = default;
    event_a(const event_a &) = delete;
  };
};

struct event_002
{
  static std::string desc()
  {
    return "Verifies that a class that is not move contructible is not "
           "compatible with 'event";
  }

  bool operator()(const src::program::options &)
  {
    static_assert(
        !src::async::traits::is_event<event_a>,
        "'event_a' is not compatible with src::async::traits::event<event_a>");

    return true;
  }

private:
  struct event_a
  {
    event_a()                = default;
    event_a(const event_a &) = default;
    event_a(event_a &&)      = delete;
  };
};

struct event_003
{
  static std::string desc()
  {
    return "Verifies that a class that does not have the output operator is "
           "not compatible with 'event";
  }

  bool operator()(const src::program::options &)
  {
    static_assert(
        !src::async::traits::is_event<event_a>,
        "'event_a' is not compatible with src::async::traits::event<event_a>");

    return true;
  }

private:
  struct event_a
  {
    event_a()                = default;
    event_a(const event_a &) = default;
    event_a(event_a &&)      = default;
  };
};

enum class event_004_event_a : char
{
  start  = 's',
  finish = 'f'
};

std::ostream &operator<<(std::ostream &p_out, event_004_event_a)
{
  return p_out;
}

struct event_004
{
  static std::string desc()
  {
    return "Verifies that an 'enum' not compatible with 'event";
  }

  bool operator()(const src::program::options &)
  {
    static_assert(
        !src::async::traits::is_event<event_004_event_a>,
        "'event_a' is not compatible with src::async::traits::event<event_a>");

    return true;
  }
};

struct event_005
{
  static std::string desc()
  {
    return "Verifies that a class is default,copy and move constructible and "
           "that implements output operator is compatible with 'event";
  }

  bool operator()(const src::program::options &)
  {
    static_assert(
        src::async::traits::is_event<event_a>,
        "'event_a' is compatible with src::async::traits::event<event_a>");

    return true;
  }

private:
  struct event_a
  {
    friend std::ostream &operator<<(std::ostream &p_out, event_a)
    {
      return p_out;
    }
  };
};

struct add_handler_tests_event
{
  friend std::ostream &operator<<(std::ostream &p_out,
                                  add_handler_tests_event)
  {
    return p_out;
  }
};

struct add_handler_000
{
  static std::string desc()
  {
    return "A struct not compatible with "
           "'src::async::traits::handler' because it does not "
           "implement call operator that receives a 'event' rvalue";
  }
  bool operator()(const src::program::options &)
  {
    static_assert(!src::async::traits::is_handler<
                      add_handler_000, add_handler_tests_event>,
                  "'handler_000' is not compatible with "
                  "src::async::traits::handler ");

    return true;
  }
};

struct add_handler_001
{
  static std::string desc()
  {
    return "A struct not compatible with "
           "'src::async::traits::handler' because it implements a "
           "call operator that receives a 'event' rvalue, but it returns bool";
  }
  bool operator()(const src::program::options &)
  {
    static_assert(!src::async::traits::is_handler<
                      add_handler_001, add_handler_tests_event>,
                  "'handler_000' is not compatible with "
                  "src::async::traits::handler ");

    return true;
  }

  bool operator()(add_handler_tests_event &&)
  {
    return false;
  }
};

struct add_handler_002
{
  static std::string desc()
  {
    return "A struct not compatible with "
           "'src::async::traits::handler' because it implements a "
           "call operator that receives a 'event' rvalue, but it also "
           "implements a call operator that receives a constant 'event' lvalue "
           "reference";
  }
  bool operator()(const src::program::options &)
  {
    static_assert(!src::async::traits::is_handler<
                      add_handler_002, add_handler_tests_event>,
                  "'handler_000' is not compatible with "
                  "src::async::traits::handler ");

    return true;
  }

  void operator()(add_handler_tests_event &&)
  {
  }

  void operator()(const add_handler_tests_event &)
  {
  }
};

struct add_handler_003
{
  static std::string desc()
  {
    return "A struct not compatible with "
           "'src::async::traits::handler' because it implements a "
           "call operator that receives a 'event' rvalue, but it also "
           "implements a call operator that receives a constant 'event' "
           "lvalue ";
  }
  bool operator()(const src::program::options &)
  {
    static_assert(!src::async::traits::is_handler<
                      add_handler_003, add_handler_tests_event>,
                  "'handler_000' is not compatible with "
                  "src::async::traits::handler ");

    return true;
  }

  void operator()(add_handler_tests_event)
  {
  }
};

struct add_handler_004
{
  static std::string desc()
  {
    return "A struct not compatible with "
           "'src::async::traits::handler' because it implements a "
           "call operator that receives a 'event' rvalue, but it also "
           "implements a call operator that receives a 'event' lvalue "
           "reference ";
  }
  bool operator()(const src::program::options &)
  {
    static_assert(!src::async::traits::is_handler<
                      add_handler_004, add_handler_tests_event>,
                  "'handler_000' is not compatible with "
                  "src::async::traits::handler ");

    return true;
  }

  void operator()(add_handler_tests_event &)
  {
  }
};

struct add_handler_005
{
  static std::string desc()
  {
    return "A struct compatible with 'src::async::traits::handler' ";
  }
  bool operator()(const src::program::options &)
  {
    static_assert(
        src::async::traits::is_handler<add_handler_005,
                                                 add_handler_tests_event>,
        "'handler_000' is not compatible with "
        "src::async::traits::handler ");

    return true;
  }

  void operator()(add_handler_tests_event &&)
  {
  }

  void handle(add_handler_tests_event &&)
  {
  }
};

struct dispatcher_000
{

  static std::string desc()
  {
    return "Basic traitrs::dispatcher test";
  }

  bool operator()(const src::program::options &)
  {
    src::log::cerr _logger;

    using my_dispatcher = dispatcher<event_a, event_b, event_c>;

    static_assert(
        src::async::traits::is_dispatcher<my_dispatcher, event_a, event_c>,
        "'my_dispatcher should be compatible with src::async::traits");

    my_dispatcher _dispatcher;

    f(_dispatcher);

    return true;
  }

private:
  struct event_a
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &)
    {
      return p_out;
    }
  };

  struct event_b
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_b &)
    {
      return p_out;
    }
  };

  struct event_c
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_c &)
    {
      return p_out;
    }
  };

  template <src::async::traits::is_event... t_events>
  struct dispatcher
  {
    using events = std::tuple<t_events...>;

    dispatcher()                   = default;
    dispatcher(const dispatcher &) = delete;
    dispatcher(dispatcher &&)      = delete;

    dispatcher &operator=(const dispatcher &) = delete;
    dispatcher &operator=(dispatcher &&)      = delete;

    void *operator new(size_t) = delete;
  };

  using my_dispatcher = dispatcher<event_a, event_b>;

  template <src::async::traits::is_dispatcher<event_a, event_b> t_dispatcher>
  void f(t_dispatcher &)
  {
  }
};

struct has_add_handling_method_000
{
  static std::string desc()
  {
    return "Checks for src::async::traits::has_add_handling_method";
  }

  bool operator()(const src::program::options &)
  {

    logger _logger;

    using dispatcher = src::async::dispatcher<logger, event_a>;

    dispatcher _dispatcher{_logger};

    abc<dispatcher> _abc(_logger, _dispatcher);

    return true;
  }

private:
  using logger = src::log::cerr;

  struct event_a
  {
    friend std ::ostream &operator<<(std::ostream &p_out, const event_a &)
    {
      return p_out;
    }
  };

  template <typename t_dispatcher>
  struct abc
  {

    using queue_a = src::container::circular_queue<logger, event_a, 100>;
    using handler_a = std::function<void(event_a &&)>;

    static_assert(src::async::traits::has_add_handling_method<
                      t_dispatcher, event_a, queue_a, handler_a>,
                  "t_dispatcher must have add_handling<event_a>()");

    abc(logger &p_logger, t_dispatcher &p_dispatcher)
        : m_dispatcher(p_dispatcher)
    {
      create_handling_1(p_logger);
      create_handling_2(p_logger);
    }

  private:
    void handler_1(event_a &&)
    {
    }

    void handler_2(event_a &&)
    {
    }

    void create_handling_1(logger &p_logger)
    {

      src::async::result _result{m_dispatcher.template add_handling<event_a>(
          "handling 1", queue_a{p_logger},
          [this](event_a &&evt) { handler_1(std::move(evt)); },
          src::async::handling_priority::medium, 2)};

      if (_result != src::async::result::OK)
      {
        TNCT_LOG_ERR(p_logger, src::format::fmt(_result));
      }
      else
      {
        TNCT_LOG_INF(p_logger, "add handling 1 OK");
      }
    }

    void create_handling_2(logger &p_logger)
    {

      src::async::result _result{m_dispatcher.template add_handling<event_a>(
          "handling 2", queue_a{p_logger},
          [this](event_a &&evt) { handler_2(std::move(evt)); },
          src::async::handling_priority::medium, 4)};

      if (_result != src::async::result::OK)
      {
        TNCT_LOG_ERR(p_logger, src::format::fmt(_result));
      }
      else
      {
        TNCT_LOG_INF(p_logger, "add handling 2 OK");
      }
    }

  private:
    t_dispatcher &m_dispatcher;
  };
};

struct has_add_handling_method_001
{
  static std::string desc()
  {
    return "Checks for src::async::traits::has_add_handling_method";
  }

  bool operator()(const src::program::options &)
  {

    logger _logger;

    using dispatcher = invalid_dispatcher;

    dispatcher _dispatcher;

    abc<dispatcher> _abc(_dispatcher);

    return true;
  }

private:
  using logger = src::log::cerr;

  struct invalid_dispatcher
  {
  };

  struct event_a
  {
    friend std ::ostream &operator<<(std::ostream &p_out, const event_a &)
    {
      return p_out;
    }
  };

  template <typename t_dispatcher>
  struct abc
  {

    using queue_a = src::container::circular_queue<logger, event_a, 100>;
    using handler_a = std::function<void(event_a &&)>;

    static_assert(!src::async::traits::has_add_handling_method<
                      t_dispatcher, event_a, handler_a, queue_a>,
                  "t_dispatcher must have add_handling<event_a>()");

    abc(t_dispatcher &p_dispatcher) : m_dispatcher(p_dispatcher)
    {
    }

    void handler_1(event_a &&)
    {
    }

    void handler_2(event_a &&)
    {
    }

  private:
    t_dispatcher &m_dispatcher;
  };
};

struct has_publish_method_000
{
  static std::string desc()
  {
    return "A valid has_publish_method";
  }

  bool operator()(const src::program::options &)
  {

    static_assert(src::async::traits::has_publish_method<publisher, event_a>,
                  "'publisher' should be compatible with "
                  "'src::async::traits::has_publish_method'");
    return true;
  }

private:
  struct event_a
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &)
    {
      return p_out;
    }
  };

  struct publisher
  {

    template <src::async::traits::is_event t_event>
    src::async::result publish(const t_event &)
    {
      return src::async::result::OK;
    }
  };
};

struct has_publish_method_001
{
  static std::string desc()
  {
    return "A invalid has_publish_method";
  }

  bool operator()(const src::program::options &)
  {

    static_assert(!src::async::traits::has_publish_method<publisher, event_b>,
                  "'publisher' should not be compatible with "
                  "'src::async::traits::has_publish_method'");
    return true;
  }

private:
  struct event_b
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_b &)
    {
      return p_out;
    }
  };

  struct event_a
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &)
    {
      return p_out;
    }
  };

  struct publisher
  {
    template <src::async::traits::is_event t_event>
    src::async::result publish(t_event &&)
    {
      return src::async::result::OK;
    }
  };
};

struct has_publish_method_002
{
  static std::string desc()
  {
    return "A valid has_publish_method";
  }

  bool operator()(const src::program::options &)
  {

    static_assert(
        src::async::traits::has_publish_method<publisher, event_a, event_b>,
        "'publisher' should be compatible with "
        "'src::async::traits::has_publish_method'");
    return true;
  }

private:
  struct event_a
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &)
    {
      return p_out;
    }
  };

  struct event_b
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_b &)
    {
      return p_out;
    }
  };

  struct publisher
  {

    template <src::async::traits::is_event t_event, typename... t_params>
    src::async::result publish(t_params &&...)
    {
      return src::async::result::OK;
    }
  };
};

struct handler_001
{
  static std::string desc()
  {
    return "A lambda function that is not "
           "src::async::traits::is_handler conformance";
  }
  bool operator()(const src::program::options &)
  {
    auto _handle = []() {};

    static_assert(
        !src::async::traits::is_handler<decltype(_handle), event>,
        "'auto _handle = []() {}' is not conformance to "
        "src::async::traits::is_handler");

    return true;
  }

private:
  struct event
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event &)
    {
      return p_out;
    }
  };
};

struct handler_002
{
  static std::string desc()
  {
    return "A 'handle' class that is src::async::traits::is_handler "
           "conformance";
  }
  bool operator()(const src::program::options &)
  {

    static_assert(src::async::traits::is_handler<handle, event>,
                  "handle class is conformance to "
                  "src::async::traits::is_handler");

    return true;
  }

private:
  struct event
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event &)
    {
      return p_out;
    }
  };

  struct handle
  {
    void operator()(event &&)
    {
    }
  };
};

struct handler_003
{
  static std::string desc()
  {
    return "A 'handle' class that is not conformant to "
           "src::async::traits::is_handler ";
  }
  bool operator()(const src::program::options &)
  {

    static_assert(!src::async::traits::is_handler<handle, event>,
                  "'handle class is not conformant to "
                  "src::async::traits::is_handler");

    return true;
  }

private:
  struct event
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event &)
    {
      return p_out;
    }
  };

  struct handle
  {
    void operator()()
    {
    }
  };
};

struct handler_004
{
  static std::string desc()
  {
    return "Another 'handle' class that is not conformant to "
           "src::async::traits::is_handler";
  }

  bool operator()(const src::program::options &)
  {
    static_assert(!src::async::traits::is_handler<handle, event>,
                  "'handle' class is not conformant to "
                  "src::async::traits::is_handler");
    return true;
  }

private:
  struct event
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event &)
    {
      return p_out;
    }
  };

  struct handle
  {
    void operator()(event)
    {
    }
  };
};

struct handler_005
{
  static std::string desc()
  {
    return "Yet another 'handle' class that is not conformant to "
           "src::async::traits::is_handler";
  }

  bool operator()(const src::program::options &)
  {
    static_assert(!src::async::traits::is_handler<handle, event>,
                  "'handle' class is not conformant to "
                  "src::async::traits::is_handler");
    return true;
  }

private:
  struct event
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event &)
    {
      return p_out;
    }
  };

  struct handle
  {
    void operator()(const event &)
    {
    }
  };
};

} // namespace tenacitas::tst::async::traits
#endif

// struct event_000
// {
//   static std::string desc()
//   {
//     return "A 'event' class is not conformant to 'src::async::traits::event'
//     "
//            "because it does not have output operator";
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(
//         !src::async::traits::event<event>,
//         "'event' should not be  conformant to 'src::async::traits::event' "
//         "because it does not output operator");

//     return true;
//   }

// private:
//   struct event
//   {
//   };
// };

// struct event_001
// {
//   static std::string desc()
//   {
//     return "A 'event' class is not conformant to 'src::async::traits::event'
//     "
//            "because its default constructor is deleted";
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(
//         !src::async::traits::event<event>,
//         "'event' should not be  conformant to 'src::async::traits::event' "
//         "because its default constructor is deleted");

//     return true;
//   }

// private:
//   struct event
//   {
//     event() = delete;
//   };
// };

// struct event_002
// {
//   static std::string desc()
//   {
//     return "A 'event' class is not conformant to 'src::async::traits::event'
//     "
//            "because its copy constructor is deleted";
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(
//         !src::async::traits::event<event>,
//         "'event' should not be  conformant to 'src::async::traits::event' "
//         "because its copy constructor is deleted");

//     return true;
//   }

// private:
//   struct event
//   {
//     event(const event &) = delete;
//   };
// };

// struct event_003
// {
//   static std::string desc()
//   {
//     return "A 'event' class is not conformant to 'src::async::traits::event'
//     "
//            "because its move constructor is deleted";
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(
//         !src::async::traits::event<event>,
//         "'event' should not be  conformant to 'src::async::traits::event' "
//         "because its move constructor is deleted");

//     return true;
//   }

// private:
//   struct event
//   {
//     event(event &&) = delete;
//   };
// };

// struct event_004
// {
//   static std::string desc()
//   {
//     return "A 'event' class is conformant to 'src::async::traits::event'";
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(
//         src::async::traits::event<event>,
//         "'event' should be  conformant to 'src::async::traits::event' ");

//     return true;
//   }

// private:
//   struct event
//   {
//     event()              = default;
//     event(event &&)      = default;
//     event(const event &) = default;
//     friend std::ostream &operator<<(std::ostream &p_out, const event &)
//     {
//       return p_out;
//     }
//   };
// };

// enum class result_like : std::int8_t
// {
//   OK      = 0,
//   ERROR_1 = -1,
//   ERROR_2 = -2
// };

// std::ostream &operator<<(std::ostream &p_out, result_like)
// {

//   return p_out;
// }

// struct result_000
// {

//   static std::string desc()
//   {
//     return {"A 'enum' compatible with 'result'"};
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(src::async::traits::result<result_like>, "This compiles");
//     return true;
//   }
// };
