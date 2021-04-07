
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include <concurrent/writer.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

#include <concurrent/writer/file_stream.h>

using namespace tenacitas;
using namespace tenacitas::logger::cerr;
using namespace tenacitas::concurrent;
using namespace tenacitas::status;

struct writer_000 {
  bool operator()() {
    writer_t<log> _writer;

    tst::file _stream("writer_000");

    const char *_data = "hello, world";

    bool _result = _writer.write_block(_stream, _data, std::strlen(_data));

    concurrent_debug(m_log, _result);

    return (_result);
  }

  static std::string desc() {
    return "Simple writer_t::write_block test to a file";
  }

private:
  logger::cerr::log m_log{"writer_000"};
};

int main(int argc, char **argv) {
  log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, writer_000);
}
