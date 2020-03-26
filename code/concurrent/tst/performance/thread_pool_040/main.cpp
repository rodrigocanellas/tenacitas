#include <concurrent/bus/thread_pool.h>
#include <concurrent/test/performance/thread_pool_tester.h>
#include <logger/cerr/log.h>


struct thread_pool_040
{
    bool operator()()
    {
        thread_pool_tester<12, 1000, 2800>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_040,
             argc,
             argv,
             "12 consumers, 1000 msgs, 2800 work sleep");
}
