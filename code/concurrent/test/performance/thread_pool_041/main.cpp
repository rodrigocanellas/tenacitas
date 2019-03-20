#include <concurrent/bus/thread_pool.h>
#include <concurrent/test/performance/thread_pool_tester.h>
#include <logger/bus/cerr.h>
#include <tester/bus/run.h>

struct thread_pool_041
{
    bool operator()()
    {
        thread_pool_tester<12, 1000, 5300>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_041,
             argc,
             argv,
             "12 consumers, 1000 msgs, 5300 work sleep");
}
