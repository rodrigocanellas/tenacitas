
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>

#include <concurrent/bus/internal//async_loop.h>
#include <concurrent/bus/thread.h>
#include <logger/bus/cerr.h>
#include <tester/bus/run.h>

struct work1
{
    bool operator()(uint32_t&& p_data)
    {
        data += p_data;
        cerr_test("1 -> ", data);
        return true;
    }
    uint32_t data = 0;
};

struct work2
{
    bool operator()(uint32_t&& p_data)
    {
        data = p_data;
        cerr_test("2 -> ", data);
        return true;
    }
    uint32_t data = 0;
};

struct provide
{
    std::pair<bool, uint32_t> operator()()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        if (m_data > 1000) {
            return std::make_pair(false, 0);
        }
        return std::make_pair(true, m_data++);
    }

  private:
    uint32_t m_data = 0;
};

typedef tenacitas::concurrent::bus::async_loop<uint32_t> async_loop_t;

struct async_loop_012
{
    bool operator()()
    {
        try {
            provide _provide;
            work1 _work_1;
            work2 _work_2;

            async_loop_t _al_1(
              [&_work_1](uint32_t&& p_data) {
                  return _work_1(std::move(p_data));
              },
              std::chrono::milliseconds(1000),
              []() { return false; },
              [&_provide]() -> std::pair<bool, uint32_t> {
                  return _provide();
              });

            async_loop_t _al_2(
              [&_work_2](uint32_t&& p_data) {
                  return _work_2(std::move(p_data));
              },
              std::chrono::milliseconds(1000),
              []() { return false; },
              [&_provide]() -> std::pair<bool, uint32_t> {
                  return _provide();
              });

            _al_1.run();
            _al_2.run();

            std::this_thread::sleep_for(std::chrono::seconds(10));

            _al_1.stop();
            _al_2.stop();

            cerr_test("work 1 data = ", _work_1.data);
            cerr_test("work 2 data = ", _work_2.data);

        } catch (std::exception& _ex) {
            cerr_error(_ex.what());
            return false;
        }

        return true;
    }
};

int
main(int argc, char** argv)
{
    run_test(
      async_loop_012,
      argc,
      argv,
      "\nWork 1 in 'async_loop' will print a provided uint32_t value, as well "
      "as Work 2."

      "\nProvider will provide incremente uint32_t values, to a Work 1 and "
      "Work 2 objects, until 1000, with a 200 ms sleep."

      "\nMain thread will run a 'sleeping_loop'; stop for 10 secs."

      "\nThe final data for Work 1 and Work 2 is unpredictable.");
}
