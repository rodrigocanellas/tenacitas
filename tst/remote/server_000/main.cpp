
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
            typedef remote::reader<remote::protocol::TCP,
                                   remote::reading::STREAM, 2 * 1024>
                reader;
            typedef remote::message message;

            typedef remote::writer<remote::protocol::TCP> writer;

            writer _writer;

            reader _reader;

            auto _new_connection_handler =
                [&](std::shared_ptr<bool>,
                    new_connection &&p_new_connection) -> void {
                async::add_handler<async::reader_t<message>::data_read>(
                    [&](std::shared_ptr<bool>,
                        async::reader_t<message>::data_read &&p_message)
                        -> void {
                        _writer(p_new_connection.connection,
                                p_message.value.str);
                    },
                    1s

                );

                _reader(p_new_connection.connection);
                //                if (_maybe) {
                //                    message _msg {std::move(*_maybe)};
                //                    INF("read: '", _msg.str, '\'');

                //                    _writer(p_new_connection.connection,
                //                    _msg.str);
                //                }
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
