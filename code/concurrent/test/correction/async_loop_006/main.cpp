
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
        std::this_thread::sleep_for(std::chrono::milliseconds(1001));
        cerr_test(p_data);
        return true;
    }
};

struct provide
{
    std::pair<bool, uint32_t> operator()()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (m_data > 200) {
            return std::make_pair(false, 0);
        }
        return std::make_pair(true, m_data++);
    }
    uint32_t m_data = 0;
};

typedef tenacitas::concurrent::business::async_loop<uint32_t> async_loop_t;

struct async_loop_006
{
    bool operator()()
    {
        try {

            async_loop_t _async_loop(work1(),
                                     std::chrono::milliseconds(1000),
                                     []() { return false; },
                                     provide());

            _async_loop.run();

            std::this_thread::sleep_for(std::chrono::seconds(4));

            _async_loop.stop();

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
      async_loop_006,
      argc,
      argv,
      "\nWork in 'async_loop' will print a provided uint32_t value, with a "
      "1001 ms sleep, which will cause a timeout."

      "\nProvider will provide uint32_t values, until 1000, with a 200 ms "
      "sleep."

      "\nMain thread will stop for 4 secs; 'async_loop' stops."

      "\n4 timeouts warnings will be printed");
}
