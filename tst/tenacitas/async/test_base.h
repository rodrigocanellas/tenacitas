/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <fstream>
#include <functional>
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
          uint16_t handlers_timeout, uint16_t timeout_at_each,
          uint16_t handler_sleep = handlers_timeout / 2>
struct test_t {

    typedef msg_t<msg_id> msg;
    typedef async::internal::handlers_t<msg> msg_handlers;
    typedef async::sleeping_loop sleeping_loop;
    typedef std::vector<uint16_t> values;
    typedef std::chrono::milliseconds time;
    //    typedef std::vector<time> times;

    test_t() : m_name("handlers_" + number::format(msg_id)) {}

    bool operator()() {

        logger::cerr<> _log{m_name.c_str()};
        _log.set_debug_level();
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

        INF(_log, "total messages to be sent: ", m_total_msgs);

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

        INF(_log, "creating handlers for sent messages");
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
                INF(_log, "all ", _sent, " messages sent; notifying");
                _all_sent_notified = true;
                _cond_sent.notify_one();
                return;
            }
            DEB(_log, _sent, " messages sent");
        });

        INF(_log, "creating the msg handlers");
        std::vector<type::ptr<handler>> _handler_list;
        for (uint16_t _i = 0; _i < num_handlers; ++_i) {
            _handler_list.push_back(std::make_shared<handler>(
                _i, &_msg_handlers, &_handled_handlers, time{handlers_timeout},
                time{handler_sleep}, m_max_msgs_timeout, timeout_at_each));
        }

        uint64_t _start = calendar::now<>::microsecs();

        INF(_log, "adding each handler to the handlers group");
        for (type::ptr<handler> &_handler : _handler_list) {
            _msg_handlers.add_handler(
                [&, _handler](type::ptr<bool> p_bool, msg &&p_msg) -> void {
                    (*_handler)(p_bool, std::move(p_msg));
                });
        }

        INF(_log, "creating and starting the senders");
        std::vector<sleeping_loop> _loops;
        for (uint16_t _i = 0; _i < num_senders; ++_i) {
            _loops.push_back(
                {_id,
                 sender(_i, &_msg_handlers, &_sent_handlers, msgs_per_sender),
                 600ms, time{interval_per_sender}});
            DEB(_log, "starting sender p", _i);
            _loops.back().start();
        }

        INF(_log, "checking if all the messages were sent");
        {
            std::unique_lock<std::mutex> _lock(_mutex_sent);
            _cond_sent.wait(_lock, [&]() -> bool {
                if (_sent >= m_total_msgs) {
                    INF(_log, "all senders have notified");
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
        DEB(_log, "capacity: ", _msg_handlers.capacity(),
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
                DEB(_log, _handled, " handled msgs so far");
                return false;
            });
        }

        uint64_t _finish = calendar::now<>::microsecs();

        INF(_log, "reporting results");
        {
            INF(_log, "amount sent = ", m_total_msgs);
            for (uint16_t _i = 0; _i < num_handlers; ++_i) {
                DEB(_log, *_handler_list[_i]);
            }
            DEB(_log, "handled: ", _handled);
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
                << "ms, and sleeping for " << handler_sleep
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
                std::chrono::milliseconds p_sleep, uint16_t p_num_max_timeouts,
                uint16_t p_cause_timeout_at_each = 0)
            : m_id('h' + std::to_string(p_id)), m_msg_handlers(p_handlers),
              m_handled_handlers(p_handled_handlers), m_timeout(p_timeout),
              m_num_max_timeouts(p_num_max_timeouts), m_sleep(p_sleep),
              m_cause_timeout_at_each(p_cause_timeout_at_each),
              m_to_timeout(m_timeout.count() * 2), m_log("handler_" + m_id) {
            m_log.set_debug_level();
            DEB(m_log, m_id, " # timeouts: ", m_max_msgs_timeout);
        }

        void operator()(std::shared_ptr<bool> p_bool, msg &&p_msg) {
            DEB(m_log, m_id, " incoming ", p_msg, '|', m_num);
            if (m_cause_timeout_at_each && m_num_aux &&
                ((m_num_aux % m_cause_timeout_at_each) == 0) &&
                (m_timeout_counter < m_num_max_timeouts)) {
                ++m_timeout_counter;
                ++m_num_aux;
                DEB(m_log, m_id, " timeouting ", p_msg, '|', m_num_aux);
                std::this_thread::sleep_for(m_to_timeout);
            }
            DEB(m_log, m_id, " continuing to handle ", p_msg,
                ", and p_bool is ", *p_bool);
            if (!p_bool) {
                ERR(m_log, m_id, " p_bool not set for ", p_msg, "!!!");
                throw std::runtime_error("p_bool not set!!");
            }
            if (*p_bool) {
                DEB(m_log, m_id, " adding ", p_msg, '|', m_num);
                m_msg_handlers->add_data(p_msg);
                return;
            }
            DEB(m_log, m_id, " continuing to handle ", p_msg);
            std::this_thread::sleep_for(m_sleep);
            ++m_num;
            ++m_num_aux;
            m_msg = std::move(p_msg);
            m_handled_handlers->add_data(handled{});
            DEB(m_log, m_id, " handling ", m_msg, '|', m_num);
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
        uint16_t m_num_aux{0};
        msg m_msg;
        uint16_t m_timeout_counter{0};
        logger::cerr<> m_log;
    };

    struct sender {
        sender(uint16_t p_idx, msg_handlers *p_handlers,
               sent_handlers *p_sent_handlers, uint16_t p_num_msg_per_sender)
            : m_idx('s' + std::to_string(p_idx)), m_handlers(p_handlers),
              m_sent_handlers(p_sent_handlers),
              m_num_msg_per_sender(p_num_msg_per_sender),
              m_msg(p_idx * m_num_msg_per_sender),
              m_start(p_idx * m_num_msg_per_sender),
              m_finish((p_idx * m_num_msg_per_sender + m_num_msg_per_sender) -
                       1),
              m_log("sender_" + m_idx) {
            m_log.set_debug_level();
            DEB(m_log, m_idx, ": start value = ", m_start,
                ", finish value = ", m_finish);
        }

        void operator()(std::shared_ptr<bool>) {
            m_log.set_debug_level();

            if (m_all_notified) {
                DEB(m_log, m_idx, " already notified");
                return;
            }

            if ((m_msg.get_value() >= (m_finish + 1))) {
                m_all_notified = true;

                DEB(m_log, m_idx, " sent ", m_num_msg_per_sender,
                    " messages: ", m_start, " -> ", m_finish);
                return;
            }

            DEB(m_log, m_idx, " sending ", m_msg);
            m_handlers->add_data(m_msg);
            m_sent_handlers->add_data(sent{});
            ++m_msg;
        }

      private:
        std::string m_idx;
        msg_handlers *m_handlers;
        sent_handlers *m_sent_handlers;
        const uint16_t m_num_msg_per_sender;

        msg m_msg;
        value m_start;
        value m_finish;

        bool m_all_notified{false};

        logger::cerr<> m_log;
    };

  private:
    const std::string m_name;
    //    uint16_t m_total_msgs;

    static constexpr uint16_t m_total_msgs{num_senders * msgs_per_sender};
    static constexpr uint16_t m_max_msgs_timeout{m_total_msgs / num_handlers *
                                                 30 / 100};
};
