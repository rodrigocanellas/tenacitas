#include <business/concurrent/thread_pool.h>
#include <test/concurrent/performance/thread_pool_tester.h>
#include <business/logger/cerr.h>
#include <business/tester/run.h>

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
