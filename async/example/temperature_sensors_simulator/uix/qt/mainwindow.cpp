#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tenacitas.lib/async/example/temperature_sensors_simulator/typ/sensor_id.h"
#include "tenacitas.lib/async/example/temperature_sensors_simulator/typ/temperature.h"
#include "tenacitas.lib/async/handling_priority.h"
#include "tenacitas.lib/async/result.h"
#include "tenacitas.lib/generic/fmt.h"
#include "tenacitas.lib/log/cerr.h"

using namespace temperature_sensors_simulator;
using namespace tenacitas::lib;

MainWindow::MainWindow(logger &p_logger, dispatcher &p_dispatcher,
                       QWidget *parent)
    : QMainWindow(parent), m_logger(p_logger), m_dispatcher(p_dispatcher),
      ui(new Ui::MainWindow) {
  ui->setupUi(this);

  m_logger.set_inf();

  auto _result(
      m_dispatcher.template subscribe<MainWindow, evt::new_temperature>(
          "new-temp", *this, async::handling_priority::high, 1));
  if (_result != async::result::OK) {
    m_logger.err(generic::fmt(_result));
  }
}

MainWindow::~MainWindow() { delete ui; }

int MainWindow::findRow(typ::sensor_id p_sensor_id) const {
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

void MainWindow::on_new_temperature(evt::new_temperature &&p_evt) {
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
    typ::sensor_id _sensor_id{
        static_cast<typ::sensor_id>(ui->txtSensorToAdd->text().toInt())};
    std::lock_guard<std::mutex> _lock{m_mutex};
    int _row = findRow(_sensor_id);
    if (_row == -1) {
      ui->tblTemperatures->insertRow(ui->tblTemperatures->rowCount());
      ui->tblTemperatures->setItem(
          ui->tblTemperatures->rowCount() - 1, 0,
          new QTableWidgetItem(QString::number(static_cast<int>(_sensor_id))));
      auto _result(m_dispatcher.template publish<MainWindow, evt::add_sensor>(
          _sensor_id));
      if (_result != async::result::OK) {
        m_logger.err(generic::fmt(_result));
      }
      ui->txtSensorToAdd->setText("");
    }
  }
}

void MainWindow::on_btnDeleteSensor_clicked() {
  if (!ui->txtSensorToDelete->text().isEmpty()) {
    typ::sensor_id _sensor_id{
        static_cast<typ::sensor_id>(ui->txtSensorToDelete->text().toInt())};

    std::lock_guard<std::mutex> _lock{m_mutex};
    int _row = findRow(_sensor_id);
    if (_row != -1) {
      ui->tblTemperatures->removeRow(_row);
      auto _result(
          m_dispatcher.publish<MainWindow, evt::remove_sensor>(_sensor_id));
      if (_result != async::result::OK) {
        m_logger.err(generic::fmt(_result));
      }
    }
  }
}

void MainWindow::on_btnSetTemperature_clicked() {
  if (ui->txtTemperatureToSet->text().isEmpty() ||
      ui->txtSensorToSet->text().isEmpty()) {
    return;
  }

  typ::sensor_id _sensor_id{
      static_cast<typ::sensor_id>(ui->txtSensorToSet->text().toInt())};

  typ::temperature _temperature{ui->txtTemperatureToSet->text().toFloat()};

  std::lock_guard<std::mutex> _lock{m_mutex};
  if (findRow(_sensor_id) != -1) {
    auto _result(m_dispatcher.publish<MainWindow, evt::set_temperature>(
        _sensor_id, _temperature));
    if (_result != async::result::OK) {
      m_logger.err(generic::fmt(_result));
    }
  }
}
