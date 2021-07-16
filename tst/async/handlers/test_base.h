/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>

#include <tenacitas.lib/async.h>
#include <tenacitas.lib/logger.h>
#include <tenacitas.lib/number.h>
#include <tenacitas.lib/tester.h>
#include <tenacitas.lib/type.h>
#include <tst/async/event.h>

using namespace tenacitas;
using namespace tenacitas::async;
using namespace std::chrono_literals;

template <event_id evt_id,
          uint16_t num_senders,
          uint16_t events_per_sender,
          uint16_t interval_per_sender,
          uint16_t num_handlers,
          uint16_t handlers_timeout,
          uint16_t timeout_at_each,
          uint16_t handler_sleep = handlers_timeout / 2>
struct test_t {

    typedef event_t<evt_id> event;
    typedef async::internal::handling_t<event> event_handlings;
    typedef async::sleeping_loop_t<> sleeping_loop;
    typedef std::vector<uint16_t> values;
    typedef std::chrono::milliseconds time;
    //    typedef std::vector<time> times;

    test_t()
        : m_name("handlers_" + number::format(evt_id)) {}

    bool operator()() {

        logger::set_debug_level();
        //        logger::set_debug_level();
        number::id _id;

        std::mutex _mutex_counter_handled;
        std::mutex _mutex_counter_sent;

        std::condition_variable _cond_sent;
        std::mutex _mutex_sent;
        sent_handlers _sent_handlers {std::chrono::seconds(2)};

        std::condition_variable _cond_handled;
        std::mutex _mutex_handled;
        handled_handlers _handled_handlers {std::chrono::seconds(2)};

        event_handlings _event_handlings {time {handlers_timeout}};

        INF("total events to be sent: ", m_total_events);

        INF("creating handlers for handled events");
        uint16_t _handled {0};
        bool _all_handled_notified {false};
        _handled_handlers.add_handler([&](type::sptr<bool>, handled &&) -> void {
            if (_all_handled_notified) {
                DEB("all handled notified");
                return;
            }
            {
                std::lock_guard<std::mutex> _lock(_mutex_counter_handled);
                ++_handled;
            }

            if (_handled >= m_total_events) {
                INF("all events handled, notifying");
                _all_handled_notified = true;
                _cond_handled.notify_one();
                return;
            }
            DEB(_handled, " events handled");
        });

        INF("creating handlers for sent events");
        uint16_t _sent {0};
        bool _all_sent_notified {false};
        _sent_handlers.add_handler([&](type::sptr<bool>, sent &&) -> void {
            if (_all_sent_notified) {
                return;
            }

            {
                std::lock_guard<std::mutex> _lock(_mutex_counter_sent);
                ++_sent;
            }

            if (_sent >= m_total_events) {
                INF("all ", _sent, " events sent; notifying");
                _all_sent_notified = true;
                _cond_sent.notify_one();
                return;
            }
            DEB(_sent, " events sent");
        });

        INF("creating the event handlers");
        std::vector<type::sptr<handler>> _handler_list;
        for (uint16_t _i = 0; _i < num_handlers; ++_i) {
            _handler_list.push_back(std::make_shared<handler>(
                _i, &_event_handlings, &_handled_handlers,
                time {handlers_timeout}, time {handler_sleep},
                m_max_events_timeout, timeout_at_each));
        }

        uint64_t _start = calendar::now<>::microsecs();

        INF("adding each handler to the handlers group");
        for (type::sptr<handler> &_handler : _handler_list) {
            _event_handlings.add_handler(
                [&, _handler](type::sptr<bool> p_bool, event &&p_event) -> void {
                    (*_handler)(p_bool, std::move(p_event));
                });
        }

        INF("creating and starting the senders");
        std::vector<sleeping_loop> _loops;
        for (uint16_t _i = 0; _i < num_senders; ++_i) {
            _loops.push_back({sender(_i, &_event_handlings, &_sent_handlers,
                                     events_per_sender),
                              600s, time {interval_per_sender}});
            DEB("starting sender p", _i);
            _loops.back().start();
        }

        INF("checking if all the events were sent");
        {
            std::unique_lock<std::mutex> _lock(_mutex_sent);
            _cond_sent.wait(_lock, [&]() -> bool {
                if (_sent >= m_total_events) {
                    INF("all senders have notified");
                    return true;
                }
                DEB(_sent, " events sent so far");
                return false;
            });
        }

        INF("stoping the senders");
        for (uint16_t _i = 0; _i < num_senders; ++_i) {
            _loops[_i].stop();
        }
        DEB("capacity: ", _event_handlings.capacity(),
            ", added: ", _event_handlings.amount_added(),
            ", occupied: ", _event_handlings.occupied());

        INF("waiting for all the handlers, until all the events are "
            "handled");
        {
            std::unique_lock<std::mutex> _lock(_mutex_handled);
            _cond_handled.wait(_lock, [&]() -> bool {
                if (_handled >= m_total_events) {
                    INF("all ", m_total_events, " events handled");
                    return true;
                }
                DEB(_handled, " handled events so far");
                return false;
            });
        }

        uint64_t _finish = calendar::now<>::microsecs();

        INF("reporting results");
        {
            INF("amount sent = ", m_total_events);
            for (uint16_t _i = 0; _i < num_handlers; ++_i) {
                DEB(*_handler_list[_i]);
            }
            DEB("handled: ", _handled);
        }

        {
            double _micro = _finish - _start;
            double _milli = _micro / 1000;
            double _sec = _milli / 1000;
            double _min = _sec / 60;
            INF("TIMES: ", _micro, "us, ", _milli, "ms, ", _sec, "s, ", _min,
                "m");
            std::ofstream _file("stats_test_handlers.csv", std::ios::app);
            _file << static_cast<uint16_t>(evt_id) << ',' << num_handlers << ','
                  << (timeout_at_each ? 's' : 'n') << ',' << num_senders << ','
                  << events_per_sender << ',' << m_total_events << ',' << _micro
                  << ',' << _milli << ',' << _sec << ',' << _min << std::endl;
        }

        return (m_total_events == _handled);
    }

    static std::string description() {
        std::stringstream _stream;

        _stream << num_senders << " sender(s), sending " << events_per_sender
                << " events each, so " << m_total_events
                << " events will be sent, at " << interval_per_sender << "ms.\n"
                << "There are " << num_handlers
                << " handler(s), each with timeout of " << handlers_timeout
                << "ms, and sleeping for " << handler_sleep
                << "ms, to simulate actual work";
        if (timeout_at_each) {
            _stream
                << ", and at each " << timeout_at_each
                << " events, each handler will cause a timeout, for at most "
                << m_max_events_timeout << " times";
        }

        _stream << ".\n"
                << "The amount of events consumed must be equal to the "
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

    typedef async::internal::handling_t<handled> handled_handlers;
    typedef async::internal::handling_t<sent> sent_handlers;

    struct handler {
        handler(uint16_t p_id,
                event_handlings *p_handlers,
                handled_handlers *p_handled_handlers,
                std::chrono::milliseconds p_timeout,
                std::chrono::milliseconds p_sleep,
                uint16_t p_num_max_timeouts,
                uint16_t p_cause_timeout_at_each = 0)
            : m_id('h' + std::to_string(p_id))
            , m_event_handlings(p_handlers)
            , m_handled_handlers(p_handled_handlers)
            , m_timeout(p_timeout)
            , m_num_max_timeouts(p_num_max_timeouts)
            , m_sleep(p_sleep)
            , m_cause_timeout_at_each(p_cause_timeout_at_each)
            , m_to_timeout(m_timeout.count() * 2) {

            DEB(m_id, " # timeouts: ", m_max_events_timeout);
        }

        void operator()(std::shared_ptr<bool> p_bool, event &&p_event) {
            DEB(m_id, " incoming ", p_event, '|', m_num);
            if (m_cause_timeout_at_each && m_num_aux &&
                ((m_num_aux % m_cause_timeout_at_each) == 0) &&
                (m_timeout_counter < m_num_max_timeouts)) {
                ++m_timeout_counter;
                ++m_num_aux;
                DEB(m_id, " timeouting ", p_event, '|', m_num_aux);
                std::this_thread::sleep_for(m_to_timeout);
            }
            DEB(m_id, " continuing to handle ", p_event, ", and p_bool is ",
                *p_bool);
            if (!p_bool) {
                ERR(m_id, " p_bool not set for ", p_event, "!!!");
                throw std::runtime_error("p_bool not set!!");
            }
            if (*p_bool) {
                DEB(m_id, " adding ", p_event, '|', m_num);
                m_event_handlings->add_event(p_event);
                return;
            }
            DEB(m_id, " continuing to handle ", p_event);
            std::this_thread::sleep_for(m_sleep);
            ++m_num;
            ++m_num_aux;
            m_event = std::move(p_event);
            m_handled_handlers->add_event(handled {});
            DEB(m_id, " handling ", m_event, '|', m_num);
        }

        friend std::ostream &operator<<(std::ostream &p_out,
                                        const handler &p_handler) {
            p_out << '(' << p_handler.get_id() << ',' << p_handler.m_event
                  << ',' << p_handler.m_num << ')';
            return p_out;
        }

        inline const std::string &get_id() const { return m_id; }
        //        inline uint16_t get_num() const { return m_num; }

    private:
        std::string m_id;
        event_handlings *m_event_handlings;
        handled_handlers *m_handled_handlers;
        const time m_timeout;
        const uint16_t m_num_max_timeouts {0};
        const time m_sleep;
        const uint16_t m_cause_timeout_at_each {0};
        const time m_to_timeout;
        uint16_t m_num {0};
        uint16_t m_num_aux {0};
        event m_event;
        uint16_t m_timeout_counter {0};
    };

    struct sender {
        sender(uint16_t p_idx,
               event_handlings *p_handlers,
               sent_handlers *p_sent_handlers,
               uint16_t p_num_event_per_sender)
            : m_idx('s' + std::to_string(p_idx))
            , m_handlers(p_handlers)
            , m_sent_handlers(p_sent_handlers)
            , m_num_event_per_sender(p_num_event_per_sender)
            , m_event(p_idx * m_num_event_per_sender)
            , m_start(p_idx * m_num_event_per_sender)
            , m_finish(
                  (p_idx * m_num_event_per_sender + m_num_event_per_sender) -
                  1) {

            DEB(m_idx, ": start value = ", m_start,
                ", finish value = ", m_finish);
        }

        void operator()(std::shared_ptr<bool>) {

            if (m_all_notified) {
                DEB(m_idx, " already notified");
                return;
            }

            if ((m_event.get_value() >= (m_finish + 1))) {
                m_all_notified = true;

                DEB(m_idx, " sent ", m_num_event_per_sender,
                    " events: ", m_start, " -> ", m_finish);
                return;
            }

            DEB(m_idx, " sending ", m_event);
            m_handlers->add_event(m_event);
            m_sent_handlers->add_event(sent {});
            ++m_event;
        }

    private:
        std::string m_idx;
        event_handlings *m_handlers;
        sent_handlers *m_sent_handlers;
        const uint16_t m_num_event_per_sender;

        event m_event;
        value m_start;
        value m_finish;

        bool m_all_notified {false};
    };

private:
    const std::string m_name;
    //    uint16_t m_total_events;

    static constexpr uint16_t m_total_events {num_senders * events_per_sender};
    static constexpr uint16_t m_max_events_timeout {m_total_events /
                                                    num_handlers * 30 / 100};
};
