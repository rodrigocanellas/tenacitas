#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <tenacitas.lib/log/log.h>

#include <tenacitas.lib/exp/temperature_sensors_simulator/dom/dat/sensor_id.h>
#include <tenacitas.lib/exp/temperature_sensors_simulator/dom/dat/temperature.h>

using namespace temperature_sensors_simulator;

MainWindow::MainWindow(asy::dispatcher::ptr p_dispatcher, QWidget *parent)
    : QMainWindow(parent), m_dispatcher(p_dispatcher), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  tenacitas::lib::log::set_info_level();

  m_dispatcher->subscribe<MainWindow, asy::new_temperature>(
      [this](auto p_evt) { on_new_temperature(std::move(p_evt)); });
}

MainWindow::~MainWindow() { delete ui; }

int MainWindow::findRow(dom::sensor_id p_sensor_id) const {
  int _row_count = ui->tblTemperatures->rowCount();
  for (int _i = 0; _i < _row_count; ++_i) {
    QString _sensor_id_str = ui->tblTemperatures->item(_i, 0)->text();
    auto _sendor_id =
        static_cast<decltype(p_sensor_id)>(_sensor_id_str.toInt());
    if (_sendor_id == p_sensor_id) {
      return _i;
    }
  }
  return -1;
}

void MainWindow::on_new_temperature(asy::new_temperature &&p_evt) {
  std::lock_guard<std::mutex> _lock{m_mutex};
  int _row = findRow(p_evt.sensor_id);
  if (_row != -1) {
    ui->tblTemperatures->setItem(
        _row, 1,
        new QTableWidgetItem(QString::number(p_evt.temperature, 'g', 4)));
  }
}

void MainWindow::on_btnAddSensor_clicked() {
  if (!ui->txtSensorToAdd->text().isEmpty()) {
    dom::sensor_id _sensor_id{
        static_cast<dom::sensor_id>(ui->txtSensorToAdd->text().toInt())};
    std::lock_guard<std::mutex> _lock{m_mutex};
    int _row = findRow(_sensor_id);
    if (_row == -1) {
      ui->tblTemperatures->insertRow(ui->tblTemperatures->rowCount());
      ui->tblTemperatures->setItem(
          ui->tblTemperatures->rowCount() - 1, 0,
          new QTableWidgetItem(QString::number(static_cast<int>(_sensor_id))));
      m_dispatcher->publish<MainWindow, asy::add_sensor>(_sensor_id);
      ui->txtSensorToAdd->setText("");
    }
  }
}

void MainWindow::on_btnDeleteSensor_clicked() {
  if (!ui->txtSensorToDelete->text().isEmpty()) {
    dom::sensor_id _sensor_id{
        static_cast<dom::sensor_id>(ui->txtSensorToDelete->text().toInt())};

    std::lock_guard<std::mutex> _lock{m_mutex};
    int _row = findRow(_sensor_id);
    if (_row != -1) {
      ui->tblTemperatures->removeRow(_row);
      m_dispatcher->publish<MainWindow, asy::remove_sensor>(_sensor_id);
    }
  }
}

void MainWindow::on_btnSetTemperature_clicked() {
  if (ui->txtTemperatureToSet->text().isEmpty() ||
      ui->txtSensorToSet->text().isEmpty()) {
    return;
  }

  dom::sensor_id _sensor_id{
      static_cast<dom::sensor_id>(ui->txtSensorToSet->text().toInt())};

  dom::temperature _temperature{ui->txtTemperatureToSet->text().toFloat()};

  std::lock_guard<std::mutex> _lock{m_mutex};
  if (findRow(_sensor_id) != -1) {
    m_dispatcher->publish<MainWindow, asy::set_temperature>(_sensor_id,
                                                            _temperature);
  }
}
