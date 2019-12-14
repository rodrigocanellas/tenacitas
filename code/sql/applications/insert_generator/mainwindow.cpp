#include "ui_mainwindow.h"

#include <cstdint>

#include <QMessageBox>
#include <QString>

#include <sql/applications/insert_generator/mainwindow.h>
#include <sql/applications/insert_generator/table_insert_generator.h>
#include <sql/entities/attribute_column.h>
#include <sql/entities/foreign_key.h>
#include <sql/entities/foreign_key_column.h>
#include <sql/entities/host.h>
#include <sql/entities/primary_key.h>
#include <sql/entities/primary_key_column.h>
#include <sql/entities/server.h>
#include <sql/entities/table.h>

using namespace capemisa::sql::entities;
using namespace capemisa::sql::generic;

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  load_hosts();
  display_hosts();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void
MainWindow::load_hosts()
{
  m_hosts = make_ptr<hosts>();

  generic::ptr<host> _host = m_hosts->add("host1", "172.152.92.27");

  generic::ptr<server> _server = _host->add_server("srv000");

  load_db001(_server);
  load_db002(_server);
}

void
MainWindow::load_db001(generic::ptr<entities::server> p_server)
{
  generic::ptr<database> _db = p_server->add_database("db001");

  {
    generic::ptr<table> _employee = _db->add_table("employee");

    generic::ptr<primary_key> _pk = _employee->get_primary_key();

    _pk->add_pk_column("id", column_type::int_4, false, true);

    _employee->add_attribute(
      "name", column_type::var_size_text, 100, false, false);
    _employee->add_attribute("birthday", column_type::date, false, false);

    generic::ptr<foreign_key> _employee_boss_fk =
      _employee->add_fk("boss_fk", true);

    _employee_boss_fk->add_column("boss_id", _pk->find_pk_column("id"));
  }
  {
    generic::ptr<table> _dependent = _db->add_table("dependent");
    _dependent->get_primary_key()->add_pk_column(
      "id", column_type::int_4, false, true);

    _dependent->add_attribute(
      "name", column_type::var_size_text, 100, false, false);
    _dependent->add_attribute("birthday", column_type::date, false, false);

    generic::ptr<table> _employee = _db->find("employee");
    generic::ptr<primary_key> _employee_pk = _employee->get_primary_key();

    generic::ptr<foreign_key> _dependent_fk = _dependent->add_fk("employee_fk");
    _dependent_fk->add_column("responsible",
                              _employee_pk->find_pk_column("id"));

    _dependent->get_primary_key()->add_fk_column(
      _dependent_fk->find_column("responsible"));
  }
  {
    generic::ptr<table> _project = _db->add_table("project");
    _project->get_primary_key()->add_pk_column(
      "id", column_type::int_4, true, true);
    _project->add_attribute(
      "name", column_type::var_size_text, 100, false, false);

    _project->get_primary_key()->add_pk_column(
      "id", column_type::int_4, true, true);
  }
  {
    generic::ptr<table> _employees_projects =
      _db->add_table("employees_projects");

    generic::ptr<table> _employee = _db->find("employee");
    generic::ptr<foreign_key> _employee_fk =
      _employees_projects->add_fk("employee_fk");
    _employee_fk->add_column(
      "employee_id", _employee->get_primary_key()->find_pk_column("id"));

    generic::ptr<table> _project = _db->find("project");
    generic::ptr<foreign_key> _project_fk =
      _employees_projects->add_fk("project_fk");
    _project_fk->add_column("project_id",
                            _project->get_primary_key()->find_pk_column("id"));

    generic::ptr<primary_key> _employees_projects_pk =
      _employees_projects->get_primary_key();

    _employees_projects_pk->add_fk_column(
      _employee_fk->find_column("employee_id"));

    _employees_projects_pk->add_fk_column(
      _project_fk->find_column("project_id"));
  }
}

void
MainWindow::load_db002(generic::ptr<entities::server> p_server)
{
  generic::ptr<database> _db = p_server->add_database("db002");
}

void
MainWindow::display_hosts()
{
  uint16_t _num_hosts = m_hosts->get_size<uint16_t>();
  for (uint16_t _count_host = 0; _count_host < _num_hosts; ++_count_host) {
    ui->lstHosts->addItem((*m_hosts)[_count_host]->get_name().c_str());
  }
}

void
MainWindow::on_lstHosts_itemClicked(QListWidgetItem* item)
{
  ui->lstServers->clear();
  name _host_name = item->text().toStdString();
  m_host = m_hosts->find(_host_name);

  uint16_t _num_servers = m_host->get_num_servers();
  for (uint16_t _count_servers = 0; _count_servers < _num_servers;
       ++_count_servers) {
    ui->lstServers->addItem(
      m_host->get_server(_count_servers)->get_name().c_str());
  }
}

void
MainWindow::on_lstServers_itemClicked(QListWidgetItem* item)
{
  ui->lstDbs->clear();
  name _server_name = item->text().toStdString();
  m_server = m_host->find(_server_name);

  uint16_t _num_dbs = m_server->get_num_databases();
  for (uint16_t _count_db = 0; _count_db < _num_dbs; ++_count_db) {
    ui->lstDbs->addItem(m_server->get_database(_count_db)->get_name().c_str());
  }
}

void
MainWindow::on_lstDbs_itemClicked(QListWidgetItem* item)
{
  ui->lstTables->clear();
  name _db_name = item->text().toStdString();
  m_db = m_server->find(_db_name);

  uint16_t _num_tables = m_db->get_num_tables();
  for (uint16_t _count_table = 0; _count_table < _num_tables; ++_count_table) {
    ui->lstTables->addItem(m_db->get_table(_count_table)->get_name().c_str());
  }
}

void
MainWindow::on_lstTables_itemClicked(QListWidgetItem* item)
{
  name _table_name = item->text().toStdString();
  m_table = m_db->find(_table_name);
}

void
MainWindow::on_btnGenerate_clicked()
{
  if (m_table == nullptr) {
    QMessageBox::warning(this, "Atenção", "É preciso escolher uma tabela");
    return;
  }
  TableInsertGenerator* _tig = nullptr;
  tables_windows::iterator _ite = m_tables_windows.find(m_table->get_name());
  if (_ite == m_tables_windows.end()) {
    _tig = new TableInsertGenerator(m_table, this);
    m_tables_windows.emplace(m_table->get_name(), _tig);
  }

  _tig->show();
  _tig->raise();
}
