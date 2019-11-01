#include <business/concurrent/thread_pool.h>
#include <test/concurrent/performance/thread_pool_tester.h>
#include <business/logger/cerr.h>
#include <business/tester/run.h>

struct thread_pool_010
{
    bool operator()()
    {
        thread_pool_tester<2, 1000, 2800>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_010,
             argc,
             argv,
             "2 consumers, 1000 msgs, 2800 work sleep");
}
