#include "main_window.h"
#include "ui_main_window.h"

#include <optional>
#include <utility>

#include <QErrorMessage>
#include <QFileDialog>
#include <QMessageBox>

#include "tenacitas.lib/async/exp/events_simulation/cfg/handlings.h"
#include "tenacitas.lib/async/exp/events_simulation/cfg/publishers.h"
#include "tenacitas.lib/async/exp/events_simulation/cfg/reader.h"
#include "tenacitas.lib/async/exp/events_simulation/eve/simulation_finished.h"
#include "tenacitas.lib/container/circular_queue.h"

using namespace tenacitas::lib;
namespace prj = tenacitas::lib::async::exp::events_simulation;

main_window::main_window(dispatcher &p_dispatcher, logger &p_logger,
                         QWidget *parent)
    : QMainWindow(parent), ui(new Ui::main_window), m_dispatcher(p_dispatcher),
      m_logger(p_logger) {
  ui->setupUi(this);

  auto _handle_simulation_finished = [&](prj::eve::simulation_finished &&) {
    m_simulation_running = false;
  };

  using queue_simulation_finished =
      container::circular_queue<logger, prj::eve::simulation_finished, 1>;

  auto _maybe_handling_id{m_dispatcher.subscribe<prj::eve::simulation_finished>(
      std::move(_handle_simulation_finished),
      queue_simulation_finished{m_logger})};

  if (!_maybe_handling_id) {
    TNCT_LOG_ERR(m_logger,
                 "could not create handling for event 'simulation_finished'");
  }
}

main_window::~main_window() { delete ui; }

void main_window::on_btnConfiguration_clicked() {
  if (m_simulation_running) {
    QMessageBox::warning(this, "",
                         "Can not start a new simulation while the current "
                         "simulation does not end");
    return;
  }
  auto _file_name = QFileDialog::getOpenFileName(
      this, tr("Open Configuration File"), "", tr("INI Files (*.ini)"));

  log::cerr _logger;

  prj::cfg::reader _reader;

  std::optional<std::pair<prj::cfg::publishers, prj::cfg::handlings>> _maybe{
      _reader(_logger, _file_name.toStdString())};

  if (!_maybe) {
    QErrorMessage _msg;
    _msg.showMessage("Could not load " + _file_name);
    return;
  }

  std::pair<prj::cfg::publishers, prj::cfg::handlings> _pair{
      std::move(_maybe.value())};

  prj::cfg::publishers _cfg_publishers{std::move(_pair.first)};
  for (const prj::cfg::publisher &_cfg_publisher : _cfg_publishers) {
    std::stringstream _stream;
    _stream << _cfg_publisher;
    ui->lstCfg->addItem(QString{_stream.str().c_str()});
  }

  prj::cfg::handlings _cfg_handlings{std::move(_pair.second)};
  for (const prj::cfg::handling &_cfg_handling : _cfg_handlings) {
    std::stringstream _stream;
    _stream << _cfg_handling;
    ui->lstCfg->addItem(QString{_stream.str().c_str()});
  }

  if (m_start_simulation(_cfg_handlings, _cfg_publishers)) {
    m_simulation_running = true;
  }
}
