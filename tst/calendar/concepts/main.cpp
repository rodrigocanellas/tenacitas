#include <cstdint>
#include <limits>

#include <tenacitas/calendar/cpt/chrono_convertible.h>
#include <tenacitas/log/alg/logger.h>
#include <tenacitas/program/alg/options.h>
#include <tenacitas/test/alg/tester.h>
using namespace tenacitas::lib;

struct test006 {
  static std::string desc() { return "test async::cpt::is_std_chrono OK"; }

  bool operator()(const program::alg::options &) {

    f<std::chrono::seconds>(5000ms);
    //    f(2s);

    return true;
  }

private:
  template <typename t_time>
  void f(calendar::cpt::chrono_convertible<t_time> auto p_time) {
    auto _time = std::chrono::duration_cast<t_time>(p_time);
    TNCT_LOG_DEB("time = ", _time.count());
  }
};

struct test007 {
  static std::string desc() { return "test async::cpt::is_std_chrono OK"; }

  bool operator()(const program::alg::options &) {

    f(4s);
    //    f(2s);

    return true;
  }

private:
  void f(calendar::cpt::convertible_to_ms auto p_time) {
    auto _time = std::chrono::duration_cast<std::chrono::milliseconds>(p_time);
    TNCT_LOG_DEB("time = ", _time.count(), "ms");
  }
};

// struct test008 {
//   static std::string desc() { return "test async::cpt::is_std_chrono OK"; }

//  bool operator()(const program::alg::options &) {

//    f(2);

//    return true;
//  }

// private:
//   void
//   f(async::cpt::chrono_convertible<std::chrono::milliseconds> auto p_time) {
//     auto _time =
//     std::chrono::duration_cast<std::chrono::milliseconds>(p_time);
//     TNCT_LOG_DEB("time = ", _time.count(), "ms");
//   }
// };

int main(int argc, char **argv) {
  using namespace tenacitas::lib;
  log::alg::set_debug_level();

  test::alg::tester _tester(argc, argv);

  run_test(_tester, test006);
  run_test(_tester, test007);

  return 0;
}
