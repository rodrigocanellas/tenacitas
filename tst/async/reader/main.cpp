
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

    typedef std::vector<uint64_t> values;

private:
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

struct reader_002 {

    typedef async::reader_t<uint64_t> reader;

    static std::string desc() {
        return "A reader that reads " + std::to_string(m_num_values) +
               " numbers from a vector. Two handlings. First handling will "
               "have three handlers, and will not modify any number. Second "
               "handling will have two handlers, and will multiply each number "
               "by 2.";
    }

    bool operator()() {
        handler_a _handler_a1;
        handler_a _handler_a2;
        handler_a _handler_a3;

        async::handling_id _handling_a = async::add_handler<reader::data_read>(
            [&](std::shared_ptr<bool> p_bool, reader::data_read &&p_data)
                -> void { _handler_a1(p_bool, std::move(p_data)); },
            100ms);

        async::add_handler<reader::data_read>(
            _handling_a,
            [&](std::shared_ptr<bool> p_bool, reader::data_read &&p_data)
                -> void { _handler_a2(p_bool, std::move(p_data)); });

        async::add_handler<reader::data_read>(
            _handling_a,
            [&](std::shared_ptr<bool> p_bool, reader::data_read &&p_data)
                -> void { _handler_a3(p_bool, std::move(p_data)); });

        handler_b _handler_b1;
        handler_b _handler_b2;

        async::handling_id _handling_b = async::add_handler<reader::data_read>(
            [&](std::shared_ptr<bool> p_bool, reader::data_read &&p_data)
                -> void { _handler_b1(p_bool, std::move(p_data)); },
            100ms);

        async::add_handler<reader::data_read>(
            _handling_b,
            [&](std::shared_ptr<bool> p_bool, reader::data_read &&p_data)
                -> void { _handler_b2(p_bool, std::move(p_data)); });

        async::reader_t<uint64_t> _read(
            [this]() -> std::optional<uint64_t> { return this->read(); });

        async::add_handler<reader::all_data_read>(
            [this](std::shared_ptr<bool>, reader::all_data_read &&) -> void {
                INF("handling done_reading");
                m_all_read = true;
            },
            50ms);

        async::add_handler<reader::all_data_read>(
            [this](std::shared_ptr<bool>, reader::all_data_read &&) -> void {
                INF("handling done_reading");
                m_all_read = true;
            },
            50ms);

        fill();

        _read.start();

        while (true) {
            DEB("while reading and handling is going on...");
            std::this_thread::sleep_for(50ms);
            DEB("_handler1 = ", _handler_a1.amount(),
                ", _handler2 = ", _handler_a2.amount(),
                ", _handler3 = ", _handler_a3.amount());
            if (m_all_read &&
                ((_handler_a1.amount() + _handler_a2.amount() +
                  _handler_a3.amount()) == m_num_values) &&
                ((_handler_b1.amount() + _handler_b2.amount()) == m_num_values)

            ) {
                DEB("all reading and handling done");
                break;
            }
        }

        INF("handlers a1 = ", _handler_a1);
        INF("handlers a2 = ", _handler_a2);
        INF("handlers a3 = ", _handler_a3);
        INF("handlers b1 = ", _handler_b1);
        INF("handlers b2 = ", _handler_b2);

        return equals_a(_handler_a1, _handler_a2, _handler_a3) &&
               equals_b(_handler_b1, _handler_b2);
    }

    typedef std::vector<uint64_t> values;

private:
    struct handler_a {

        void operator()(std::shared_ptr<bool>, reader::data_read &&p_data) {
            m_values.push_back(p_data.value);
        }

        uint64_t amount() const { return m_values.size(); }

        const values &get_values() const { return m_values; }

        friend std::ostream &operator<<(std::ostream &p_out,
                                        const handler_a &p_handler) {
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

    struct handler_b {

        void operator()(std::shared_ptr<bool>, reader::data_read &&p_data) {
            m_values.push_back(2 * p_data.value);
        }

        uint64_t amount() const { return m_values.size(); }

        const values &get_values() const { return m_values; }

        friend std::ostream &operator<<(std::ostream &p_out,
                                        const handler_b &p_handler) {
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

    bool equals_a(const values &p_read) {
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

    bool equals_a(const handler_a &p_handler_a1,
                  const handler_a &p_handler_a2,
                  const handler_a &p_handler_a3) {

        values _all;

        _all.insert(_all.end(), p_handler_a1.get_values().begin(),
                    p_handler_a1.get_values().end());
        _all.insert(_all.end(), p_handler_a2.get_values().begin(),
                    p_handler_a2.get_values().end());
        _all.insert(_all.end(), p_handler_a3.get_values().begin(),
                    p_handler_a3.get_values().end());

        std::sort(_all.begin(), _all.end());

        return equals_a(_all);
    }

    bool equals_b(const values &p_read) {
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
            if ((2 * m_source[_i]) != p_read[_i]) {
                DEB("leaving equal with false for index ", _i);
                return false;
            }
        }
        DEB("leaving equal with true");
        return true;
    }
    bool equals_b(const handler_b &p_handler_b1,
                  const handler_b &p_handler_b2) {

        values _all;

        _all.insert(_all.end(), p_handler_b1.get_values().begin(),
                    p_handler_b1.get_values().end());
        _all.insert(_all.end(), p_handler_b2.get_values().begin(),
                    p_handler_b2.get_values().end());

        std::sort(_all.begin(), _all.end());

        return equals_b(_all);
    }

private:
    static constexpr values::size_type m_num_values {40};
    bool m_all_read {false};
    values::const_iterator m_read_ite;
    values m_source;
};

struct reader_003 {
    static std::string desc() {
        return "'reader' immediately returns all data was read";
    }

    bool operator()() {

        typedef async::reader_t<int16_t> reader;

        reader _reader([]() -> std::optional<int16_t> {
            std::this_thread::sleep_for(1s);
            return {};
        });

        async::add_handler<reader::data_read>(
            [](std::shared_ptr<bool>, reader::data_read &&p_data) -> void {
                ERR("data ", p_data.value, " read, when there should be none");
                throw std::runtime_error("data read when it should be none");
            },
            100ms);

        bool _done {false};
        async::add_handler<reader::all_data_read>(
            [&](std::shared_ptr<bool>, reader::all_data_read &&) -> void {
                INF("all data read");
                _done = true;
            },
            100ms);

        try {
            _reader.start();

            while (!_done) {
                std::this_thread::sleep_for(200ms);
            }

        } catch (std::exception &_ex) {
            ERR(_ex.what());
        }

        return true;
    }
};

int main(int argc, char **argv) {
    logger::set_debug_level();
    tester::test _test(argc, argv);

    run_test(_test, reader_000);
    run_test(_test, reader_001);
    run_test(_test, reader_002);
    run_test(_test, reader_003);
}
