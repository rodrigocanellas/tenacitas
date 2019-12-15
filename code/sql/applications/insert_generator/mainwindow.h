#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <map>

#include <QListWidget>
#include <QListWidgetItem>
#include <QMainWindow>

#include <sql/applications/insert_generator/table_insert_generator.h>
#include <sql/entities/hosts.h>
#include <sql/entities/server.h>
#include <sql/entities/tables_values.h>
#include <sql/generic/ptr.h>

using namespace capemisa::sql::generic;
using namespace capemisa::sql::entities;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

private slots:
  void on_lstHosts_itemClicked(QListWidgetItem* item);

  void on_lstServers_itemClicked(QListWidgetItem* item);

  void on_lstDbs_itemClicked(QListWidgetItem* item);

  void on_lstTables_itemClicked(QListWidgetItem* item);

  void on_btnGenerate_clicked();

  void on_pks_generated(std::string p_table_name);
  void on_fks_generated(std::string p_table_name);

  void on_tblTables_cellClicked(int row, int column);

private:
  void load_hosts();
  void load_db001(ptr<server> p_server);
  void load_db002(ptr<server> p_server);

  void display_hosts();

private:
  typedef std::map<name, TableInsertGenerator*> tables_windows;

private:
  Ui::MainWindow* ui;
  ptr<tables_values> m_all_pks;
  ptr<hosts> m_hosts;
  ptr<host> m_host;
  ptr<server> m_server;
  ptr<database> m_db;
  ptr<table> m_table;

  tables_windows m_tables_windows;
};
#endif // MAINWINDOW_H
