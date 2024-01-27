#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <tuple>

#include <QMainWindow>

#include <tenacitas.lib/exp/temperature_sensors_simulator/asy/dispatcher.h>
#include <tenacitas.lib/exp/temperature_sensors_simulator/asy/add_sensor.h>
#include <tenacitas.lib/exp/temperature_sensors_simulator/asy/new_temperature.h>
#include <tenacitas.lib/exp/temperature_sensors_simulator/asy/remove_sensor.h>
#include <tenacitas.lib/exp/temperature_sensors_simulator/asy/set_temperature.h>

using namespace temperature_sensors_simulator;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  using events_subscribed = std::tuple<asy::new_temperature>;
  using events_published =
      std::tuple<asy::add_sensor, asy::remove_sensor, asy::set_temperature>;

public:
  MainWindow(asy::dispatcher::ptr p_dispatcher, QWidget *parent = nullptr);
  ~MainWindow();

private:
  void on_new_temperature(asy::new_temperature &&);
  int findRow(dom::sensor_id) const;

private slots:
  void on_btnAddSensor_clicked();
  void on_btnDeleteSensor_clicked();
  void on_btnSetTemperature_clicked();

private:
  asy::dispatcher::ptr m_dispatcher;
  Ui::MainWindow *ui;
  std::mutex m_mutex;
};
#endif // MAINWINDOW_H
