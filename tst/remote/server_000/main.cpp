
#include <tenacitas.lib/async.h>
#include <tenacitas.lib/logger.h>
#include <tenacitas.lib/remote.h>
#include <tenacitas.lib/tester.h>

using namespace tenacitas;

struct test {
    static std::string desc() { return ""; }

    bool operator()() {

        try {
            typedef remote::server<remote::protocol::TCP> server;
            typedef server::connection connection;
            typedef remote::new_connection<connection> new_connection;
            typedef remote::reader<std::string, remote::reading::STREAM,
                                   2 * 1024>
                reader;
            typedef remote::message<std::string> message;

            typedef remote::writer<remote::protocol::TCP> writer;

            reader _reader;

            auto _new_connection_handler =
                [&](std::shared_ptr<bool>,
                    new_connection &&p_new_connection) -> void {
                auto _handler = [&](connection p_connection,
                                    message &&p_msg) -> void {
                    writer _writer;
                    _writer(p_connection, p_msg.str);
                };

                _reader(p_new_connection.connection, _handler);
            };

            async::add_handler<new_connection>(_new_connection_handler, 15min);
            remote::server<remote::protocol::TCP> _server;
            INF("server created");
            _server.start_sync(15678);
            return true;
        } catch (std::exception &_ex) {
            FAT(_ex.what());
        }
        return false;
    }
};

int main(int argc, char **argv) {
    logger::set_trace_level();
    tester::test _test(argc, argv);
    run_test(_test, test);
}
