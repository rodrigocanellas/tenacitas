
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <calendar/business/epoch.h>
#include <concurrent/business/sleeping_loop.h>
#include <concurrent/business/traits.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

struct msg
{

    friend std::ostream& operator<<(std::ostream&, const msg&);

    msg() = default;

    msg(int32_t p_i, float p_f, std::string p_s)
      : m_i(p_i)
      , m_f(p_f)
      , m_s(p_s)
    {}

    msg(msg&&) noexcept = default;
    msg(const msg&) = default;

    msg& operator=(msg&& p_msg) noexcept
    {
        if (this != &p_msg) {
            m_i = std::move(p_msg.m_i);
            m_f = std::move(p_msg.m_f);
            m_s = std::move(p_msg.m_s);
        }
        return *this;
    }

    msg& operator=(const msg&) = delete;

    ~msg() = default;

  private:
    int32_t m_i = 0;
    float m_f = 0.0;
    std::string m_s;
};

std::ostream&
operator<<(std::ostream& p_out, const msg& p_msg)
{
    p_out << p_msg.m_i << "-" << p_msg.m_f << "- '" << p_msg.m_s << "'";
    return p_out;
}

typedef tenacitas::concurrent::business::sleeping_loop<msg> loop_t;

struct provider
{
    bool data(msg* p_msg)
    {
        std::stringstream m_stream;
        ++m_i;
        m_stream << "(" << m_i << "," << static_cast<float>(m_i) * m_f << ")";
        m_s = m_stream.str();
        *p_msg = msg(m_i, m_f, m_s);
        return true;
    }

  private:
    int32_t m_i = 0;
    float m_f = 3.14f;
    std::string m_s = "";
};

struct sleeping_loop_013
{

    bool operator()()
    {
        provider _provider;

        loop_t _loop_1(
          std::chrono::milliseconds(1000),
          [this](msg&& p_msg) {
              if ((this->m_i % 2) == 0) {
                  cerr_test(
                    tenacitas::calendar::business::epoch::millisecs(), ", '", p_msg, "'");
              } else {
                  cerr_test("m_i = ", m_i, ". Sleeping for a while...");
                  std::this_thread::sleep_for(std::chrono::milliseconds(101));
              }
              m_i = m_i + 3;
              return true;
          },
          std::bind(
            &provider::data, std::ref(_provider), std::placeholders::_1),
          std::chrono::milliseconds(100));

        _loop_1.run();

        std::this_thread::sleep_for(std::chrono::seconds(12));

        cerr_test("stopping");
        _loop_1.stop();

        return true;
    }

  private:
    int16_t m_i = { 13 };
};

int
main(int argc, char** argv)
{
    run_test(sleeping_loop_013,
             argc,
             argv,
             "'sleeping_loop' with 1000 ms of interval, work time out of 100 "
             "ms, and the work function will sleep for 101 ms when a integer "
             "attribute, starting from 13, and incremented by 3, is even, "
             "causing a timeout.\nThe main thread will sleep for 12 seconds, "
             "then the 'sleeping_loop' will be stopped by calling 'stop'");
}
