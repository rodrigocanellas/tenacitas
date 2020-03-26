#include <concurrent/bus/thread_pool.h>
#include <concurrent/test/performance/thread_pool_tester.h>
#include <logger/cerr/log.h>


struct thread_pool_012
{
    bool operator()()
    {
        thread_pool_tester<2, 10000, 300>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_012,
             argc,
             argv,
             "2 consumers, 10000 msgs, 300 work sleep");
}
