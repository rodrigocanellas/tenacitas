#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <tuple>

#include <QMainWindow>

#include <tenacitas.lib/async/example/temperature_sensors_simulator/evt/add_sensor.h>
#include <tenacitas.lib/async/example/temperature_sensors_simulator/evt/new_temperature.h>
#include <tenacitas.lib/async/example/temperature_sensors_simulator/evt/remove_sensor.h>
#include <tenacitas.lib/async/example/temperature_sensors_simulator/evt/set_temperature.h>
#include <tenacitas.lib/async/example/temperature_sensors_simulator/supplier.h>

using namespace temperature_sensors_simulator;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  using events_subscribed = std::tuple<evt::new_temperature>;
  using events_published =
      std::tuple<evt::add_sensor, evt::remove_sensor, evt::set_temperature>;

public:
  MainWindow(logger &p_logger, dispatcher &p_dispatcher,
             QWidget *parent = nullptr);
  ~MainWindow();

  void handle(evt::new_temperature &&p_evt) {
    on_new_temperature(std::move(p_evt));
  }

private:
  void on_new_temperature(evt::new_temperature &&);
  int findRow(typ::sensor_id) const;

private slots:
  void on_btnAddSensor_clicked();
  void on_btnDeleteSensor_clicked();
  void on_btnSetTemperature_clicked();

private:
  logger &m_logger;
  dispatcher &m_dispatcher;
  Ui::MainWindow *ui;
  std::mutex m_mutex;
};
#endif // MAINWINDOW_H
