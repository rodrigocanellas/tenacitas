/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

#include <tenacitas/async.h>
#include <tenacitas/internal/internal_async.h>
#include <tenacitas/logger.h>
#include <tenacitas/number.h>
#include <tenacitas/tester.h>
#include <tenacitas/type.h>
#include <tst/tenacitas/async/msg.h>

using namespace tenacitas;
using namespace tenacitas::async;
using namespace std::chrono_literals;

template <message_id msg_id, uint16_t num_senders, uint16_t msgs_per_sender,
          uint16_t interval_per_sender, uint16_t num_handlers,
          uint16_t handlers_timeout, uint16_t timeout_at_each>
struct test_t {

    typedef msg_t<msg_id> msg;
    typedef async::internal::handlers_t<msg> msg_handlers;
    typedef async::sleeping_loop sleeping_loop;
    typedef std::vector<uint16_t> values;
    typedef std::chrono::milliseconds time;
    typedef std::vector<time> times;

    test_t() : m_name("handlers_" + number::format(msg_id)) {}

    bool operator()() {

        logger::cerr<> _log{m_name.c_str()};
        _log.set_info_level();
        //        logger::set_debug_level();
        number::id _id;

        std::mutex _mutex_counter_handled;
        std::mutex _mutex_counter_sent;

        std::condition_variable _cond_sent;
        std::mutex _mutex_sent;
        sent_handlers _sent_handlers{_id, std::chrono::seconds(2)};

        std::condition_variable _cond_handled;
        std::mutex _mutex_handled;
        handled_handlers _handled_handlers{_id, std::chrono::seconds(2)};

        msg_handlers _msg_handlers{_id, time{handlers_timeout}};

        DEB(_log, "total messages to be sent: ", m_total_msgs);

        INF(_log, "creating and starting the senders");
        std::vector<sleeping_loop> _loops;
        for (uint16_t _i = 0; _i < num_senders; ++_i) {
            _loops.push_back({_id,
                              sender(_i, &_msg_handlers, &_sent_handlers,
                                     &_cond_sent, msgs_per_sender),
                              60000ms, time{interval_per_sender}});
            DEB(_log, "starting sender p", _i);
            _loops.back().start();
        }

        INF(_log, "creating handlers for handled messages");
        uint16_t _handled{0};
        bool _all_handled_notified{false};
        _handled_handlers.add_handler([&](type::ptr<bool>, handled &&) -> void {
            if (_all_handled_notified) {
                DEB(_log, "all handled notified");
                return;
            }
            {
                std::lock_guard<std::mutex> _lock(_mutex_counter_handled);
                ++_handled;
            }

            if (_handled >= m_total_msgs) {
                INF(_log, "all messages handled, notifying");
                _all_handled_notified = true;
                _cond_handled.notify_one();
                return;
            }
            DEB(_log, _handled, " messages handled");
        });

        INF(_log, "creating handlers for messages sent");
        uint16_t _sent{0};
        bool _all_sent_notified{false};
        _sent_handlers.add_handler([&](type::ptr<bool>, sent &&) -> void {
            if (_all_sent_notified) {
                return;
            }

            {
                std::lock_guard<std::mutex> _lock(_mutex_counter_sent);
                ++_sent;
            }

            if (_sent >= m_total_msgs) {
                INF(_log, "all messages sent, notifying");
                _all_sent_notified = true;
                _cond_sent.notify_one();
                return;
            }
            INF(_log, _sent, " messages sent");
        });

        INF(_log, "creating the msg handlers");
        std::vector<type::ptr<handler>> _handler_list;
        for (uint16_t _i = 0; _i < num_handlers; ++_i) {
            _handler_list.push_back(std::make_shared<handler>(
                _i, &_msg_handlers, &_handled_handlers, time{handlers_timeout},
                m_max_msgs_timeout, timeout_at_each));
        }

        uint64_t _start = calendar::now<>::microsecs();

        INF(_log, "adding each handler to the handlers group");
        for (type::ptr<handler> &_handler : _handler_list) {
            _msg_handlers.add_handler(
                [&, _handler](type::ptr<bool> p_bool, msg &&p_msg) -> void {
                    (*_handler)(p_bool, std::move(p_msg));
                });
        }

        INF(_log, "checking if all the messages were sent");
        {
            std::unique_lock<std::mutex> _lock(_mutex_sent);
            _cond_sent.wait(_lock, [&]() -> bool {
                if (_sent >= m_total_msgs) {
                    DEB(_log, "all senders have notified");
                    return true;
                }
                DEB(_log, _sent, " msgs sent so far");
                return false;
            });
        }

        INF(_log, "stoping the senders");
        for (uint16_t _i = 0; _i < num_senders; ++_i) {
            _loops[_i].stop();
        }
        INF(_log, "capacity: ", _msg_handlers.capacity(),
            ", added: ", _msg_handlers.amount_added(),
            ", occupied: ", _msg_handlers.occupied());

        INF(_log, "waiting for all the handlers, until all the messages are "
                  "handled");
        {
            std::unique_lock<std::mutex> _lock(_mutex_handled);
            _cond_handled.wait(_lock, [&]() -> bool {
                if (_handled >= m_total_msgs) {
                    INF(_log, "all ", m_total_msgs, " msgs handled");
                    return true;
                }
                INF(_log, _handled, " handled msgs so far");
                return false;
            });
        }

        uint64_t _finish = calendar::now<>::microsecs();

        INF(_log, "reporting results");
        {
            DEB(_log, "amount sent = ", m_total_msgs);
            for (uint16_t _i = 0; _i < num_handlers; ++_i) {
                DEB(_log, *_handler_list[_i]);
            }
            INF(_log, "handled: ", _handled);
        }

        {
            double _micro = _finish - _start;
            double _milli = _micro / 1000;
            double _sec = _milli / 1000;
            double _min = _sec / 60;
            INF(_log, "TIMES: ", _micro, "us, ", _milli, "ms, ", _sec, "s, ",
                _min, "m");
            std::ofstream _file("stats_test_handlers.csv", std::ios::app);
            _file << static_cast<uint16_t>(msg_id) << ',' << num_handlers << ','
                  << (timeout_at_each ? 's' : 'n') << ',' << num_senders << ','
                  << msgs_per_sender << ',' << m_total_msgs << ',' << _micro
                  << ',' << _milli << ',' << _sec << ',' << _min << std::endl;
        }

        return (m_total_msgs == _handled);
    }

    static std::string description() {
        std::stringstream _stream;

        _stream << num_senders << " sender(s), sending " << msgs_per_sender
                << " messages each, so " << m_total_msgs
                << " messages will be sent, at " << interval_per_sender
                << "ms.\n"
                << "There are " << num_handlers
                << " handler(s), each with timeout of " << handlers_timeout
                << "ms, and sleeping for " << handlers_timeout / 2
                << "ms, to simulate actual work";
        if (timeout_at_each) {
            _stream
                << ", and at each " << timeout_at_each
                << " messages, each handler will cause a timeout, for at most "
                << m_max_msgs_timeout << " times";
        }

        _stream << ".\n"
                << "The amount of messages consumed must be equal to the "
                   "sent.";

        return _stream.str();
    }

  private:
    struct handled {
        friend std::ostream &operator<<(std::ostream &p_out, const handled &) {
            p_out << "handled";
            return p_out;
        }
    };

    struct sent {
        friend std::ostream &operator<<(std::ostream &p_out, const sent &) {
            p_out << "sent";
            return p_out;
        }
    };

    typedef async::internal::handlers_t<handled> handled_handlers;
    typedef async::internal::handlers_t<sent> sent_handlers;

    struct handler {
        handler(uint16_t p_id, msg_handlers *p_handlers,
                handled_handlers *p_handled_handlers,
                std::chrono::milliseconds p_timeout,
                uint16_t p_num_max_timeouts,
                uint16_t p_cause_timeout_at_each = 0)
            : m_id('h' + std::to_string(p_id)), m_msg_handlers(p_handlers),
              m_handled_handlers(p_handled_handlers), m_timeout(p_timeout),
              m_num_max_timeouts(p_num_max_timeouts),
              m_sleep(m_timeout.count() / 2),
              m_cause_timeout_at_each(p_cause_timeout_at_each),
              m_to_timeout(m_timeout.count() * 2), m_log("handler_" + m_id) {}
        void operator()(std::shared_ptr<bool> p_bool, msg &&p_msg) {
            //            m_log.set_info_level();

            if (m_cause_timeout_at_each &&
                ((m_num % m_cause_timeout_at_each) == 0) &&
                (m_timeout_counter < m_num_max_timeouts)) {
                ++m_timeout_counter;
                INF(m_log, m_id, " causing timeout for msg number ", m_num,
                    " as a timeout must occurr at each ",
                    m_cause_timeout_at_each, " messages");
                std::this_thread::sleep_for(m_to_timeout);
            }
            if (*p_bool) {
                m_msg_handlers->add_data(p_msg);
                return;
            }
            std::this_thread::sleep_for(m_sleep);
            ++m_num;
            m_msg = std::move(p_msg);
            DEB(m_log, m_id, " adding handled");
            m_handled_handlers->add_data(handled{});
            INF(m_log, m_id, " handling ", m_msg, '|', m_num);
        }

        friend std::ostream &operator<<(std::ostream &p_out,
                                        const handler &p_handler) {
            p_out << '(' << p_handler.get_id() << ',' << p_handler.m_msg << ','
                  << p_handler.m_num << ')';
            return p_out;
        }

        inline const std::string &get_id() const { return m_id; }
        //        inline uint16_t get_num() const { return m_num; }

      private:
        std::string m_id;
        msg_handlers *m_msg_handlers;
        handled_handlers *m_handled_handlers;
        const time m_timeout;
        const uint16_t m_num_max_timeouts{0};
        const time m_sleep;
        const uint16_t m_cause_timeout_at_each{0};
        const time m_to_timeout;
        uint16_t m_num{0};
        msg m_msg;
        uint16_t m_timeout_counter{0};
        logger::cerr<> m_log;
    };

    struct sender {
        sender(uint16_t p_idx, msg_handlers *p_handlers,
               sent_handlers *p_sent_handlers, std::condition_variable *p_cond,
               uint16_t p_num_msg_per_sender)
            : m_idx('s' + std::to_string(p_idx)), m_handlers(p_handlers),
              m_sent_handlers(p_sent_handlers), m_cond(p_cond),
              m_num_msg_per_sender(p_num_msg_per_sender),
              m_msg(p_idx * m_num_msg_per_sender),
              m_start(p_idx * m_num_msg_per_sender),
              m_finish((p_idx * m_num_msg_per_sender + m_num_msg_per_sender) -
                       1),
              m_log("sender_" + m_idx) {
            INF(m_log, m_idx, ": start value = ", m_start,
                ", finish value = ", m_finish);
        }

        void operator()(std::shared_ptr<bool>) {
            if (m_notified) {
                INF(m_log, m_idx, " already notified");
                return;
            }

            if ((m_msg.get_value() == (m_finish + 1))) {
                m_notified = true;

                INF(m_log, m_idx, " sent ", m_num_msg_per_sender,
                    " messages: ", m_start, " -> ", m_finish);
                return;
            }

            INF(m_log, m_idx, " sending ", m_msg);
            m_handlers->add_data(m_msg);
            m_sent_handlers->add_data(sent{});
            ++m_msg;
        }

      private:
        std::string m_idx;
        msg_handlers *m_handlers;
        sent_handlers *m_sent_handlers;
        std::condition_variable *m_cond;
        const uint16_t m_num_msg_per_sender;

        msg m_msg;
        value m_start;
        value m_finish;

        bool m_notified{false};

        logger::cerr<> m_log;
    };

  private:
    const std::string m_name;
    //    uint16_t m_total_msgs;

    static constexpr uint16_t m_total_msgs{num_senders * msgs_per_sender};
    static constexpr uint16_t m_max_msgs_timeout{m_total_msgs * 30 / 100};
};

struct test_1 {
    typedef test_t<1 /* msg id */, 1 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_2 {
    typedef test_t<2 /* msg id */, 1 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_3 {
    typedef test_t<3 /* msg id */, 10 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_4 {
    typedef test_t<4 /* msg id */, 10 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_5 {
    typedef test_t<5 /* msg id */, 1 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_6 {
    typedef test_t<6 /* msg id */, 1 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_7 {
    typedef test_t<7 /* msg id */, 10 /* num senders */,
                   5 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_8 {
    typedef test_t<8 /* msg id */, 10 /* num senders */,
                   5 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_9 {
    typedef test_t<9 /* msg id */, 50 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_10 {
    typedef test_t<10 /* msg id */, 50 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_11 {
    typedef test_t<11 /* msg id */, 1 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_12 {
    typedef test_t<12 /* msg id */, 1 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_13 {
    typedef test_t<13 /* msg id */, 10 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_14 {
    typedef test_t<14 /* msg id */, 10 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_15 {
    typedef test_t<15 /* msg id */, 50 /* num senders */,
                   2 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_16 {
    typedef test_t<16 /* msg id */, 50 /* num senders */,
                   2 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_17 {
    typedef test_t<17 /* msg id */, 1 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_18 {
    typedef test_t<18 /* msg id */, 1 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_19 {
    typedef test_t<19 /* msg id */, 10 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_20 {
    typedef test_t<20 /* msg id */, 10 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_21 {
    typedef test_t<21 /* msg id */, 50 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_22 {
    typedef test_t<22 /* msg id */, 50 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_23 {
    typedef test_t<23 /* msg id */, 1 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_24 {
    typedef test_t<24 /* msg id */, 1 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_25 {
    typedef test_t<25 /* msg id */, 10 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_26 {
    typedef test_t<26 /* msg id */, 10 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_27 {
    typedef test_t<27 /* msg id */, 50 /* num senders */,
                   20 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_28 {
    typedef test_t<28 /* msg id */, 50 /* num senders */,
                   20 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_29 {
    typedef test_t<29 /* msg id */, 1 /* num senders */,
                   5000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_30 {
    typedef test_t<30 /* msg id */, 1 /* num senders */,
                   5000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_31 {
    typedef test_t<31 /* msg id */, 10 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_32 {
    typedef test_t<32 /* msg id */, 10 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_33 {
    typedef test_t<33 /* msg id */, 50 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_34 {
    typedef test_t<34 /* msg id */, 50 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_35 {
    typedef test_t<35 /* msg id */, 1 /* num senders */,
                   10000 /* msgs per sender */, 150 /* sender interval (ms)
                                                     */
                   ,
                   1 /* num handlers */,
                   400 /* timeout for each handler
(ms) */, 0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_36 {
    typedef test_t<36 /* msg id */, 1 /* num senders */,
                   10000 /* msgs per sender */, 150 /* sender interval (ms)
                                                     */
                   ,
                   1 /* num handlers */,
                   400 /* timeout for each handler
(ms) */, 3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_37 {
    typedef test_t<37 /* msg id */, 10 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_38 {
    typedef test_t<38 /* msg id */, 10 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_39 {
    typedef test_t<39 /* msg id */, 50 /* num senders */,
                   200 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_40 {
    typedef test_t<40 /* msg id */, 50 /* num senders */,
                   200 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_41 {
    typedef test_t<41 /* msg id */, 1 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_42 {
    typedef test_t<42 /* msg id */, 1 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_43 {
    typedef test_t<43 /* msg id */, 10 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_44 {
    typedef test_t<44 /* msg id */, 10 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_45 {
    typedef test_t<45 /* msg id */, 1 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_46 {
    typedef test_t<46 /* msg id */, 1 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_47 {
    typedef test_t<47 /* msg id */, 10 /* num senders */,
                   5 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_48 {
    typedef test_t<48 /* msg id */, 10 /* num senders */,
                   5 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_49 {
    typedef test_t<49 /* msg id */, 50 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_50 {
    typedef test_t<50 /* msg id */, 50 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_51 {
    typedef test_t<51 /* msg id */, 1 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_52 {
    typedef test_t<52 /* msg id */, 1 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_53 {
    typedef test_t<53 /* msg id */, 10 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_54 {
    typedef test_t<54 /* msg id */, 10 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_55 {
    typedef test_t<55 /* msg id */, 50 /* num senders */,
                   2 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_56 {
    typedef test_t<56 /* msg id */, 50 /* num senders */,
                   2 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_57 {
    typedef test_t<57 /* msg id */, 1 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_58 {
    typedef test_t<58 /* msg id */, 1 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_59 {
    typedef test_t<59 /* msg id */, 10 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_60 {
    typedef test_t<60 /* msg id */, 10 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_61 {
    typedef test_t<61 /* msg id */, 50 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_62 {
    typedef test_t<62 /* msg id */, 50 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_63 {
    typedef test_t<63 /* msg id */, 1 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_64 {
    typedef test_t<64 /* msg id */, 1 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_65 {
    typedef test_t<65 /* msg id */, 10 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_66 {
    typedef test_t<66 /* msg id */, 10 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_67 {
    typedef test_t<67 /* msg id */, 50 /* num senders */,
                   20 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_68 {
    typedef test_t<68 /* msg id */, 50 /* num senders */,
                   20 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_69 {
    typedef test_t<69 /* msg id */, 1 /* num senders */,
                   5000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_70 {
    typedef test_t<70 /* msg id */, 1 /* num senders */,
                   5000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_71 {
    typedef test_t<71 /* msg id */, 10 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_72 {
    typedef test_t<72 /* msg id */, 10 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_73 {
    typedef test_t<73 /* msg id */, 50 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_74 {
    typedef test_t<74 /* msg id */, 50 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_75 {
    typedef test_t<75 /* msg id */, 1 /* num senders */,
                   10000 /* msgs per sender */, 150 /* sender interval (ms)
                                                     */
                   ,
                   10 /* num handlers */,
                   400 /* timeout for each handler
(ms) */, 0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_76 {
    typedef test_t<76 /* msg id */, 1 /* num senders */,
                   10000 /* msgs per sender */, 150 /* sender interval (ms)
                                                     */
                   ,
                   10 /* num handlers */,
                   400 /* timeout for each handler
(ms) */, 3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_77 {
    typedef test_t<77 /* msg id */, 10 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_78 {
    typedef test_t<78 /* msg id */, 10 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_79 {
    typedef test_t<79 /* msg id */, 50 /* num senders */,
                   200 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_80 {
    typedef test_t<80 /* msg id */, 50 /* num senders */,
                   200 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_81 {
    typedef test_t<81 /* msg id */, 1 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_82 {
    typedef test_t<82 /* msg id */, 1 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_83 {
    typedef test_t<83 /* msg id */, 10 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_84 {
    typedef test_t<84 /* msg id */, 10 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_85 {
    typedef test_t<85 /* msg id */, 1 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_86 {
    typedef test_t<86 /* msg id */, 1 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_87 {
    typedef test_t<87 /* msg id */, 10 /* num senders */,
                   5 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_88 {
    typedef test_t<88 /* msg id */, 10 /* num senders */,
                   5 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_89 {
    typedef test_t<89 /* msg id */, 50 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_90 {
    typedef test_t<90 /* msg id */, 50 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_91 {
    typedef test_t<91 /* msg id */, 1 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_92 {
    typedef test_t<92 /* msg id */, 1 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_93 {
    typedef test_t<93 /* msg id */, 10 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_94 {
    typedef test_t<94 /* msg id */, 10 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_95 {
    typedef test_t<95 /* msg id */, 50 /* num senders */,
                   2 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_96 {
    typedef test_t<96 /* msg id */, 50 /* num senders */,
                   2 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_97 {
    typedef test_t<97 /* msg id */, 1 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_98 {
    typedef test_t<98 /* msg id */, 1 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_99 {
    typedef test_t<99 /* msg id */, 10 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_100 {
    typedef test_t<100 /* msg id */, 10 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_101 {
    typedef test_t<101 /* msg id */, 50 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_102 {
    typedef test_t<102 /* msg id */, 50 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_103 {
    typedef test_t<103 /* msg id */, 1 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_104 {
    typedef test_t<104 /* msg id */, 1 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_105 {
    typedef test_t<105 /* msg id */, 10 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_106 {
    typedef test_t<106 /* msg id */, 10 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_107 {
    typedef test_t<107 /* msg id */, 50 /* num senders */,
                   20 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_108 {
    typedef test_t<108 /* msg id */, 50 /* num senders */,
                   20 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_109 {
    typedef test_t<109 /* msg id */, 1 /* num senders */,
                   5000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_110 {
    typedef test_t<110 /* msg id */, 1 /* num senders */,
                   5000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_111 {
    typedef test_t<111 /* msg id */, 10 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_112 {
    typedef test_t<112 /* msg id */, 10 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_113 {
    typedef test_t<113 /* msg id */, 50 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_114 {
    typedef test_t<114 /* msg id */, 50 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_115 {
    typedef test_t<115 /* msg id */, 1 /* num senders */,
                   10000 /* msgs per sender */, 150 /* sender interval (ms)
                                                     */
                   ,
                   50 /* num handlers */,
                   400 /* timeout for each handler
(ms) */, 0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_116 {
    typedef test_t<116 /* msg id */, 1 /* num senders */,
                   10000 /* msgs per sender */, 150 /* sender interval (ms)
                                                     */
                   ,
                   50 /* num handlers */,
                   400 /* timeout for each handler
(ms) */, 3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_117 {
    typedef test_t<117 /* msg id */, 10 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_118 {
    typedef test_t<118 /* msg id */, 10 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_119 {
    typedef test_t<119 /* msg id */, 50 /* num senders */,
                   200 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_120 {
    typedef test_t<120 /* msg id */, 50 /* num senders */,
                   200 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

int main(int argc, char **argv) {
    logger::set_info_level();
    tester::test<> _test(argc, argv);
    run_test(_test, test_1);
    run_test(_test, test_2);
    run_test(_test, test_3);
    run_test(_test, test_4);
    run_test(_test, test_5);
    run_test(_test, test_6);
    run_test(_test, test_7);
    run_test(_test, test_8);
    run_test(_test, test_9);
    run_test(_test, test_10);
    run_test(_test, test_11);
    run_test(_test, test_12);
    run_test(_test, test_13);
    run_test(_test, test_14);
    run_test(_test, test_15);
    run_test(_test, test_16);
    run_test(_test, test_17);
    run_test(_test, test_18);
    run_test(_test, test_19);
    run_test(_test, test_20);
    run_test(_test, test_21);
    run_test(_test, test_22);
    run_test(_test, test_23);
    run_test(_test, test_24);
    run_test(_test, test_25);
    run_test(_test, test_26);
    run_test(_test, test_27);
    run_test(_test, test_28);
    run_test(_test, test_29);
    run_test(_test, test_30);
    run_test(_test, test_31);
    run_test(_test, test_32);
    run_test(_test, test_33);
    run_test(_test, test_34);
    run_test(_test, test_35);
    run_test(_test, test_36);
    run_test(_test, test_37);
    run_test(_test, test_38);
    run_test(_test, test_39);
    run_test(_test, test_40);
    run_test(_test, test_41);
    run_test(_test, test_42);
    run_test(_test, test_43);
    run_test(_test, test_44);
    run_test(_test, test_45);
    run_test(_test, test_46);
    run_test(_test, test_47);
    run_test(_test, test_48);
    run_test(_test, test_49);
    run_test(_test, test_50);
    run_test(_test, test_51);
    run_test(_test, test_52);
    run_test(_test, test_53);
    run_test(_test, test_54);
    run_test(_test, test_55);
    run_test(_test, test_56);
    run_test(_test, test_57);
    run_test(_test, test_58);
    run_test(_test, test_59);
    run_test(_test, test_60);
    run_test(_test, test_61);
    run_test(_test, test_62);
    run_test(_test, test_63);
    run_test(_test, test_64);
    run_test(_test, test_65);
    run_test(_test, test_66);
    run_test(_test, test_67);
    run_test(_test, test_68);
    run_test(_test, test_69);
    run_test(_test, test_70);
    run_test(_test, test_71);
    run_test(_test, test_72);
    run_test(_test, test_73);
    run_test(_test, test_74);
    run_test(_test, test_75);
    run_test(_test, test_76);
    run_test(_test, test_77);
    run_test(_test, test_78);
    run_test(_test, test_79);
    run_test(_test, test_80);
    run_test(_test, test_81);
    run_test(_test, test_82);
    run_test(_test, test_83);
    run_test(_test, test_84);
    run_test(_test, test_85);
    run_test(_test, test_86);
    run_test(_test, test_87);
    run_test(_test, test_88);
    run_test(_test, test_89);
    run_test(_test, test_90);
    run_test(_test, test_91);
    run_test(_test, test_92);
    run_test(_test, test_93);
    run_test(_test, test_94);
    run_test(_test, test_95);
    run_test(_test, test_96);
    run_test(_test, test_97);
    run_test(_test, test_98);
    run_test(_test, test_99);
    run_test(_test, test_100);
    run_test(_test, test_101);
    run_test(_test, test_102);
    run_test(_test, test_103);
    run_test(_test, test_104);
    run_test(_test, test_105);
    run_test(_test, test_106);
    run_test(_test, test_107);
    run_test(_test, test_108);
    run_test(_test, test_109);
    run_test(_test, test_110);
    run_test(_test, test_111);
    run_test(_test, test_112);
    run_test(_test, test_113);
    run_test(_test, test_114);
    run_test(_test, test_115);
    run_test(_test, test_116);
    run_test(_test, test_117);
    run_test(_test, test_118);
    run_test(_test, test_119);
    run_test(_test, test_120);
}
