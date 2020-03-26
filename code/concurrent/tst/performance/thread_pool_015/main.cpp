#include <concurrent/bus/thread_pool.h>
#include <concurrent/test/performance/thread_pool_tester.h>
#include <logger/cerr/log.h>


struct thread_pool_015
{
    bool operator()()
    {
        thread_pool_tester<7, 1, 300>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_015,
             argc,
             argv,
             "7 consumers, 1 msgs, 300 work sleep");
}
