
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include <concurrent/writer.h>
#include <logger/cerr/log.h>
#include <status/result.h>
#include <tester/test.h>

using namespace tenacitas;
using namespace tenacitas::logger::cerr;
using namespace tenacitas::concurrent;
using namespace tenacitas::status;

struct writer_000
{
  bool operator()()
  {
    writer_t<log> _writer;

    std::ofstream _stream("writer_000");

    const char* _data = "hello, world";

    result _result = _writer.write_block(
      [&_stream](const char* p_data, size_t p_size) -> result {
        _stream.write(p_data, p_size);
        if (_stream.good()) {
          return ok;
        }
        return error_writing;
      },
      _data,
      std::strlen(_data));

    concurrent_log_debug(log, _result);

    return (_result == ok ? true : false);
  }

  static std::string desc() { return "Simple writer_t::write_block test"; }
};

int main(int argc, char **argv) {
  tester::test _test(argc, argv);
  run_test(_test, writer_000);
}
