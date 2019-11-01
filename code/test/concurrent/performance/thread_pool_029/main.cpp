#include <business/concurrent/thread_pool.h>
#include <test/concurrent/performance/thread_pool_tester.h>
#include <business/logger/cerr.h>
#include <business/tester/run.h>

struct thread_pool_029
{
    bool operator()()
    {
        thread_pool_tester<7, 10000, 5300>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_029,
             argc,
             argv,
             "7 consumers, 10000 msgs, 5300 work sleep");
}
