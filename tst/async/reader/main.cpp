
#include <algorithm>
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
    typedef async::reader_t<uint64_t> reader;

    static std::string desc() {
        return "A reader that reads 500 numbers from a vector, and a single "
               "handling with one handler ";
    }

    ~reader_000() { TRA("leaving"); }

    bool operator()() {

        async::add_handler<reader::data_read>(
            [this](std::shared_ptr<bool>, reader::data_read &&p_data) -> void {
                m_read.push_back(p_data.value);
                DEB("handled ", p_data.value,
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

        async::add_handler<reader::all_data_read>(
            [this](std::shared_ptr<bool>, reader::all_data_read &&) -> void {
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

    void handler(std::shared_ptr<bool>, reader::data_read &&p_data) {
        INF("value handled: ", p_data.value);
        m_read.push_back(p_data.value);
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

struct reader_001 {

    typedef async::reader_t<uint64_t> reader;

    static std::string desc() {
        return "A reader that reads " + std::to_string(m_num_values) +
               " numbers from a vector. A single "
               "handling with three handlers";
    }

    bool operator()() {

        handler _handler1;
        handler _handler2;
        handler _handler3;

        async::handling_id _handling = async::add_handler<reader::data_read>(
            [&](std::shared_ptr<bool> p_bool, reader::data_read &&p_data)
                -> void { _handler1(p_bool, std::move(p_data)); },
            100ms);

        async::add_handler<reader::data_read>(
            _handling,
            [&](std::shared_ptr<bool> p_bool, reader::data_read &&p_data)
                -> void { _handler2(p_bool, std::move(p_data)); });

        async::add_handler<reader::data_read>(
            _handling,
            [&](std::shared_ptr<bool> p_bool, reader::data_read &&p_data)
                -> void { _handler3(p_bool, std::move(p_data)); });

        async::reader_t<uint64_t> _read(
            [this]() -> std::optional<uint64_t> { return this->read(); });

        async::add_handler<reader::all_data_read>(
            [this](std::shared_ptr<bool>, reader::all_data_read &&) -> void {
                INF("handling done_reading");
                m_all_read = true;
            },
            50ms);

        fill();

        _read.start();

        while (true) {
            DEB("while reading is going on...");
            std::this_thread::sleep_for(50ms);
            DEB("_handler1 = ", _handler1.amount(),
                ", _handler2 = ", _handler2.amount(),
                ", _handler3 = ", _handler3.amount());
            if (m_all_read && ((_handler1.amount() + _handler2.amount() +
                                _handler3.amount()) == m_num_values)) {
                DEB("all reading done");
                break;
            }
        }

        INF("handlers 1 = ", _handler1);
        INF("handlers 2 = ", _handler2);
        INF("handlers 3 = ", _handler3);

        return equals(_handler1.get_values(), _handler2.get_values(),
                      _handler3.get_values());
    }

private:
    typedef std::vector<uint64_t> values;

    struct handler {

        void operator()(std::shared_ptr<bool>, reader::data_read &&p_data) {
            m_values.push_back(p_data.value);
        }

        uint64_t amount() const { return m_values.size(); }

        const values &get_values() const { return m_values; }

        friend std::ostream &operator<<(std::ostream &p_out,
                                        const handler &p_handler) {
            values::const_iterator _end = p_handler.get_values().end();
            for (values::const_iterator _ite = p_handler.get_values().begin();
                 _ite != _end; ++_ite) {
                p_out << *_ite << ' ';
            }
            return p_out;
        }

    private:
        values m_values;
    };

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

    std::optional<uint64_t> read() {
        if (m_read_ite == m_source.end()) {
            return {};
        }
        auto _value = *m_read_ite;
        INF("value read: ", _value);
        ++m_read_ite;
        return {_value};
    }

    bool equals(const values &p_read) {
        DEB("starting comparision");
        values::size_type _source_size = m_source.size();
        DEB("source size: ", _source_size);

        values::size_type _read_size = p_read.size();
        DEB("read size: ", _source_size);

        if (_read_size != _source_size) {
            return false;
        }
        for (values::size_type _i = 0; _i < _source_size; ++_i) {
            DEB("source[", _i, "] = ", m_source[_i], ", read[", _i,
                "] = ", p_read[_i]);
            if (m_source[_i] != p_read[_i]) {
                DEB("leaving equal with false");
                return false;
            }
        }
        DEB("leaving equal with true");
        return true;
    }

    bool equals(const values &p_values_1,
                const values &p_values_2,
                const values &p_values_3) {

        values _all;

        _all.insert(_all.end(), p_values_1.begin(), p_values_1.end());
        _all.insert(_all.end(), p_values_2.begin(), p_values_2.end());
        _all.insert(_all.end(), p_values_3.begin(), p_values_3.end());

        std::sort(_all.begin(), _all.end());

        return equals(_all);
    }

private:
    static constexpr values::size_type m_num_values {40};
    bool m_all_read {false};
    values::const_iterator m_read_ite;
    values m_source;
};

int main(int argc, char **argv) {
    logger::set_info_level();
    tester::test _test(argc, argv);

    run_test(_test, reader_000);
    run_test(_test, reader_001);
}
