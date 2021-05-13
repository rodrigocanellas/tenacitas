/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

#include <tst/tenacitas/async/handlers/test_base.h>

struct test_000 {
    typedef test_t<1 /* msg id */, 1 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_2 {
    typedef test_t<2 /* msg id */, 1 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_3 {
    typedef test_t<3 /* msg id */, 10 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_4 {
    typedef test_t<4 /* msg id */, 10 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_5 {
    typedef test_t<5 /* msg id */, 1 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_6 {
    typedef test_t<6 /* msg id */, 1 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_7 {
    typedef test_t<7 /* msg id */, 10 /* num senders */,
                   5 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_8 {
    typedef test_t<8 /* msg id */, 10 /* num senders */,
                   5 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_9 {
    typedef test_t<9 /* msg id */, 50 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_10 {
    typedef test_t<10 /* msg id */, 50 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_11 {
    typedef test_t<11 /* msg id */, 1 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_12 {
    typedef test_t<12 /* msg id */, 1 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_13 {
    typedef test_t<13 /* msg id */, 10 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_14 {
    typedef test_t<14 /* msg id */, 10 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_15 {
    typedef test_t<15 /* msg id */, 50 /* num senders */,
                   2 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_16 {
    typedef test_t<16 /* msg id */, 50 /* num senders */,
                   2 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_17 {
    typedef test_t<17 /* msg id */, 1 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_18 {
    typedef test_t<18 /* msg id */, 1 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_19 {
    typedef test_t<19 /* msg id */, 10 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_20 {
    typedef test_t<20 /* msg id */, 10 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_21 {
    typedef test_t<21 /* msg id */, 50 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_22 {
    typedef test_t<22 /* msg id */, 50 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_23 {
    typedef test_t<23 /* msg id */, 1 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_24 {
    typedef test_t<24 /* msg id */, 1 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_25 {
    typedef test_t<25 /* msg id */, 10 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_26 {
    typedef test_t<26 /* msg id */, 10 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_27 {
    typedef test_t<27 /* msg id */, 50 /* num senders */,
                   20 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_28 {
    typedef test_t<28 /* msg id */, 50 /* num senders */,
                   20 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_29 {
    typedef test_t<29 /* msg id */, 1 /* num senders */,
                   5000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_30 {
    typedef test_t<30 /* msg id */, 1 /* num senders */,
                   5000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_31 {
    typedef test_t<31 /* msg id */, 10 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_32 {
    typedef test_t<32 /* msg id */, 10 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_33 {
    typedef test_t<33 /* msg id */, 50 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_34 {
    typedef test_t<34 /* msg id */, 50 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_35 {
    typedef test_t<35 /* msg id */, 1 /* num senders */,
                   10000 /* msgs per sender */, 150 /* sender interval (ms)
                                                     */
                   ,
                   1 /* num handlers */,
                   400 /* timeout for each handler
(ms) */, 0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_36 {
    typedef test_t<36 /* msg id */, 1 /* num senders */,
                   10000 /* msgs per sender */, 150 /* sender interval (ms)
                                                     */
                   ,
                   1 /* num handlers */,
                   400 /* timeout for each handler
(ms) */, 3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_37 {
    typedef test_t<37 /* msg id */, 10 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_38 {
    typedef test_t<38 /* msg id */, 10 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_39 {
    typedef test_t<39 /* msg id */, 50 /* num senders */,
                   200 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_40 {
    typedef test_t<40 /* msg id */, 50 /* num senders */,
                   200 /* msgs per sender */, 150 /* sender interval (ms) */,
                   1 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_41 {
    typedef test_t<41 /* msg id */, 1 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_42 {
    typedef test_t<42 /* msg id */, 1 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_43 {
    typedef test_t<43 /* msg id */, 10 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_44 {
    typedef test_t<44 /* msg id */, 10 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_45 {
    typedef test_t<45 /* msg id */, 1 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_46 {
    typedef test_t<46 /* msg id */, 1 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_47 {
    typedef test_t<47 /* msg id */, 10 /* num senders */,
                   5 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_48 {
    typedef test_t<48 /* msg id */, 10 /* num senders */,
                   5 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_49 {
    typedef test_t<49 /* msg id */, 50 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_50 {
    typedef test_t<50 /* msg id */, 50 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_51 {
    typedef test_t<51 /* msg id */, 1 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_52 {
    typedef test_t<52 /* msg id */, 1 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_53 {
    typedef test_t<53 /* msg id */, 10 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_54 {
    typedef test_t<54 /* msg id */, 10 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_55 {
    typedef test_t<55 /* msg id */, 50 /* num senders */,
                   2 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_56 {
    typedef test_t<56 /* msg id */, 50 /* num senders */,
                   2 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_57 {
    typedef test_t<57 /* msg id */, 1 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_58 {
    typedef test_t<58 /* msg id */, 1 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_59 {
    typedef test_t<59 /* msg id */, 10 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_60 {
    typedef test_t<60 /* msg id */, 10 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_61 {
    typedef test_t<61 /* msg id */, 50 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_62 {
    typedef test_t<62 /* msg id */, 50 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_63 {
    typedef test_t<63 /* msg id */, 1 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_64 {
    typedef test_t<64 /* msg id */, 1 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_65 {
    typedef test_t<65 /* msg id */, 10 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_66 {
    typedef test_t<66 /* msg id */, 10 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_67 {
    typedef test_t<67 /* msg id */, 50 /* num senders */,
                   20 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_68 {
    typedef test_t<68 /* msg id */, 50 /* num senders */,
                   20 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_69 {
    typedef test_t<69 /* msg id */, 1 /* num senders */,
                   5000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_70 {
    typedef test_t<70 /* msg id */, 1 /* num senders */,
                   5000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_71 {
    typedef test_t<71 /* msg id */, 10 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_72 {
    typedef test_t<72 /* msg id */, 10 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_73 {
    typedef test_t<73 /* msg id */, 50 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_74 {
    typedef test_t<74 /* msg id */, 50 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_75 {
    typedef test_t<75 /* msg id */, 1 /* num senders */,
                   10000 /* msgs per sender */, 150 /* sender interval (ms)
                                                     */
                   ,
                   10 /* num handlers */,
                   400 /* timeout for each handler
(ms) */, 0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_76 {
    typedef test_t<76 /* msg id */, 1 /* num senders */,
                   10000 /* msgs per sender */, 150 /* sender interval (ms)
                                                     */
                   ,
                   10 /* num handlers */,
                   400 /* timeout for each handler
(ms) */, 3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_77 {
    typedef test_t<77 /* msg id */, 10 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_78 {
    typedef test_t<78 /* msg id */, 10 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_79 {
    typedef test_t<79 /* msg id */, 50 /* num senders */,
                   200 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_80 {
    typedef test_t<80 /* msg id */, 50 /* num senders */,
                   200 /* msgs per sender */, 150 /* sender interval (ms) */,
                   10 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_81 {
    typedef test_t<81 /* msg id */, 1 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_82 {
    typedef test_t<82 /* msg id */, 1 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_83 {
    typedef test_t<83 /* msg id */, 10 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_84 {
    typedef test_t<84 /* msg id */, 10 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_85 {
    typedef test_t<85 /* msg id */, 1 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_86 {
    typedef test_t<86 /* msg id */, 1 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_87 {
    typedef test_t<87 /* msg id */, 10 /* num senders */,
                   5 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_88 {
    typedef test_t<88 /* msg id */, 10 /* num senders */,
                   5 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_89 {
    typedef test_t<89 /* msg id */, 50 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_90 {
    typedef test_t<90 /* msg id */, 50 /* num senders */,
                   1 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_91 {
    typedef test_t<91 /* msg id */, 1 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_92 {
    typedef test_t<92 /* msg id */, 1 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_93 {
    typedef test_t<93 /* msg id */, 10 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_94 {
    typedef test_t<94 /* msg id */, 10 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_95 {
    typedef test_t<95 /* msg id */, 50 /* num senders */,
                   2 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_96 {
    typedef test_t<96 /* msg id */, 50 /* num senders */,
                   2 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_97 {
    typedef test_t<97 /* msg id */, 1 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_98 {
    typedef test_t<98 /* msg id */, 1 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_99 {
    typedef test_t<99 /* msg id */, 10 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_100 {
    typedef test_t<100 /* msg id */, 10 /* num senders */,
                   50 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_101 {
    typedef test_t<101 /* msg id */, 50 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_102 {
    typedef test_t<102 /* msg id */, 50 /* num senders */,
                   10 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_103 {
    typedef test_t<103 /* msg id */, 1 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_104 {
    typedef test_t<104 /* msg id */, 1 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_105 {
    typedef test_t<105 /* msg id */, 10 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_106 {
    typedef test_t<106 /* msg id */, 10 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_107 {
    typedef test_t<107 /* msg id */, 50 /* num senders */,
                   20 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_108 {
    typedef test_t<108 /* msg id */, 50 /* num senders */,
                   20 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_109 {
    typedef test_t<109 /* msg id */, 1 /* num senders */,
                   5000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_110 {
    typedef test_t<110 /* msg id */, 1 /* num senders */,
                   5000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_111 {
    typedef test_t<111 /* msg id */, 10 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_112 {
    typedef test_t<112 /* msg id */, 10 /* num senders */,
                   500 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_113 {
    typedef test_t<113 /* msg id */, 50 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_114 {
    typedef test_t<114 /* msg id */, 50 /* num senders */,
                   100 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_115 {
    typedef test_t<115 /* msg id */, 1 /* num senders */,
                   10000 /* msgs per sender */, 150 /* sender interval (ms)
                                                     */
                   ,
                   50 /* num handlers */,
                   400 /* timeout for each handler
(ms) */, 0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_116 {
    typedef test_t<116 /* msg id */, 1 /* num senders */,
                   10000 /* msgs per sender */, 150 /* sender interval (ms)
                                                     */
                   ,
                   50 /* num handlers */,
                   400 /* timeout for each handler
(ms) */, 3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_117 {
    typedef test_t<117 /* msg id */, 10 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_118 {
    typedef test_t<118 /* msg id */, 10 /* num senders */,
                   1000 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_119 {
    typedef test_t<119 /* msg id */, 50 /* num senders */,
                   200 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   0 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_120 {
    typedef test_t<120 /* msg id */, 50 /* num senders */,
                   200 /* msgs per sender */, 150 /* sender interval (ms) */,
                   50 /* num handlers */,
                   400 /* timeout for each handler (ms) */,
                   3 /* handler timeout at each */>
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

int main(int argc, char **argv) {
    logger::set_info_level();
    tester::test<> _test(argc, argv);
    run_test(_test, test_000);
    run_test(_test, test_2);
    run_test(_test, test_3);
    run_test(_test, test_4);
    run_test(_test, test_5);
    run_test(_test, test_6);
    run_test(_test, test_7);
    run_test(_test, test_8);
    run_test(_test, test_9);
    run_test(_test, test_10);
    run_test(_test, test_11);
    run_test(_test, test_12);
    run_test(_test, test_13);
    run_test(_test, test_14);
    run_test(_test, test_15);
    run_test(_test, test_16);
    run_test(_test, test_17);
    run_test(_test, test_18);
    run_test(_test, test_19);
    run_test(_test, test_20);
    run_test(_test, test_21);
    run_test(_test, test_22);
    run_test(_test, test_23);
    run_test(_test, test_24);
    run_test(_test, test_25);
    run_test(_test, test_26);
    run_test(_test, test_27);
    run_test(_test, test_28);
    run_test(_test, test_29);
    run_test(_test, test_30);
    run_test(_test, test_31);
    run_test(_test, test_32);
    run_test(_test, test_33);
    run_test(_test, test_34);
    run_test(_test, test_35);
    run_test(_test, test_36);
    run_test(_test, test_37);
    run_test(_test, test_38);
    run_test(_test, test_39);
    run_test(_test, test_40);
    run_test(_test, test_41);
    run_test(_test, test_42);
    run_test(_test, test_43);
    run_test(_test, test_44);
    run_test(_test, test_45);
    run_test(_test, test_46);
    run_test(_test, test_47);
    run_test(_test, test_48);
    run_test(_test, test_49);
    run_test(_test, test_50);
    run_test(_test, test_51);
    run_test(_test, test_52);
    run_test(_test, test_53);
    run_test(_test, test_54);
    run_test(_test, test_55);
    run_test(_test, test_56);
    run_test(_test, test_57);
    run_test(_test, test_58);
    run_test(_test, test_59);
    run_test(_test, test_60);
    run_test(_test, test_61);
    run_test(_test, test_62);
    run_test(_test, test_63);
    run_test(_test, test_64);
    run_test(_test, test_65);
    run_test(_test, test_66);
    run_test(_test, test_67);
    run_test(_test, test_68);
    run_test(_test, test_69);
    run_test(_test, test_70);
    run_test(_test, test_71);
    run_test(_test, test_72);
    run_test(_test, test_73);
    run_test(_test, test_74);
    run_test(_test, test_75);
    run_test(_test, test_76);
    run_test(_test, test_77);
    run_test(_test, test_78);
    run_test(_test, test_79);
    run_test(_test, test_80);
    run_test(_test, test_81);
    run_test(_test, test_82);
    run_test(_test, test_83);
    run_test(_test, test_84);
    run_test(_test, test_85);
    run_test(_test, test_86);
    run_test(_test, test_87);
    run_test(_test, test_88);
    run_test(_test, test_89);
    run_test(_test, test_90);
    run_test(_test, test_91);
    run_test(_test, test_92);
    run_test(_test, test_93);
    run_test(_test, test_94);
    run_test(_test, test_95);
    run_test(_test, test_96);
    run_test(_test, test_97);
    run_test(_test, test_98);
    run_test(_test, test_99);
    run_test(_test, test_100);
    run_test(_test, test_101);
    run_test(_test, test_102);
    run_test(_test, test_103);
    run_test(_test, test_104);
    run_test(_test, test_105);
    run_test(_test, test_106);
    run_test(_test, test_107);
    run_test(_test, test_108);
    run_test(_test, test_109);
    run_test(_test, test_110);
    run_test(_test, test_111);
    run_test(_test, test_112);
    run_test(_test, test_113);
    run_test(_test, test_114);
    run_test(_test, test_115);
    run_test(_test, test_116);
    run_test(_test, test_117);
    run_test(_test, test_118);
    run_test(_test, test_119);
    run_test(_test, test_120);
}
