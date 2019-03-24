
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>

#include <concurrent/business/internal//async_loop.h>
#include <concurrent/business/thread.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

struct work1
{
    bool operator()(uint32_t&& p_data)
    {
        data = p_data;
        cerr_test(p_data);
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

    uint32_t get_data() const { return m_data; }

  private:
    uint32_t m_data = 1;
};

typedef tenacitas::concurrent::business::async_loop<uint32_t> async_loop_t;

struct async_loop_007
{
    bool operator()()
    {
        try {
            work1 _work;
            provide _provide;
            async_loop_t _async_loop_1(
              [&_work](uint32_t&& p_data) { return _work(std::move(p_data)); },
              std::chrono::milliseconds(1000),
              []() { return false; },
              [&_provide]() -> std::pair<bool, uint32_t> {
                  return _provide();
              });

            _async_loop_1.run();

            std::this_thread::sleep_for(std::chrono::seconds(4));

            _async_loop_1.stop();

            cerr_test("work data = ",
                      _work.data,
                      ", provider data = ",
                      _provide.get_data());

            if (_work.data != 19) {
                cerr_error("wrong value for data, it should be 19");
                return false;
            }

            cerr_test("work data = ",
                      _work.data,
                      ", provider data = ",
                      _provide.get_data());

            async_loop_t _async_loop_2(
              [&_work](uint32_t&& p_data) { return _work(std::move(p_data)); },
              _async_loop_1.get_timeout(),
              _async_loop_1.get_break(),
              [&_provide]() -> std::pair<bool, uint32_t> {
                  return _provide();
              });

            cerr_test("work data = ",
                      _work.data,
                      ", provider data = ",
                      _provide.get_data());

            std::this_thread::sleep_for(std::chrono::seconds(2));

            cerr_test("work data = ",
                      _work.data,
                      ", provider data = ",
                      _provide.get_data());

            _async_loop_2.run();

            std::this_thread::sleep_for(std::chrono::seconds(3));

            _async_loop_2.stop();

            cerr_test("work data = ",
                      _work.data,
                      ", provider data = ",
                      _provide.get_data());

            if (_work.data != 34) {
                cerr_error("wrong value for data, it should be 35");
                return false;
            }

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
      async_loop_007,
      argc,
      argv,
      "\nWork in 'async_loop' will print a provided uint32_t value."

      "\nProvider will provide uint32_t values, until 1000, with a 200 ms "
      "sleep."

      "\nFirst 'async_loop' will run, main thread will sleep for 4 secs, then "
      "'async_loop' will stop."

      "\nA second 'async_loop' is created, based on first one. Main thread "
      "sleeps for 2 secs, second 'async_loop' will run. Main thread sleeps for "
      "3 secs, and second 'async_loop' stops. "

      "\nData in work should be 34.");
}
