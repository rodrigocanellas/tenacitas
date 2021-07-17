/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include <tenacitas.lib/async.h>

#include <tenacitas.lib/logger.h>
#include <tenacitas.lib/number.h>
#include <tenacitas.lib/program.h>
#include <tenacitas.lib/tester.h>

using namespace tenacitas;
using namespace tenacitas::type;
using namespace std::chrono_literals;

struct exit_000 {
    static std::string desc() { return "Tests 'application' exit"; }

    bool operator()() {
        logger::set_debug_level();

        //  _log.set_debug_level();

        auto _function = [this](type::sptr<bool>) -> void {
            if (m_counter > m_max) {
                DEB("about to publish 'exit_app'");
                async::dispatch(program::exit_app());
            } else {
                DEB(m_counter++);
            }
        };

        async::sleeping_loop_t<> _loop(_function, 200ms, 1s);

        async::add_handler<program::exit_app>(
            [&_loop](sptr<const bool>, sptr<const program::exit_app> &&)
                -> void { _loop.stop(); },
            1s, async::priority {255});

        program::application _app(2s, [&_loop]() { _loop.start(); });

        return true;
    }

private:
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

        auto _function = [this](type::sptr<bool> p_bool) -> void {
            if (m_counter > m_max) {

                DEB("about to publish 'exit_app'");
                async::dispatch(program::exit_app());
                DEB("sleeping...");
                std::this_thread::sleep_for(5s);
                if (p_bool && *p_bool) {
                    return;
                }
                DEB("waking up...");
            } else {
                DEB(m_counter++);
            }
        };

        async::sleeping_loop_t<> _loop(_function, 200ms, 1s);

        async::add_handler<program::exit_app>(
            [&_loop](sptr<const bool>, sptr<const program::exit_app> &&)
                -> void { _loop.stop(); },
            1s, async::priority {255});

        program::application _app(2s, [&_loop]() { _loop.start(); });

        return true;
    }

private:
    const uint16_t m_max {5};
    uint16_t m_counter {0};
};

int main(int argc, char **argv) {
    logger::set_debug_level();

    tester::test _test(argc, argv);
    run_test(_test, exit_000);
    run_test(_test, exit_001);
}
