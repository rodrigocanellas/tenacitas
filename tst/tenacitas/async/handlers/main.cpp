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

template <message_id msg_id> struct test {

    typedef msg_t<msg_id> msg;
    typedef async::internal::handlers_t<msg> handlers;
    typedef async::sleeping_loop sleeping_loop;

    test(std::string &&p_name, std::vector<value> &&p_max_msg_per_sender,
         std::vector<std::chrono::milliseconds> &&p_interval_per_sender,
         std::vector<std::chrono::milliseconds> &&p_timeout_per_sender,
         std::chrono::milliseconds p_handlers_timeout,
         std::vector<std::chrono::milliseconds> &&p_sleep_per_handler)
        : m_name(std::move(p_name)), m_num_handlers(0),
          m_max_msg_per_sender(std::move(p_max_msg_per_sender)),
          m_interval_per_sender(std::move(p_interval_per_sender)),
          m_timeout_per_sender(std::move(p_timeout_per_sender)),
          m_total_msgs(0), m_num_senders(0),
          m_handlers_timeout(p_handlers_timeout),
          m_sleep_per_handler(std::move(p_sleep_per_handler)) {

        if (m_max_msg_per_sender.size() != m_interval_per_sender.size()) {
            throw std::runtime_error(
                "num max msgs per sender != num interval per sender");
        }

        if (m_max_msg_per_sender.size() != m_timeout_per_sender.size()) {
            throw std::runtime_error(
                "num max msgs per sender != num timeout per sender");
        }

        if (m_interval_per_sender.size() != m_timeout_per_sender.size()) {
            throw std::runtime_error(
                "num max interval per sender != num timeout per sender");
        }
        m_num_senders = m_interval_per_sender.size();

        m_num_handlers = m_sleep_per_handler.size();

        total_msgs();
    }

    bool operator()() {

        logger::cerr<> _log{"handlers_004"};
        //        logger::set_debug_level();
        _log.set_debug_level();
        number::id _id;
        std::condition_variable _cond_produced;
        std::mutex _mutex_produced;
        handlers _handlers{_id, m_handlers_timeout};

        INF(_log, "creating the handlers");
        std::vector<type::ptr<handler>> _handler_list;
        for (uint16_t _i = 0; _i < m_num_handlers; ++_i) {
            _handler_list.push_back(
                std::make_shared<handler>("c" + std::to_string(_i)), &_handlers,
                m_handlers_timeout, m_sleep_per_handler[_i]);
        }

        INF(_log, "creating and starting the producers loops");
        std::vector<sleeping_loop> _loops;
        for (uint16_t _i = 0; _i < m_num_senders; ++_i) {
            _loops.push_back({_id, sender{_i, &_handlers, &_cond_produced},
                              m_max_msg_per_sender[_i],
                              m_timeout_per_sender[_i],
                              m_interval_per_sender[_i]});
            DEB(_log, "starting sender p", _i);
            _loops.back().start();
        }

        INF(_log, "adding each handler to the handlers group");
        for (type::ptr<handler> &_handler : _handler_list) {
            _handlers.add_handler(
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
                    DEB(_log, "first notification");
                    _first = false;
                    return false;
                }

                DEB(_log, "not first notification");

                ++_num_notifications;
                if (_num_notifications == m_num_senders) {
                    DEB(_log, "all producers notified");
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

        INF(_log, "waiting for all the consumers, until all the messages are "
                  "consumed");
        uint16_t _handled{0};

        while (true) {
            _handled = 0;
            for (uint16_t _i = 0; _i < m_num_handlers; ++_i) {
                DEB(_log, "consumer ", _handler_list[_i]->get_id(), ": ",
                    _handler_list[_i]->get_num());
                _handled += _handler_list[_i]->get_num();
            }

            if (_handled >= m_total_msgs) {
                break;
            }
            DEB(_log, "still waiting... produced: ", m_total_msgs,
                ", total handled: ", _handled);
            std::this_thread::sleep_for(200ms);
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

  private:
    void total_msgs() {
        for (uint16_t _i = 0; _i < m_num_senders; ++_i) {
            m_total_msgs += m_max_msg_per_sender[_i];
        }
    }

    struct handler {
        handler(std::string &&p_id, handlers *p_handlers,
                std::chrono::milliseconds p_timeout,
                std::chrono::milliseconds p_sleep)
            : m_id(p_id), m_handlers(p_handlers), m_timeout(p_timeout),
              m_sleep(p_sleep) {}
        void operator()(std::shared_ptr<bool> p_bool, msg &&p_msg) {
            m_log.set_debug_level();
            std::this_thread::sleep_for(m_sleep);
            if (*p_bool) {
                m_handlers->add_data(p_msg);
            }
            ++m_num;
            m_msg = std::move(p_msg);
            DEB(m_log, "handling msg|", m_id, "|", m_msg, "|", m_num);
        }

        friend std::ostream &operator<<(std::ostream &p_out,
                                        const handler &p_handler) {
            p_out << '(' << p_handler.get_id() << ',' << p_handler.m_msg << ','
                  << p_handler.get_num() << ')';
            return p_out;
        }

        inline const std::string &get_id() const { return m_id; }
        inline uint16_t get_num() const { return m_num; }

      private:
        std::string m_id;
        handlers *m_handlers;
        const std::chrono::milliseconds m_timeout;
        const std::chrono::milliseconds m_sleep;
        uint16_t m_num{0};
        msg m_msg;
        logger::cerr<> m_log{"handler"};
    };

    struct sender {
        sender(uint16_t p_idx, handlers *p_handlers,
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
        handlers *m_handlers;
        std::condition_variable *m_cond;
        const uint16_t m_max_per_sender;

        msg m_msg;
        value m_start;
        value m_finish;

        logger::cerr<> m_log;
    };

  private:
    std::string m_name;

    uint16_t m_num_senders;
    const std::vector<value> m_max_msg_per_sender;
    const std::vector<std::chrono::milliseconds> m_interval_per_sender;
    const std::vector<std::chrono::milliseconds> m_timeout_per_sender;
    uint16_t m_total_msgs;

    uint16_t m_num_handlers;
    const std::chrono::milliseconds m_handlers_timeout;
    const std::vector<std::chrono::milliseconds> m_sleep_per_handler;
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
    typedef msg_t<'B'> msg;
    typedef async::sleeping_loop sleeping_loop;
    typedef async::internal::handlers_t<msg> handlers;

    static std::string desc() {
        std::stringstream _stream;

        _stream
            << "A sender sends " << m_max << " messages, at each "
            << m_interval.count() << "ms, to a 'handlers' with timeout of "
            << m_handlers_timeout.count()
            << "s, with one handler that sleeps for " << m_handler_sleep.count()
            << "ms.\n"
            << "The amount of messages consumed must be equal to the produced";

        return _stream.str();
    }

    bool operator()() {

        logger::cerr<> _log{"handlers_001"};
        number::id _id;
        std::condition_variable _cond_produced;
        std::mutex _mutex_produced;
        std::condition_variable _cond_handled;
        std::mutex _mutex_handled;

        handlers _handlers{_id, m_handlers_timeout};

        msg _sent;
        auto _sender = [&](type::ptr<bool>) -> void {
            if (_sent.get_value() == m_max) {
                _cond_produced.notify_one();
                return;
            }

            ++_sent;
            DEB(_log, "going to add ", _sent);
            _handlers.add_data(_sent);

            DEB(_log, "sent msg: ", _sent, "; capacity: ", _handlers.capacity(),
                ", occupied: ", _handlers.occupied());
        };

        sleeping_loop _loop{_id, _sender, m_sleeping_loop_timeout, m_interval};

        logger::set_debug_level();

        DEB(_log, "capacity = ", _handlers.capacity(),
            ", occupied = ", _handlers.occupied());

        msg _handled;

        auto _handler = [&](type::ptr<bool>, msg &&p_msg) -> void {
            _handled = std::move(p_msg);

            DEB(_log, "handling msg ", _handled);

            if (_handled.get_value() == m_max) {
                DEB(_log, "last msg handled = ", _handled);
                _cond_handled.notify_one();
                return;
            }

            DEB(_log, "handler is going to sleep");
            std::this_thread::sleep_for(m_handler_sleep);
            DEB(_log, "handler is waking up");
        };

        DEB(_log, "adding handler");
        _handlers.add_handler(_handler);

        DEB(_log, "starting the sender loop");
        _loop.start();

        {
            std::unique_lock<std::mutex> _lock(_mutex_produced);
            _cond_produced.wait(_lock);
        }

        DEB(_log, "stoping the sender loop");
        _loop.stop();

        {
            std::unique_lock<std::mutex> _lock(_mutex_handled);
            _cond_handled.wait(_lock);
        }

        DEB(_log, "sent = ", _sent.get_value(),
            ", handled = ", _handled.get_value());
        if (_handled.get_value() != _sent.get_value()) {
            ERR(_log, "Data value handled should be equal to sent");
            return false;
        }

        return true;
    }

  private:
    static const value m_max{10};
    static const std::chrono::milliseconds m_interval;
    static const std::chrono::seconds m_handlers_timeout;
    static const std::chrono::milliseconds m_handler_sleep;
    static const std::chrono::seconds m_sleeping_loop_timeout;
};
const std::chrono::milliseconds handlers_001::m_interval{150};
const std::chrono::seconds handlers_001::m_handlers_timeout{2};
const std::chrono::milliseconds handlers_001::m_handler_sleep{300};
const std::chrono::seconds handlers_001::m_sleeping_loop_timeout{1};

struct handlers_002 {

    typedef msg_t<'C'> msg;

    typedef async::internal::handlers_t<msg> handlers;
    typedef async::sleeping_loop sleeping_loop;

    static std::string desc() {
        std::stringstream _stream;
        _stream << "Produces " << m_max
                << " messages, and waits for all to be consumed by "
                << m_mum_handler_list << " consumers";

        return _stream.str();
    }

    bool operator()() {
        logger::set_debug_level();

        logger::cerr<> _log{"handlers_002"};
        number::id _id;
        std::condition_variable _cond_produced;
        std::mutex _mutex_produced;
        handlers _handlers{_id, m_handlers_timeout};
        producer _producer{&_handlers, &_cond_produced};

        DEB(_log, "id = ", _id);

        std::vector<consumer> _handler_list;
        for (uint16_t _i = 0; _i < m_mum_handler_list; ++_i) {
            _handler_list.push_back({"c" + std::to_string(_i)});
        }

        for (consumer &_consumer : _handler_list) {
            DEB(_log, "adding consumer ", _consumer.get_id());
            _handlers.add_handler([&_consumer](std::shared_ptr<bool> p_bool,
                                               msg &&p_msg) -> void {
                _consumer(p_bool, std::move(p_msg));
            });
        }

        sleeping_loop _sleeping_loop{
            _id,
            [&](std::shared_ptr<bool> p_bool) -> void { _producer(p_bool); },
            m_sleeping_loop_timeout, m_interval};

        DEB(_log, "starting the producer loop");
        _sleeping_loop.start();

        {
            std::unique_lock<std::mutex> _lock(_mutex_produced);
            _cond_produced.wait(_lock);
        }

        DEB(_log, "stoping the producer loop");
        _sleeping_loop.stop();

        uint16_t _num{0};
        {
            while (true) {

                for (const consumer &_consumer : _handler_list) {
                    _num += _consumer.get_num();
                }

                if (_num == _producer.get_num()) {
                    break;
                }
                DEB(_log, "still waiting... produced: ", _producer.get_num(),
                    ", total consumed: ", _num);
                std::this_thread::sleep_for(200ms);
            }
        }

        {
            std::stringstream _stream;
            for (const consumer &_consumer : _handler_list) {
                _stream << _consumer << " ";
            }

            DEB(_log, "amount added = ", _producer.get_num(),
                ", consumed: ", _stream.str(), " - total = ", _num);
        }

        return (_producer.get_num() == _num);
    }

  private:
    struct consumer {
        consumer(std::string &&p_id) : m_id(p_id) { m_log.set_debug_level(); }
        void operator()(std::shared_ptr<bool>, msg &&p_msg) {
            ++m_num;
            m_msg = std::move(p_msg);
            DEB(m_log, "handling msg|", m_id, "|", m_msg, "|", m_num);
            std::this_thread::sleep_for(
                std::chrono::seconds(m_handlers_timeout.count() / 4));
        }

        friend std::ostream &operator<<(std::ostream &p_out,
                                        const consumer &p_consumer) {
            p_out << '(' << p_consumer.get_id() << ',' << p_consumer.m_msg
                  << ',' << p_consumer.get_num() << ')';
            return p_out;
        }

        const std::string &get_id() const { return m_id; }
        inline uint16_t get_num() const { return m_num; }

      private:
        std::string m_id;
        uint16_t m_num{0};
        msg m_msg;
        logger::cerr<> m_log{"consumer"};
    };

    struct producer {
        producer(handlers *p_handlers, std::condition_variable *p_cond)
            : m_handlers(p_handlers), m_cond(p_cond) {}

        void operator()(std::shared_ptr<bool>) {
            if (m_msg.get_value() == m_max) {
                m_cond->notify_one();
                return;
            }

            ++m_msg;
            ++m_num;
            msg _msg(m_msg.get_value());
            DEB(m_log, "going to add ", _msg);

            m_handlers->add_data(_msg);
            DEB(m_log, "added msg ", _msg,
                "; capacity = ", m_handlers->capacity(),
                ", occupied = ", m_handlers->occupied());
        }

        inline uint16_t get_num() const { return m_num; }

      private:
        handlers *m_handlers;
        std::condition_variable *m_cond;
        msg m_msg{0};
        uint16_t m_num{0};

        logger::cerr<> m_log{"producer"};
    };

  private:
    static const value m_max{3000};
    static const uint16_t m_mum_handler_list{12};
    static const std::chrono::milliseconds m_interval;
    static const std::chrono::seconds m_handlers_timeout;
    static const std::chrono::milliseconds m_handler_sleep;
    static const std::chrono::seconds m_sleeping_loop_timeout;
};

const std::chrono::milliseconds handlers_002::m_interval{150};
const std::chrono::seconds handlers_002::m_handlers_timeout{2};
const std::chrono::milliseconds handlers_002::m_handler_sleep{300};
const std::chrono::seconds handlers_002::m_sleeping_loop_timeout{1};

struct handlers_003 {

    typedef msg_t<'D'> msg;

    typedef async::internal::handlers_t<msg> handlers;
    typedef async::sleeping_loop sleeping_loop;

    static std::string desc() {
        std::stringstream _stream;
        _stream
            << "Produces " << m_max
            << " messages, and waits for all to be consumed by "
            << m_mum_handler_list << " consumers.\n"
            << "At each " << m_num_msgs_to_cause_timeout
            << " messages, every consumer will cause a timeout, the message "
               "will be added again to the pool, and will eventually be "
               "handled, may be by another worker.\n"
            << "As a consequence, they will appear in the log in different a "
            << "position from the sequencial.";

        return _stream.str();
    }

    bool operator()() {
        logger::cerr<> _log{"handlers_003"};
        //        _log.set_debug_level();
        logger::set_debug_level();

        number::id _id;
        handlers _handlers{_id, m_handlers_timeout};
        std::vector<consumer> _handler_list;
        for (uint16_t _i = 0; _i < m_mum_handler_list; ++_i) {
            _handler_list.push_back(
                {"c" + std::to_string(_i), m_handlers_timeout, &_handlers});
        }
        msg _msg{0};
        std::condition_variable _cond_produced;
        std::mutex _mutex_produced;

        producer _producer{&_handlers, &_cond_produced};

        for (consumer &_consumer : _handler_list) {
            DEB(_log, "adding consumer ", _consumer.get_id());
            _handlers.add_handler([&_consumer](std::shared_ptr<bool> p_bool,
                                               msg &&p_msg) -> void {
                _consumer(p_bool, std::move(p_msg));
            });
        }
        {
            sleeping_loop _sleeping_loop{
                _id,
                [&](std::shared_ptr<bool> p_bool) -> void {
                    _producer(p_bool);
                },
                m_sleeping_loop_timeout, m_interval};

            DEB(_log, "starting the producer loop");
            _sleeping_loop.start();

            {
                std::unique_lock<std::mutex> _lock(_mutex_produced);
                _cond_produced.wait(_lock);
            }

            DEB(_log, "stoping the producer loop");
            _sleeping_loop.stop();
        }

        while (_handlers.occupied() != 0) {
            DEB(_log, "msg queue still not empty");
            std::this_thread::sleep_for(200ms);
        }

        DEB(_log, "**** message queue empty");

        uint16_t _num_timeouts{0};
        uint16_t _consumed{0};

        while (true) {
            _consumed = 0;
            _num_timeouts = 0;
            for (const consumer &_consumer : _handler_list) {
                _consumed += _consumer.get_num();
                _num_timeouts += _consumer.get_num_timeouts();
            }

            if (_consumed >= _producer.get_num()) {
                DEB(_log, "consumed: ", _consumed,
                    ", produced: ", _producer.get_num());
                break;
            }

            DEB(_log, "amount added = ", _producer.get_num(),
                ", consumed = ", _consumed, ", num timeouts = ", _num_timeouts);
            std::this_thread::sleep_for(200ms);
        }
        for (const consumer &_consumer : _handler_list) {
            DEB(_log, _consumer);
        }

        return (_producer.get_num() == _consumed);
    }

  private:
    struct producer {
        producer(handlers *p_handlers, std::condition_variable *p_cond)
            : m_handlers(p_handlers), m_cond(p_cond) {}

        void operator()(std::shared_ptr<bool>) {
            m_log.set_debug_level();
            if (m_msg.get_value() == m_max) {
                m_cond->notify_one();
                return;
            }

            ++m_msg;
            ++m_num;
            msg _msg(m_msg.get_value());
            DEB(m_log, "==> ", _msg);

            m_handlers->add_data(_msg);
            //            DEB(m_log, "added msg ", _msg,
            //                "; capacity = ", m_handlers->capacity(),
            //                ", occupied = ", m_handlers->occupied());
        }

        inline uint16_t get_num() const { return m_num; }

      private:
        handlers *m_handlers;
        std::condition_variable *m_cond;
        msg m_msg{0};
        uint16_t m_num{0};
        std::condition_variable m_cond_produced;
        std::mutex m_mutex_produced;

        logger::cerr<> m_log{"producer"};
    };

    struct consumer {
        consumer(std::string &&p_id, std::chrono::milliseconds p_timeout,
                 handlers *p_handlers)
            : m_id(p_id), m_timeout(p_timeout), m_handlers(p_handlers),
              m_over_timeout(p_timeout.count() * 2),
              m_sleep(p_timeout.count() / 2) {}

        void operator()(std::shared_ptr<bool> p_bool, msg &&p_msg) {
            m_log.set_debug_level();
            if (((m_count % m_num_msgs_to_cause_timeout) == 0) &&
                (m_timeout_counter < m_max_timeouts)) {
                WAR(m_log, "causing timeout for ", m_id, " when msg = ", p_msg,
                    ", count = ", m_count,
                    ", timeout counter = ", m_timeout_counter);
                ++m_timeout_counter;
                m_count = 1;
                std::this_thread::sleep_for(m_over_timeout);
            } else {
                m_msg = p_msg;
                ++m_num;
                ++m_count;
                DEB(m_log, "<== ", m_id, "|", m_msg, "|", m_num);
                //                std::this_thread::sleep_for(m_sleep);
            }
            if (*p_bool) {
                DEB(m_log, m_id, " adding msg ", p_msg,
                    " to the handlers again");
                m_handlers->add_data(p_msg);
            }
        }

        friend std::ostream &operator<<(std::ostream &p_out,
                                        const consumer &p_consumer) {
            p_out << "(" << p_consumer.get_id() << "," << p_consumer.get_msg()
                  << "," << p_consumer.get_num() << ")";
            return p_out;
        }

        const std::string &get_id() const { return m_id; }
        inline const msg &get_msg() const { return m_msg; }
        inline value get_num() const { return m_num; }
        std::chrono::milliseconds get_timeout() const { return m_timeout; }
        uint16_t get_num_timeouts() const { return m_timeout_counter; }

      private:
        std::string m_id;
        std::chrono::milliseconds m_timeout;
        handlers *m_handlers;
        std::chrono::milliseconds m_over_timeout;
        std::chrono::milliseconds m_sleep;
        msg m_msg;

        value m_num{0};
        value m_count{1};
        uint16_t m_timeout_counter{0};
        uint16_t m_max_timeouts{m_max / 10};
        logger::cerr<> m_log{"consumer"};
    };

    static const uint16_t m_mum_handler_list{12};
    static const value m_max{3000};
    static const std::chrono::milliseconds m_handlers_timeout;
    static const uint16_t m_num_msgs_to_cause_timeout{8};
    static const uint16_t m_max_timeouts{5};
    static const std::chrono::milliseconds m_interval;
    static const std::chrono::seconds m_sleeping_loop_timeout;
};

const std::chrono::milliseconds handlers_003::m_interval{100};
const std::chrono::milliseconds handlers_003::m_handlers_timeout{250};
const std::chrono::seconds handlers_003::m_sleeping_loop_timeout{1};

struct handlers_004 {

    typedef msg_t<'E'> msg;

    typedef async::internal::handlers_t<msg> handlers;
    typedef async::sleeping_loop sleeping_loop;

    static std::string desc() {
        std::stringstream _stream;
        _stream << m_num_senders << " producers produce "
                << m_max_msg_per_sender
                << " messages each, and waits for all to be consumed by "
                << m_num_handlers << " consumers";

        return _stream.str();
    }

    bool operator()() {

        logger::cerr<> _log{"handlers_004"};
        //        logger::set_debug_level();
        _log.set_debug_level();
        number::id _id;
        std::condition_variable _cond_produced;
        std::mutex _mutex_produced;
        handlers _handlers{_id, m_handlers_timeout};

        DEB(_log, "id = ", _id);

        DEB(_log, "creating the consumers");
        std::vector<type::ptr<consumer>> _handler_list;
        for (uint16_t _i = 0; _i < m_num_handlers; ++_i) {
            _handler_list.push_back(
                std::make_shared<consumer>("c" + std::to_string(_i)));
        }

        DEB(_log, "creating and starting the producers loops");
        std::vector<sleeping_loop> _loops;
        for (uint16_t _i = 0; _i < m_num_senders; ++_i) {
            _loops.push_back({_id, producer{_i, &_handlers, &_cond_produced},
                              m_sleeping_loop_timeout, m_interval});
            DEB(_log, "starting producer p", _i);
            _loops.back().start();
        }

        DEB(_log, "adding each consumer to the consumers group");
        for (type::ptr<consumer> &_consumer : _handler_list) {
            _handlers.add_handler(
                [&, _consumer](type::ptr<bool> p_bool, msg &&p_msg) -> void {
                    (*_consumer)(p_bool, std::move(p_msg));
                });
        }

        DEB(_log, "checking if all the messages were produced");
        {
            bool _first{true};
            uint16_t _num_notifications{0};
            std::unique_lock<std::mutex> _lock(_mutex_produced);
            _cond_produced.wait(_lock, [&]() -> bool {
                if (_first) {
                    DEB(_log, "first notification");
                    _first = false;
                    return false;
                }

                DEB(_log, "not first notification");

                ++_num_notifications;
                if (_num_notifications == m_num_senders) {
                    DEB(_log, "all producers notified");
                    return true;
                }
                DEB(_log, _num_notifications, " producers notified, ",
                    m_num_senders, " expected");
                return false;
            });
        }

        DEB(_log, "stoping the producers loops");
        for (uint16_t _i = 0; _i < m_num_senders; ++_i) {
            _loops[_i].stop();
        }

        DEB(_log, "waiting for all the consumers, until all the messages are "
                  "consumed");
        uint16_t _consumed{0};
        {

            while (true) {
                _consumed = 0;
                for (uint16_t _i = 0; _i < m_num_handlers; ++_i) {
                    DEB(_log, "consumer ", _handler_list[_i]->get_id(), ": ",
                        _handler_list[_i]->get_num());
                    _consumed += _handler_list[_i]->get_num();
                }

                if (_consumed >= m_num_senders * m_max_msg_per_sender) {
                    break;
                }
                DEB(_log, "still waiting... produced: ",
                    m_num_senders * m_max_msg_per_sender,
                    ", total consumed: ", _consumed);
                std::this_thread::sleep_for(200ms);
            }
        }

        DEB(_log, "reporting results");
        {
            DEB(_log, "amount added = ", m_num_senders * m_max_msg_per_sender);
            for (uint16_t _i = 0; _i < m_num_handlers; ++_i) {
                DEB(_log, *_handler_list[_i]);
            }
            DEB(_log, "consumed: ", _consumed);
        }

        return (m_num_senders * m_max_msg_per_sender == _consumed);
    }

  private:
    struct consumer {
        consumer(std::string &&p_id) : m_id(p_id) {}
        void operator()(std::shared_ptr<bool>, msg &&p_msg) {
            m_log.set_debug_level();
            ++m_num;
            m_msg = std::move(p_msg);
            DEB(m_log, "handling msg|", m_id, "|", m_msg, "|", m_num);
            std::this_thread::sleep_for(
                std::chrono::seconds(m_handlers_timeout.count() / 4));
        }

        friend std::ostream &operator<<(std::ostream &p_out,
                                        const consumer &p_consumer) {
            p_out << '(' << p_consumer.get_id() << ',' << p_consumer.m_msg
                  << ',' << p_consumer.get_num() << ')';
            return p_out;
        }

        const std::string &get_id() const { return m_id; }
        inline uint16_t get_num() const { return m_num; }

      private:
        std::string m_id;
        uint16_t m_num{0};
        msg m_msg;
        logger::cerr<> m_log{"consumer"};
    };

    struct producer {
        producer(uint16_t p_idx, handlers *p_handlers,
                 std::condition_variable *p_cond)
            : m_idx('p' + std::to_string(p_idx)), m_handlers(p_handlers),
              m_cond(p_cond), m_msg(p_idx * m_max_msg_per_sender),
              m_start(p_idx * m_max_msg_per_sender),
              m_finish((p_idx * m_max_msg_per_sender + m_max_msg_per_sender) -
                       1),
              m_log("producer p" + std::to_string(p_idx)) {
            m_log.set_debug_level();
            DEB(m_log, m_idx, ": start value = ", m_start,
                ", finish value = ", m_finish, ", msg start = ", m_msg);
        }

        void operator()(std::shared_ptr<bool>) {
            m_log.set_debug_level();
            DEB(m_log, m_idx, " current ", m_msg);

            if (m_msg.get_value() == (m_finish + 1)) {
                DEB(m_log, m_idx, " produced ", m_max_msg_per_sender,
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
        handlers *m_handlers;
        std::condition_variable *m_cond;

        msg m_msg;
        value m_start;
        value m_finish;

        logger::cerr<> m_log;
    };

  private:
    static const uint16_t m_num_handlers;
    static const uint16_t m_num_senders;
    static const value m_max_msg_per_sender;
    static const std::chrono::milliseconds m_interval;
    static const std::chrono::seconds m_handlers_timeout;
    //    static const std::chrono::milliseconds m_handler_sleep;
    static const std::chrono::seconds m_sleeping_loop_timeout;
};

const uint16_t handlers_004::m_num_handlers{2};
const uint16_t handlers_004::m_num_senders{2};
const value handlers_004::m_max_msg_per_sender{3};
const std::chrono::milliseconds handlers_004::m_interval{150};
const std::chrono::seconds handlers_004::m_handlers_timeout{2};
// const std::chrono::milliseconds handlers_004::m_handler_sleep{300};
const std::chrono::seconds handlers_004::m_sleeping_loop_timeout{1};

int main(int argc, char **argv) {
    //  logger::set_debug_level();
    tester::test<> _test(argc, argv);
    run_test(_test, handlers_000);
    run_test(_test, handlers_001);
    run_test(_test, handlers_002);
    run_test(_test, handlers_003);
    run_test(_test, handlers_004);
}
