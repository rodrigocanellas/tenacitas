/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

#include <tenacitas.h>

using namespace std::chrono_literals;
using namespace tenacitas::lib;

struct sleeping_loop_000 {

  static const std::string desc() { return "'sleeping_loop' creation test"; }

  bool operator()(const tncta::program_options &) {

    using loop = tncta::sleeping_loop;

    auto _operation = []() -> void { TNCT_LOG_DEB("loop1"); };

    loop _loop(_operation, 1s);

    return true;
  }
};

struct sleeping_loop_001 {
  using loop = tncta::sleeping_loop;

  typedef uint16_t value;

  static const std::string desc() {
    std::stringstream _stream;
    _stream << "'sleeping_loop' with interval of " << m_interval_secs
            << "s, increments a counter, and just prints."

            << "\nCounter should be " << m_amount;
    return _stream.str();
  }

  bool operator()(const tncta::program_options &) {

    operation1 _op(&m_cond);

    loop _loop([&_op]() { return _op(); },
               std::chrono::seconds(m_interval_secs));

    _loop.start();

    {
      std::unique_lock<std::mutex> _lock(m_mutex);
      m_cond.wait(_lock, [&_op]() -> bool { return _op.counter >= m_amount; });
    }

    _loop.stop();

    if (_op.counter != m_amount) {
      TNCT_LOG_ERR("counter should be ", m_amount, ", but it is ", _op.counter);
      return false;
    }

    TNCT_LOG_INF("counter should be ", m_amount, ", and it really is ",
                 _op.counter);

    return true;
  }

private:
  struct operation1 {

    operation1(std::condition_variable *p_cond) : m_cond(p_cond) {}

    void operator()() {
      if (counter < m_amount) {
        ++counter;
        TNCT_LOG_DEB("counter = ", counter);
        std::this_thread::sleep_for(m_sleep);
      } else {
        m_cond->notify_one();
      }
    }

    value counter{0};

  private:
    std::condition_variable *m_cond;
  };

private:
  std::mutex m_mutex;
  std::condition_variable m_cond;

  static constexpr value m_interval_secs{1};
  static constexpr value m_amount{14};
  static constexpr std::chrono::milliseconds m_sleep{200};
};

// struct sleeping_loop_002 {
//    static std::string desc() { return ""; }

//    bool operator()(const tncta::program_options &) {

//        std::vector<sleeping_loop> _loops;

//        return true;
//    }

// private:
//    struct sent {
//        friend std::ostream &operator<<(std::ostream &p_out, const sent &) {
//            p_out << "sent";
//            return p_out;
//        }
//    };

//    typedef event_t<2> event;
//    typedef async::internal::publishing_t<event> event_publishings;
//    typedef tncta::sleeping_loop sleeping_loop;
//    // event sent each time a work event is sent

//    typedef async::internal::publishing_t<sent> sent_subscribers;

//    struct sender {
//        sender(size_t p_idx,
//               event_publishings *p_subscribers,
//               sent_subscribers *p_sent_subscribers,
//               size_t p_num_event_per_sender)
//            : m_idx('s' + std::to_string(p_idx))
//            , m_subscribers(p_subscribers)
//            , m_sent_subscribers(p_sent_subscribers)
//            , m_num_event_per_sender(p_num_event_per_sender)
//            , m_event(std::make_shared<event>(p_idx * m_num_event_per_sender))
//            , m_start(p_idx * m_num_event_per_sender)
//            , m_finish(
//                  (p_idx * m_num_event_per_sender + m_num_event_per_sender) -
//                  1) {

//            TNCT_LOG_TST(m_idx, ": start value = ", m_start,
//                ", finish value = ", m_finish);
//        }

//        void operator()(std::shared_ptr<const bool>) {

//            if (m_all_notified) {
//                //                TNCT_LOG_TST(m_idx, " already notified");
//                return;
//            }

//            if ((m_event->get_value() >= (m_finish + 1))) {
//                m_all_notified = true;

//                TNCT_LOG_TST(m_idx, " sent ", m_num_event_per_sender,
//                    " events: ", m_start, " -> ", m_finish);
//                return;
//            }

//            TNCT_LOG_TST(m_idx, " sending ", *m_event);
//            m_subscribers->add_event(m_event);
//            m_sent_subscribers->add_event(std::make_shared<sent>());
//            ++(*m_event);
//        }

//    private:
//        std::string m_idx;
//        event_publishings *m_subscribers;
//        sent_subscribers *m_sent_subscribers;
//        const size_t m_num_event_per_sender;

//        std::shared_ptr<event> m_event;
//        value m_start;
//        value m_finish;

//        bool m_all_notified {false};
//    };
//};

int main(int argc, char **argv) {
  using namespace tenacitas::lib;

  tncta::set_debug_level();

  tncta::tester<> _tester(argc, argv);

  run_test(_tester, sleeping_loop_000);
  run_test(_tester, sleeping_loop_001);
}
