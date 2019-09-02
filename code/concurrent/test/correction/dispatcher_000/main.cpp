/// \example
#include <chrono>
#include <cstdint>
#include <sstream>
#include <string>

#include <concurrent/business/dispatcher.h>
#include <concurrent/business/sleeping_loop.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas;

// ############################## messages
struct msg_a
{
    friend std::ostringstream& operator<<(std::ostringstream& p_out,
                                          const msg_a& p_msg)
    {
        p_out << "|msg a|i|" << p_msg.i;
        return p_out;
    }

    explicit msg_a(int16_t p_i = 0)
      : i(p_i)
    {}

    int16_t i = { 0 };
};

struct msg_b
{
    friend std::ostringstream& operator<<(std::ostringstream& p_out,
                                          const msg_b& p_msg)
    {
        p_out << "|msg b|i|" << p_msg.i;
        return p_out;
    }

    explicit msg_b(int32_t p_i = 0)
      : i(p_i)
    {}
    int32_t i = { 0 };
};

struct msg_c
{
    friend std::ostringstream& operator<<(std::ostringstream& p_out,
                                          const msg_c& p_msg)
    {
        p_out << "|msg c|d|" << p_msg.d;
        return p_out;
    }

    explicit msg_c(double p_d = 0.0)
      : d(p_d)

    {}

    double d = { 0.0 };
};

// ############################## publishers
struct publisher_1
{
  public:
    bool operator()()
    {
        msg_a _msg(++i);
        cerr_test("P 1", _msg);

        concurrent::business::dispatcher_t<msg_a>::publish(_msg);
        return true;
    }
    int16_t i = { 10 };
};

struct publisher_2
{
  public:
    bool operator()()
    {
        i += 10;
        msg_a _msg_a(i);
        cerr_test("P 2", _msg_a);
        concurrent::business::dispatcher_t<msg_a>::publish(_msg_a);

        d *= 2.5;
        msg_c _msg_c(d);
        cerr_test("P 2", _msg_c);
        concurrent::business::dispatcher_t<msg_c>::publish(_msg_c);

        return true;
    }
    int16_t i = { -100 };
    double d = { 1.0 };
};

struct publisher_3
{
  public:
    bool operator()()
    {

        i += 300;
        msg_b _msg_b(i);

        cerr_test("P 3", _msg_b);
        concurrent::business::dispatcher_t<msg_b>::publish(_msg_b);

        return true;
    }

    int32_t i = { 5000 };
};

// ############################## subscribers
struct subscriber_1
{
    bool operator()(msg_a&& p_msg)
    {
        cerr_test("S 1", p_msg);
        return true;
    }
};

struct subscriber_2
{
    bool operator()(msg_a&& p_msg)
    {
        cerr_test("S 2", p_msg);
        return true;
    }
    bool operator()(msg_b&& p_msg)
    {
        cerr_test("S 2", p_msg);
        return true;
    }
};

struct subscriber_3
{
    bool operator()(msg_b&& p_msg)
    {
        cerr_test("S 3", p_msg);
        return true;
    }
};

struct subscriber_4
{
    bool operator()(msg_a&& p_msg)
    {
        cerr_test("S 4", p_msg);
        return true;
    }
    bool operator()(msg_b&& p_msg)
    {
        cerr_test("S 4", p_msg);
        return true;
    }
    bool operator()(msg_c&& p_msg)
    {
        cerr_test("S 4", p_msg);
        return true;
    }
};

typedef tenacitas::concurrent::business::sleeping_loop_t<void> sleeping_loop_t;

// ############################## subscribers
struct dispatcher_000
{

    bool operator()()
    {
        sleeping_loop_t _sl1(std::chrono::milliseconds(1000),
                             publisher_1(),
                             std::chrono::milliseconds(1000));
        sleeping_loop_t _sl2(std::chrono::milliseconds(2000),
                             publisher_2(),
                             std::chrono::milliseconds(1000));
        sleeping_loop_t _sl3(std::chrono::milliseconds(1500),
                             publisher_3(),
                             std::chrono::milliseconds(1000));
        using namespace tenacitas::concurrent::business;
        bool _rc = true;
        std::chrono::milliseconds _work_timeout(500);
        dispatcher_t<msg_a>::subscribe(
          []() { return subscriber_1(); }, 2, _work_timeout);
        dispatcher_t<msg_a>::subscribe(subscriber_2(), _work_timeout);
        dispatcher_t<msg_a>::subscribe(subscriber_4(), _work_timeout);
        dispatcher_t<msg_b>::subscribe(subscriber_2(), _work_timeout);
        dispatcher_t<msg_b>::subscribe(subscriber_3(), _work_timeout);
        dispatcher_t<msg_b>::subscribe(subscriber_4(), _work_timeout);
        dispatcher_t<msg_c>::subscribe(subscriber_4(), _work_timeout);

        _sl1.run();
        _sl2.run();
        _sl3.run();

        cerr_test("------> starting to sleep");
        std::this_thread::sleep_for(std::chrono::seconds(10));
        cerr_test("------> waking up");
        return _rc;
    }

  private:
};

int
main(int argc, char** argv)
{
    run_test(
      dispatcher_000,
      argc,
      argv,
      "\n3 types of messages, 4 subscribers, 3 publishers."
      "\n\nMessage 'a' has an int16_t attribute; message 'b' has an int32_t "
      "attribute; message 'c' has a double attribute."

      "\n\nSubscriber 1 for message 'a', with two instances; subscriber 2 for "
      "messages 'a' and 'b'; subscriber 3 for message 'b'; "
      "subscriber 4 for messages 'a', 'b' and 'c'."

      "\n\nPublisher 1 publishes 'a', starting at 10, increasing 1 at each "
      "publishing; "
      "publisher 2 publishes 'a' stating at -100, and incrementing "
      "10, and 'c', starting at 1.0 and multiplying times 2.5; "
      "publisher 3 publishes 'b', stating at 5000, and incrementing 300."

      "\n\nA 'sleeping_loop' will call Publisher 1 to send messages at each "
      "1000ms; "
      "another 'sleeping_loop' will call Publisher 2 to send messages at each "
      "2000ms; "
      "and a third 'sleeping_loop' will call Publisher 3 to send messages at "
      "each 1500ms."

      "\n\nMain thread will sleep for 10 seconds, while publishers send "
      "messages.");
}
