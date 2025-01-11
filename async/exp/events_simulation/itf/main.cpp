#include "main_window.h"

#include <QApplication>

#include "tenacitas.lib/async/exp/events_simulation/eve/dispatcher.h"
#include "tenacitas.lib/log/cerr.h"

using namespace tenacitas::lib;
namespace prj = tenacitas::lib::async::exp::events_simulation;

using logger = log::cerr;
using dispatcher = prj::eve::dispatcher<logger>;

int main(int argc, char *argv[]) {
  logger _logger;
  dispatcher _dispatcher(_logger);
  QApplication a(argc, argv);
  main_window w(_dispatcher, _logger);
  w.show();
  return a.exec();
}
