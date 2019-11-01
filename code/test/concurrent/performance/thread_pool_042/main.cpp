#include <test/concurrent/performance/thread_pool_tester.h>
#include <business/concurrent/thread_pool.h>
#include <business/logger/cerr.h>
#include <business/tester/run.h>

struct thread_pool_042
{
    bool operator()()
    {
        thread_pool_tester<12, 10000, 300>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(
      thread_pool_042, argc, argv, "12 consumers, 10000 msgs, 300 work sleep");
}
