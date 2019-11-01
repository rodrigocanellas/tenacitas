#include <business/concurrent/thread_pool.h>
#include <test/concurrent/performance/thread_pool_tester.h>
#include <business/logger/cerr.h>
#include <business/tester/run.h>

struct thread_pool_044
{
    bool operator()()
    {
        thread_pool_tester<12, 10000, 5300>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_044,
             argc,
             argv,
             "12 consumers, 10000 msgs, 5300 work sleep");
}
