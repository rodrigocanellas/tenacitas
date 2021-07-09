
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
            typedef message<char> message;
            typedef remote::writer<connection, message::type> writer;

            typedef remote::reader<message::type, remote::reading::AT_MOST>
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

            reader _reader;
            writer _writer;

            std::cout << "Enter text, no space, or -1 to stop\n\n";

            while (true) {
                std::string _str;
                std::cout << "-> ";
                std::cin >> _str;

                if (_str == "-1") {
                    break;
                }

                _writer(_conn, _str);

                std::optional<message> _maybe = _reader(_conn);
                if (!_maybe) {
                    break;
                }

                INF("echoed '", _maybe.value(), '\'');

                std::cout << '\n';
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
