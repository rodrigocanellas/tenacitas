
#include <tenacitas.lib/logger.h>
#include <tenacitas.lib/remote.h>
#include <tenacitas.lib/tester.h>

using namespace tenacitas;

struct test {
    static std::string desc() { return ""; }
    bool operator()() {
        using namespace tenacitas::remote;

        try {
            typedef client<remote::protocol::TCP> client;
            typedef client::connection connection;
            typedef remote::writer<remote::protocol::TCP> writer;
            typedef remote::reader<remote::protocol::TCP,
                                   remote::reading::RECORD, 2 * 1024>
                reader;

            client _client;
            std::optional<connection> _maybe_conn =
                _client.connect("localhost", 15678);

            if (!_maybe_conn) {
                ERR("fail to connect to localhost:15678");
                return false;
            }
            INF("connected to localhost:15678");
            connection _conn {*_maybe_conn};

            writer _writer;
            reader _reader;

            _writer(_conn, "hello!!");
            std::optional<message> _maybe_read {_reader(_conn)};
            if (_maybe_read) {
                message _msg {std::move(*_maybe_read)};
                INF("read: '", _msg.str, '\'');
            }

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
