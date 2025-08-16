
#include "mainwindow.h"

#include <QApplication>
#include <QString>

// #include "grid_solved.h"
// #include "tnct/crosswords/bus/grid_creator.h"

#include "tnct/crosswords/evt/dispatcher.h"
#include "tnct/log/cerr.h"

using namespace tnct;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  log::cerr _cerr;

  _cerr.set_deb();

  crosswords::evt::dispatcher _dispatcher{_cerr};

  // crosswords::dat::grid_creator _grid_creator(_dispatcher);
  //  GridSolved _gridSolved(_dispatcher);
  MainWindow w(_cerr, _dispatcher);
  w.show();
  return a.exec();
}
