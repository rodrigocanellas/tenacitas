#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "tnct/async/exp/temperature_sensors_simulator/evt/new_temperature.h"
#include "tnct/async/exp/temperature_sensors_simulator/supplier.h"
#include "tnct/container/dat/circular_queue.h"

using namespace tnct;
using namespace tnct::async::exp::temperature_sensors_simulator;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(logger &p_logger, dispatcher &p_dispatcher,
             QWidget *parent = nullptr);
  ~MainWindow();

  void operator()(evt::new_temperature &&p_evt)
  {
    on_new_temperature(std::move(p_evt));
  }

private:
  struct handler_new_temperature
  {
    using events_handled = std::tuple<evt::new_temperature>;
    handler_new_temperature(MainWindow *p_main_window)
        : m_main_window(p_main_window)
    {
    }

    void operator()(evt::new_temperature &&p_new_temperature)
    {
      (*m_main_window)(std::move(p_new_temperature));
    }

  private:
    MainWindow *m_main_window{nullptr};
  };

  using queue_new_temperature =
      container::dat::circular_queue<logger, evt::new_temperature, 10>;

private:
  void on_new_temperature(evt::new_temperature &&);
  int  findRow(dat::sensor_id) const;

private slots:
  void on_btnAddSensor_clicked();
  void on_btnDeleteSensor_clicked();
  void on_btnSetTemperature_clicked();

private:
  logger         &m_logger;
  dispatcher     &m_dispatcher;
  Ui::MainWindow *ui;
  std::mutex      m_mutex;
};
#endif // MAINWINDOW_H
