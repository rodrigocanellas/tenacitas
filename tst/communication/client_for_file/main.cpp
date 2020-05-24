
#include <cstdint>
#include <iostream>
#include <string>

#include <communication/client.h>
#include <communication/file_connection.h>
#include <communication/status.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct client_for_file {
  bool operator()() {
    typedef communication::tst::file_connector connector;
    typedef communication::status status;
    typedef communication::client_t<logger::cerr::log, connector> client;

    connector _connector;
    client _client(_connector);

    //    auto _f = [](const std::string &p_file_name) {
    //      connector _connector;
    //      return _connector.connect(p_file_name);
    //    };

    const std::string _file_name("./client_for_file.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      std::cerr << "erro " << _status << std::endl;
      return false;
    }

    _status = _client.send(std::string("ola!!!"));
    if (_status != status::ok) {
      std::cerr << "erro " << _status << std::endl;
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "'client_t' with a connection to a text file, using 'send' to write "
           "a message to the file";
  }
};

int main(int argc, char **argv) {
  tester::test _test(argc, argv);
  run_test(_test, client_for_file);
}
