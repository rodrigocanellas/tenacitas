
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>

#include <concurrent/inc/async_loop.h>
#include <concurrent/inc/sleeping_loop_controller.h>
#include <concurrent/bus/thread.h>
#include <logger/bus/cerr.h>
#include <tester/bus/run.h>

typedef tenacitas::concurrent::bus::sleeping_loop_controller controller_t;
typedef tenacitas::concurrent::bus::async_loop<controller_t, int16_t>
  async_loop_t;
typedef tenacitas::concurrent::bus::thread thread_t;

struct provider
{
    bool data(int16_t* p_value)
    {
        *p_value = 12;
        return true;
    }
};

struct work1
{

    bool operator()(const int16_t& p_value)
    {
        if (m_counter > 100) {
            return false;
        }
        std::stringstream stream;
        stream << "work1: (" << m_counter << ", " << std::time(nullptr) << ")";
        cerr_debug(stream.str());
        m_counter += p_value;
        return true;
    }

  private:
    int32_t m_counter = 0;
};

struct test010
{
    bool operator()()
    {
        try {
            cerr_debug("############ test010");
            async_loop_t sl1(
              controller_t::create(std::chrono::milliseconds(2000)),
              work1(),
              std::bind(&provider::data, provider(), std::placeholders::_1));
            sl1();

            cerr_debug("---- going to sleep 1");
            std::this_thread::sleep_for(std::chrono::seconds(10));
            cerr_debug("---- woke up 1");

            sl1.interrupt();

            async_loop_t sl2(std::move(sl1));

            cerr_debug("---- going to sleep 2");
            std::this_thread::sleep_for(std::chrono::seconds(15));
            cerr_debug("---- woke up 2");

            sl2.interrupt();
        } catch (std::exception& _ex) {
            cerr_error(_ex.what());
            return false;
        }
        return true;
    }
};

int
main()
{
    tenacitas::logger::bus::configure_cerr_log();
    cerr_set_debug();
    run(test010);
}
