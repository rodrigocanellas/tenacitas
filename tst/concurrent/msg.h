#ifndef TENACITAS_CONCURRENT_TST_MSG_A_H
#define TENACITAS_CONCURRENT_TST_MSG_A_H

#include <chrono>
#include <cstdint>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <tuple>
#include <vector>

#include <tenacitas/concurrent.h>
#include <tenacitas/number.h>

namespace tenacitas {
namespace concurrent {
namespace test {

typedef char msg_id;
typedef uint16_t pool_num;
typedef uint16_t sub_id;
typedef uint16_t instance_id;
typedef uint16_t publish_id;
typedef uint32_t value;

typedef concurrent::sleeping_loop_t<void> publisher;

template <msg_id id = 'A'> struct msg {

  explicit msg(value p_value = 0)
      : m_counter(p_value), m_up(up()), m_down(down()), m_d(2.5 * m_counter) {}

  friend std::ostream &operator<<(std::ostream &p_out, const msg &p_msg) {
    p_out << "[" << id << ","
          << tenacitas::number::format<value>(p_msg.m_counter) << ","
          << p_msg.m_up << "," << p_msg.m_down << "," << p_msg.m_d << "]";
    return p_out;
  }

  inline value get_value() const { return m_counter; }

  msg &operator++() {
    ++m_counter;
    m_up = up();
    m_down = down();
    m_d = 2.5 * m_counter;
    return *this;
  }

private:
  inline std::string up() {
    return tenacitas::number::format<value>(std::numeric_limits<value>::min() +
                                            m_counter);
  }

  inline std::string down() {
    return tenacitas::number::format<value>(std::numeric_limits<value>::max() -
                                            m_counter);
  }

private:
  value m_counter{0};
  std::string m_up;
  std::string m_down;
  double m_d;
};

concurrent::id pool_id(msg_id p_msg_id, pool_num p_pool_num) {
  return concurrent::id(std::string("pool ") + std::string(1, p_msg_id) +
                        std::string(" ") + std::to_string(p_pool_num));
}

typedef msg<'A'> msg_a;
typedef msg<'B'> msg_b;
typedef msg<'C'> msg_c;
typedef msg<'D'> msg_d;
typedef msg<'E'> msg_e;

struct update {
  update(msg_id p_msg_id, pool_num p_pool_num, sub_id p_sub_id,
         instance_id p_instance_id, value p_number)
      : m_msg_id(p_msg_id), m_pool_num(p_pool_num), m_sub_id(p_sub_id),
        m_instance_id(p_instance_id), m_number(p_number) {}

  update()
      : m_msg_id(std::numeric_limits<msg_id>::max()),
        m_pool_num(std::numeric_limits<pool_num>::max()),
        m_sub_id(std::numeric_limits<sub_id>::max()),
        m_instance_id(std::numeric_limits<instance_id>::max()),
        m_number(std::numeric_limits<value>::max()) {}

  bool operator<(const update &p_update) const {
    if (m_msg_id < p_update.m_msg_id) {
      return true;
    }

    if (m_msg_id > p_update.m_msg_id) {
      return false;
    }

    if (m_sub_id < p_update.m_sub_id) {
      return true;
    }

    if (m_sub_id > p_update.m_sub_id) {
      return false;
    }

    if (m_instance_id < p_update.m_instance_id) {
      return true;
    }

    return false;
  }

  bool operator==(const update &p_update) const {
    return ((m_msg_id == p_update.m_msg_id) &&
            (m_sub_id == p_update.m_sub_id) &&
            (m_instance_id == p_update.m_instance_id));
  }

  friend std::ostream &operator<<(std::ostream &p_out, const update &p_update) {
    p_out << "(" << p_update.m_msg_id << "," << p_update.m_pool_num << ","
          << p_update.m_sub_id << "," << p_update.m_instance_id << ","
          << p_update.m_number << ")";

    return p_out;
  }

  msg_id m_msg_id;
  pool_num m_pool_num;
  sub_id m_sub_id;
  instance_id m_instance_id;
  value m_number;
};

struct end_publishing {
  end_publishing()
      : m_msg_id(std::numeric_limits<msg_id>::max()),
        m_publish_id(std::numeric_limits<publish_id>::max()),
        m_last(std::numeric_limits<value>::max()) {}

  end_publishing(msg_id p_msg_id, publish_id p_publish_id, value p_last)
      : m_msg_id(p_msg_id), m_publish_id(p_publish_id), m_last(p_last) {}

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const end_publishing &p_end_publishing) {
    p_out << "(" << p_end_publishing.m_msg_id << ", "
          << p_end_publishing.m_publish_id << ", " << p_end_publishing.m_last
          << ")";
    return p_out;
  }

  msg_id m_msg_id;
  publish_id m_publish_id;
  value m_last;
};

typedef concurrent::messenger_t<msg_a> messenger_a;
typedef concurrent::messenger_t<msg_b> messenger_b;
typedef concurrent::messenger_t<msg_c> messenger_c;
typedef concurrent::messenger_t<msg_d> messenger_d;
typedef concurrent::messenger_t<msg_e> messenger_e;
typedef concurrent::messenger_t<update> messenger_update;
typedef concurrent::messenger_t<end_publishing> messenger_end_publishing;

template <msg_id id> struct subscriber {
  inline subscriber(
      pool_num p_pool_num, sub_id p_sub_id,
      std::function<void(const msg<id> &)> p_function =
          [](const msg<id> &) -> void {})
      : m_pool_num(p_pool_num), m_sub_id(p_sub_id), m_instance_id(0),
        m_sleep(100ms), m_function(p_function), m_log(log_name()) {}

  inline subscriber(
      pool_num p_pool_num, sub_id p_sub_id, std::chrono::milliseconds p_sleep,
      instance_id p_instance,
      std::function<void(const msg<id> &)> p_function =
          [](const msg<id> &) -> void {})
      : m_pool_num(p_pool_num), m_sub_id(p_sub_id), m_instance_id(p_instance),
        m_sleep(p_sleep), m_function(p_function), m_log(log_name()),
        m_actual_sleep(true) {}

  inline subscriber(
      pool_num p_pool_num, sub_id p_sub_id, std::chrono::milliseconds p_sleep,
      std::function<void(const msg<id> &)> p_function =
          [](const msg<id> &) -> void {})
      : m_pool_num(p_pool_num), m_sub_id(p_sub_id), m_instance_id(1),
        m_sleep(p_sleep), m_function(p_function), m_log(log_name()),
        m_actual_sleep(true) {}

  inline subscriber(
      pool_num p_pool_num, sub_id p_sub_id, instance_id p_instance,
      std::function<void(const msg<id> &)> p_function =
          [](const msg<id> &) -> void {})
      : m_pool_num(p_pool_num), m_sub_id(p_sub_id), m_instance_id(p_instance),
        m_sleep(100ms), m_function(p_function), m_log(log_name()) {}

  inline msg_id get_msg_id() const { return id; }

  inline sub_id get_id() const { return m_sub_id; }

  inline instance_id get_instance() const { return m_instance_id; }

  inline void set_sleep(std::chrono::milliseconds p_sleep) {
    m_sleep = p_sleep;
    m_actual_sleep = true;
  }

  inline value total() const { return m_number; }

  inline value inc() {
    ++m_number;
    return m_number;
  };

  ~subscriber() {
    if (m_number > 0) {
      INF(m_log, "consumed: ", m_number);
    }
  }

  void operator()(const msg<id> &p_msg) {
    INF(m_log, "msg: ", p_msg, ", counter:", m_number);
    inc();
    messenger_update::publish(
        {id, m_pool_num, m_sub_id, m_instance_id, p_msg.get_value()});
    m_function(p_msg);
    if (m_actual_sleep) {
      std::this_thread::sleep_for(m_sleep);
    }
  };

private:
  std::string log_name() {
    return ("subscriber " + std::string(1, id) + "-" +
            std ::to_string(m_pool_num) + "-" + std::to_string(m_sub_id) + "-" +
            std::to_string(m_instance_id));
  }

private:
  pool_num m_pool_num;
  sub_id m_sub_id;
  instance_id m_instance_id;
  std::chrono::milliseconds m_sleep;
  std::function<void(const msg<id> &)> m_function;
  logger::cerr<> m_log;
  value m_number{0};
  bool m_actual_sleep{false};
};

template <msg_id id> struct publish {
  publish() = delete;
  publish(publish_id p_publish_id, value p_max)
      : m_publish_id(p_publish_id), m_msg(), m_max(p_max),
        m_log("publish " + std::string(1, id) + std::string("-") +
              std::to_string(p_publish_id)) {}

  void operator()() {
    if (m_ended) {
      return;
    }

    if (m_msg.get_value() > m_max) {
      m_ended = true;
      DEB(m_log, "sending end of publishing");
      messenger_end_publishing::publish({id, m_publish_id, m_msg.get_value()});
      return;
    }

    DEB(m_log, "publishing ", m_msg);
    concurrent::messenger_t<msg<id>>::publish(m_msg);
    ++m_msg;
  }

  value get_max() const { return m_max; }

private:
  publish_id m_publish_id;
  msg<id> m_msg;
  const value m_max;
  bool m_ended{false};
  logger::cerr<> m_log;
};

template <msg_id id> struct publish_list {

  static publish<id> &add(publish<id> &&p_publish) {
    m_list.push_back(std::move(p_publish));
    return m_list.back();
  }

  static value get_total() {
    value _acc{0};
    for (const publish<id> &_pub : m_list) {
      _acc += _pub.get_max();
    }
    return _acc;
  }

private:
  typedef std::vector<publish<id>> list;

private:
  static list m_list;
};

template <msg_id id> struct subscriber_list {

  static publish<id> &add(subscriber<id> &&p_subscriber) {
    m_list.push_back(std::move(p_subscriber));
    return m_list.back();
  }

private:
  typedef std::vector<subscriber<id>> list;

private:
  static list m_list;
};

template <msg_id id> std::vector<publish<id>> publish_list<id>::m_list;

struct test_base {

  test_base(const char *p_name) : m_log(p_name) {

    messenger_end_publishing::add_subscriber(
        messenger_end_publishing::add_worker_pool(2s),
        [this](const end_publishing &p_end_publishing) -> void {
          on_end_publishing(p_end_publishing);
        });

    messenger_update::add_subscriber(
        messenger_update::add_worker_pool(2s),
        [this](const update &p_update) -> void { on_update(p_update); });
  }

  template <msg_id id, typename t_time>
  void add_publisher(t_time p_interval, publish_id p_publish_id, value p_max) {
    publish<id> _publish(p_publish_id, p_max);
    publish<id> &_added = publish_list<id>::add(std::move(_publish));
    publisher _publisher(500ms, p_interval, [&_added]() -> void { _added(); });
    m_publishers.push_back(std::move(_publisher));
    m_publishing_ended.push_back({id, p_publish_id, false});
  }

  template <msg_id id, typename t_time>
  void add_pool(pool_num p_pool_num, const concurrent::priority &p_priority,
                t_time p_timeout) {
    concurrent::messenger_t<msg<id>>::add_worker_pool(pool_id(id, p_pool_num),
                                                      p_priority, p_timeout);
  }

  template <msg_id id>
  void add_subscriber(
      pool_num p_pool_num, sub_id p_sub_id,
      std::function<void(const msg<id> &)> p_function =
          [](const msg<id> &) -> void {}) {
    concurrent::messenger_t<msg<id>>::add_subscriber(
        pool_id(id, p_pool_num),
        subscriber<id>(p_pool_num, p_sub_id, p_function));
    update_totals<id>(p_pool_num);
  }

  template <msg_id id, typename t_time>
  void add_subscriber(
      pool_num p_pool_num, sub_id p_sub_id, t_time p_sleep,
      std::function<void(const msg<id> &)> p_function =
          [](const msg<id> &) -> void {}) {
    concurrent::messenger_t<msg<id>>::add_subscriber(
        pool_id(id, p_pool_num),
        subscriber<id>(p_pool_num, p_sub_id, p_sleep, p_function));
    update_totals<id>(p_pool_num);
  }

  template <msg_id id, typename t_time>
  void add_subscriber(
      pool_num p_pool_num, sub_id p_sub_id, t_time p_sleep,
      instance_id p_instance_id,
      std::function<void(const msg<id> &)> p_function =
          [](const msg<id> &) -> void {}) {
    concurrent::messenger_t<msg<id>>::add_subscriber(
        pool_id(id, p_pool_num), subscriber<id>(p_pool_num, p_sub_id, p_sleep,
                                                p_instance_id, p_function));
    update_totals<id>(p_pool_num);
  }

  template <msg_id id, typename t_time>
  void add_subscriber(
      pool_num p_pool_num, sub_id p_sub_id, instance_id p_instance_id,
      std::function<void(const msg<id> &)> p_function =
          [](const msg<id> &) -> void {}) {
    concurrent::messenger_t<msg<id>>::add_subscriber(
        pool_id(id, p_pool_num),
        subscriber<id>(p_pool_num, p_sub_id, p_instance_id, p_function));
    update_totals<id>(p_pool_num);
  }

  template <typename t_time>
  bool operator()(t_time p_wait_for_all_produced_to_be_consumed) {

    DEB(m_log, "trying to start publishers");
    if (m_publishers.empty()) {
      ERR(m_log, "no publisher created");
      return false;
    }

    start_publishing();

    DEB(m_log, "wait for finish");
    {
      std::unique_lock<std::mutex> _lock(m_mutex_end_publishing);
      m_cond_end_publishing.wait(_lock);
    }

    DEB(m_log, "finished");

    //    while (true) {
    //      if (all_produced_was_consumed()) {
    //        INF(m_log, "ALL PRODUCED WAS CONSUMED");
    //        return true;
    //      }
    //      INF(m_log, "still waiting for all produced to be consumed");
    //      std::this_thread::sleep_for(50ms);
    //    }
    {
      DEB(m_log, "waiting for all produced to be consumed");
      std::unique_lock<std::mutex> _lock(m_mutex_all_consumed);
      if (m_cond_all_consumed.wait_for(
              _lock, p_wait_for_all_produced_to_be_consumed,
              [this]() -> bool { return m_all_consumed; })) {
        if (!m_all_consumed) {
          ERR(m_log, "waited a lot, but not all produced was consumed");
          return false;
        }
      }
    }

    INF(m_log, "ALL PRODUCED WAS CONSUMED");

    return true;
  };

private:
  typedef std::vector<publisher> publishers;
  typedef typename publishers::iterator publishers_iterator;
  typedef std::vector<std::tuple<msg_id, publish_id, bool>> publishing_ended;
  typedef std::map<std::pair<msg_id, pool_num>, value> totals;

private:
  void start_publishing() {
    for (publisher &_publisher : m_publishers) {
      _publisher.start();
    }
  }

  bool all_produced_was_consumed() {

    value _expected[5] = {0, 0, 0, 0, 0};

    for (const totals::value_type &_value : m_totals) {
      switch (_value.first.first) {
      case 'A':
        _expected[0] += _value.second;
        break;
      case 'B':
        _expected[1] += _value.second;
        break;
      case 'C':
        _expected[2] += _value.second;
        break;
      case 'D':
        _expected[3] += _value.second;
        break;
      case 'E':
        _expected[4] += _value.second;
        break;
      }
    }

    value _current[5] = {0, 0, 0, 0, 0};
    for (const update &_update : m_updates) {
      switch (_update.m_msg_id) {
      case 'A':
        _current[0] += _update.m_number;
        break;
      case 'B':
        _current[1] += _update.m_number;
        break;
      case 'C':
        _current[2] += _update.m_number;
        break;
      case 'D':
        _current[3] += _update.m_number;
        break;
      case 'E':
        _current[4] += _update.m_number;
        break;
      }
    }

    INF(m_log, "expected: {", _expected[0], ",", _expected[1], ",",
        _expected[2], ",", _expected[3], ",", _expected[4], "} ", "current: {",
        _current[0], ",", _current[1], ",", _current[2], ",", _current[3], ",",
        _current[4], "}");

    for (auto _i = 0; _i < 5; ++_i) {
      if (_expected[_i] != (_current[_i])) {
        return false;
      }
    }

    return true;
  }

  template <msg_id id> void update_totals(pool_num p_pool_num) {
    //    bool _found{false};

    m_totals[{id, p_pool_num}] = publish_list<id>::get_total();

    //    for (totals::value_type &_value : m_totals) {
    //      if ((std::get<0>(_value) == id) && (std::get<1>(_value) ==
    //      p_pool_num)) {
    //        DEB(m_log, "total for msg ", std::get<0>(_value), ", pool ",
    //            std::get<0>(_value), " already inserted");
    //        return;
    //      }
    //    }

    //    if (!_found) {

    //      std::lock_guard<std::mutex> _lock(m_mutex_totals);
    //      auto _total{publish_list<id>::get_total()};
    //      DEB(m_log, "another consumer for ", id, ", in pool ", p_pool_num,
    //          " adding ", _total);
    //      m_totals.push_back({id, p_pool_num, _total});
    //    }
  }

  void on_end_publishing(const end_publishing &p_end_publishing) {
    DEB(m_log, "end of publishing for ", p_end_publishing);
    bool _all_ended{true};
    for (publishing_ended::value_type &_tuple : m_publishing_ended) {
      if ((std::get<0>(_tuple) == p_end_publishing.m_msg_id) &&
          (std::get<1>(_tuple) == p_end_publishing.m_publish_id)) {
        std::get<2>(_tuple) = true;
        //        for (publisher & _publisher)
        if (!_all_ended) {
          break;
        }
      }
      _all_ended = _all_ended && std::get<2>(_tuple);
    }
    if (_all_ended) {
      m_cond_end_publishing.notify_one();
    }
  }

  void on_update(const update &p_update) {
    DEB(m_log, "update = ", p_update);
    std::lock_guard<std::mutex> _lock(m_mutex_update);
    for (update &_update : m_updates) {
      if ((_update.m_msg_id == p_update.m_msg_id) &&
          (_update.m_pool_num == p_update.m_pool_num)) {
        _update.m_number = p_update.m_number;
        if (all_produced_was_consumed()) {
          DEB(m_log, "notifying all produced was consumed");
          m_all_consumed = true;
          m_cond_all_consumed.notify_one();
        }
        return;
      }
    }
    m_updates.push_back(p_update);
  }

private:
  publishers m_publishers;
  publishing_ended m_publishing_ended;
  std::mutex m_mutex_end_publishing;
  std::condition_variable m_cond_end_publishing;
  logger::cerr<> m_log;
  std::vector<update> m_updates;
  std::mutex m_mutex_update;
  std::mutex m_mutex_totals;
  totals m_totals;
  std::condition_variable m_cond_all_consumed;
  std::mutex m_mutex_all_consumed;
  bool m_all_consumed{false};
};

} // namespace test
} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_TST_MSG_A_H
