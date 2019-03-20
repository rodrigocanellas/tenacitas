
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

  private:
    uint32_t m_data = 0;
};

typedef tenacitas::concurrent::bus::async_loop<uint32_t> async_loop_t;

struct dummy
{
    explicit dummy(async_loop_t::ptr_t&& p_al)
      : m_al(std::move(p_al))
    {
        cerr_test("stopping in dummy");
        m_al->stop();
    }

    void run()
    {

        cerr_test("starting in dummy");
        m_al->run();
    }

    void stop() { m_al->stop(); }

  private:
    async_loop_t::ptr_t m_al;
};

struct test008
{
    bool operator()()
    {
        try {
            work1 _work;
            async_loop_t::ptr_t sl1(async_loop_t::create(
              [&_work](uint32_t&& p_data) { return _work(std::move(p_data)); },
              std::chrono::milliseconds(1000),
              []() { return false; },
              provide()));

            sl1->run();

            std::this_thread::sleep_for(std::chrono::seconds(5));

            dummy _dummy(std::move(sl1));

            cerr_test("data 1 = ", _work.data);
            if (_work.data != 24) {
                cerr_error("wrong value for data, it should be 24");
                return false;
            }

            _dummy.run();

            std::this_thread::sleep_for(std::chrono::seconds(4));

            _dummy.stop();

            cerr_test("data 2 = ", _work.data);
            if (_work.data != 44) {
                cerr_error("wrong value for data, it should be 44");
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
      test008,
      argc,
      argv,
      "\nWork in 'async_loop' will print a provided uint32_t value."

      "\nProvider will provide the uint32_t values, until 1000, with a 200 ms "
      "sleep."

      "\nMain thread will run a 'sleeping_loop'; stop for 5 secs; the "
      "'sleeping_loop' pointer will be moved as attribute of an auxiliar "
      "structure instance."

      "\nIn the constructor of this auxiliar structure, the moved "
      "'sleeping_loop' pointer will call 'stop'. In the 'run' method, the "
      "pointer will call 'run'"

      "\nThe main thread calls 'run' of the auxiliar object, will sleep for 4 "
      "secs, and the auxiliar structure instance will stop."

      "\nThe counter of the work should be 44");
}
