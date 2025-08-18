
#include "mainwindow.h"

#include <QApplication>
#include <QString>

// #include "grid_solved.h"
#include "tnct/crosswords/bus/grid_creator.h"

#include "tnct/crosswords/evt/dispatcher.h"
#include "tnct/log/cerr.h"

using namespace tnct;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  log::cerr _logger;

  _logger.set_deb();

  crosswords::evt::dispatcher _dispatcher{_logger};

  crosswords::bus::grid_creator<log::cerr, crosswords::evt::dispatcher>
      _grid_creator(_logger, _dispatcher);

  MainWindow w(_logger, _dispatcher);
  w.show();
  return a.exec();
}
