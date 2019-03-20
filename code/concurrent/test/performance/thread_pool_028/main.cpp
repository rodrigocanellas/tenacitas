#include <concurrent/bus/thread_pool.h>
#include <concurrent/test/performance/thread_pool_tester.h>
#include <logger/bus/cerr.h>
#include <tester/bus/run.h>

struct thread_pool_028
{
    bool operator()()
    {
        thread_pool_tester<7, 10000, 2800>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_028,
             argc,
             argv,
             "7 consumers, 10000 msgs, 2800 work sleep");
}
