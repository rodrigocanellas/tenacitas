#include <concurrent/test/performance/thread_pool_tester.h>
#include <concurrent/business/thread_pool.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

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
