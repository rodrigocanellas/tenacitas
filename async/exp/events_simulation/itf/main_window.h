#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

#include "tenacitas.lib/async/exp/events_simulation/eve/dispatcher.h"
#include "tenacitas.lib/log/cerr.h"

#include "tenacitas.lib/async/exp/events_simulation/bus/configuration_defined_handler.h"

using namespace tenacitas::lib;
namespace prj = tenacitas::lib::async::exp::events_simulation;

using logger = log::cerr;
using dispatcher = prj::eve::dispatcher<log::cerr>;

namespace Ui {
class main_window;
}

class main_window : public QMainWindow {
  Q_OBJECT

public:
  explicit main_window(dispatcher &p_dispatcher, logger &p_logger,
                       QWidget *parent = nullptr);
  ~main_window();

private slots:
  void on_btnConfiguration_clicked();

private:
  Ui::main_window *ui;
  dispatcher &m_dispatcher;
  logger &m_logger;
  bool m_simulation_running{false};

  prj::bus::configuration_defined_handler<logger>
      m_configuration_defined_handler{m_logger};
};

#endif // MAIN_WINDOW_H
