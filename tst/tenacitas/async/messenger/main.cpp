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

struct handler_definition {
    uint16_t num_handlers;
    std::chrono::milliseconds sleep;
    uint16_t timeout_at_each;
};

typedef std::vector<handler_definition> handlers_definitions;

struct handling_definition {
    handling_definition(
        std::chrono::milliseconds p_handling_timeout,
        std::vector<handler_definition> &&p_handlers_definitions)
        : timeout(p_handling_timeout)
        , handlers_defs(std::move(p_handlers_definitions)) {}

    std::chrono::milliseconds timeout;
    handlers_definitions handlers_defs;
};

typedef std::vector<handling_definition> handlings_definitions;

struct sender_definition {
    uint16_t num_msgs;
    std::chrono::milliseconds interval;
};

typedef std::vector<sender_definition> senders_definitions;

template <message_id msg_id>
struct test_t {
    typedef msg_t<msg_id> msg;
    typedef async::internal::handlers_t<msg> msg_handlers;
    typedef async::sleeping_loop sleeping_loop;
    typedef std::vector<uint16_t> values;
    typedef std::chrono::milliseconds time;

    test_t(handlings_definitions &&p_handlings_definitions,
           senders_definitions &&p_senders_definitions)
        : m_name("messenger_" + number::format(msg_id))
        , m_handlings_definitions(std::move(p_handlings_definitions))
        , m_senders_definitions(std::move(p_senders_definitions)) {

        for (const sender_definition &_sender_def : m_senders_definitions) {
            m_total_msgs += _sender_def.num_msgs;
        }

        // m_max_msgs_timeout
        //        m_total_msgs /
        //            m_num_handlers * 30 / 100};
    }

    bool operator()() {
        logger::cerr<> _log {m_name.c_str()};
        _log.set_debug_level();
        //        logger::set_debug_level();
        number::id _id;

        std::mutex _mutex_counter_handled;
        std::mutex _mutex_counter_sent;

        std::condition_variable _cond_sent;
        std::mutex _mutex_sent;
        sent_handlers _sent_handlers {_id, std::chrono::seconds(2)};

        std::condition_variable _cond_handled;
        std::mutex _mutex_handled;
        handled_handlers _handled_handlers {_id, std::chrono::seconds(2)};

        INF(_log, "creating handlings and handlers");
        for (const handling_definition &_handling_def :
             m_handlings_definitions) {
            number::id _handling_id =
                async::add_handling<msg>(_handling_def.timeout);

            uint16_t _handler_id {0};
            for (const handler_definition &_handler_def :
                 _handling_def.handlers_defs) {
                handler _handler {_handler_id++, &_handled_handlers,
                                  _handling_def.timeout, _handler_def.sleep,
                                  _handler_def.timeout_at_each};
                async::add_handler<msg>(_handler_id, std::move(_handler));
            }
        }

        // HERE!

        msg_handlers _msg_handlers {_id, m_handling_timeout};

        INF(_log, "total messages to be sent: ", m_total_msgs);

        INF(_log, "creating handlers for handled messages");
        uint16_t _handled {0};
        bool _all_handled_notified {false};
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
        uint16_t _sent {0};
        bool _all_sent_notified {false};
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
        for (uint16_t _i = 0; _i < m_num_handlers; ++_i) {
            _handler_list.push_back(std::make_shared<handler>(
                _i, &_msg_handlers, &_handled_handlers, time {handlers_timeout},
                time {handler_sleep}, m_max_msgs_timeout, timeout_at_each));
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
        for (uint16_t _i = 0; _i < m_num_senders; ++_i) {
            _loops.push_back(
                {_id,
                 sender(_i, &_msg_handlers, &_sent_handlers, msgs_per_sender),
                 600ms, time {interval_per_sender}});
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
        for (uint16_t _i = 0; _i < m_num_senders; ++_i) {
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
            for (uint16_t _i = 0; _i < m_num_handlers; ++_i) {
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
            _file << static_cast<uint16_t>(msg_id) << ',' << m_num_handlers
                  << ',' << (timeout_at_each ? 's' : 'n') << ','
                  << m_num_senders << ',' << msgs_per_sender << ','
                  << m_total_msgs << ',' << _micro << ',' << _milli << ','
                  << _sec << ',' << _min << std::endl;
        }

        return (m_total_msgs == _handled);
    }

    static std::string description() {
        std::stringstream _stream;

        //        _stream << m_num_senders << " sender(s), sending " <<
        //        msgs_per_sender
        //                << " messages each, so " << m_total_msgs
        //                << " messages will be sent, at " <<
        //                interval_per_sender
        //                << "ms.\n"
        //                << "There are " << m_num_handlers
        //                << " handler(s), each with timeout of " <<
        //                handlers_timeout
        //                << "ms, and sleeping for " << handler_sleep
        //                << "ms, to simulate actual work";
        //        if (timeout_at_each) {
        //            _stream
        //                << ", and at each " << timeout_at_each
        //                << " messages, each handler will cause a timeout, for
        //                at most "
        //                << m_max_msgs_timeout << " times";
        //        }

        //        _stream << ".\n"
        //                << "The amount of messages consumed must be equal to
        //                the "
        //                   "sent.";

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
        handler(uint16_t p_id,
                msg_handlers *p_handlers,
                handled_handlers *p_handled_handlers,
                std::chrono::milliseconds p_timeout,
                std::chrono::milliseconds p_sleep,
                uint16_t p_cause_timeout_at_each = 0)
            : m_id('h' + std::to_string(p_id))
            , m_msg_handlers(p_handlers)
            , m_handled_handlers(p_handled_handlers)
            , m_timeout(p_timeout)
            , m_num_max_timeouts(3)
            , m_sleep(p_sleep)
            , m_cause_timeout_at_each(p_cause_timeout_at_each)
            , m_to_timeout(m_timeout.count() * 2)
            , m_log("handler_" + m_id) {
            m_log.set_debug_level();
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
            m_handled_handlers->add_data(handled {});
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
        sender(uint16_t p_idx,
               msg_handlers *p_handlers,
               sent_handlers *p_sent_handlers,
               uint16_t p_num_msg_per_sender)
            : m_idx('s' + std::to_string(p_idx))
            , m_handlers(p_handlers)
            , m_sent_handlers(p_sent_handlers)
            , m_num_msg_per_sender(p_num_msg_per_sender)
            , m_msg(p_idx * m_num_msg_per_sender)
            , m_start(p_idx * m_num_msg_per_sender)
            , m_finish((p_idx * m_num_msg_per_sender + m_num_msg_per_sender) -
                       1)
            , m_log("sender_" + m_idx) {
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
            m_sent_handlers->add_data(sent {});
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

        bool m_all_notified {false};

        logger::cerr<> m_log;
    };

private:
    const std::string m_name;
    handlings_definitions m_handlings_definitions;
    senders_definitions m_senders_definitions;
    uint16_t m_total_msgs {0};
    //    uint16_t m_max_msgs_timeout {0};
};

struct messenger_999 {
    static std::string desc() { return ""; }

    bool operator()() {
        /*
handling_test<33> _ht33(....);
handling_test<55> _ht55(....);
return _ht33() && _ht55();

*/

        return true;
    }
};

// #######################################

struct messenger_000 {
    typedef int16_t data;
    typedef logger::cerr<> log;
    typedef async::sleeping_loop sleeping_loop;

    static std::string desc() {
        std::stringstream _stream;
        _stream << "unamed, one worker, timeout";
        return _stream.str();
    }

    bool operator()() {
        using namespace std;

        //    m_log.set_debug_level();

        const data _data_to_produce {30};
        data _data_produced {0};
        data _data_consumed {0};
        const std::chrono::milliseconds _subscriber_timeout {800ms};

        number::id _id = async::add_handling<data>(_subscriber_timeout,
                                                   async::priority::lowest);

        auto _subscriber = [this, &_data_consumed, _subscriber_timeout](
                               std::shared_ptr<bool>, data &&p_data) -> void {
            ++_data_consumed;
            DEB(m_log, "subscriber sleeping for ", _data_consumed, ", ",
                p_data);
            std::this_thread::sleep_for(
                decltype(_subscriber_timeout)(_subscriber_timeout.count() / 2));
            DEB(m_log, "subscriber waking for ", _data_consumed, ", ", p_data);

            DEB(m_log, "consuming ", _data_consumed, ", ", p_data);
            if (_data_consumed == _data_to_produce) {
                m_cond_consumer.notify_one();
            }
        };

        async::add_handler<data>(_id, std::move(_subscriber));

        function<void(std::shared_ptr<bool>)> _slepper =
            [this, &_data_produced](std::shared_ptr<bool>) -> void {
            if (_data_produced == _data_to_produce) {
                m_cond_producer.notify_one();
            } else {
                ++_data_produced;
                async::send(_data_produced);
                DEB(m_log, "published ", _data_produced);
            }
        };

        sleeping_loop _sleeping_loop(
            m_id, _slepper, 300ms,
            decltype(_subscriber_timeout)(_subscriber_timeout.count() / 4));

        _sleeping_loop.start();

        DEB(m_log, "waiting for the producer to notify");
        {
            unique_lock<mutex> _lock_producer(m_mutex_producer);
            m_cond_producer.wait(_lock_producer);
        }
        _sleeping_loop.stop();

        DEB(m_log, "last data produced = ", _data_produced);

        DEB(m_log, "waiting for the consumer to notify");
        unique_lock<mutex> _lock_consumer(m_mutex_consumer);
        cv_status _status =
            m_cond_consumer.wait_for(_lock_consumer, chrono::seconds(60));
        DEB(m_log, "consumer notified");

        if (_status == cv_status::timeout) {
            ERR(m_log, "it took more time than nowed for the consumer");
            return false;
        }

        if (_data_consumed != _data_produced) {
            ERR(m_log, "data consumed = ", _data_consumed,
                ", but it should be ", _data_produced);
            return false;
        }

        DEB(m_log, "data consumed = ", _data_consumed,
            ", and is equal to data produced = ", _data_produced);

        return true;
    }

private:
    log m_log {"messenger_000"};
    std::mutex m_mutex_producer;
    std::condition_variable m_cond_producer;
    std::mutex m_mutex_consumer;
    std::condition_variable m_cond_consumer;
    number::id m_id;
};

// struct messenger_001 {
//    static std::string desc() {
//        return "compiling";
//    }

//    bool operator()() {
//        //    typedef async::internal::messenger_t<int16_t, async::priority>
//        //    messenger;

//        DEB(m_log, "starting");

//        auto _subscriber = [this](type::ptr<bool>, int16_t&& p_int) {
//            DEB(m_log, "i = ", p_int);
//            m_cond.notify_one();
//        };

//        DEB(m_log, "adding worker pool");
//        number::id _id =
//            async::add_handlers<int16_t>(1s, async::priority::lowest);
//        DEB(m_log, "worker pool, ", _id, " added");

//        DEB(m_log, "getting priority");
//        std::optional<async::priority> _maybe =
//            async::get_priority<int16_t>(_id);

//        if (_maybe) {
//            DEB(m_log, "priority = ", *_maybe);
//        }

//        DEB(m_log, "resetting priority");
//        async::set_priority<int16_t>(_id, async::priority::middle);
//        DEB(m_log, "priority reset");

//        DEB(m_log, "adding subscriber");
//        async::add_handler<int16_t>(_id, std::move(_subscriber));
//        DEB(m_log, "subscriber added");

//        DEB(m_log, "publishing");
//        async::send<int16_t>(42);
//        DEB(m_log, "published");

//        DEB(m_log, "wating notification");
//        {
//            std::unique_lock<std::mutex> _lock(m_mutex);
//            m_cond.wait(_lock);
//        }

//        DEB(m_log, "notification arrived");

//        return true;
//    }

//  private:
//    logger::cerr<> m_log {"messenger_001"};
//    std::condition_variable m_cond;
//    std::mutex m_mutex;
//};

// struct messenger_002 {
//    typedef int16_t data;
//    typedef async::sleeping_loop sleeping_loop;

//    static std::string desc() {
//        std::stringstream _stream;

//        _stream << "unamed, one worker, no timeout";

//        return _stream.str();
//    }

//    bool operator()() {
//        //    m_log.set_debug_level();
//        using namespace std;

//        const data _total_to_produce {58};
//        data _data_produced {0};
//        data _data_consumed {0};

//        number::id _id = async::add_handlers<data>(3s,
//        async::priority::lowest);

//        auto _subscriber = [this, &_data_consumed](
//                               type::ptr<bool>, data&& p_data) -> void {
//            DEB(m_log, "consuming ", p_data);
//            _data_consumed = p_data;
//            this_thread::sleep_for(chrono::seconds(1));
//            if (p_data == _total_to_produce) {
//                m_cond_consumer.notify_one();
//            }
//        };

//        async::add_handler<data>(_id, _subscriber);

//        auto _sleeper = [this, &_data_produced, _total_to_produce](
//                            type::ptr<bool>) -> void {
//            DEB(m_log, "data produced = ", _data_produced,
//                ", total to produce ", _total_to_produce);
//            if (_data_produced >= _total_to_produce) {
//                DEB(m_log, "notifying");
//                m_cond_producer.notify_one();
//            } else {
//                ++_data_produced;
//                async::send<data>(_data_produced);
//                DEB(m_log, "published data ", _data_produced);
//            }
//        };

//        sleeping_loop _sleeping_loop(m_id, _sleeper, 100ms, 500ms);

//        _sleeping_loop.start();

//        DEB(m_log, "waiting for the producer to notify");
//        {
//            unique_lock<mutex> _lock_producer(m_mutex_producer);
//            m_cond_producer.wait(_lock_producer);
//        }
//        DEB(m_log, "producer notified");

//        _sleeping_loop.stop();

//        DEB(m_log, "last data produced = ", _data_produced);

//        DEB(m_log, "waiting for the consumer to notify");
//        {
//            unique_lock<mutex> _lock_consumer(m_mutex_consumer);
//            cv_status _status =
//                m_cond_consumer.wait_for(_lock_consumer, chrono::seconds(5));

//            DEB(m_log, "consumer notified");

//            if (_status == cv_status::timeout) {
//                INF(m_log,
//                    "it took more time than allowed for the consumer, "
//                    "as expected");
//                //        messenger::wait();
//                return true;
//            }
//        }

//        ERR(m_log, "data consumed = ", _data_consumed, ", but it should be ",
//            _data_produced);
//        return false;
//    }

//  private:
//    logger::cerr<> m_log {"messenger_002"};
//    std::mutex m_mutex_producer;
//    std::condition_variable m_cond_producer;
//    std::mutex m_mutex_consumer;
//    std::condition_variable m_cond_consumer;
//    number::id m_id;
//};

// struct messenger_004 {
//    typedef int16_t data;
//    typedef async::sleeping_loop sleeping_loop;

//    static std::string desc() {
//        std::stringstream _stream;

//        _stream << "unamed, one worker, no timeout, using wrapper functions";

//        return _stream.str();
//    }

//    bool operator()() {
//        //    m_log.set_debug_level();
//        using namespace std;

//        const data _total_to_produce {58};
//        data _data_produced {0};
//        data _data_consumed {0};

//        number::id _id = async::add_handlers<data>(3s,
//        async::priority::lowest);

//        auto _subscriber = [this, &_data_consumed](
//                               type::ptr<bool>, data&& p_data) -> void {
//            DEB(m_log, "consuming ", p_data);
//            _data_consumed = p_data;
//            this_thread::sleep_for(chrono::seconds(1));
//            if (p_data == _total_to_produce) {
//                m_cond_consumer.notify_one();
//            }
//        };

//        async::add_handler<data>(_id, _subscriber);

//        auto _sleeper = [this, &_data_produced, _total_to_produce](
//                            type::ptr<bool>) -> void {
//            DEB(m_log, "data produced = ", _data_produced,
//                ", total to produce ", _total_to_produce);
//            if (_data_produced >= _total_to_produce) {
//                DEB(m_log, "notifying");
//                m_cond_producer.notify_one();
//            } else {
//                ++_data_produced;
//                async::send<data>(_data_produced);
//                DEB(m_log, "published data ", _data_produced);
//            }
//        };

//        sleeping_loop _sleeping_loop(m_id, _sleeper, 100ms, 500ms);

//        _sleeping_loop.start();

//        DEB(m_log, "waiting for the producer to notify");
//        {
//            unique_lock<mutex> _lock_producer(m_mutex_producer);
//            m_cond_producer.wait(_lock_producer);
//        }
//        DEB(m_log, "producer notified");

//        _sleeping_loop.stop();

//        DEB(m_log, "last data produced = ", _data_produced);

//        DEB(m_log, "waiting for the consumer to notify");
//        {
//            unique_lock<mutex> _lock_consumer(m_mutex_consumer);
//            cv_status _status =
//                m_cond_consumer.wait_for(_lock_consumer, chrono::seconds(5));

//            DEB(m_log, "consumer notified");

//            if (_status == cv_status::timeout) {
//                INF(m_log,
//                    "it took more time than allowed for the consumer, "
//                    "as expected");
//                //        messenger::wait();
//                return true;
//            }
//        }

//        ERR(m_log, "data consumed = ", _data_consumed, ", but it should be ",
//            _data_produced);
//        return false;
//    }

//  private:
//    logger::cerr<> m_log {"messenger_004"};
//    std::mutex m_mutex_producer;
//    std::condition_variable m_cond_producer;
//    std::mutex m_mutex_consumer;
//    std::condition_variable m_cond_consumer;
//    number::id m_id;
//};

// struct messenger_003 {
//    static std::string desc() {
//        return "Testing order of worker pools";
//    }

//    bool operator()() {
//        typedef int32_t data;

//        const number::id _p2 =
//            async::add_handlers<data>(4s, async::priority::low);

//        const number::id _p1 =
//            async::add_handlers<data>(1s, async::priority::middle);

//        bool _first {true};
//        bool _result {true};
//        auto _visitor = [this, &_first, &_result, _p1, _p2](
//                            const number::id& p_id,
//                            const async::priority& p_priority,
//                            const std::chrono::milliseconds& p_timeout) {
//            if (_first) {
//                if (p_id != _p2) {
//                    _result = false;
//                } else {
//                    _first = false;
//                }
//            } else {
//                if (p_id != _p1) {
//                    _result = false;
//                }
//            }
//            INF(m_log, p_id, ':', p_priority, ':', p_timeout.count());
//        };

//        async::traverse<data>(_visitor);
//        return _result;
//    }

//  private:
//    logger::cerr<> m_log {"messenger_003"};
//};

template <message_id msg_id>
struct publish_t {
    template <typename t_time>
    explicit publish_t(const number::id &p_owner,
                       value p_max,
                       std::condition_variable *p_cond,
                       t_time p_interval)
        : m_owner(p_owner)
        , m_max(p_max)
        , m_cond(p_cond)
        , m_log("publish_" + std::string(1, msg_id))
        , m_loop(
              m_owner,
              [this](type::ptr<bool>) -> void {
                  if (m_current >= m_max) {
                      DEB(m_log, m_current, " == ", m_max, " for publisher ",
                          msg_id);
                      m_cond->notify_one();
                      return;
                  }
                  msg_t<msg_id> _msg {++m_current};
                  DEB(m_log, "publishing ", _msg);
                  async::send<msg_t<msg_id>>(_msg);
              },
              2s,
              p_interval) {
        m_log.set_debug_level();
    }

    inline void start() { m_loop.start(); }
    inline void stop() { m_loop.stop(); }
    inline bool over() const {
        return (m_loop.is_stopped() ? true : m_current == m_max);
    }

private:
    number::id m_owner;
    const value m_max;
    std::condition_variable *m_cond;
    logger::cerr<> m_log;

    async::sleeping_loop m_loop;
    value m_current {0};
};

template <message_id msg_id>
struct subscriber_t {
    subscriber_t(const number::id &p_handlers, uint16_t p_number)
        : m_handlers(p_handlers)
        , m_number(p_number)
        , m_log("subscriber_" + std::string(1, msg_id) + "_" +
                p_handlers.str() + "_" + std::to_string(p_number)) {
        async::add_handler<msg_t<msg_id>>(
            m_handlers, [this](type::ptr<bool>, msg_t<msg_id> &&p_msg) -> void {
                DEB(m_log, "handling ", p_msg);
                m_current = p_msg.get_value();
            });
    }

    inline value get_last() const { return m_current; }

private:
private:
    const number::id m_handlers;
    const uint16_t m_number;
    logger::cerr<> m_log;
    value m_current;
};

// struct messenger_005 {
//    static std::string desc() { return ""; }

//    bool operator()() {
//        m_log.set_debug_level();
//        typedef msg_t<31> msg;

//        number::id _e_1 = async::add_handlers<msg>(1s);
//            number::id _f_1 = async::add_handlers<msg>(1s);

//        msg _current;

//        auto _sub_e_1_1 = [&](type::ptr<bool>, msg &&p_msg) -> void {
//            DEB(m_log, "handling ", p_msg);
//            _current = p_msg;
//        };
//        async::add_handler<msg>(_e_1, _sub_e_1_1);

//            subscriber_t<31> _sub_e_1_1(_e_1, 1);
//            subscriber_t<31> _sub_f_1_1(_f_1, 1);

//        publish();

//        while (true) {
//            // uint16_t _amount = _sub_e_1_1.get_last() +
//            _sub_f_1_1.get_last();
//            uint16_t _amount = _current.get_value();
//            if (_amount == 8) {
//                DEB(m_log, "all ", _amount, " messages where handled");
//                break;
//            }
//            DEB(m_log, _amount, " messages were handled so far");
//            std::this_thread::sleep_for(1s);
//        }

//        return true;
//    }

// private:
//    void publish() {
//        publish_t<42> _pub_e(m_id, value {5}, &m_cond_publish, 1s);
//        publish_t<43> _pub_f(m_id, value {3}, &m_cond_publish, 2s);

//        _pub_e.start();
//        //    _pub_f.start();

//        {
//            std::unique_lock<std::mutex> _lock(m_mutex_publish);
//            m_cond_publish.wait(_lock, [&]() -> bool {
//                bool _e = _pub_e.over();
//                bool _f = _pub_f.over();

//                if (_e) {
//                    _pub_e.stop();
//                }

//                if (_f) {
//                    _pub_f.stop();
//                }

//                if (_e && _f) {
//                    DEB(m_log, "publisher E and F are over");
//                    return true;
//                }
//                DEB(m_log, "waiting for all the publihsers to finish");
//                return false;
//            });
//        }
//    }

// private:
//    number::id m_id;
//    std::condition_variable m_cond_publish;
//    std::mutex m_mutex_publish;
//    logger::cerr<> m_log {"messenger_005"};
//};

struct messenger_006 {
    typedef msg_t<94> data;
    typedef logger::cerr<> log;
    typedef async::sleeping_loop sleeping_loop;

    static std::string desc() {
        std::stringstream _stream;
        _stream << "unamed, one worker, timeout";
        return _stream.str();
    }

    bool operator()() {
        using namespace std;

        //    m_log.set_debug_level();

        const data _data_to_produce {30};
        data _data_produced {0};
        data _data_consumed {0};
        const std::chrono::milliseconds _subscriber_timeout {800ms};

        number::id _id = async::add_handling<data>(_subscriber_timeout,
                                                   async::priority::lowest);

        auto _subscriber = [this, &_data_consumed, &_data_to_produce,
                            _subscriber_timeout](std::shared_ptr<bool>,
                                                 data &&p_data) -> void {
            _data_consumed = p_data;
            DEB(m_log, "subscriber sleeping for ", p_data);
            std::this_thread::sleep_for(
                decltype(_subscriber_timeout)(_subscriber_timeout.count() / 2));
            DEB(m_log, "subscriber waking for ", p_data);

            DEB(m_log, "consuming ", p_data);
            if (_data_consumed == _data_to_produce) {
                m_cond_consumer.notify_one();
            }
        };

        async::add_handler<data>(_id, std::move(_subscriber));

        function<void(std::shared_ptr<bool>)> _slepper =
            [this, &_data_produced,
             &_data_to_produce](std::shared_ptr<bool>) -> void {
            if (_data_produced == _data_to_produce) {
                m_cond_producer.notify_one();
            } else {
                ++_data_produced;
                async::send(_data_produced);
                DEB(m_log, "published ", _data_produced);
            }
        };

        sleeping_loop _sleeping_loop(
            m_id, _slepper, 300ms,
            decltype(_subscriber_timeout)(_subscriber_timeout.count() / 4));

        _sleeping_loop.start();

        DEB(m_log, "waiting for the producer to notify");
        {
            unique_lock<mutex> _lock_producer(m_mutex_producer);
            m_cond_producer.wait(_lock_producer);
        }
        _sleeping_loop.stop();

        DEB(m_log, "last data produced = ", _data_produced);

        DEB(m_log, "waiting for the consumer to notify");
        unique_lock<mutex> _lock_consumer(m_mutex_consumer);
        cv_status _status =
            m_cond_consumer.wait_for(_lock_consumer, chrono::seconds(60));
        DEB(m_log, "consumer notified");

        if (_status == cv_status::timeout) {
            ERR(m_log, "it took more time than nowed for the consumer");
            return false;
        }

        if (_data_consumed != _data_produced) {
            ERR(m_log, "data consumed = ", _data_consumed,
                ", but it should be ", _data_produced);
            return false;
        }

        DEB(m_log, "data consumed = ", _data_consumed,
            ", and is equal to data produced = ", _data_produced);

        return true;
    }

private:
    log m_log {"messenger_006"};
    std::mutex m_mutex_producer;
    std::condition_variable m_cond_producer;
    std::mutex m_mutex_consumer;
    std::condition_variable m_cond_consumer;
    number::id m_id;
};

// struct messenger_007 {
//    typedef msg_t<51> msg;
//    typedef async::sleeping_loop sleeping_loop;
//    typedef async::internal::handlers_t<msg> handlers;

//    static std::string desc() {
//        std::stringstream _stream;

//        _stream
//            << "A 'sleeping_loop' sends " << m_max << " messages, at each "
//            << m_interval.count() << "ms, to a 'handlers' with timeout of "
//            << m_handlers_timeout.count()
//            << "s, with one handler that sleeps for " <<
//            m_handler_sleep.count()
//            << "ms.\n"
//            << "The amount of messages consumed must be equal to the
//            produced";

//        return _stream.str();
//    }

//    bool operator()() {
//        logger::set_debug_level();

//        DEB(m_log, "capacity = ", m_handlers.capacity(),
//            ", occupied = ", m_handlers.occupied());

//        DEB(m_log, "adding consumer to the worker");
//        number::id _handlers = async::add_handlers<msg>(m_handlers_timeout);
//        async::add_handler<msg>(_handlers,
//            [this](std::shared_ptr<bool> p_bool, msg&& p_msg) -> void {
//                m_consumer(p_bool, std::move(p_msg));
//            });

//        producer _producer {_handlers, &m_cond_produced, &m_msg};
//        sleeping_loop _loop {
//            m_id, _producer, m_sleeping_loop_timeout, m_interval};

//        DEB(m_log, "starting the producer loop");
//        _loop.start();

//        {
//            std::unique_lock<std::mutex> _lock(m_mutex_produced);
//            m_cond_produced.wait(_lock);
//        }

//        DEB(m_log, "stoping the producer loop");
//        _loop.stop();

//        {
//            std::unique_lock<std::mutex> _lock(m_mutex_handled);
//            m_cond_handled.wait(_lock);
//        }

//        DEB(m_log, "produced = ", m_msg.get_value(),
//            ", consumed = ", m_consumer.get_msg().get_value());
//        if (m_consumer.get_msg().get_value() != m_msg.get_value()) {
//            ERR(m_log, "Data value consumed should be equal to provided");
//            return false;
//        }

//        return true;
//    }

//  private:
//    struct producer {
//        producer(const number::id& p_handlers, std::condition_variable*
//        p_cond,
//            msg* p_msg) :
//            m_handlers(p_handlers), m_cond(p_cond), m_msg(p_msg) {}

//        void operator()(std::shared_ptr<bool>) {
//            if (m_msg->get_value() == m_max) {
//                m_cond->notify_one();
//                return;
//            }

//            ++(*m_msg);
//            msg _msg(m_msg->get_value());
//            DEB(m_log, "going to add ", _msg);

//            async::send<msg>(_msg);
//            //      DEB(m_log, "added msg ", _msg, "; capacity = ",
//            //      m_handlers->capacity(),
//            //          ", occupied = ", m_handlers->occupied());
//        }

//      private:
//        const number::id m_handlers;
//        std::condition_variable* m_cond;
//        msg* m_msg;

//        logger::cerr<> m_log {"producer"};
//    };

//    struct consumer {
//        consumer(std::condition_variable* p_cond) : m_cond(p_cond) {}
//        void operator()(std::shared_ptr<bool>, msg&& p_msg) {
//            m_msg = std::move(p_msg);

//            DEB(m_log, "handling msg ", m_msg);

//            if (m_msg.get_value() == m_max) {
//                DEB(m_log, "Last msg handled = ", m_msg);
//                m_cond->notify_one();
//                return;
//            }

//            DEB(m_log, "worker is going to sleep");
//            std::this_thread::sleep_for(m_handler_sleep);
//            DEB(m_log, "worker is waking up");
//        }
//        const msg& get_msg() const {
//            return m_msg;
//        }

//      private:
//        msg m_msg;
//        std::condition_variable* m_cond;
//        logger::cerr<> m_log {"consumer"};
//    };

//    logger::cerr<> m_log {"messenger_007"};
//    number::id m_id;
//    std::condition_variable m_cond_produced;
//    std::mutex m_mutex_produced;
//    std::condition_variable m_cond_handled;
//    std::mutex m_mutex_handled;
//    consumer m_consumer {&m_cond_handled};
//    msg m_msg {0};
//    handlers m_handlers {m_id, m_handlers_timeout};
//    static const value m_max {10};
//    static const std::chrono::milliseconds m_interval;
//    static const std::chrono::seconds m_handlers_timeout;
//    static const std::chrono::milliseconds m_handler_sleep;
//    static const std::chrono::seconds m_sleeping_loop_timeout;
//};

// const std::chrono::milliseconds messenger_007::m_interval {150};
// const std::chrono::seconds messenger_007::m_handlers_timeout {2};
// const std::chrono::milliseconds messenger_007::m_handler_sleep {300};
// const std::chrono::seconds messenger_007::m_sleeping_loop_timeout {1};

struct messenger_008 {
    static std::string desc() { return ""; }

    bool operator()() {
        typedef int16_t msg;
        logger::cerr<> _log {"messegenr_008"};
        auto _handler = [&](type::ptr<bool>, msg &&p_msg) {
            INF(_log, "msg = ", p_msg);
        };

        number::id _handling_id = async::add_handling<msg>(500ms);

        async::add_handler<msg>(_handling_id, _handler);

        async::send<msg>(-93);

        std::this_thread::sleep_for(5s);

        return true;
    }
};

int main(int argc, char **argv) {
    logger::set_debug_level();
    tester::test _tester(argc, argv);

    //    run_test(_tester, messenger_000);
    //    run_test(_tester, messenger_001);
    //    run_test(_tester, messenger_002);
    //    run_test(_tester, messenger_003);
    //    run_test(_tester, messenger_004);
    //    run_test(_tester, messenger_005);
    run_test(_tester, messenger_006);
    //    run_test(_tester, messenger_007);
    run_test(_tester, messenger_008);
}
