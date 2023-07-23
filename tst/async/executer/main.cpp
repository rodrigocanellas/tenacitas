
/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <tenacitas/async/alg/execute.h>
#include <tenacitas/log/alg/logger.h>
#include <tenacitas/test/alg/tester.h>
#include <tenacitas/traits/alg/traits.h>

using namespace std::chrono_literals;
using namespace tenacitas::lib;

namespace alg {
struct executer_000 {
  static std::string desc() {
    return "Function with one parameter, no returning, with no timeout";
  }
  bool operator()(const program::alg::options &) {
    auto _function = [](std::function<bool()>, int &&p_i) -> void {
      TNCT_LOG_TRA("i = ", p_i);
    };
    int _i{4};

    return async::alg::execute(200ms, _function, std::move(_i));
  }
};

struct executer_001 {
  static std::string desc() {
    return "Function with one parameter, no returning, with timeout";
  }
  bool operator()(const program::alg::options &) {
    auto _function = [](std::function<bool()> p_timeout, int &&p_i) -> void {
      std::this_thread::sleep_for(1s);
      if (p_timeout()) {
        TNCT_LOG_WAR("TIMEOUT");
      } else {
        TNCT_LOG_DEB("i = ", p_i);
      }
    };
    int _i{4};

    //    return !async::alg::execute(200ms, _function, std::move(_i));
    return !async::alg::execute(200ms, _function, std::move(_i));
  }
};

struct executer_002 {
  static std::string desc() {
    return "Function with no parameter, no returning, with no timeout";
  }
  bool operator()(const program::alg::options &) {
    auto _function = [](std::function<bool()>) -> void {
      TNCT_LOG_DEB("hello");
    };

    return async::alg::execute(200ms, _function);
  }
};

struct executer_003 {
  static std::string desc() {
    return "Function with no parameter, no returning, with timeout";
  }

  bool operator()(const program::alg::options &) {
    auto _function = [](std::function<bool()> p_timeout) -> void {
      std::this_thread::sleep_for(1s);
      if (p_timeout()) {
        TNCT_LOG_WAR("TIMEOUT");
      } else {
        TNCT_LOG_DEB("hello");
      }
    };

    return !async::alg::execute(200ms, _function);
  }
};

struct executer_004 {
  static std::string desc() {
    return "Function with 3 parameters, no returning, with no timeout";
  }

  bool operator()(const program::alg::options &) {
    auto _function = [](std::function<bool()>, int p_i, std::string &&p_str,
                        const char *p_char) -> void {
      TNCT_LOG_DEB(p_i, ',', p_str, ',', p_char);
    };

    std::string _str{"hello"};

    return async::alg::execute(200ms, _function, 4, std::move(_str), "goodbye");
  }
};

struct executer_005 {
  static std::string desc() {
    return "Function with 3 parameters, no returning, with timeout";
  }

  bool operator()(const program::alg::options &) {
    auto _function = [](std::function<bool()> p_timeout, int p_i,
                        std::string &&p_str, const char *p_char) -> void {
      std::this_thread::sleep_for(1s);
      if (p_timeout()) {
        TNCT_LOG_WAR("TIMEOUT");
      } else {
        TNCT_LOG_DEB(p_i, ',', p_str, ',', p_char);
      }
    };

    std::string _str{"hello"};

    return !async::alg::execute(200ms, _function, 4, std::move(_str),
                                "goodbye");
  }
};

struct executer_006 {
  static std::string desc() {
    return "Function with one parameter, with returning, with no timeout";
  }

  bool operator()(const program::alg::options &) {
    auto _function = [](std::function<bool()>, int16_t &&p_i) -> int16_t {
      const int16_t _res = 2 * p_i;
      TNCT_LOG_TRA("i = ", p_i, ", _res = ", _res);
      return _res;
    };

    int _i{4};

    std::optional<int16_t> _ret =
        async::alg::execute(200ms, _function, std::move(_i));

    if (!_ret) {
      TNCT_LOG_ERR("timeout, when it should not");
      return false;
    }
    int16_t _value = _ret.value();
    if (_value != 8) {
      TNCT_LOG_ERR("return should be 8, but it is ", _value);
      return false;
    }
    return true;
  }
};

struct executer_007 {
  static std::string desc() {
    return "Function with one parameter, with returning, with timeout";
  }

  bool operator()(const program::alg::options &) {
    auto _function = [](std::function<bool()> p_timeout, int &&p_i) -> int16_t {
      std::this_thread::sleep_for(1s);
      if (p_timeout()) {
        TNCT_LOG_WAR("timeout");
        return -1;
      } else {
        int16_t _res = p_i * 2;
        TNCT_LOG_TRA("i = ", p_i, ", res = ", _res);
        return _res;
      }
    };
    int _i{4};

    std::optional<int16_t> _maybe =
        async::alg::execute(200ms, _function, std::move(_i));

    if (_maybe) {
      TNCT_LOG_ERR("function returned ", *_maybe, ", but it should not");
      return false;
    }
    return true;
  }
};

struct executer_008 {
  static std::string desc() {
    return "Function with no parameter, with returning, with no timeout";
  }

  bool operator()(const program::alg::options &) {
    auto _function = [](std::function<bool()>) -> int16_t {
      TNCT_LOG_DEB("hello");
      return 94;
    };

    std::optional<int16_t> _maybe = async::alg::execute(200ms, _function);
    if (!_maybe) {
      TNCT_LOG_ERR("function timedout, but it should not");
      return false;
    }
    int16_t _value = *_maybe;
    if (_value != 94) {
      TNCT_LOG_ERR("value returned should be 94, but it is ", _value);
      return false;
    }
    return true;
  }
};

struct executer_009 {
  static std::string desc() {
    return "Function with no parameter, with returning, with timeout";
  }

  bool operator()(const program::alg::options &) {
    auto _function = [](std::function<bool()> p_timeout) -> int16_t {
      std::this_thread::sleep_for(1s);
      if (p_timeout()) {
        TNCT_LOG_WAR("TIMEOUT");
        return -1;
      }
      return 53;
    };

    std::optional<int16_t> _maybe = async::alg::execute(200ms, _function);
    if (_maybe) {
      TNCT_LOG_ERR("function not timedout, as expected, and returned ",
                   *_maybe);
      return false;
    }
    return true;
  }
};

struct executer_010 {
  static std::string desc() {
    return "Function with 2 parameters, with returning, with no timeout";
  }

  bool operator()(const program::alg::options &) {
    auto _function = [](std::function<bool()>, int16_t p_i,
                        float p_f) -> float { return p_f * p_i; };

    std::optional<float> _maybe =
        async::alg::execute(200ms, _function, 4, -2.5);
    if (!_maybe) {
      TNCT_LOG_ERR("function timeout, when it should not");
      return false;
    }
    float _value = *_maybe;
    if (_value != static_cast<float>(-10)) {
      TNCT_LOG_ERR("value should be ", static_cast<float>(-10), ", but it is ",
                   _value);
    }
    return true;
  }
};

struct executer_011 {
  static std::string desc() {
    return "Function with 2 parameters, with returning, with timeout";
  }

  bool operator()(const program::alg::options &) {
    auto _function = [](std::function<bool()> p_timeout, int16_t p_i,
                        float p_f) -> float {
      std::this_thread::sleep_for(1s);
      if (p_timeout()) {
        TNCT_LOG_WAR("timeout");
        return -1;
      }
      return p_f * p_i;
    };

    std::optional<float> _maybe =
        async::alg::execute(200ms, _function, 4, -2.5);
    if (_maybe) {
      TNCT_LOG_ERR("function should timeout, but it has not, and returned ",
                   *_maybe);
      return false;
    }
    return true;
  }
};
} // namespace alg
int main(int argc, char **argv) {
  using namespace tenacitas::lib;

  log::alg::set_trace_level();
  test::alg::tester _tester(argc, argv);

  run_test(_tester, alg::executer_000);
  run_test(_tester, alg::executer_001);
  run_test(_tester, alg::executer_002);
  run_test(_tester, alg::executer_003);
  run_test(_tester, alg::executer_004);
  run_test(_tester, alg::executer_005);
  run_test(_tester, alg::executer_006);
  run_test(_tester, alg::executer_007);
  run_test(_tester, alg::executer_008);
  run_test(_tester, alg::executer_009);
  run_test(_tester, alg::executer_010);
  run_test(_tester, alg::executer_011);

  //  std::this_thread::sleep_for(8s);
}
