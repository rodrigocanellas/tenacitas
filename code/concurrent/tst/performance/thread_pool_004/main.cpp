#include <concurrent/bus/thread_pool.h>
#include <concurrent/test/performance/thread_pool_tester.h>
#include <logger/cerr/log.h>


struct thread_pool_004
{
    bool operator()()
    {
        thread_pool_tester<2, 10, 2800>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_004,
             argc,
             argv,
             "2 consumers, 10 msgs, 2800 work sleep");
}
