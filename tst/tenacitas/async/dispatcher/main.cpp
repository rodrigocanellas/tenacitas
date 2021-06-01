/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <chrono>
#include <cmath>
#include <complex>
#include <condition_variable>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <mutex>
#include <set>
#include <sstream>
#include <string>

#include <tenacitas/async.h>
#include <tenacitas/calendar.h>
#include <tenacitas/logger.h>
#include <tenacitas/number.h>
#include <tenacitas/tester.h>
#include <tenacitas/type.h>
#include <tst/tenacitas/async/event.h>

using namespace tenacitas;
using namespace std::chrono_literals;

template<event_id evt_id>
struct test_t {
    typedef event_t<evt_id> event;

    typedef async::sleeping_loop sleeping_loop;
    typedef std::vector<uint16_t> values;
    typedef std::chrono::milliseconds time;

    struct handler_definition {
        handler_definition(uint16_t p_timeout_at_each) : timeout_at_each(p_timeout_at_each) {}
        uint16_t timeout_at_each;
    };

    typedef std::vector<handler_definition> handlers_definitions;

    struct handling_definition {
        handling_definition(std::chrono::milliseconds p_handling_timeout, std::chrono::milliseconds p_sleep) :
            timeout(p_handling_timeout)

            ,
            sleep(p_sleep) {}

        void add_handler(handler_definition&& p_handler_definition) {
            handlers_defs.push_back(std::move(p_handler_definition));
        }

        std::chrono::milliseconds timeout;
        handlers_definitions handlers_defs;
        std::chrono::milliseconds sleep;
    };

    typedef std::vector<handling_definition> handlings_definitions;

    struct sender_definition {
        uint16_t num_events;
        std::chrono::milliseconds interval;
    };

    typedef std::vector<sender_definition> senders_definitions;

    static void
    set_definitions(handlings_definitions&& p_handlings_definitions, senders_definitions&& p_senders_definitions) {
        m_handlings_definitions = std::move(std::move(p_handlings_definitions));
        m_senders_definitions = std::move(std::move(p_senders_definitions));
    }

    bool operator()() {
        m_log.set_debug_level();

        for (const sender_definition& _sender_def : m_senders_definitions) {
            m_total_sent += _sender_def.num_events;
        }
        m_total_handled = m_total_sent * m_handlings_definitions.size();

        DEB(m_log, "total events to be sent: ", m_total_sent, ", total events to be handled: ", m_total_handled);

        create_handlers_for_work_events();

        creating_handlers_for_handled_events();

        create_handlers_for_sent_events();

        uint64_t _start = calendar::now<>::microsecs();

        create_and_start_senders();

        wait_for_all_senders();

        wait_for_all_the_handlers();

        uint64_t _finish = calendar::now<>::microsecs();

        report_results(_start, _finish);

        return (m_total_handled == m_handled);
    }

    static std::string description() {
        std::stringstream _stream;

        _stream << '\n'
                << "event " << static_cast<uint16_t>(evt_id) << "\n\t" << m_handlings_definitions.size()
                << " handling(s)" << '\n';

        uint16_t _handling_idx {0};
        for (typename handlings_definitions::const_iterator _hs = m_handlings_definitions.begin();
             _hs != m_handlings_definitions.end();
             ++_hs) {
            _stream << "\thandling " << _handling_idx++ << ":\n"
                    << "\t\ttimeout: " << _hs->timeout.count() << "ms\n"
                    << "\t\tsleep of handlers: " << _hs->sleep.count() << "ms\n"
                    << "\t\t# handler(s): " << _hs->handlers_defs.size() << '\n';
            uint16_t _handler_idx {0};
            for (typename handlers_definitions::const_iterator _h = _hs->handlers_defs.begin();
                 _h != _hs->handlers_defs.end();
                 ++_h) {
                _stream << "\t\t\thandler " << _handler_idx;
                if (_h->timeout_at_each) {
                    _stream << " timeout at each " << _h->timeout_at_each << " events ";
                } else {
                    _stream << " with no timeout";
                }
                _stream << '\n';
                ++_handler_idx;
            }
        }

        _stream << "\n\t" << m_senders_definitions.size() << " sender(s)\n";
        uint16_t _sender_idx {0};
        for (typename senders_definitions::const_iterator _ss = m_senders_definitions.begin();
             _ss != m_senders_definitions.end();
             ++_ss) {
            _stream << "\t  sender " << _sender_idx << ": # events: " << _ss->num_events
                    << ", interval: " << _ss->interval.count() << "ms\n";
            ++_sender_idx;
        }

        return _stream.str();
    }

private:
    struct handled {
        friend std::ostream& operator<<(std::ostream& p_out, const handled&) {
            p_out << "'handled'";
            return p_out;
        }
    };

    struct sent {
        friend std::ostream& operator<<(std::ostream& p_out, const sent&) {
            p_out << "'sent'";
            return p_out;
        }
    };

    struct handler {
        handler(
            number::id p_handling_id,
            uint16_t p_id,
            std::chrono::milliseconds p_timeout,
            std::chrono::milliseconds p_sleep,
            uint16_t p_cause_timeout_at_each = 0) :
            m_id('h' + p_handling_id.str() + ':' + std::to_string(p_id)),
            m_timeout(p_timeout),
            m_num_max_timeouts(3),
            m_sleep(p_sleep),
            m_cause_timeout_at_each(p_cause_timeout_at_each),
            m_to_timeout(m_timeout.count() * 2),
            m_log("handler_" + m_id) {
            //            m_log.set_debug_level();
        }

        void operator()(std::shared_ptr<bool> p_bool, event&& p_event) {
            m_log.set_debug_level();
            DEB(m_log, m_id, " incoming ", p_event, '|', m_num);
            if (m_cause_timeout_at_each && m_num_aux && ((m_num_aux % m_cause_timeout_at_each) == 0)
                && (m_timeout_counter < m_num_max_timeouts)) {
                ++m_timeout_counter;
                ++m_num_aux;
                DEB(m_log, m_id, " timeouting ", p_event, '|', m_num_aux);
                std::this_thread::sleep_for(m_to_timeout);
            }
            if (!p_bool) {
                ERR(m_log, m_id, " p_bool not set for ", p_event, "!!!");
                throw std::runtime_error("p_bool not set!!");
            }
            if (*p_bool) {
                DEB(m_log, m_id, "p_bool is ", *p_bool, ", adding ", p_event, '|', m_num);
                async::send<event>(p_event);
                return;
            }
            std::this_thread::sleep_for(m_sleep);
            ++m_num;
            ++m_num_aux;
            m_event = std::move(p_event);
            async::send<handled>(handled {});
            DEB(m_log, m_id, " handling ", m_event, '|', m_num);
        }

        friend std::ostream& operator<<(std::ostream& p_out, const handler& p_handler) {
            p_out << "(handler: " << p_handler.get_id() << ", last event: " << p_handler.m_event
                  << ", total events: " << p_handler.m_num << ')';
            return p_out;
        }

        inline const std::string& get_id() const {
            return m_id;
        }
        //        inline uint16_t get_num() const { return m_num; }

    private:
        std::string m_id;
        const time m_timeout;
        const uint16_t m_num_max_timeouts {0};
        const time m_sleep;
        const uint16_t m_cause_timeout_at_each {0};
        const time m_to_timeout;
        uint16_t m_num {0};
        uint16_t m_num_aux {0};
        event m_event;
        uint16_t m_timeout_counter {0};
        logger::cerr<> m_log;
    };

    struct sender {
        sender(uint16_t p_idx, value p_start, uint16_t p_num_event_per_sender) :
            m_idx('s' + std::to_string(p_idx)),
            m_num_event_per_sender(p_num_event_per_sender),
            m_event(p_start),
            m_start(p_start),
            m_finish(p_start + m_num_event_per_sender - 1),
            m_log("sender_" + m_idx) {
            //            m_log.set_debug_level();
            INF(m_log, m_idx, ": start value = ", m_start, ", finish value = ", m_finish);
        }

        void operator()(std::shared_ptr<bool>) {
            m_log.set_debug_level();

            if (m_all_notified) {
                DEB(m_log, m_idx, " already notified");
                async::send<event>(m_event);
                return;
            }

            if ((m_event.get_value() >= (m_finish + 1))) {
                m_all_notified = true;

                DEB(m_log, m_idx, " sent ", m_num_event_per_sender, " events: ", m_start, " -> ", m_finish);
                async::send<event>(m_event);
                return;
            }

            DEB(m_log, m_idx, " sending ", m_event);
            async::send<event>(m_event);
            async::send<sent>(sent {});
            ++m_event;
        }

    private:
        std::string m_idx;
        const uint16_t m_num_event_per_sender;

        event m_event;
        value m_start;
        value m_finish;

        bool m_all_notified {false};

        logger::cerr<> m_log;
    };

    typedef std::vector<sleeping_loop> loops;
    typedef std::vector<type::ptr<handler>> handlers;

private:
    void create_handlers_for_work_events() {
        INF(m_log, "creating handlers for work events");
        for (const handling_definition& _handling_def : m_handlings_definitions) {
            number::id _handling_id = async::add_handling<event>(_handling_def.timeout);

            uint16_t _handler_id {0};
            for (const handler_definition& _handler_def : _handling_def.handlers_defs) {
                type::ptr<handler> _handler {std::make_shared<handler>(
                    _handling_id,
                    _handler_id++,
                    _handling_def.timeout,
                    _handling_def.sleep,
                    _handler_def.timeout_at_each)};

                m_handlers.push_back(_handler);

                async::add_handler<event>(_handling_id, [_handler](type::ptr<bool> p_bool, event&& p_event) -> void {
                    (*_handler)(p_bool, std::move(p_event));
                });
            }
            std::this_thread::sleep_for(100ms);
        }
    }

    void creating_handlers_for_handled_events() {
        INF(m_log, "creating handlers for handled events");

        m_all_handled_notified = false;

        async::add_handler<handled>(
            [this](type::ptr<bool>, handled&&) -> void {
                if (m_all_handled_notified) {
                    DEB(m_log, "all handled notified");
                    return;
                }
                {
                    std::lock_guard<std::mutex> _lock(m_mutex_counter_handled);
                    ++m_handled;
                }

                if (m_handled >= m_total_handled) {
                    DEB(m_log, "all events handled, notifying");
                    m_all_handled_notified = true;
                    m_cond_handled.notify_one();
                    return;
                }
                INF(m_log, m_handled, " events handled");
            },
            2s);
    }

    void create_handlers_for_sent_events() {
        INF(m_log, "creating handlers for sent events");

        m_all_sent_notified = false;
        DEB(m_log, "m_all_sent_notified = ", m_all_sent_notified ? 'T' : 'F');

        async::add_handler<sent>(
            [this](type::ptr<bool>, sent&&) -> void {
                std::lock_guard<std::mutex> _lock(m_mutex_counter_sent);
                DEB(m_log,
                    "entering with m_sent = ",
                    m_sent,
                    ", and m_all_sent_notified = ",
                    m_all_sent_notified ? 'T' : 'F');
                if (m_all_sent_notified) {
                    DEB(m_log, "m_all_sent_notified = ", m_all_sent_notified ? 'T' : 'F');
                    return;
                }

                DEB(m_log,
                    "before incrementing m_sent is ",
                    m_sent,
                    ", and m_all_sent_notified is ",
                    m_all_sent_notified ? 'T' : 'F');
                ++m_sent;
                DEB(m_log,
                    "after incrementing m_sent now is ",
                    m_sent,
                    ", and m_all_sent_notified is ",
                    m_all_sent_notified ? 'T' : 'F');

                if (m_sent >= m_total_sent) {
                    m_all_sent_notified = true;
                    DEB(m_log,
                        "all ",
                        m_sent,
                        " events sent; notifying and m_all_sent_notified is ",
                        m_all_sent_notified ? 'T' : 'F');
                    m_cond_sent.notify_one();
                    return;
                }
                DEB(m_log, m_sent, " events sent");
            },
            1s);
    }

    void create_and_start_senders() {
        INF(m_log, "creating and starting the senders");

        uint16_t _i {0};
        value _start {0};
        for (const sender_definition& _sender_def : m_senders_definitions) {
            DEB(m_log, "starting sender s", _i);
            m_loops.push_back({m_id, sender {_i++, _start, _sender_def.num_events}, 600ms, _sender_def.interval});
            _start = _start + _sender_def.num_events;
        }

        for (loops::iterator _loop = m_loops.begin(); _loop != m_loops.end(); ++_loop) {
            _loop->start();
        }
    }

    void wait_for_all_senders() {
        m_log.set_debug_level();
        INF(m_log, "checking if all the events were sent");

        std::unique_lock<std::mutex> _lock(m_mutex_sent);
        m_cond_sent.wait(_lock, [&]() -> bool {
            if (m_sent >= m_total_sent) {
                DEB(m_log, "all senders have notified");
                return true;
            }
            DEB(m_log, m_sent, " events sent so far");
            return false;
        });

        stop_senders();
    }

    void stop_senders() {
        DEB(m_log, "stoping the senders");
        for (std::vector<sleeping_loop>::iterator _ite = m_loops.begin(); _ite != m_loops.end(); ++_ite) {
            _ite->stop();
        }
    }

    void wait_for_all_the_handlers() {
        INF(m_log,
            "waiting for all the handlers, until all the events are "
            "handled");
        {
            std::unique_lock<std::mutex> _lock(m_mutex_handled);
            m_cond_handled.wait(_lock, [&]() -> bool {
                if (m_handled >= m_total_handled) {
                    DEB(m_log, "all ", m_total_handled, " events handled");
                    return true;
                }
                DEB(m_log, m_handled, " handled events so far");
                return false;
            });
        }
        //        stop_senders();
    }

    void report_results(uint64_t p_start, uint64_t p_finish) {
        std::stringstream _stream;
        _stream << "RESULTS for event " << static_cast<uint16_t>(evt_id) << ": "
                << "amount to be sent = " << m_total_sent << ", amount sent = " << m_sent
                << ", amount handled = " << m_handled << "; distribution: ";

        for (typename handlers::const_iterator _ite = m_handlers.begin(); _ite != m_handlers.end(); ++_ite) {
            _stream << *(*_ite) << " ";
        }
        double _micro = p_finish - p_start;
        double _milli = _micro / 1000;
        double _sec = _milli / 1000;
        double _min = _sec / 60;
        _stream << "; TIMES: " << _micro << "us, " << _milli << "ms, " << _sec << "s, " << _min << "m";
        INF(m_log, _stream.str());
    }

private:
    uint16_t m_total_sent {0};
    uint16_t m_total_handled {0};

    logger::cerr<> m_log {m_name.c_str()};
    number::id m_id;

    std::mutex m_mutex_counter_handled;
    std::mutex m_mutex_counter_sent;

    std::condition_variable m_cond_sent;
    std::mutex m_mutex_sent;

    std::condition_variable m_cond_handled;
    std::mutex m_mutex_handled;

    loops m_loops;
    handlers m_handlers;

    uint16_t m_sent {0};
    uint16_t m_handled {0};

    bool m_all_handled_notified {false};
    bool m_all_sent_notified {false};

    static std::string m_name;
    static handlings_definitions m_handlings_definitions;
    static senders_definitions m_senders_definitions;
};

template<event_id evt_id>
std::string test_t<evt_id>::m_name {"test_" + number::format(evt_id)};

template<event_id evt_id>
typename test_t<evt_id>::handlings_definitions test_t<evt_id>::m_handlings_definitions;

template<event_id evt_id>
typename test_t<evt_id>::senders_definitions test_t<evt_id>::m_senders_definitions;

// 1 event, 1 handling, 1 handler, no timeout, 1 sender
struct test_dispatcher {
    typedef test_t<0> test_0;
    typedef test_t<1> test_1;
    typedef test_t<2> test_2;
    typedef test_t<3> test_3;

    static std::string desc() {
        set();
        //        return test_0::description() + test_1::description() +
        //               test_2::description() + test_3::description();
        //        return test_0::description();
        return test_1::description();
    }

    bool operator()() {
        //        test_0 _test_0;
        test_1 _test_1;
        //        test_2 _test_2;
        //        test_3 _test_3;

        //        return _test_3() && _test_2() && _test_1() && _test_0();

        //        std::future<bool> _future_0 = std::async(
        //            std::launch::deferred, [&_test_0]() -> bool { return
        //            _test_0(); });

        //        return _future_0.get();

        std::future<bool> _future_1 = std::async(std::launch::deferred, [&_test_1]() -> bool { return _test_1(); });

        return _future_1.get();
    }

private:
    static void set() {
        //        set_0();
        set_1();
        //        set_2();
        //        set_3();
    }

    template<event_id evt_id>
    static typename test_t<evt_id>::senders_definitions set_senders() {
        typedef typename test_t<evt_id>::senders_definitions senders_definitions;
        typedef typename test_t<evt_id>::sender_definition sender_definition;

        senders_definitions _senders_definitions;

        _senders_definitions.push_back(sender_definition {2, 1000ms});
        //        _senders_definitions.push_back(sender_definition {4, 800ms});
        //        _senders_definitions.push_back(sender_definition {6, 600ms});
        //        _senders_definitions.push_back(sender_definition {8, 400ms});
        //        _senders_definitions.push_back(sender_definition {10, 200ms});
        return _senders_definitions;
    }

    static void set_0() {
        // defining handlings
        test_0::handlings_definitions _handlings_definitions;

        test_0::handling_definition _handling_definition {1000ms, 500ms};
        _handling_definition.add_handler({0});
        //        _handling_definition.add_handler({4});
        //        _handling_definition.add_handler({0});
        //        _handling_definition.add_handler({4});

        // adding handling to dispatcher
        _handlings_definitions.push_back(std::move(_handling_definition));

        test_0::set_definitions(std::move(_handlings_definitions), set_senders<0>());
    }

    static void set_1() {
        // defining handlings
        test_1::handlings_definitions _handlings_definitions;

        test_1::handling_definition _handling_definition_0 {1000ms, 500ms};
        _handling_definition_0.add_handler({0});
        //        _handling_definition_0.add_handler({4});
        //        _handling_definition_0.add_handler({0});
        //        _handling_definition_0.add_handler({4});

        test_1::handling_definition _handling_definition_1 {800ms, 400ms};
        _handling_definition_1.add_handler({0});

        // adding handling to dispatcher
        _handlings_definitions.push_back(std::move(_handling_definition_0));
        _handlings_definitions.push_back(std::move(_handling_definition_1));

        test_1::set_definitions(std::move(_handlings_definitions), set_senders<1>());
    }
    static void set_2() {}
    static void set_3() {}
};

int main(int argc, char** argv) {
    logger::set_debug_level();
    tester::test _tester(argc, argv);

    run_test(_tester, test_dispatcher);
}
