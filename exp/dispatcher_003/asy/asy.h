#ifndef TENACITAS_TST_ASYNC_DISPATCHER_EVT_H
#define TENACITAS_TST_ASYNC_DISPATCHER_EVT_H

#include <iostream>

#include <tenacitas.lib/exp/dispatcher_003/dom/dat.h>
#include <tenacitas.lib/asy/dispatcher.h>
#include <tenacitas.lib/asy/queue_id.h>
#include <tenacitas.lib/fmt/format_number.h>
#include <tenacitas.lib/sof/id.h>

namespace asy {

struct pressure_generated {
  pressure_generated() = default;
  pressure_generated(const pressure_generated &) = default;
  pressure_generated(pressure_generated &&) = default;
  pressure_generated &operator=(const pressure_generated &) = default;
  pressure_generated &operator=(pressure_generated &&) = default;
  explicit pressure_generated(tenacitas::lib::sof::id p_generator,
                              dom::pressure p_value = 1.5)
      : m_generator(p_generator), m_pressure(p_value) {}

  inline bool operator==(const pressure_generated &p_pressure) const {
    return m_pressure == p_pressure.m_pressure;
  }

  inline bool operator!=(const pressure_generated &p_pressure) const {
    return m_pressure != p_pressure.m_pressure;
  }

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const pressure_generated &p_pressure) {
    p_out << "'generated (" << p_pressure.m_generator << ','
          << tenacitas::lib::fmt::format_number(p_pressure.m_pressure) << ")'";
    return p_out;
  }

  pressure_generated &operator++() {
    ++m_pressure;
    return *this;
  }

  pressure_generated &operator++(int) {
    m_pressure++;
    return *this;
  }

  pressure_generated &operator+=(const pressure_generated &p_pressure) {
    m_pressure += p_pressure.m_pressure;
    return *this;
  }

  pressure_generated &operator-=(const pressure_generated &p_pressure) {
    m_pressure -= p_pressure.m_pressure;
    return *this;
  }

private:
  tenacitas::lib::sof::id m_generator;
  dom::pressure m_pressure{0};
};

struct pressure_sent {
  pressure_sent() = default;
  pressure_sent(tenacitas::lib::sof::id p_generator)
      : m_generator(p_generator) {}

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const pressure_sent &p_pressure_sent) {
    p_out << "'sent (" << p_pressure_sent.m_generator << ")'";
    return p_out;
  }

private:
  tenacitas::lib::sof::id m_generator;
};

struct pressure_handled {
  pressure_handled() = default;

  pressure_handled(tenacitas::lib::asy::queue_id p_queue,
                   tenacitas::lib::sof::id p_subscriber)
      : queue(p_queue), subscriber(p_subscriber) {}

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const pressure_handled &p_handled) {
    p_out << "'handled (" << p_handled.queue << "," << p_handled.subscriber
          << ")'";
    return p_out;
  }

  tenacitas::lib::asy::queue_id queue;

  tenacitas::lib::sof::id subscriber;
};

using dispatcher =
    tenacitas::lib::asy::dispatcher<pressure_generated, pressure_sent,
                                    pressure_handled>;

} // namespace asy

#endif // EVT_H
