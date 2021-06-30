
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <tenacitas.lib/async.h>
#include <tenacitas.lib/calendar.h>

#include <tenacitas.lib/logger.h>
#include <tenacitas.lib/tester.h>

using namespace tenacitas;

struct reader_000 {
    typedef async::data_read<uint64_t> new_data;

    static std::string desc() {
        return "A reader that reads 500 numbers from a vector, and a single "
               "handling with one handler ";
    }

    ~reader_000() { TRA("leaving"); }

    bool operator()() {

        async::add_handler<new_data>(
            [this](std::shared_ptr<bool>, new_data &&p_data) -> void {
                m_read.push_back(p_data.data);
                DEB("handled ", p_data.data,
                    ", m_read.size() = ", m_read.size());
                if (m_handled == (m_num_values - 1)) {
                    DEB("all handled");
                    m_all_handled = true;
                    return;
                }
                ++m_handled;
                std::this_thread::sleep_for(75ms);
            },
            150ms);

        async::add_handler<async::all_data_read>(
            [this](std::shared_ptr<bool>, async::all_data_read &&) -> void {
                INF("handling done_reading");
                m_all_read = true;
            },
            50ms);

        fill();

        async::reader_t<uint64_t> _read(
            [this]() -> std::optional<uint64_t> { return this->read(); });

        _read.start();

        while (true) {
            DEB("while reading and handling is going on...");
            std::this_thread::sleep_for(50ms);
            if (m_all_read && m_all_handled) {
                DEB("done all reading and handling");
                break;
            }
        }

        bool _equal = equal();

        DEB("equal = ", _equal);

        return _equal;
    }

private:
    typedef std::vector<uint64_t> values;

private:
    void fill() {
        for (values::size_type _i = 0; _i < m_num_values; ++_i) {
            //            auto _value = calendar::now<>::microsecs();
            //            INF("value #", _i, " inserted: ", _value);
            INF("value #", _i, " inserted");
            m_source.push_back(_i);
            std::this_thread::sleep_for(20ms);
        }
        m_read_ite = m_source.begin();
    }

    void handler(std::shared_ptr<bool>, async::data_read<uint64_t> &&p_data) {
        INF("value handled: ", p_data.data);
        m_read.push_back(p_data.data);
    }

    std::optional<uint64_t> read() {
        if (m_read_ite == m_source.end()) {
            return {};
        }
        auto _value = *m_read_ite;
        INF("value read: ", _value);
        ++m_read_ite;
        return {_value};
    }

    bool equal() {
        DEB("starting comparision");
        values::size_type _source_size = m_source.size();
        DEB("source size: ", _source_size);

        values::size_type _read_size = m_read.size();
        DEB("read size: ", _source_size);

        if (_read_size != _source_size) {
            return false;
        }
        for (values::size_type _i = 0; _i < _source_size; ++_i) {
            DEB("source[", _i, "] = ", m_source[_i], ", read[", _i,
                "] = ", m_read[_i]);
            if (m_source[_i] != m_read[_i]) {
                DEB("leaving equal with false");
                return false;
            }
        }
        DEB("leaving equal with true");
        return true;
    }

private:
    values m_read;
    values m_source;
    values::const_iterator m_read_ite;
    values::size_type m_handled {0};
    bool m_all_read {false};
    bool m_all_handled {false};
    static constexpr values::size_type m_num_values {500};
};

int main(int argc, char **argv) {
    logger::set_trace_level();
    tester::test _test(argc, argv);

    run_test(_test, reader_000);

    TRA("leaving");
}
