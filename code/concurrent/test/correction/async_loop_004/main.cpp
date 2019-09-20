
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>

#include <concurrent/business/internal/async_loop.h>
#include <concurrent/business/internal/log.h>
#include <concurrent/business/thread.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

struct work1
{
    bool operator()(uint32_t&& p_data)
    {
        using namespace tenacitas::logger::business;
        data = p_data;
        concurrent_log_test(log, p_data);
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
        return std::make_pair(true, ++m_data);
    }

  private:
    uint32_t m_data = 0;
};

typedef tenacitas::concurrent::business::
  async_loop_t<uint32_t, tenacitas::logger::business::log>
    async_loop_t;

struct async_loop_004
{
    bool operator()()
    {
        using namespace tenacitas::logger::business;
        try {
            work1 _work;
            async_loop_t _async_loop(
              [&_work](uint32_t&& p_data) { return _work(std::move(p_data)); },
              std::chrono::milliseconds(1000),
              []() { return false; },
              provide());

            _async_loop.run();

            std::this_thread::sleep_for(std::chrono::seconds(20));

            _async_loop.stop();

            concurrent_log_test(log, "data 1 ", _work.data);
            if (_work.data != 99) {
                concurrent_log_error(log,
                                     "wrong value for data, it should be 99");
                return false;
            }

            std::this_thread::sleep_for(std::chrono::seconds(4));

            _async_loop.run();

            std::this_thread::sleep_for(std::chrono::seconds(20));

            _async_loop.stop();

            concurrent_log_test(log, "data 2 ", _work.data);
            if (_work.data != 199) {
                concurrent_log_error(log,
                                     "wrong value for data, it should be 199");
                return false;
            }

        } catch (std::exception& _ex) {
            concurrent_log_error(log, _ex.what());
            return false;
        }

        return true;
    }
};

int
main(int argc, char** argv)
{
    tenacitas::logger::business::configure_cerr_log();
    run_test(
      async_loop_004,
      argc,
      argv,
      "\nWork in 'async_loop' will print a provided uint32_t value."

      "\nProvider will provide uint32_t values, until 1000, with a 200 ms "
      "sleep."

      "\nMain thread will stop for 20 secs; 'async_loop' will stop; main "
      "thread will sleep for 4 secs; 'async_loop' will run again; main thread "
      "will sleep for 20 secs; 'async_loop' stops."

      "\nData in work should be 199")
}
