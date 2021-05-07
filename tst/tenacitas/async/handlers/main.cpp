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

template <message_id msg_id> struct test_t {

    typedef msg_t<msg_id> msg;
    typedef async::internal::handlers_t<msg> msg_handlers;
    typedef async::sleeping_loop sleeping_loop;
    typedef std::vector<uint16_t> values;
    typedef std::chrono::milliseconds time;
    typedef std::vector<time> times;

    test_t() = delete;

    test_t(std::string &&p_name, const values &p_max_msg_per_sender,
           const times &p_interval_per_sender, uint16_t p_num_handlers,
           time p_handlers_timeout, uint16_t p_timeout_at_each = 0)
        : m_name(std::move(p_name)), m_max_msg_per_sender(p_max_msg_per_sender),
          m_interval_per_sender(p_interval_per_sender),
          m_num_handlers(p_num_handlers),
          m_handlers_timeout(p_handlers_timeout),

          m_timeout_at_each(p_timeout_at_each), m_num_senders(0),
          m_total_msgs(0) {

        if (m_max_msg_per_sender.size() != m_interval_per_sender.size()) {
            throw std::runtime_error(
                "num max msgs per sender != num interval per sender");
        }

        m_num_senders = m_interval_per_sender.size();

        for (uint16_t _i = 0; _i < m_num_senders; ++_i) {
            m_total_msgs += m_max_msg_per_sender[_i];
        }
    }

    std::string desc() {
        std::stringstream _stream;

        _stream << m_num_senders << " senders:\n";
        for (uint16_t _i = 0; _i < m_num_handlers; ++_i) {
            _stream << "\t(#" << number::format(_i) << " : "
                    << number::format(m_max_msg_per_sender[_i]) << "msgs : "
                    << number::format(m_interval_per_sender[_i].count())
                    << "ms)\n";
        }
        _stream << m_num_handlers << " handlers, with timeout of "
                << m_handlers_timeout.count() << "ms, sleeping for "
                << m_handlers_timeout.count() / 2 << "ms";

        if (m_timeout_at_each) {
            _stream << ", timeout at each " << m_timeout_at_each << " messages";
        }

        _stream << std::endl;

        return _stream.str();
    }

    bool operator()() {

        logger::cerr<> _log{"handlers_004"};
        //        logger::set_debug_level();
        _log.set_debug_level();
        number::id _id;
        std::condition_variable _cond_produced;
        std::mutex _mutex_produced;
        msg_handlers _msg_handlers{_id, m_handlers_timeout};

        std::condition_variable _cond_handled;
        std::mutex _mutex_handled;
        handled_handlers _handled_handlers{_id, std::chrono::seconds(2)};

        DEB(_log, "total messages to be sent: ", m_total_msgs);

        INF(_log, "creating and starting the producers loops");
        std::vector<sleeping_loop> _loops;
        for (uint16_t _i = 0; _i < m_num_senders; ++_i) {
            _loops.push_back({_id,
                              sender{_i, &_msg_handlers, &_cond_produced,
                                     m_max_msg_per_sender[_i]},
                              1000ms, m_interval_per_sender[_i]});
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
        for (uint16_t _i = 0; _i < m_num_handlers; ++_i) {
            _handler_list.push_back(std::make_shared<handler>(
                std::string("c" + std::to_string(_i)), &_msg_handlers,

                &_handled_handlers, m_handlers_timeout, m_timeout_at_each));
        }

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
                if (_num_notifications == m_num_senders) {
                    DEB(_log, "all senders have notified");
                    return true;
                }
                DEB(_log, _num_notifications, " producers notified, ",
                    m_num_senders, " expected");
                return false;
            });
        }

        INF(_log, "stoping the senders");
        for (uint16_t _i = 0; _i < m_num_senders; ++_i) {
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

        INF(_log, "reporting results");
        {
            DEB(_log, "amount sent = ", m_total_msgs);
            for (uint16_t _i = 0; _i < m_num_handlers; ++_i) {
                DEB(_log, *_handler_list[_i]);
            }
            DEB(_log, "handled: ", _handled);
        }

        return (m_total_msgs == _handled);
    }

    static std::string description(const value &p_max, uint16_t p_interval,
                                   uint16_t p_handlers_timeout,
                                   uint16_t p_num_handlers,
                                   uint16_t p_handler_sleep,
                                   uint16_t p_timeout_at_each) {
        std::stringstream _stream;

        _stream << "A sender sends " << p_max << " messages, at each "
                << p_interval << "ms, to a 'handlers' with timeout of "
                << p_handlers_timeout << "ms, with " << p_num_handlers
                << " handler(s), each one sleeps for " << p_handler_sleep
                << "ms.";
        if (p_timeout_at_each) {
            _stream << " At each " << p_timeout_at_each
                    << " messages, each handler will cause a timeout.";
        }

        _stream
            << "\n"
            << "The amount of messages consumed must be equal to the produced";

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
               std::condition_variable *p_cond, uint16_t p_max_per_sender)
            : m_idx('p' + std::to_string(p_idx)), m_handlers(p_handlers),
              m_cond(p_cond), m_max_per_sender(p_max_per_sender),
              m_msg(p_idx * m_max_per_sender),
              m_start(p_idx * m_max_per_sender),
              m_finish((p_idx * m_max_per_sender + m_max_per_sender) - 1),
              m_log("producer p" + std::to_string(p_idx)) {
            m_log.set_debug_level();
            DEB(m_log, m_idx, ": start value = ", m_start,
                ", finish value = ", m_finish, ", msg start = ", m_msg);
        }

        void operator()(std::shared_ptr<bool>) {
            m_log.set_debug_level();
            DEB(m_log, m_idx, " current ", m_msg);

            if (m_msg.get_value() == (m_finish + 1)) {
                DEB(m_log, m_idx, " produced ", m_max_per_sender,
                    " messages: ", m_start, " -> ", m_finish);
                m_cond->notify_one();
                return;
            }

            m_handlers->add_data(m_msg);
            ++m_msg;
            DEB(m_log, m_idx, " next msg to be added ", m_msg);
        }

      private:
        std::string m_idx;
        msg_handlers *m_handlers;
        std::condition_variable *m_cond;
        const uint16_t m_max_per_sender;

        msg m_msg;
        value m_start;
        value m_finish;

        logger::cerr<> m_log;
    };

  private:
    const std::string m_name;
    const values m_max_msg_per_sender;
    const times m_interval_per_sender;
    const uint16_t m_num_handlers;
    const time m_handlers_timeout;
    const uint16_t m_timeout_at_each;

    uint16_t m_num_senders;
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

template <message_id msg_id>
static std::string
description(const typename test_t<msg_id>::values &p_max_msg_per_sender,
            const typename test_t<msg_id>::times &p_interval_per_sender,
            uint16_t p_num_handlers,
            typename test_t<msg_id>::time p_handlers_timeout,
            uint16_t p_timeout_at_each) {

    std::stringstream _stream;

    _stream << "\n" << p_max_msg_per_sender.size() << " sender(s):\n";
    _stream << "     # | num msgs |  interval (ms)\n"
            << "---------------------------------\n";
    for (uint16_t _i = 0; _i < p_max_msg_per_sender.size(); ++_i) {
        _stream << number::format<uint16_t>(_i + 1, ' ') << "  |"
                << number::format(p_max_msg_per_sender[_i], ' ') << "     |"
                << number::format<uint16_t>(p_interval_per_sender[_i].count(),
                                            ' ')
                << "\n";
    }

    uint16_t _total_msgs{0};
    {
        uint16_t _num_senders = p_interval_per_sender.size();
        for (uint16_t _i = 0; _i < _num_senders; ++_i) {
            _total_msgs += p_max_msg_per_sender[_i];
        }
    }

    _stream << "\n"
            << p_num_handlers << " handler(s), with timeout of "
            << p_handlers_timeout.count() << "ms, sleeping for "
            << p_handlers_timeout.count() / 2 << "ms, handling " << _total_msgs
            << " messages";

    if (p_timeout_at_each) {
        _stream << ", timeout at each " << p_timeout_at_each << " messages";
    }

    _stream << std::endl;

    return _stream.str();
}

struct handlers_001 {
    typedef msg_t<'B'> msg;
    typedef async::sleeping_loop sleeping_loop;
    typedef async::internal::handlers_t<msg> handlers;
    typedef test_t<msg::id> test;

    static std::string desc() {
        return description<msg::id>(m_max_msg_per_sender, m_interval_per_sender,
                                    m_num_handlers, m_handlers_timeout,
                                    m_timeout_at_each);
    }

    bool operator()() {

        test _test{"handlers_001", m_max_msg_per_sender, m_interval_per_sender,
                   m_num_handlers, m_handlers_timeout,   m_timeout_at_each};
        return _test();
    }

  private:
    static const test::values m_max_msg_per_sender;
    static const test::times m_interval_per_sender;
    static const uint16_t m_num_handlers{1};
    static const test::time m_handlers_timeout;
    static const uint16_t m_timeout_at_each{0};
};

const handlers_001::test::values handlers_001::m_max_msg_per_sender{value{10}};
const handlers_001::test::times handlers_001::m_interval_per_sender{
    handlers_001::test::time{150}};
const handlers_001::test::time handlers_001::m_handlers_timeout{2000};

struct handlers_002 {
    typedef msg_t<'B'> msg;
    typedef async::sleeping_loop sleeping_loop;
    typedef async::internal::handlers_t<msg> handlers;
    typedef test_t<msg::id> test;

    static std::string desc() {
        return description<msg::id>(m_max_msg_per_sender, m_interval_per_sender,
                                    m_num_handlers, m_handlers_timeout,
                                    m_timeout_at_each);
    }

    bool operator()() {

        test _test{"handlers_002", m_max_msg_per_sender, m_interval_per_sender,
                   m_num_handlers, m_handlers_timeout,   m_timeout_at_each};
        return _test();
    }

  private:
    static const test::values m_max_msg_per_sender;
    static const test::times m_interval_per_sender;
    static const uint16_t m_num_handlers{1};
    static const test::time m_handlers_timeout;
    static const uint16_t m_timeout_at_each{3};
};

const handlers_002::test::values handlers_002::m_max_msg_per_sender{value{15}};
const handlers_002::test::times handlers_002::m_interval_per_sender{
    handlers_002::test::time{150}};
const handlers_002::test::time handlers_002::m_handlers_timeout{2000};

int main(int argc, char **argv) {
    //  logger::set_debug_level();
    tester::test<> _test(argc, argv);
    run_test(_test, handlers_000);
    run_test(_test, handlers_001);
    run_test(_test, handlers_002);
}
