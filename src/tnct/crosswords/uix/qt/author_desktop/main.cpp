
#include "mainwindow.h"

#include <QApplication>
#include <QString>

#include "tnct/crosswords/bus/grid_creator.h"
#include "tnct/crosswords/evt/dispatcher.h"
#include "tnct/log/cerr.h"

using namespace tnct;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  using logger = log::cerr;
  using crosswords::evt::dispatcher;
  using grid_creator = crosswords::bus::grid_creator<logger, dispatcher>;

  logger _logger;

  _logger.set_deb();

  dispatcher _dispatcher{_logger};

  grid_creator _grid_creator(_logger, _dispatcher);

  MainWindow w(_logger, _dispatcher);
  w.show();
  return a.exec();
}
