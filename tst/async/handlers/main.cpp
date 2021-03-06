/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory
/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

#include <tst/async/handlers/test_base.h>

struct test_001 {
    typedef test_t<1,   // msg id
                   1,   // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_002 {
    typedef test_t<2,   // msg id
                   1,   // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_003 {
    typedef test_t<3,   // msg id
                   10,  // num senders
                   1,   // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_004 {
    typedef test_t<4,   // msg id
                   10,  // num senders
                   1,   // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_005 {
    typedef test_t<5,   // msg id
                   1,   // num senders
                   50,  // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_006 {
    typedef test_t<6,   // msg id
                   1,   // num senders
                   50,  // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_007 {
    typedef test_t<7,   // msg id
                   10,  // num senders
                   5,   // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_008 {
    typedef test_t<8,   // msg id
                   10,  // num senders
                   5,   // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_009 {
    typedef test_t<9,   // msg id
                   50,  // num senders
                   1,   // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_010 {
    typedef test_t<10,  // msg id
                   50,  // num senders
                   1,   // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_011 {
    typedef test_t<11,  // msg id
                   1,   // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_012 {
    typedef test_t<12,  // msg id
                   1,   // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_013 {
    typedef test_t<13,  // msg id
                   10,  // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_014 {
    typedef test_t<14,  // msg id
                   10,  // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_015 {
    typedef test_t<15,  // msg id
                   50,  // num senders
                   2,   // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_016 {
    typedef test_t<16,  // msg id
                   50,  // num senders
                   2,   // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_017 {
    typedef test_t<17,  // msg id
                   1,   // num senders
                   500, // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_018 {
    typedef test_t<18,  // msg id
                   1,   // num senders
                   500, // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_019 {
    typedef test_t<19,  // msg id
                   10,  // num senders
                   50,  // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_020 {
    typedef test_t<20,  // msg id
                   10,  // num senders
                   50,  // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_021 {
    typedef test_t<21,  // msg id
                   50,  // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_022 {
    typedef test_t<22,  // msg id
                   50,  // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_023 {
    typedef test_t<23,   // msg id
                   1,    // num senders
                   1000, // msgs per sender
                   150,  // sender interval (ms)
                   1,    // num handlers
                   400,  // timeout for each handler (ms)
                   0     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_024 {
    typedef test_t<24,   // msg id
                   1,    // num senders
                   1000, // msgs per sender
                   150,  // sender interval (ms)
                   1,    // num handlers
                   400,  // timeout for each handler (ms)
                   3     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_025 {
    typedef test_t<25,  // msg id
                   10,  // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_026 {
    typedef test_t<26,  // msg id
                   10,  // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_027 {
    typedef test_t<27,  // msg id
                   50,  // num senders
                   20,  // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_028 {
    typedef test_t<28,  // msg id
                   50,  // num senders
                   20,  // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_029 {
    typedef test_t<29,   // msg id
                   1,    // num senders
                   5000, // msgs per sender
                   150,  // sender interval (ms)
                   1,    // num handlers
                   400,  // timeout for each handler (ms)
                   0     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_030 {
    typedef test_t<30,   // msg id
                   1,    // num senders
                   5000, // msgs per sender
                   150,  // sender interval (ms)
                   1,    // num handlers
                   400,  // timeout for each handler (ms)
                   3     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_031 {
    typedef test_t<31,  // msg id
                   10,  // num senders
                   500, // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_032 {
    typedef test_t<32,  // msg id
                   10,  // num senders
                   500, // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_033 {
    typedef test_t<33,  // msg id
                   50,  // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_034 {
    typedef test_t<34,  // msg id
                   50,  // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_035 {
    typedef test_t<35,    // msg id
                   1,     // num senders
                   10000, // msgs per sender
                   150,   // sender interval (ms)
                   1,     // num handlers
                   400,   // timeout for each handler (ms)
                   0      // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_036 {
    typedef test_t<36,    // msg id
                   1,     // num senders
                   10000, // msgs per sender
                   150,   // sender interval (ms)
                   1,     // num handlers
                   400,   // timeout for each handler (ms)
                   3      // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_037 {
    typedef test_t<37,   // msg id
                   10,   // num senders
                   1000, // msgs per sender
                   150,  // sender interval (ms)
                   1,    // num handlers
                   400,  // timeout for each handler (ms)
                   0     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_038 {
    typedef test_t<38,   // msg id
                   10,   // num senders
                   1000, // msgs per sender
                   150,  // sender interval (ms)
                   1,    // num handlers
                   400,  // timeout for each handler (ms)
                   3     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_039 {
    typedef test_t<39,  // msg id
                   50,  // num senders
                   200, // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_040 {
    typedef test_t<40,  // msg id
                   50,  // num senders
                   200, // msgs per sender
                   150, // sender interval (ms)
                   1,   // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_041 {
    typedef test_t<41,  // msg id
                   1,   // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_042 {
    typedef test_t<42,  // msg id
                   1,   // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_043 {
    typedef test_t<43,  // msg id
                   10,  // num senders
                   1,   // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_044 {
    typedef test_t<44,  // msg id
                   10,  // num senders
                   1,   // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_045 {
    typedef test_t<45,  // msg id
                   1,   // num senders
                   50,  // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_046 {
    typedef test_t<46,  // msg id
                   1,   // num senders
                   50,  // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_047 {
    typedef test_t<47,  // msg id
                   10,  // num senders
                   5,   // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_048 {
    typedef test_t<48,  // msg id
                   10,  // num senders
                   5,   // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_049 {
    typedef test_t<49,  // msg id
                   50,  // num senders
                   1,   // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_050 {
    typedef test_t<50,  // msg id
                   50,  // num senders
                   1,   // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_051 {
    typedef test_t<51,  // msg id
                   1,   // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_052 {
    typedef test_t<52,  // msg id
                   1,   // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_053 {
    typedef test_t<53,  // msg id
                   10,  // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_054 {
    typedef test_t<54,  // msg id
                   10,  // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_055 {
    typedef test_t<55,  // msg id
                   50,  // num senders
                   2,   // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_056 {
    typedef test_t<56,  // msg id
                   50,  // num senders
                   2,   // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_057 {
    typedef test_t<57,  // msg id
                   1,   // num senders
                   500, // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_058 {
    typedef test_t<58,  // msg id
                   1,   // num senders
                   500, // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_059 {
    typedef test_t<59,  // msg id
                   10,  // num senders
                   50,  // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_060 {
    typedef test_t<60,  // msg id
                   10,  // num senders
                   50,  // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_061 {
    typedef test_t<61,  // msg id
                   50,  // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_062 {
    typedef test_t<62,  // msg id
                   50,  // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_063 {
    typedef test_t<63,   // msg id
                   1,    // num senders
                   1000, // msgs per sender
                   150,  // sender interval (ms)
                   10,   // num handlers
                   400,  // timeout for each handler (ms)
                   0     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_064 {
    typedef test_t<64,   // msg id
                   1,    // num senders
                   1000, // msgs per sender
                   150,  // sender interval (ms)
                   10,   // num handlers
                   400,  // timeout for each handler (ms)
                   3     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_065 {
    typedef test_t<65,  // msg id
                   10,  // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_066 {
    typedef test_t<66,  // msg id
                   10,  // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_067 {
    typedef test_t<67,  // msg id
                   50,  // num senders
                   20,  // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_068 {
    typedef test_t<68,  // msg id
                   50,  // num senders
                   20,  // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_069 {
    typedef test_t<69,   // msg id
                   1,    // num senders
                   5000, // msgs per sender
                   150,  // sender interval (ms)
                   10,   // num handlers
                   400,  // timeout for each handler (ms)
                   0     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_070 {
    typedef test_t<70,   // msg id
                   1,    // num senders
                   5000, // msgs per sender
                   150,  // sender interval (ms)
                   10,   // num handlers
                   400,  // timeout for each handler (ms)
                   3     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_071 {
    typedef test_t<71,  // msg id
                   10,  // num senders
                   500, // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_072 {
    typedef test_t<72,  // msg id
                   10,  // num senders
                   500, // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_073 {
    typedef test_t<73,  // msg id
                   50,  // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_074 {
    typedef test_t<74,  // msg id
                   50,  // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_075 {
    typedef test_t<75,    // msg id
                   1,     // num senders
                   10000, // msgs per sender
                   150,   // sender interval (ms)
                   10,    // num handlers
                   400,   // timeout for each handler (ms)
                   0      // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_076 {
    typedef test_t<76,    // msg id
                   1,     // num senders
                   10000, // msgs per sender
                   150,   // sender interval (ms)
                   10,    // num handlers
                   400,   // timeout for each handler (ms)
                   3      // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_077 {
    typedef test_t<77,   // msg id
                   10,   // num senders
                   1000, // msgs per sender
                   150,  // sender interval (ms)
                   10,   // num handlers
                   400,  // timeout for each handler (ms)
                   0     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_078 {
    typedef test_t<78,   // msg id
                   10,   // num senders
                   1000, // msgs per sender
                   150,  // sender interval (ms)
                   10,   // num handlers
                   400,  // timeout for each handler (ms)
                   3     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_079 {
    typedef test_t<79,  // msg id
                   50,  // num senders
                   200, // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_080 {
    typedef test_t<80,  // msg id
                   50,  // num senders
                   200, // msgs per sender
                   150, // sender interval (ms)
                   10,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_081 {
    typedef test_t<81,  // msg id
                   1,   // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_082 {
    typedef test_t<82,  // msg id
                   1,   // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_083 {
    typedef test_t<83,  // msg id
                   10,  // num senders
                   1,   // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_084 {
    typedef test_t<84,  // msg id
                   10,  // num senders
                   1,   // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_085 {
    typedef test_t<85,  // msg id
                   1,   // num senders
                   50,  // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_086 {
    typedef test_t<86,  // msg id
                   1,   // num senders
                   50,  // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_087 {
    typedef test_t<87,  // msg id
                   10,  // num senders
                   5,   // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_088 {
    typedef test_t<88,  // msg id
                   10,  // num senders
                   5,   // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_089 {
    typedef test_t<89,  // msg id
                   50,  // num senders
                   1,   // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_090 {
    typedef test_t<90,  // msg id
                   50,  // num senders
                   1,   // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_091 {
    typedef test_t<91,  // msg id
                   1,   // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_092 {
    typedef test_t<92,  // msg id
                   1,   // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_093 {
    typedef test_t<93,  // msg id
                   10,  // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_094 {
    typedef test_t<94,  // msg id
                   10,  // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_095 {
    typedef test_t<95,  // msg id
                   50,  // num senders
                   2,   // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_096 {
    typedef test_t<96,  // msg id
                   50,  // num senders
                   2,   // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_097 {
    typedef test_t<97,  // msg id
                   1,   // num senders
                   500, // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_098 {
    typedef test_t<98,  // msg id
                   1,   // num senders
                   500, // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_099 {
    typedef test_t<99,  // msg id
                   10,  // num senders
                   50,  // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_100 {
    typedef test_t<100, // msg id
                   10,  // num senders
                   50,  // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_101 {
    typedef test_t<101, // msg id
                   50,  // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_102 {
    typedef test_t<102, // msg id
                   50,  // num senders
                   10,  // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_103 {
    typedef test_t<103,  // msg id
                   1,    // num senders
                   1000, // msgs per sender
                   150,  // sender interval (ms)
                   50,   // num handlers
                   400,  // timeout for each handler (ms)
                   0     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_104 {
    typedef test_t<104,  // msg id
                   1,    // num senders
                   1000, // msgs per sender
                   150,  // sender interval (ms)
                   50,   // num handlers
                   400,  // timeout for each handler (ms)
                   3     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_105 {
    typedef test_t<105, // msg id
                   10,  // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_106 {
    typedef test_t<106, // msg id
                   10,  // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_107 {
    typedef test_t<107, // msg id
                   50,  // num senders
                   20,  // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_108 {
    typedef test_t<108, // msg id
                   50,  // num senders
                   20,  // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_109 {
    typedef test_t<109,  // msg id
                   1,    // num senders
                   5000, // msgs per sender
                   150,  // sender interval (ms)
                   50,   // num handlers
                   400,  // timeout for each handler (ms)
                   0     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_110 {
    typedef test_t<110,  // msg id
                   1,    // num senders
                   5000, // msgs per sender
                   150,  // sender interval (ms)
                   50,   // num handlers
                   400,  // timeout for each handler (ms)
                   3     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_111 {
    typedef test_t<111, // msg id
                   10,  // num senders
                   500, // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_112 {
    typedef test_t<112, // msg id
                   10,  // num senders
                   500, // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_113 {
    typedef test_t<113, // msg id
                   50,  // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_114 {
    typedef test_t<114, // msg id
                   50,  // num senders
                   100, // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_115 {
    typedef test_t<115, // msg id
                   1,   // num senders

                   10000, // msgs per sender
                   150,   // sender interval (ms)
                   50,    // num handlers
                   400,   // timeout for each handler (ms)
                   0      // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_116 {
    typedef test_t<116,   // msg id
                   1,     // num senders
                   10000, // msgs per sender
                   150,   // sender interval (ms)
                   50,    // num handlers
                   400,   // timeout for each handler (ms)
                   3      // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_117 {
    typedef test_t<117,  // msg id
                   10,   // num senders
                   1000, // msgs per sender
                   150,  // sender interval (ms)
                   50,   // num handlers
                   400,  // timeout for each handler (ms)
                   0     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_118 {
    typedef test_t<118,  // msg id
                   10,   // num senders
                   1000, // msgs per sender
                   150,  // sender interval (ms)
                   50,   // num handlers
                   400,  // timeout for each handler (ms)
                   3     // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_119 {
    typedef test_t<119, // msg id
                   50,  // num senders
                   200, // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   0    // handler timeout at each
                   >
        test;
    static std::string desc() { return test::description(); }
    bool operator()() {
        test _test;
        return _test();
    }
};

struct test_120 {
    typedef test_t<120, // msg id
                   50,  // num senders
                   200, // msgs per sender
                   150, // sender interval (ms)
                   50,  // num handlers
                   400, // timeout for each handler (ms)
                   3    // handler timeout at each
                   >
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
    run_test(_test, test_001);
    run_test(_test, test_002);
    run_test(_test, test_003);
    run_test(_test, test_004);
    run_test(_test, test_005);
    run_test(_test, test_006);
    run_test(_test, test_007);
    run_test(_test, test_008);
    run_test(_test, test_009);
    run_test(_test, test_010);
    run_test(_test, test_011);
    run_test(_test, test_012);
    run_test(_test, test_013);
    run_test(_test, test_014);
    run_test(_test, test_015);
    run_test(_test, test_016);
    run_test(_test, test_017);
    run_test(_test, test_018);
    run_test(_test, test_019);
    run_test(_test, test_020);
    run_test(_test, test_021);
    run_test(_test, test_022);
    run_test(_test, test_023);
    run_test(_test, test_024);
    run_test(_test, test_025);
    run_test(_test, test_026);
    run_test(_test, test_027);
    run_test(_test, test_028);
    run_test(_test, test_029);
    run_test(_test, test_030);
    run_test(_test, test_031);
    run_test(_test, test_032);
    run_test(_test, test_033);
    run_test(_test, test_034);
    run_test(_test, test_035);
    run_test(_test, test_036);
    run_test(_test, test_037);
    run_test(_test, test_038);
    run_test(_test, test_039);
    run_test(_test, test_040);
    run_test(_test, test_041);
    run_test(_test, test_042);
    run_test(_test, test_043);
    run_test(_test, test_044);
    run_test(_test, test_045);
    run_test(_test, test_046);
    run_test(_test, test_047);
    run_test(_test, test_048);
    run_test(_test, test_049);
    run_test(_test, test_050);
    run_test(_test, test_051);
    run_test(_test, test_052);
    run_test(_test, test_053);
    run_test(_test, test_054);
    run_test(_test, test_055);
    run_test(_test, test_056);
    run_test(_test, test_057);
    run_test(_test, test_058);
    run_test(_test, test_059);
    run_test(_test, test_060);
    run_test(_test, test_061);
    run_test(_test, test_062);
    run_test(_test, test_063);
    run_test(_test, test_064);
    run_test(_test, test_065);
    run_test(_test, test_066);
    run_test(_test, test_067);
    run_test(_test, test_068);
    run_test(_test, test_069);
    run_test(_test, test_070);
    run_test(_test, test_071);
    run_test(_test, test_072);
    run_test(_test, test_073);
    run_test(_test, test_074);
    run_test(_test, test_075);
    run_test(_test, test_076);
    run_test(_test, test_077);
    run_test(_test, test_078);
    run_test(_test, test_079);
    run_test(_test, test_080);
    run_test(_test, test_081);
    run_test(_test, test_082);
    run_test(_test, test_083);
    run_test(_test, test_084);
    run_test(_test, test_085);
    run_test(_test, test_086);
    run_test(_test, test_087);
    run_test(_test, test_088);
    run_test(_test, test_089);
    run_test(_test, test_090);
    run_test(_test, test_091);
    run_test(_test, test_092);
    run_test(_test, test_093);
    run_test(_test, test_094);
    run_test(_test, test_095);
    run_test(_test, test_096);
    run_test(_test, test_097);
    run_test(_test, test_098);
    run_test(_test, test_099);
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
