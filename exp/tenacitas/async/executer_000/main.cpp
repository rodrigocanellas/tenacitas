/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example executer_000

#include <chrono>
#include <iostream>
#include <optional>
#include <thread>
#include <type_traits>

#include <tenacitas/async.h>

using namespace std::chrono_literals;

double f1(int i) {
  std::cout << "f2: ";
  return i / 4;
}

double f2(int i, float f) {
  std::cout << "f2: ";
  return f * i / 4;
}

double f3() {
  std::cout << "f3: ";
  return 3.14;
}

void f4(int i) { std::cout << "f4: " << i / 4 << " "; }

void f5(int i, float f) { std::cout << "f5: " << f * i / 4 << " "; }

void f6() { std::cout << "f6: " << 3.14 << " "; }

double f7(int i) {
  std::cout << "f7: ";
  std::this_thread::sleep_for(2s);

  return i / 4;
}

double f8(int i, float f) {

  std::cout << "f8: ";
  std::this_thread::sleep_for(2s);
  return f * i / 4;
}

double f9() {
  std::cout << "f9: ";
  std::this_thread::sleep_for(2s);

  return 3.14;
}

void f10(int i) {
  std::cout << "f10: ";
  std::this_thread::sleep_for(2s);
  std::cout << i / 4 << " ";
}

void f11(int i, float f) {
  std::cout << "f11: ";
  std::this_thread::sleep_for(2s);
  std::cout << f * i / 4 << " ";
}

void f12() {
  std::cout << "f6: ";
  std::this_thread::sleep_for(2s);
  std::cout << 3.14 << " ";
}

void f13(bool &b) { std::cout << "f13: " << (b ? "true" : "false") << " "; }

void f14(bool &&b) {
  std::cout << "f14: ";
  std::this_thread::sleep_for(2s);
  std::cout << (b ? "true" : "false") << " ";
}

void f15(bool &b) {
  std::cout << "f15: ";
  std::this_thread::sleep_for(2s);
  std::cout << (b ? "true" : "false") << " ";
  b = !b;
}

int main() {
  using namespace tenacitas;

  {
    std::optional<double> _maybe = async::execute(300ms, f1, 8);
    if (_maybe) {
      std::cout << *_maybe;
    } else {
      std::cout << " timeout!";
    }
    std::cout << std::endl;
  }

  {
    std::optional<double> _maybe = async::execute(300ms, f2, 8, 4.2f);
    if (_maybe) {
      std::cout << *_maybe;
    } else {
      std::cout << "timeout!";
    }
    std::cout << std::endl;
  }

  {
    std::optional<double> _maybe = async::execute(300ms, f3);
    if (_maybe) {
      std::cout << *_maybe;
    } else {
      std::cout << "timeout!";
    }
    std::cout << std::endl;
  }

  {
    if (async::execute(300ms, f4, -8)) {
      std::cout << "ok";
    } else {
      std::cout << "timeout!";
    }
    std::cout << std::endl;
  }

  {
    if (async::execute(300ms, f5, -8, 3.56)) {
      std::cout << "ok";
    } else {
      std::cout << "timeout!";
    }
    std::cout << std::endl;
  }

  {
    if (async::execute(300ms, f6)) {
      std::cout << "ok";
    } else {
      std::cout << "timeout!";
    }
    std::cout << std::endl;
  }

  {
    std::optional<double> _maybe = async::execute(300ms, f7, 8);
    if (_maybe) {
      std::cout << "with exec: " << *_maybe;
    } else {
      std::cout << "timeout!";
    }
    std::cout << std::endl;
  }

  {
    std::optional<double> _maybe = async::execute(300ms, f8, 8, 4.2f);
    if (_maybe) {
      std::cout << *_maybe;
    } else {
      std::cout << "timeout!";
    }
    std::cout << std::endl;
  }

  {
    std::optional<double> _maybe = async::execute(300ms, f9);
    if (_maybe) {
      std::cout << *_maybe;
    } else {
      std::cout << "timeout!";
    }
    std::cout << std::endl;
  }

  {
    if (async::execute(300ms, f10, -18)) {
      std::cout << "ok";
    } else {
      std::cout << "timeout!";
    }
    std::cout << std::endl;
  }

  {
    if (async::execute(300ms, f11, -18, 3.3)) {
      std::cout << "ok";
    } else {
      std::cout << "timeout!";
    }
    std::cout << std::endl;
  }

  {
    if (async::execute(300ms, f12)) {
      std::cout << "ok";
    } else {
      std::cout << "timeout!";
    }
    std::cout << std::endl;
  }

  {
    bool b{false};
    if (async::execute(300ms, f13, std::ref(b))) {
      std::cout << "ok";
    } else {
      std::cout << "timeout";
    }
    std::cout << std::endl;
  }

  {
    bool b{false};
    if (async::execute(300ms, f14, std::move(b))) {
      std::cout << "ok";
    } else {
      std::cout << "timeout";
    }
    std::cout << std::endl;
  }
  {
    bool b{false};
    if (async::execute(300ms, f15, std::ref(b))) {
      std::cout << "ok";
    } else {
      std::cout << "timeout";
    }
    std::cout << std::endl;
  }
}
