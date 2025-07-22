/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example temperature_sensors_simulator/uix/qt/main.cpp

#include "mainwindow.h"

#include <QApplication>

#include "tenacitas/exp/async/temperature_sensors_simulator/per/sensors.h"
#include "tenacitas/exp/async/temperature_sensors_simulator/supplier.h"

int main(int argc, char *argv[]) {
  using namespace tenacitas::exp::async::temperature_sensors_simulator;

  logger _logger;
  dispatcher _dispatcher(_logger);

  per::sensors<logger, dispatcher> _sensors(_logger, _dispatcher);

  QApplication a(argc, argv);
  MainWindow w(_logger, _dispatcher);
  w.show();
  return a.exec();
}

/*
/home/rodrigo/development/tenacitas/src/tnct/async/temperature_sensors_simulator/uix/qt/main.cpp:21:
error: Constraints not satisfied for class template 'sensors' [with t_logger =
tnct::src::log::internal::logger<tnct::src::log::cerr_writer>, t_dispatcher =
tnct::async::dispatcher<tnct::src::log::internal::logger<tnct::src::log::cerr_writer>,
tnct::async::temperature_sensors_simulator::evt::add_sensor,
tnct::async::temperature_sensors_simulator::evt::new_temperature,
tnct::async::temperature_sensors_simulator::evt::remove_sensor,
tnct::async::temperature_sensors_simulator::evt::set_temperature>]

 */
