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
#include <tst/tenacitas/async/msg.h>

using namespace tenacitas;
using namespace std::chrono_literals;

template <message_id msg_id>
struct test_t {
    typedef msg_t<msg_id> msg;

    typedef async::sleeping_loop sleeping_loop;
    typedef std::vector<uint16_t> values;
    typedef std::chrono::milliseconds time;

    struct handler_definition {
        handler_definition(uint16_t p_timeout_at_each)
            : timeout_at_each(p_timeout_at_each) {}
        uint16_t timeout_at_each;
    };

    typedef std::vector<handler_definition> handlers_definitions;

    struct handling_definition {
        handling_definition(std::chrono::milliseconds p_handling_timeout,

                            std::chrono::milliseconds p_sleep)
            : timeout(p_handling_timeout)

            , sleep(p_sleep) {}

        void add_handler(handler_definition &&p_handler_definition) {
            handlers_defs.push_back(std::move(p_handler_definition));
        }

        std::chrono::milliseconds timeout;
        handlers_definitions handlers_defs;
        std::chrono::milliseconds sleep;
    };

    typedef std::vector<handling_definition> handlings_definitions;

    struct sender_definition {
        uint16_t num_msgs;
        std::chrono::milliseconds interval;
    };

    typedef std::vector<sender_definition> senders_definitions;

    static void set_definitions(handlings_definitions &&p_handlings_definitions,
                                senders_definitions &&p_senders_definitions) {
        m_handlings_definitions = std::move(std::move(p_handlings_definitions));
        m_senders_definitions = std::move(std::move(p_senders_definitions));
    }

    bool operator()() {

        //        m_log.set_debug_level();

        for (const sender_definition &_sender_def : m_senders_definitions) {
            m_total_msgs += _sender_def.num_msgs;
        }

        DEB(m_log, "total messages to be sent: ", m_total_msgs);

        create_handlers_for_work_messages();

        creating_handlers_for_handled_messages();

        create_handlers_for_sent_messages();

        uint64_t _start = calendar::now<>::microsecs();

        create_and_start_senders();

        wait_for_all_senders();

        wait_for_all_the_handlers();

        uint64_t _finish = calendar::now<>::microsecs();

        report_results(_start, _finish);

        return (m_total_msgs == m_handled);
    }

    static std::string description() {
        std::stringstream _stream;

        _stream << '\n'
                << "msg " << static_cast<uint16_t>(msg_id) << "\n\t"
                << m_handlings_definitions.size() << " handling(s)" << '\n';

        uint16_t _handling_idx {0};
        for (typename handlings_definitions::const_iterator _hs =
                 m_handlings_definitions.begin();
             _hs != m_handlings_definitions.end(); ++_hs) {
            _stream << "\thandling " << _handling_idx++ << ":\n"
                    << "\t\ttimeout: " << _hs->timeout.count() << "ms\n"
                    << "\t\tsleep of handlers: " << _hs->sleep.count() << "ms\n"
                    << "\t\t# handler(s): " << _hs->handlers_defs.size()
                    << '\n';
            uint16_t _handler_idx {0};
            for (typename handlers_definitions::const_iterator _h =
                     _hs->handlers_defs.begin();
                 _h != _hs->handlers_defs.end(); ++_h) {
                _stream << "\t\t\thandler " << _handler_idx;
                if (_h->timeout_at_each) {
                    _stream << " timeout at each " << _h->timeout_at_each
                            << " messages ";
                } else {
                    _stream << " with no timeout";
                }
                _stream << '\n';
                ++_handler_idx;
            }
        }

        _stream << "\n\t" << m_senders_definitions.size() << " sender(s)\n";
        uint16_t _sender_idx {0};
        for (typename senders_definitions::const_iterator _ss =
                 m_senders_definitions.begin();
             _ss != m_senders_definitions.end(); ++_ss) {
            _stream << "\t  sender " << _sender_idx
                    << ": # msgs: " << _ss->num_msgs
                    << ", interval: " << _ss->interval.count() << "ms\n";
            ++_sender_idx;
        }

        return _stream.str();
    }

private:
    struct handled {
        friend std::ostream &operator<<(std::ostream &p_out, const handled &) {
            p_out << "'handled'";
            return p_out;
        }
    };

    struct sent {
        friend std::ostream &operator<<(std::ostream &p_out, const sent &) {
            p_out << "'sent'";
            return p_out;
        }
    };

    struct handler {
        handler(uint16_t p_id,
                std::chrono::milliseconds p_timeout,
                std::chrono::milliseconds p_sleep,
                uint16_t p_cause_timeout_at_each = 0)
            : m_id('h' + std::to_string(p_id))
            , m_timeout(p_timeout)
            , m_num_max_timeouts(3)
            , m_sleep(p_sleep)
            , m_cause_timeout_at_each(p_cause_timeout_at_each)
            , m_to_timeout(m_timeout.count() * 2)
            , m_log("handler_" + m_id) {
            //            m_log.set_debug_level();
        }

        void operator()(std::shared_ptr<bool> p_bool, msg &&p_msg) {
            //            m_log.set_debug_level();
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
                async::send<msg>(p_msg);
                return;
            }
            DEB(m_log, m_id, " continuing to handle ", p_msg);
            std::this_thread::sleep_for(m_sleep);
            ++m_num;
            ++m_num_aux;
            m_msg = std::move(p_msg);
            async::send<handled>(handled {});
            DEB(m_log, m_id, " handling ", m_msg, '|', m_num);
        }

        friend std::ostream &operator<<(std::ostream &p_out,
                                        const handler &p_handler) {
            p_out << "(handler: " << p_handler.get_id()
                  << ", last msg: " << p_handler.m_msg
                  << ", total msgs: " << p_handler.m_num << ')';
            return p_out;
        }

        inline const std::string &get_id() const { return m_id; }
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
        msg m_msg;
        uint16_t m_timeout_counter {0};
        logger::cerr<> m_log;
    };

    struct sender {
        sender(uint16_t p_idx, uint16_t p_num_msg_per_sender)
            : m_idx('s' + std::to_string(p_idx))
            , m_num_msg_per_sender(p_num_msg_per_sender)
            , m_msg(p_idx * m_num_msg_per_sender)
            , m_start(p_idx * m_num_msg_per_sender)
            , m_finish((p_idx * m_num_msg_per_sender + m_num_msg_per_sender) -
                       1)
            , m_log("sender_" + m_idx) {
            //            m_log.set_debug_level();
            INF(m_log, m_idx, ": start value = ", m_start,
                ", finish value = ", m_finish);
        }

        void operator()(std::shared_ptr<bool>) {
            //            m_log.set_debug_level();

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
            async::send<msg>(m_msg);
            async::send<sent>(sent {});
            ++m_msg;
        }

    private:
        std::string m_idx;
        const uint16_t m_num_msg_per_sender;

        msg m_msg;
        value m_start;
        value m_finish;

        bool m_all_notified {false};

        logger::cerr<> m_log;
    };

    typedef std::vector<sleeping_loop> loops;
    typedef std::vector<type::ptr<handler>> handlers;

private:
    void create_handlers_for_work_messages() {
        INF(m_log, "creating handlers for work messages");
        for (const handling_definition &_handling_def :
             m_handlings_definitions) {
            number::id _handling_id =
                async::add_handling<msg>(_handling_def.timeout);

            uint16_t _handler_id {0};
            for (const handler_definition &_handler_def :
                 _handling_def.handlers_defs) {

                type::ptr<handler> _handler {std::make_shared<handler>(
                    _handler_id++, _handling_def.timeout, _handling_def.sleep,
                    _handler_def.timeout_at_each)};

                m_handlers.push_back(_handler);

                async::add_handler<msg>(
                    _handling_id,
                    [&, _handler](type::ptr<bool> p_bool, msg &&p_msg) -> void {
                        (*_handler)(p_bool, std::move(p_msg));
                    });
            }
        }
    }

    void creating_handlers_for_handled_messages() {
        INF(m_log, "creating handlers for handled messages");
        bool _all_handled_notified {false};
        async::add_handler<handled>(
            [&](type::ptr<bool>, handled &&) -> void {
                if (_all_handled_notified) {
                    DEB(m_log, "all handled notified");
                    return;
                }
                {
                    std::lock_guard<std::mutex> _lock(m_mutex_counter_handled);
                    ++m_handled;
                }

                if (m_handled >= m_total_msgs) {
                    DEB(m_log, "all messages handled, notifying");
                    _all_handled_notified = true;
                    m_cond_handled.notify_one();
                    return;
                }
                DEB(m_log, m_handled, " messages handled");
            },
            2s);
    }

    void create_handlers_for_sent_messages() {
        INF(m_log, "creating handlers for sent messages");

        bool _all_sent_notified {false};
        async::add_handler<sent>(
            [&](type::ptr<bool>, sent &&) -> void {
                if (_all_sent_notified) {
                    return;
                }

                {
                    std::lock_guard<std::mutex> _lock(m_mutex_counter_sent);
                    ++m_sent;
                }

                if (m_sent >= m_total_msgs) {
                    DEB(m_log, "all ", m_sent, " messages sent; notifying");
                    _all_sent_notified = true;
                    m_cond_sent.notify_one();
                    return;
                }
                DEB(m_log, m_sent, " messages sent");
            },
            1s);
    }

    void create_and_start_senders() {
        INF(m_log, "creating and starting the senders");

        uint16_t _i {0};
        for (const sender_definition &_sender_def : m_senders_definitions) {
            DEB(m_log, "starting sender p", _i);
            m_loops.push_back({m_id, sender {_i++, _sender_def.num_msgs}, 600ms,
                               _sender_def.interval});

            m_loops.back().start();
        }
    }

    void wait_for_all_senders() {
        INF(m_log, "checking if all the messages were sent");

        std::unique_lock<std::mutex> _lock(m_mutex_sent);
        m_cond_sent.wait(_lock, [&]() -> bool {
            if (m_sent >= m_total_msgs) {
                DEB(m_log, "all senders have notified");
                return true;
            }
            DEB(m_log, m_sent, " msgs sent so far");
            return false;
        });

        DEB(m_log, "stoping the senders");
        for (std::vector<sleeping_loop>::iterator _ite = m_loops.begin();
             _ite != m_loops.end(); ++_ite) {
            _ite->stop();
        }
    }

    void wait_for_all_the_handlers() {

        INF(m_log, "waiting for all the handlers, until all the messages are "
                   "handled");
        {
            std::unique_lock<std::mutex> _lock(m_mutex_handled);
            m_cond_handled.wait(_lock, [&]() -> bool {
                if (m_handled >= m_total_msgs) {
                    DEB(m_log, "all ", m_total_msgs, " msgs handled");
                    return true;
                }
                DEB(m_log, m_handled, " handled msgs so far");
                return false;
            });
        }
    }

    void report_results(uint64_t p_start, uint64_t p_finish) {
        std::stringstream _stream;
        _stream << "\nRESULTS for msg " << static_cast<uint16_t>(msg_id)
                << ": \n"
                << "\tamount to be sent = " << m_total_msgs << '\n'
                << "\tamount sent = " << m_sent << '\n'
                << "\tamount handled =  " << m_handled << '\n'
                << "\tdistribution: \n";

        for (typename handlers::const_iterator _ite = m_handlers.begin();
             _ite != m_handlers.end(); ++_ite) {
            _stream << "\t\t" << *(*_ite) << '\n';
        }
        double _micro = p_finish - p_start;
        double _milli = _micro / 1000;
        double _sec = _milli / 1000;
        double _min = _sec / 60;
        _stream << "\tTIMES: " << _micro << "us, " << _milli << "ms, " << _sec
                << "s, " << _min << "m";
        INF(m_log, _stream.str());
    }

private:
    uint16_t m_total_msgs {0};

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

    static std::string m_name;
    static handlings_definitions m_handlings_definitions;
    static senders_definitions m_senders_definitions;
};

template <message_id msg_id>
std::string test_t<msg_id>::m_name {"test_" + number::format(msg_id)};

template <message_id msg_id>
typename test_t<msg_id>::handlings_definitions
    test_t<msg_id>::m_handlings_definitions;

template <message_id msg_id>
typename test_t<msg_id>::senders_definitions
    test_t<msg_id>::m_senders_definitions;

struct test_000 {
    typedef test_t<100> test;
    static std::string desc() {
        set();
        return test::description();
    }

    bool operator()() {

        test _test;

        return _test();
    }

private:
    static void set() {
        test::handlings_definitions _handlings_definitions;
        test::senders_definitions _senders_definitions;

        // defining handling
        test::handling_definition _handling_definition {300ms, 100ms};
        _handling_definition.add_handler({0});

        // adding handling to messenger
        _handlings_definitions.push_back(std::move(_handling_definition));

        // defining senders
        _senders_definitions.push_back({5, 100ms});

        test::set_definitions(std::move(_handlings_definitions),
                              std::move(_senders_definitions));
    }
};

struct test_001 {
    typedef test_t<101> test;

    static std::string desc() {
        set();
        return test::description();
    }

    bool operator()() {
        test _test;

        return _test();
    }

private:
    static void set() {

        test::handlings_definitions _handlings_definitions;
        test::senders_definitions _senders_definitions;

        // defining handling
        test::handling_definition _handling_definition {300ms, 100ms};
        _handling_definition.add_handler({0});
        _handling_definition.add_handler({0});

        // adding handling to messenger
        _handlings_definitions.push_back(std::move(_handling_definition));

        // defining senders
        _senders_definitions.push_back({5, 100ms});

        test::set_definitions(std::move(_handlings_definitions),
                              std::move(_senders_definitions));
    }
};

struct test_002 {
    typedef test_t<102> test_2;
    typedef test_t<103> test_3;

    static std::string desc() {
        set();
        return test_2::description() + "\n\n" + test_3::description();
    }

    bool operator()() {
        test_2 _test_2;
        test_3 _test_3;

        return _test_2() && _test_3();
    }

private:
    static void set() {

        {
            test_2::handlings_definitions _handlings_definitions;
            test_2::senders_definitions _senders_definitions;

            // defining handling
            test_2::handling_definition _handling_definition {300ms, 100ms};
            _handling_definition.add_handler({0});
            _handling_definition.add_handler({0});

            // adding handling to messenger
            _handlings_definitions.push_back(std::move(_handling_definition));

            // defining senders
            _senders_definitions.push_back({5, 100ms});

            test_2::set_definitions(std::move(_handlings_definitions),
                                    std::move(_senders_definitions));
        }

        {
            test_3::handlings_definitions _handlings_definitions;
            test_3::senders_definitions _senders_definitions;

            // defining handling
            test_3::handling_definition _handling_definition {300ms, 250ms};
            _handling_definition.add_handler({0});

            // adding handling to messenger
            _handlings_definitions.push_back(std::move(_handling_definition));

            // defining senders
            _senders_definitions.push_back({5, 100ms});

            test_3::set_definitions(std::move(_handlings_definitions),
                                    std::move(_senders_definitions));
        }
    }
};

int main(int argc, char **argv) {
    logger::set_info_level();
    tester::test _tester(argc, argv);

    run_test(_tester, test_000);
    run_test(_tester, test_001);
    run_test(_tester, test_002);
}
