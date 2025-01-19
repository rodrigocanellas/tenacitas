#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

#include "tenacitas.lib/async/exp/events_simulation/bus/start_simulation.h"
#include "tenacitas.lib/async/exp/events_simulation/eve/dispatcher.h"
#include "tenacitas.lib/log/cerr.h"

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
  prj::bus::start_simulation<logger, dispatcher> m_start_simulation{
      m_logger, m_dispatcher};
  bool m_simulation_running{false};
};

#endif // MAIN_WINDOW_H
