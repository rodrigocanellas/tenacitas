/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include <tenacitas/async.h>
#include <tenacitas/event.h>
#include <tenacitas/logger.h>
#include <tenacitas/number.h>
#include <tenacitas/program.h>
#include <tenacitas/tester.h>

using namespace tenacitas;
using namespace std::chrono_literals;

struct exit_000 {
    static std::string desc() { return "Tests 'application' exit"; }

    bool operator()() {
        logger::set_debug_level();

        //  _log.set_debug_level();

        auto _function = [this](type::ptr<bool>) -> void {
            if (m_counter > m_max) {
                DEB(m_log, "about to publish 'exit_app'");
                async::dispatch(event::exit_app());
            } else {
                DEB(m_log, m_counter++);
            }
        };

        async::sleeping_loop _loop(m_id, _function, 200ms, 1s);

        async::add_handler<event::exit_app>(
            [&_loop](type::ptr<bool>, event::exit_app &&) -> void {
                _loop.stop();
            },
            1s, async::priority {255});

        program::application _app(2s, [&_loop]() { _loop.start(); });

        return true;
    }

private:
    number::id m_id;
    logger::cerr<> m_log {"main"};
    uint16_t m_counter {0};
    const uint16_t m_max {5};
};

struct exit_001 {
    static std::string desc() {
        return "Tests 'application' exit, with 'function' taking too long to "
               "finish";
    }

    bool operator()() {
        logger::set_debug_level();

        auto _function = [this](type::ptr<bool> p_bool) -> void {
            if (m_counter > m_max) {

                DEB(m_log, "about to publish 'exit_app'");
                async::dispatch(event::exit_app());
                DEB(m_log, "sleeping...");
                std::this_thread::sleep_for(5s);
                if (p_bool && *p_bool) {
                    return;
                }
                DEB(m_log, "waking up...");
            } else {
                DEB(m_log, m_counter++);
            }
        };

        async::sleeping_loop _loop(m_id, _function, 200ms, 1s);

        async::add_handler<event::exit_app>(
            [&_loop](type::ptr<bool>, event::exit_app &&) -> void {
                _loop.stop();
            },
            1s, async::priority {255});

        program::application _app(2s, [&_loop]() { _loop.start(); });

        return true;
    }

private:
    number::id m_id;
    logger::cerr<> m_log {"main"};

    const uint16_t m_max {5};
    uint16_t m_counter {0};
};

int main(int argc, char **argv) {
    logger::set_debug_level();

    tester::test _test(argc, argv);
    run_test(_test, exit_000);
    run_test(_test, exit_001);
}
