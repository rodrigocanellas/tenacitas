/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TEST_TRAITS_SUBSCRIBER_H
#define TENACITAS_LIB_TEST_TRAITS_SUBSCRIBER_H

#include <string>
#include <tuple>

#ifndef TENACITAS_LOG
#define TENACITAS_LOG
#endif

#include <tenacitas.lib/program/options.h>
#include <tenacitas.lib/log/cerr.h>
#include <tenacitas.lib/traits/event.h>
#include <tenacitas.lib/traits/subscriber.h>

using namespace tenacitas::lib;
using namespace std::chrono_literals;

namespace subscriber_test {

struct ev0 {};

struct ev1 {};

} // namespace subscriber_test

struct subscriber_000 {
  static std::string desc() {
    return "Declaration of a 'subscriber' with 'events_subscribed' and one "
           "'handle' method for each event";
  }

  bool operator()(const program::options &) {
    subscriber _subscriber(m_log);

    test(_subscriber);

    return true;
  }

private:
  struct subscriber {
    using events_subscribed =
        std::tuple<subscriber_test::ev0, subscriber_test::ev1>;
    subscriber(log::cerr &p_log) : m_log(p_log) {}

    void handle(subscriber_test::ev0 &&) { m_log.tst("ev0"); }

    void handle(subscriber_test::ev1 &&) { m_log.tst("ev1"); }

  private:
    log::cerr &m_log;
  };

  template <traits::subscriber t_subscriber>
  void test(t_subscriber &p_subscriber) {
    p_subscriber.handle(subscriber_test::ev0());
    p_subscriber.handle(subscriber_test::ev1());
  }

  log::cerr m_log;
};

// // the test below does not compile, as expected, because
// // 'subscriber_001::subscriber' does not implement
// 'handle(subscriber_test::ev1
// // &&)'
// struct subscriber_001 {
//   static std::string desc() {
//     return "Declaration of a 'subscriber' with 'events_subscribed' and a "
//            "'handle' method for one event";
//   }

//   bool operator()(const program::options &) {
//     subscriber _subscriber(m_log);

//     test(_subscriber);

//     return true;
//   }

// private:
//   struct subscriber {
//     using events_subscribed =
//         std::tuple<subscriber_test::ev0, subscriber_test::ev1>;
//     subscriber(log::cerr &p_log) : m_log(p_log) {}

//     void handle(subscriber_test::ev0 &&) { m_log.tst("ev0"); }

//     // void handle(subscriber_test::ev1 &&) { m_log.tst("ev1"); }

//   private:
//     log::cerr &m_log;
//   };

//   template <traits::subscriber t_subscriber>
//   void test(t_subscriber &p_subscriber) {
//     p_subscriber.handle(subscriber_test::ev0());
//     p_subscriber.handle(subscriber_test::ev1());
//   }

//   log::cerr m_log;
// };

// // the test below does not compile, as expected, because
// // 'subscriber_001::subscriber' does not implement 'handle(const
// // subscriber_test::ev1
// // &)' neither 'handle(subscriber_test::ev1)'
// struct subscriber_002 {
//   static std::string desc() {
//     return "Declaration of a 'subscriber' with 'events_subscribed' and one "
//            "'handle' method for each event";
//   }

//   bool operator()(const program::options &) {
//     subscriber _subscriber(m_log);

//     test(_subscriber);

//     return true;
//   }

// private:
//   struct subscriber {
//     using events_subscribed =
//         std::tuple<subscriber_test::ev0, subscriber_test::ev1>;
//     subscriber(log::cerr &p_log) : m_log(p_log) {}

//     void handle(subscriber_test::ev0 &&) { m_log.tst("ev0"); }

//     void handle(subscriber_test::ev1 &&) { m_log.tst("ev1"); }

//   private:
//     log::cerr &m_log;
//   };

//   template <traits::subscriber t_subscriber>
//   void test(t_subscriber &p_subscriber) {
//     p_subscriber.handle(subscriber_test::ev0());
//     subscriber_test::ev1 _ev1;
//     p_subscriber.handle(_ev1);
//   }

//   log::cerr m_log;
// };

// // the test below does not compile, as expected, because
// // 'subscriber_001::subscriber' does not implement
// // 'handle(subscriber_test::ev1 &&)'
// struct subscriber_003 {
//   static std::string desc() {
//     return "Declaration of a 'subscriber' with 'events_subscribed' and one "
//            "'handle' method for each event";
//   }

//   bool operator()(const program::options &) {
//     subscriber _subscriber(m_log);

//     test(_subscriber);

//     return true;
//   }

// private:
//   struct subscriber {
//     using events_subscribed =
//         std::tuple<subscriber_test::ev0, subscriber_test::ev1>;
//     subscriber(log::cerr &p_log) : m_log(p_log) {}

//     void handle(subscriber_test::ev0 &&) { m_log.tst("ev0"); }

//   private:
//     log::cerr &m_log;
//   };

//   template <traits::subscriber t_subscriber>
//   void test(t_subscriber &p_subscriber) {
//     p_subscriber.handle(subscriber_test::ev0());
//   }

//   log::cerr m_log;
// };

#endif
