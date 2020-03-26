#include <concurrent/bus/thread_pool.h>
#include <concurrent/test/performance/thread_pool_tester.h>
#include <logger/cerr/log.h>


struct thread_pool_039
{
    bool operator()()
    {
        thread_pool_tester<12, 1000, 300>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_039,
             argc,
             argv,
             "12 consumers, 1000 msgs, 300 work sleep");
}
