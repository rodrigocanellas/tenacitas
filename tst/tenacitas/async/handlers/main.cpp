/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
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

template <message_id msg_id, uint8_t test_number, uint16_t num_senders,
          uint16_t msgs_per_sender, uint16_t interval_per_sender,
          uint16_t num_handlers, uint16_t handlers_timeout,
          uint16_t timeout_at_each>
struct test_t {

    typedef msg_t<msg_id> msg;
    typedef async::internal::handlers_t<msg> msg_handlers;
    typedef async::sleeping_loop sleeping_loop;
    typedef std::vector<uint16_t> values;
    typedef std::chrono::milliseconds time;
    typedef std::vector<time> times;

    test_t()
        : m_name("handlers_" + number::format(test_number)),
          m_total_msgs(num_senders * msgs_per_sender) {}

    bool operator()() {

        logger::cerr<> _log{m_name.c_str()};
        _log.set_debug_level();
        number::id _id;
        std::condition_variable _cond_produced;
        std::mutex _mutex_produced;
        msg_handlers _msg_handlers{_id, time{handlers_timeout}};

        std::condition_variable _cond_handled;
        std::mutex _mutex_handled;
        handled_handlers _handled_handlers{_id, std::chrono::seconds(2)};

        DEB(_log, "total messages to be sent: ", m_total_msgs);

        INF(_log, "creating and starting the producers loops");
        std::vector<sleeping_loop> _loops;
        for (uint16_t _i = 0; _i < num_senders; ++_i) {

            _loops.push_back(
                {_id,
                 sender(_i, &_msg_handlers, &_cond_produced, msgs_per_sender),
                 1000ms, time{interval_per_sender}});
            DEB(_log, "starting sender p", _i);
            _loops.back().start();
        }

        INF(_log, "creating handlers for handled messages");
        uint16_t _handled{0};
        bool _all_handled_notified{false};
        _handled_handlers.add_handler([&](type::ptr<bool>, handled &&) -> void {
            if (_all_handled_notified) {
                return;
            }

            ++_handled;

            if (_handled >= m_total_msgs) {
                DEB(_log, "all messages handled, notifying");
                _all_handled_notified = true;
                _cond_handled.notify_one();
                return;
            }
            DEB(_log, _handled, " messages handled");
        });

        INF(_log, "creating the msg handlers");
        std::vector<type::ptr<handler>> _handler_list;
        for (uint16_t _i = 0; _i < num_handlers; ++_i) {
            _handler_list.push_back(std::make_shared<handler>(
                std::string("c" + std::to_string(_i)), &_msg_handlers,
                &_handled_handlers, time{handlers_timeout}, timeout_at_each));
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
            bool _first{true};
            uint16_t _num_notifications{0};
            std::unique_lock<std::mutex> _lock(_mutex_produced);
            _cond_produced.wait(_lock, [&]() -> bool {
                if (_first) {
                    DEB(_log, "first sender notification");
                    _first = false;
                    return false;
                }

                ++_num_notifications;
                if (_num_notifications == num_senders) {
                    DEB(_log, "all senders have notified");
                    return true;
                }
                DEB(_log, _num_notifications, " producers notified, ",
                    num_senders, " expected");
                return false;
            });
        }

        INF(_log, "stoping the senders");
        for (uint16_t _i = 0; _i < num_senders; ++_i) {
            _loops[_i].stop();
        }

        INF(_log, "waiting for all the handlers, until all the messages are "
                  "handled");
        {
            std::unique_lock<std::mutex> _lock(_mutex_handled);
            _cond_handled.wait(_lock, [&]() -> bool {
                if (_handled == m_total_msgs) {
                    DEB(_log, "all ", m_total_msgs, " msgs handled");
                    return true;
                }
                DEB(_log, _handled, " handled msgs so far");
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
            DEB(_log, "handled: ", _handled);
        }

        INF(_log, "TIME: ", _finish - _start, "us - ",
            static_cast<double>(_finish - _start) / 1000, "ms - ",
            static_cast<double>(_finish - _start) / 1000000, "s");

        return (m_total_msgs == _handled);
    }

    static std::string description() {
        std::stringstream _stream;

        _stream << num_senders << " sender(s), sending " << msgs_per_sender
                << " messages each, so " << num_senders * msgs_per_sender
                << " messages will be sent, at " << interval_per_sender
                << "ms.\n"
                << "There are " << num_handlers
                << " handler(s), each with timeout of " << handlers_timeout
                << "ms, and sleeping for " << handlers_timeout / 2
                << "ms, to simulate actual work";
        if (timeout_at_each) {
            _stream << ", and at each " << timeout_at_each
                    << " messages, each handler will cause a timeout";
        }

        _stream << ".\n"
                << "The amount of messages consumed must be equal to the "
                   "produced.";

        return _stream.str();
    }

  private:
    struct handled {
        friend std::ostream &operator<<(std::ostream &p_out, const handled &) {
            p_out << "handled";
            return p_out;
        }
    };

    typedef async::internal::handlers_t<handled> handled_handlers;

    struct handler {
        handler(std::string &&p_id, msg_handlers *p_handlers,
                handled_handlers *p_handled_handlers,
                std::chrono::milliseconds p_timeout,
                uint16_t p_cause_timeout_at_each = 0)
            : m_id(p_id), m_msg_handlers(p_handlers),
              m_handled_handlers(p_handled_handlers), m_timeout(p_timeout),
              m_sleep(m_timeout.count() / 2),
              m_cause_timeout_at_each(p_cause_timeout_at_each),
              m_to_timeout(m_timeout.count() * 2) {}
        void operator()(std::shared_ptr<bool> p_bool, msg &&p_msg) {
            m_log.set_debug_level();
            std::this_thread::sleep_for(m_sleep);
            if (m_cause_timeout_at_each &&
                ((m_num % m_cause_timeout_at_each) == 0)) {
                DEB(m_log, m_id, " causing timeout for msg number ", m_num,
                    " as it a timeout must occurr at each ",
                    m_cause_timeout_at_each, " messages");
                std::this_thread::sleep_for(m_to_timeout);
            }
            if (*p_bool) {
                m_msg_handlers->add_data(p_msg);
                return;
            }
            ++m_num;
            m_msg = std::move(p_msg);
            m_handled_handlers->add_data(handled{});
            DEB(m_log, "handling msg|", m_id, "|", m_msg, "|", m_num);
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
        const time m_sleep;
        const uint16_t m_cause_timeout_at_each;
        const time m_to_timeout;
        uint16_t m_num{0};
        msg m_msg;
        logger::cerr<> m_log{"handler"};
    };

    struct sender {
        sender(uint16_t p_idx, msg_handlers *p_handlers,
               std::condition_variable *p_cond, uint16_t p_num_msg_per_sender)
            : m_idx('p' + std::to_string(p_idx)), m_handlers(p_handlers),
              m_cond(p_cond), m_num_msg_per_sender(p_num_msg_per_sender),
              m_msg(p_idx * m_num_msg_per_sender),
              m_start(p_idx * m_num_msg_per_sender),
              m_finish((p_idx * m_num_msg_per_sender + m_num_msg_per_sender) -
                       1),
              m_log("producer p" + std::to_string(p_idx)) {
            m_log.set_debug_level();
            DEB(m_log, m_idx, ": start value = ", m_start,
                ", finish value = ", m_finish, ", msg start = ", m_msg);
        }

        void operator()(std::shared_ptr<bool>) {
            m_log.set_debug_level();

            if (m_msg.get_value() == (m_finish + 1)) {
                DEB(m_log, m_idx, " produced ", m_num_msg_per_sender,
                    " messages: ", m_start, " -> ", m_finish);
                m_cond->notify_one();
                return;
            }

            DEB(m_log, m_idx, " sending ", m_msg);
            m_handlers->add_data(m_msg);
            ++m_msg;
        }

      private:
        std::string m_idx;
        msg_handlers *m_handlers;
        std::condition_variable *m_cond;
        const uint16_t m_num_msg_per_sender;

        msg m_msg;
        value m_start;
        value m_finish;

        logger::cerr<> m_log;
    };

  private:
    const std::string m_name;
    //    const uint16_t m_num_senders;
    //    const uint16_t m_num_msg_per_sender;
    //    const time m_interval_per_sender;
    //    const uint16_t m_num_handlers;
    //    const time m_handlers_timeout;
    //    const uint16_t m_timeout_at_each;

    uint16_t m_total_msgs;
};

struct handlers_000 {
    typedef msg_t<'A'> data;
    typedef async::internal::handlers_t<data> handlers;

    static std::string desc() {
        return "Simple test, creating a worker, adding a single data, "
               "and starting";
    }

    bool operator()() {
        logger::set_debug_level();

        logger::cerr<> _log{"handlers_000"};
        number::id _id;

        DEB(_log, "starting");

        handlers _handlers(_id, 3s);

        DEB(_log, "adding handler");

        auto _consumer = [&_log](type ::ptr<bool>, data &&p_data) {
            INF(_log, "value = ", p_data);
        };

        _handlers.add_handler(_consumer);

        DEB(_log, "capacity = ", _handlers.capacity(),
            ", occupied = ", _handlers.occupied());

        _handlers.add_data(data(19));

        DEB(_log, "capacity = ", _handlers.capacity(),
            ", occupied = ", _handlers.occupied());

        DEB(_log, "sleeping");
        std::this_thread::sleep_for(1s);
        DEB(_log, "waking up");

        return true;
    }
};

struct handlers_001 {
    typedef test_t<'B' /* msg id */, 1 /* test id */, 1 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   2000 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;

    static std::string desc() { return test::description(); }

    bool operator()() {
        test _test;
        return _test();
    }
};

int main(int argc, char **argv) {
    //  logger::set_debug_level();
    tester::test<> _test(argc, argv);
    run_test(_test, handlers_000);
    run_test(_test, handlers_001);
    //    run_test(_test, handlers_002);
}
