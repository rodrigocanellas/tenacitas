#ifndef TENACITAS_ASYNC_TST_MSG_A_H
#define TENACITAS_ASYNC_TST_MSG_A_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <tuple>
#include <vector>

#include <tenacitas/async.h>
#include <tenacitas/internal/internal_async.h>
#include <tenacitas/number.h>
#include <tenacitas/type.h>

using namespace tenacitas;

typedef uint8_t message_id;
typedef uint16_t pool_num;
typedef uint16_t sub_id;
typedef uint16_t publish_id;
typedef uint32_t value;

// data message
template <message_id msg_id = 0> struct msg_t {

    static const message_id id{msg_id};

    explicit msg_t(value p_value = 0)
        : m_counter(p_value), m_up(up()), m_down(down()), m_d(2.5 * m_counter) {
    }

    msg_t(const msg_t &p_msg)
        : m_counter(p_msg.m_counter), m_up(p_msg.m_up), m_down(p_msg.m_down),
          m_d(p_msg.m_d) {}

    msg_t(msg_t &&p_msg)
        : m_counter(p_msg.m_counter), m_up(std::move(p_msg.m_up)),
          m_down(std::move(p_msg.m_down)), m_d(p_msg.m_d) {}

    msg_t &operator=(const msg_t &p_msg) {
        if (this != &p_msg) {
            m_counter = p_msg.m_counter;
            m_up = p_msg.m_up;
            m_down = p_msg.m_down;
            m_d = p_msg.m_d;
        }
        return *this;
    }

    msg_t &operator=(msg_t &&p_msg) {
        if (this != &p_msg) {
            m_counter = p_msg.m_counter;
            m_up = std::move(p_msg.m_up);
            m_down = std::move(p_msg.m_down);
            m_d = p_msg.m_d;
        }
        return *this;
    }

    inline bool operator==(const msg_t &p_msg) const {
        return m_counter == p_msg.m_counter;
    }

    inline bool operator!=(const msg_t &p_msg) const {
        return m_counter != p_msg.m_counter;
    }

    friend std::ostream &operator<<(std::ostream &p_out, const msg_t &p_msg) {
        p_out << "[" << static_cast<uint16_t>(id) << ","
              << number::format<value>(p_msg.m_counter) << "," << p_msg.m_up
              << "," << p_msg.m_down << "," << p_msg.m_d << "]";
        return p_out;
    }

    inline value get_value() const { return m_counter; }

    msg_t &operator++() {
        ++m_counter;
        m_up = up();
        m_down = down();
        m_d = 2.5 * m_counter;
        return *this;
    }

    msg_t &operator++(int) {
        m_counter++;
        m_up = up();
        m_down = down();
        m_d = 2.5 * m_counter;
        return *this;
    }

  private:
    inline std::string up() {
        return number::format<value>(std::numeric_limits<value>::min() +
                                     m_counter);
    }

    inline std::string down() {
        return number::format<value>(std::numeric_limits<value>::max() -
                                     m_counter);
    }

  private:
    value m_counter{0};
    std::string m_up;
    std::string m_down;
    double m_d;
};

typedef msg_t<'A'> msg_a;
typedef msg_t<'B'> msg_b;
typedef msg_t<'C'> msg_c;
typedef msg_t<'D'> msg_d;
typedef msg_t<'E'> msg_e;

//// update message
// struct update {
//  update(msg_id p_msg_id, pool_num p_pool_num, sub_id p_sub_id, value
//  p_number)
//      : m_msg_id(p_msg_id), m_pool_num(p_pool_num), m_sub_id(p_sub_id),
//        m_number(p_number) {}

//  update()
//      : m_msg_id(std::numeric_limits<msg_id>::max()),
//        m_pool_num(std::numeric_limits<pool_num>::max()),
//        m_sub_id(std::numeric_limits<sub_id>::max()),
//        m_number(std::numeric_limits<value>::max()) {}

//  bool operator==(const update &p_update) const {
//    return (m_msg_id == p_update.m_msg_id) &&
//           (m_pool_num == p_update.m_pool_num) &&
//           (m_sub_id == p_update.m_sub_id);
//  }

//  bool operator<(const update &p_update) const {
//    if (m_msg_id < p_update.m_msg_id) {
//      return true;
//    }

//    if (m_msg_id > p_update.m_msg_id) {
//      return false;
//    }

//    if (m_pool_num < p_update.m_pool_num) {
//      return true;
//    }

//    if (m_pool_num > p_update.m_pool_num) {
//      return false;
//    }

//    if (m_sub_id < p_update.m_sub_id) {
//      return true;
//    }

//    return false;
//  }

//  friend std::ostream &operator<<(std::ostream &p_out, const update &p_update)
//  {
//    p_out << "(" << p_update.m_msg_id << "," << p_update.m_pool_num << ","
//          << p_update.m_sub_id << "," << p_update.m_number << ")";

//    return p_out;
//  }

//  msg_id m_msg_id;
//  pool_num m_pool_num;
//  sub_id m_sub_id;
//  value m_number;
//};

// typedef std::vector<update> updates;

// namespace internal {

// typedef async::sleeping_loop publisher;

// number::id pool_id(msg_id /*p_msg_id*/, pool_num /*p_pool_num*/) {
//  //  return async::id(std::string("pool ") + std::string(1, p_msg_id) +
//  //                   std::string(" ") + std::to_string(p_pool_num));
//  return number::id();
//}

//// end publishing message
// struct end_publishing {
//  end_publishing()
//      : m_msg_id(std::numeric_limits<msg_id>::max()),
//        m_publish_id(std::numeric_limits<publish_id>::max()),
//        m_last(std::numeric_limits<value>::max()) {}

//  end_publishing(msg_id p_msg_id, publish_id p_publish_id, value p_last)
//      : m_msg_id(p_msg_id), m_publish_id(p_publish_id), m_last(p_last) {}

//  friend std::ostream &operator<<(std::ostream &p_out,
//                                  const end_publishing &p_end_publishing) {
//    p_out << "(" << p_end_publishing.m_msg_id << ", "
//          << p_end_publishing.m_publish_id << ", " << p_end_publishing.m_last
//          << ")";
//    return p_out;
//  }

//  msg_id m_msg_id;
//  publish_id m_publish_id;
//  value m_last;
//};

//// generic subscriber
// template <msg_id id> struct subscriber {
//  inline subscriber(
//      pool_num p_pool_num, sub_id p_sub_id,
//      async::handler_t<msg_t<id>> p_function = [](type::ptr<bool>,
//                                                  msg_t<id> &&) -> void {})
//      : m_pool_num(p_pool_num), m_sub_id(p_sub_id), m_sleep(100ms),
//        m_function(p_function), m_log(log_name()) {}

//  inline subscriber(
//      pool_num p_pool_num, sub_id p_sub_id, std::chrono::milliseconds p_sleep,
//      async::handler_t<msg_t<id>> p_function = [](type ::ptr<bool>,
//                                                  msg_t<id> &&) -> void {})
//      : m_pool_num(p_pool_num), m_sub_id(p_sub_id), m_sleep(p_sleep),
//        m_function(p_function), m_log(log_name()), m_actual_sleep(true) {}

//  inline msg_id get_msg_id() const { return id; }

//  inline sub_id get_id() const { return m_sub_id; }

//  inline void set_sleep(std::chrono::milliseconds p_sleep) {
//    m_sleep = p_sleep;
//    m_actual_sleep = true;
//  }

//  inline value total() const { return m_number; }

//  inline value inc() {
//    ++m_number;
//    return m_number;
//  };

//  ~subscriber() = default;

//  void operator()(type::ptr<bool> p_bool, msg_t<id> &&p_msg) {
//    if (m_actual_sleep) {
//      std::this_thread::sleep_for(m_sleep);
//    } else {
//      inc();
//      INF(m_log, "msg: ", p_msg, ", counter:", m_number);
//      async::send<update>(update{id, m_pool_num, m_sub_id, m_number});
//      m_function(p_bool, std::move(p_msg));
//    }
//  };

// private:
//  std::string log_name() {
//    return ("subscriber " + std::string(1, id) + "-" +
//            std ::to_string(m_pool_num) + "-" + std::to_string(m_sub_id));
//  }

// private:
//  pool_num m_pool_num;
//  sub_id m_sub_id;
//  std::chrono::milliseconds m_sleep;
//  async::handler_t<msg_t<id>> m_function;
//  logger::cerr<> m_log;
//  value m_number{0};
//  bool m_actual_sleep{false};
//};

//// generic publisher
// template <msg_id id> struct publish {
//  publish() = delete;
//  publish(publish_id p_publish_id, value p_max)
//      : m_publish_id(p_publish_id), m_msg(), m_max(p_max),
//        m_log("publish " + std::string(1, id) + std::string("-") +
//              std::to_string(p_publish_id)) {}

//  void operator()(type::ptr<bool>) {
//    if (m_ended) {
//      return;
//    }

//    if (m_msg.get_value() >= m_max) {
//      m_ended = true;
//      DEB(m_log, "sending end of publishing");
//      async::send<end_publishing>(
//          end_publishing{id, m_publish_id, m_msg.get_value()});
//      return;
//    }

//    DEB(m_log, "publishing ", m_msg);
//    async::send<msg_t<id>>(m_msg);
//    ++m_msg;
//  }

//  value get_max() const { return m_max; }

// private:
//  publish_id m_publish_id;
//  msg_t<id> m_msg;
//  const value m_max;
//  bool m_ended{false};
//  logger::cerr<> m_log;
//};

//// list of generic publishers
// template <msg_id id> struct publish_list {

//  static publish<id> &add(publish<id> &&p_publish) {
//    m_list.push_back(std::move(p_publish));
//    return m_list.back();
//  }

//  static value get_total() {
//    value _acc{0};
//    for (const publish<id> &_pub : m_list) {
//      _acc += _pub.get_max();
//    }
//    return _acc;
//  }

// private:
//  typedef std::vector<publish<id>> list;

// private:
//  static list m_list;
//};

//// list of generic subscriber
// template <msg_id id> struct subscriber_list {

//  static publish<id> &add(subscriber<id> &&p_subscriber) {
//    m_list.push_back(std::move(p_subscriber));
//    return m_list.back();
//  }

// private:
//  typedef std::vector<subscriber<id>> list;

// private:
//  static list m_list;
//};

// template <msg_id id> std::vector<publish<id>> publish_list<id>::m_list;

//} // namespace internal

//// class to create tests using async namespace
// struct test_base {

//  test_base(const char *p_name) : m_log(p_name) {

//    async::add_handler<internal::end_publishing>(
//        [this](type::ptr<bool> p_bool,
//               internal::end_publishing &&p_end_publishing) -> void {
//          on_end_publishing(p_bool, std::move(p_end_publishing));
//        },
//        2s);
//    //    internal::messenger_end_publishing::add_handler(
//    //        internal::messenger_end_publishing::add_handlers(2s),
//    //        [this](const internal::end_publishing &p_end_publishing) -> void
//    //    {
//    //          on_end_publishing(p_end_publishing);
//    //        });

//    //    internal::messenger_update::add_handler(
//    //        internal::messenger_update::add_handlers(2s),
//    //        [this](const update &p_update) -> void { on_update(p_update);
//    //    });
//    async::add_handler<update>(
//        [this](type::ptr<bool> p_bool, update &&p_update) -> void {
//          on_update(p_bool, std::move(p_update));
//        },
//        2s);
//  }

//  template <msg_id id, typename t_time>
//  void add_pool(const t_time &p_timeout, const async::priority &p_priority) {
//    async::add_handlers<msg_t<id>>(p_timeout, p_priority);
//    //    //    async::internal::messenger_t<msg_t<id>,
//    //    async::priority>::add_handlers(
//    //    //        p_priority, p_timeout);
//  }

//  template <msg_id id, typename t_time>
//  void add_publisher(const number::id &p_owner, t_time p_interval,
//                     publish_id p_publish_id, value p_max) {
//    internal::publish<id> _publish(p_publish_id, p_max);
//    internal::publish<id> &_added =
//        internal::publish_list<id>::add(std::move(_publish));
//    internal::publisher _publisher(
//        p_owner, [&_added](type::ptr<bool> p_bool) -> void { _added(p_bool);
//        }, 500ms, p_interval);
//    m_publishers.push_back(std::move(_publisher));
//    m_publishing_ended.push_back({id, p_publish_id, false});
//  }

//  template <msg_id id>
//  void add_subscriber(
//      pool_num p_pool_num, sub_id p_sub_id,
//      async::handler_t<msg_t<id>> p_function = [](type::ptr<bool>,
//                                                  msg_t<id> &&) -> void {}) {
//    async::add_handler<msg_t<id>>(
//        internal::pool_id(id, p_pool_num),
//        internal::subscriber<id>(p_pool_num, p_sub_id, p_function));
//    update_totals<id>(p_pool_num);
//  }

//  template <msg_id id, typename t_time>
//  void add_subscriber(
//      pool_num p_pool_num, sub_id p_sub_id, t_time p_sleep,
//      async::handler_t<msg_t<id>> p_function = [](type::ptr<bool>,
//                                                  msg_t<id> &&) -> void {}) {
//    async::add_handler<msg_t<id>>(
//        internal::pool_id(id, p_pool_num),
//        internal::subscriber<id>(p_pool_num, p_sub_id, p_sleep, p_function));
//    update_totals<id>(p_pool_num);
//  }

//  template <typename t_time>
//  bool operator()(t_time p_wait_for_all_produced_to_be_consumed) {

//    DEB(m_log, "trying to start publishers");
//    if (m_publishers.empty()) {
//      ERR(m_log, "no publisher created");
//      return false;
//    }

//    start_publishing();

//    DEB(m_log, "wait for finish");
//    {
//      std::unique_lock<std::mutex> _lock(m_mutex_end_publishing);
//      m_cond_end_publishing.wait(_lock);
//    }

//    DEB(m_log, "finished");

//    {
//      DEB(m_log, "waiting for all produced to be consumed");
//      std::unique_lock<std::mutex> _lock(m_mutex_all_consumed);
//      if (m_cond_all_consumed.wait_for(
//              _lock, p_wait_for_all_produced_to_be_consumed,
//              [this]() -> bool { return m_all_consumed; })) {
//        if (!m_all_consumed) {
//          ERR(m_log, "waited a lot, but not all produced was consumed");
//          return false;
//        }
//      }
//    }

//    INF(m_log, "ALL PRODUCED WAS CONSUMED");

//    return true;
//  };

//  const updates &get_updates() {
//    std::sort(m_updates.begin(), m_updates.end());
//    return m_updates;
//  }

// private:
//  typedef std::vector<internal::publisher> publishers;
//  typedef typename publishers::iterator publishers_iterator;
//  typedef std::vector<std::tuple<msg_id, publish_id, bool>> publishing_ended;
//  typedef std::map<std::pair<msg_id, pool_num>, value> totals;

// private:
//  void start_publishing() {
//    for (internal::publisher &_publisher : m_publishers) {
//      _publisher.start();
//    }
//  }

//  bool all_produced_was_consumed() {

//    value _expected[5] = {0, 0, 0, 0, 0};

//    for (const totals::value_type &_value : m_totals) {
//      switch (_value.first.first) {
//      case 'A':
//        _expected[0] += _value.second;
//        break;
//      case 'B':
//        _expected[1] += _value.second;
//        break;
//      case 'C':
//        _expected[2] += _value.second;
//        break;
//      case 'D':
//        _expected[3] += _value.second;
//        break;
//      case 'E':
//        _expected[4] += _value.second;
//        break;
//      }
//    }

//    value _current[5] = {0, 0, 0, 0, 0};
//    for (const update &_update : m_updates) {
//      switch (_update.m_msg_id) {
//      case 'A':
//        _current[0] += _update.m_number;
//        break;
//      case 'B':
//        _current[1] += _update.m_number;
//        break;
//      case 'C':
//        _current[2] += _update.m_number;
//        break;
//      case 'D':
//        _current[3] += _update.m_number;
//        break;
//      case 'E':
//        _current[4] += _update.m_number;
//        break;
//      }
//    }

//    INF(m_log, "expected: {", _expected[0], ",", _expected[1], ",",
//        _expected[2], ",", _expected[3], ",", _expected[4], "} current: {",
//        _current[0], ",", _current[1], ",", _current[2], ",", _current[3],
//        ",", _current[4], "}");

//    for (auto _i = 0; _i < 5; ++_i) {
//      if (_expected[_i] != (_current[_i])) {
//        return false;
//      }
//    }

//    return true;
//  }

//  template <msg_id id> void update_totals(pool_num p_pool_num) {
//    m_totals[{id, p_pool_num}] = internal::publish_list<id>::get_total();
//  }

//  void on_end_publishing(type::ptr<bool>,
//                         internal::end_publishing &&p_end_publishing) {
//    DEB(m_log, "end of publishing for ", p_end_publishing);
//    bool _all_ended{true};
//    for (publishing_ended::value_type &_tuple : m_publishing_ended) {
//      if ((std::get<0>(_tuple) == p_end_publishing.m_msg_id) &&
//          (std::get<1>(_tuple) == p_end_publishing.m_publish_id)) {
//        std::get<2>(_tuple) = true;
//        //        for (publisher & _publisher)
//        if (!_all_ended) {
//          break;
//        }
//      }
//      _all_ended = _all_ended && std::get<2>(_tuple);
//    }
//    if (_all_ended) {
//      m_cond_end_publishing.notify_one();
//    }
//  }

//  void on_update(std::shared_ptr<bool>, const update &p_update) {
//    DEB(m_log, "update = ", p_update);
//    std::lock_guard<std::mutex> _lock(m_mutex_update);
//    for (update &_update : m_updates) {
//      if (_update == p_update) {
//        _update.m_number = p_update.m_number;
//        if (all_produced_was_consumed()) {
//          DEB(m_log, "notifying all produced was consumed");
//          m_all_consumed = true;
//          m_cond_all_consumed.notify_one();
//        }
//        return;
//      }
//    }
//    m_updates.push_back(p_update);
//  }

// private:
//  publishers m_publishers;
//  publishing_ended m_publishing_ended;
//  std::mutex m_mutex_end_publishing;
//  std::condition_variable m_cond_end_publishing;
//  logger::cerr<> m_log;
//  updates m_updates;
//  std::mutex m_mutex_update;
//  std::mutex m_mutex_totals;
//  totals m_totals;
//  std::condition_variable m_cond_all_consumed;
//  std::mutex m_mutex_all_consumed;
//  bool m_all_consumed{false};
//};

#endif // TENACITAS_ASYNC_TST_MSG_A_H