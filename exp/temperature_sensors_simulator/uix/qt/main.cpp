/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example temperature_sensors_simulator/uix/qt/main.cpp

#include "mainwindow.h"

#include <QApplication>

#include <tenacitas.lib/exp/temperature_sensors_simulator/asy/dispatcher.h>
#include <tenacitas.lib/exp/temperature_sensors_simulator/per/sensors.h>

using namespace temperature_sensors_simulator;

int main(int argc, char *argv[]) {
  using namespace temperature_sensors_simulator;

  asy::dispatcher::ptr _dispatcher{asy::dispatcher::create()};

  per::sensors _sensors{_dispatcher};

  QApplication a(argc, argv);
  MainWindow w(_dispatcher);
  w.show();
  return a.exec();
}
