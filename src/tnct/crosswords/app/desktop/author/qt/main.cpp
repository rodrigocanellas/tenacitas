
#include "mainwindow.h"

#include <QApplication>
#include <QString>

#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"

using namespace tnct;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  log::cerr _logger;

  _logger.set_deb();

  MainWindow w(_logger);
  w.show();
  return a.exec();
}
