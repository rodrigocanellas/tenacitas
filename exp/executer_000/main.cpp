/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example executer_000/main.cpp

#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>

#include <tenacitas.lib/src/alg/exec_sync.h>
#include <tenacitas.lib/src/alg/log.h>

using namespace std::chrono_literals;

void executer_000() {

  auto _function = [](std::function<bool()>, int &&p_i) -> void {
    std::cout << "i = " << p_i << '\n';
  };
  int _i{4};

  std::cout << (tla::exec_sync(200ms, _function, std::move(_i)) ? "true"
                                                                : "false")
            << '\n';
}

void executer_001() {
  auto _function = [](std::function<bool()> p_timeout, int &&p_i) -> void {
    std::this_thread::sleep_for(1s);
    if (p_timeout()) {
      std::cout << "TIMEOUT\n";
    } else {
      std::cout << "i = " << p_i << '\n';
    }
  };
  int _i{4};

  std::cout << (tla::exec_sync(200ms, _function, std::move(_i)) ? "true"
                                                                : "false")
            << '\n';
}

void executer_002() {

  auto _function = [](std::function<bool()>) -> void { TNCT_LOG_DEB("hello"); };

  std::cout << (tla::exec_sync(200ms, _function) ? "true" : "false") << '\n';
}

void executer_003() {
  auto _function = [](std::function<bool()> p_timeout) -> void {
    std::this_thread::sleep_for(1s);
    if (p_timeout()) {
      std::cout << "TIMEOUT\n";
    } else {
      std::cout << "hello\n";
    }
  };

  std::cout << (tla::exec_sync(200ms, _function) ? "true" : "false") << '\n';
}

void executer_004() {

  auto _function = [](std::function<bool()>, int p_i, std::string &&p_str,
                      const char *p_char) -> void {
    TNCT_LOG_DEB(p_i, ',', p_str, ',', p_char);
  };

  std::string _str{"hello"};

  std::cout << (tla::exec_sync(200ms, _function, 4, std::move(_str), "goodbye")
                    ? "true"
                    : "false")
            << '\n';
}

void executer_005() {
  auto _function = [](std::function<bool()> p_timeout, int p_i,
                      std::string &&p_str, const char *p_char) -> void {
    std::this_thread::sleep_for(1s);
    if (p_timeout()) {
      std::cout << "TIMEOUT\n";
    } else {
      std::cout << p_i << ',' << p_str << ',' << p_char << '\n';
    }
  };

  std::string _str{"hello"};

  std::cout << (tla::exec_sync(200ms, _function, 4, std::move(_str), "goodbye")
                    ? "true"
                    : "false")
            << '\n';
}

// ----

void executer_006() {

  auto _function = [](std::function<bool()>, int16_t &&p_i) -> int16_t {
    const int16_t _res = 2 * p_i;
    std::cout << "i = " << p_i << ", _res = " << _res << '\n';
    return _res;
  };

  int _i{4};

  std::optional<int16_t> _maybe =
      tla::exec_sync(200ms, _function, std::move(_i));

  if (!_maybe) {
    std::cout << "timeout, when it should not\n";
    return;
  }
  int16_t _value = _maybe.value();
  if (_value != 8) {
    std::cout << "return should be 8, but it is " << _value << '\n';
    return;
  }
}

void executer_007() {
  auto _function = [](std::function<bool()> p_timeout, int &&p_i) -> int16_t {
    std::this_thread::sleep_for(1s);
    if (p_timeout()) {
      std::cout << "timeout\n";
      return -1;
    } else {
      int16_t _res = p_i * 2;
      std::cout << "i = " << p_i << ", res = " << _res << '\n';
      return _res;
    }
  };
  int _i{4};

  std::optional<int16_t> _maybe =
      tla::exec_sync(200ms, _function, std::move(_i));

  if (_maybe) {
    std::cout << "function returned " << *_maybe << ", but it should not"
              << '\n';
    return;
  }
}

void executer_008() {

  auto _function = [](std::function<bool()>) -> int16_t {
    std::cout << "hello\n";
    return 94;
  };

  std::optional<int16_t> _maybe = tla::exec_sync(200ms, _function);
  if (!_maybe) {
    std::cout << "function timedout, but it should not\n";
    return;
  }
  int16_t _value = *_maybe;
  if (_value != 94) {
    std::cout << "value returned should be 94, but it is " << _value << '\n';
    return;
  }
}

void executer_009() {
  auto _function = [](std::function<bool()> p_timeout) -> int16_t {
    std::this_thread::sleep_for(1s);
    if (p_timeout()) {
      std::cout << "TIMEOUT\n";
      return -1;
    }
    return 53;
  };

  std::optional<int16_t> _maybe = tla::exec_sync(200ms, _function);
  if (_maybe) {
    std::cout << "function not timedout, as expected, and returned " << *_maybe
              << '\n';
    return;
  }
}

void executer_010() {

  auto _function = [](std::function<bool()>, int16_t p_i, float p_f) -> float {
    return p_f * p_i;
  };

  std::optional<float> _maybe = tla::exec_sync(200ms, _function, 4, -2.5);
  if (!_maybe) {
    std::cout << "function timeout, when it should not\n";
    return;
  }
  float _value = *_maybe;
  if (_value != static_cast<float>(-10)) {
    std::cout << "value should be " << static_cast<float>(-10) << ", but it is "
              << _value << '\n';
  }
}

void executer_011() {
  auto _function = [](std::function<bool()> p_timeout, int16_t p_i,
                      float p_f) -> float {
    std::this_thread::sleep_for(1s);
    if (p_timeout()) {
      std::cout << "timeout\n";
      return -1;
    }
    return p_f * p_i;
  };

  std::optional<float> _maybe = tla::exec_sync(200ms, _function, 4, -2.5);
  if (_maybe) {
    std::cout << "function should timeout, but it has not, and returned "
              << *_maybe << '\n';
    return;
  }
}

int main() {
  executer_000();
  executer_001();
  executer_002();
  executer_003();
  executer_004();
  executer_005();
  executer_006();
  executer_007();
  executer_008();
  executer_009();
  executer_010();
  executer_011();
}
