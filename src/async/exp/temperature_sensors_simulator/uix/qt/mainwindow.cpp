#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tenacitas/src/async/exp/temperature_sensors_simulator/dat/sensor_id.h"
#include "tenacitas/src/async/exp/temperature_sensors_simulator/dat/temperature.h"
#include "tenacitas/src/async/handling_priority.h"
#include "tenacitas/src/async/result.h"
#include "tenacitas/src/format/fmt.h"

using namespace tenacitas::src::async::exp::temperature_sensors_simulator;
using namespace tenacitas;

MainWindow::MainWindow(logger &p_logger, dispatcher &p_dispatcher,
                       QWidget *parent)
    : QMainWindow(parent), m_logger(p_logger), m_dispatcher(p_dispatcher),
      ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  m_logger.set_inf();

  auto _result(m_dispatcher.template add_handling<evt::new_temperature>(
      "new-temperature", queue_new_temperature{m_logger},
      handler_new_temperature{this},

      src::async::handling_priority::high, 1));

  if (_result != src::async::result::OK)
  {
    TNCT_LOG_ERR(m_logger, "error creating handling for 'new_temperature");
    return;
  }
}

MainWindow::~MainWindow()
{
  delete ui;
}

int MainWindow::findRow(dat::sensor_id p_sensor_id) const
{
  int _row_count = ui->tblTemperatures->rowCount();
  for (int _i = 0; _i < _row_count; ++_i)
  {
    QString _sensor_id_str = ui->tblTemperatures->item(_i, 0)->text();
    auto    _sendor_id =
        static_cast<decltype(p_sensor_id)>(_sensor_id_str.toInt());
    if (_sendor_id == p_sensor_id)
    {
      return _i;
    }
  }
  return -1;
}

void MainWindow::on_new_temperature(evt::new_temperature &&p_evt)
{
  std::lock_guard<std::mutex> _lock{m_mutex};
  int                         _row = findRow(p_evt.sensor_id);
  if (_row != -1)
  {
    ui->tblTemperatures->setItem(
        _row, 1,
        new QTableWidgetItem(QString::number(p_evt.temperature, 'g', 4)));
  }
}

void MainWindow::on_btnAddSensor_clicked()
{
  if (!ui->txtSensorToAdd->text().isEmpty())
  {
    dat::sensor_id _sensor_id{
        static_cast<dat::sensor_id>(ui->txtSensorToAdd->text().toInt())};
    std::lock_guard<std::mutex> _lock{m_mutex};
    int                         _row = findRow(_sensor_id);
    if (_row == -1)
    {
      ui->tblTemperatures->insertRow(ui->tblTemperatures->rowCount());
      ui->tblTemperatures->setItem(
          ui->tblTemperatures->rowCount() - 1, 0,
          new QTableWidgetItem(QString::number(static_cast<int>(_sensor_id))));
      auto _result(m_dispatcher.template publish<evt::add_sensor>(_sensor_id));
      if (_result != src::async::result::OK)
      {
        m_logger.err(src::format::fmt(_result));
      }
      ui->txtSensorToAdd->setText("");
    }
  }
}

void MainWindow::on_btnDeleteSensor_clicked()
{
  if (!ui->txtSensorToDelete->text().isEmpty())
  {
    dat::sensor_id _sensor_id{
        static_cast<dat::sensor_id>(ui->txtSensorToDelete->text().toInt())};

    std::lock_guard<std::mutex> _lock{m_mutex};
    int                         _row = findRow(_sensor_id);
    if (_row != -1)
    {
      ui->tblTemperatures->removeRow(_row);
      auto _result(m_dispatcher.publish<evt::remove_sensor>(_sensor_id));
      if (_result != src::async::result::OK)
      {
        m_logger.err(src::format::fmt(_result));
      }
    }
  }
}

void MainWindow::on_btnSetTemperature_clicked()
{
  if (ui->txtTemperatureToSet->text().isEmpty()
      || ui->txtSensorToSet->text().isEmpty())
  {
    return;
  }

  dat::sensor_id _sensor_id{
      static_cast<dat::sensor_id>(ui->txtSensorToSet->text().toInt())};

  dat::temperature _temperature{ui->txtTemperatureToSet->text().toFloat()};

  std::lock_guard<std::mutex> _lock{m_mutex};
  if (findRow(_sensor_id) != -1)
  {
    auto _result(
        m_dispatcher.publish<evt::set_temperature>(_sensor_id, _temperature));
    if (_result != src::async::result::OK)
    {
      m_logger.err(src::format::fmt(_result));
    }
  }
}
