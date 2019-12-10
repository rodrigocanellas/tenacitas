#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <map>

#include <QListWidget>
#include <QListWidgetItem>
#include <QMainWindow>

#include <sql/entities/hosts.h>
#include <sql/entities/server.h>
#include <sql/generic/ptr.h>

using namespace capemisa::sql;

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

private:
  void load_hosts();
  void load_db001(generic::ptr<entities::server> p_server);
  void load_db002(generic::ptr<entities::server> p_server);

  void display_hosts();

private:
  typedef std::map<generic::name, QMainWindow*> tables_windows;

private:
  Ui::MainWindow* ui;
  generic::ptr<entities::hosts> m_hosts;
  generic::ptr<entities::host> m_host;
  generic::ptr<entities::server> m_server;
  generic::ptr<entities::database> m_db;
  generic::ptr<entities::table> m_table;
  tables_windows m_tables_windows;
};
#endif // MAINWINDOW_H
