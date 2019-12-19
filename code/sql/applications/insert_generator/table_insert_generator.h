#ifndef TABLE_INSERT_GENERATOR_H
#define TABLE_INSERT_GENERATOR_H

#include <utility>

#include <QCloseEvent>
#include <QMainWindow>
#include <QShowEvent>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <sql/applications/insert_generator/generator_definition_window_factory.h>
#include <sql/applications/insert_generator/number_value_generator_definition.h>
#include <sql/applications/insert_generator/show_sql.h>
#include <sql/business/sql_generator.h>
#include <sql/business/table_generator.h>
#include <sql/entities/table.h>
#include <sql/generic/ptr.h>

using namespace capemisa::sql;
using namespace capemisa::sql::generic;
using namespace capemisa::sql::business;
using namespace capemisa::sql::entities;

namespace Ui {
class TableInsertGenerator;
}

class TableInsertGenerator : public QMainWindow
{
  Q_OBJECT

public:
  explicit TableInsertGenerator(ptr<table> p_table,
                                ptr<tables_values> p_all_pks,
                                QWidget* parent = nullptr);
  ~TableInsertGenerator() override;

  void set_generator_params(const QString& p_params);

  const std::string & gen_sql() ;



private:
  void header_pks_definitions();
  void fill_pks_definitions();

  void header_fks_definitions();
  void fill_fks_definitions();

  void header_attrs_definitions();
  void fill_attrs_definitions();

  void setup_table_values();

  void add_to_table_values(ptr<column_values> p_column_values);

  bool already_generated(const QString& p_col_name) const;

  void adjust_grids();

public slots:
  void on_pk_generated(std::string p_table_name);

private slots:
  void on_currentPrimaryKeyIndexChanged(const QString& text);

  void on_currentForeignKeyIndexChanged(const QString& text);

  void on_currentAttributeIndexChanged(const QString& text);

  void on_btnGenPks_clicked();

  void on_btnGenSql_clicked();

  void on_btnGenFks_clicked();

  void on_btnMain_clicked();

  void on_btnGenAttrs_clicked();

  void resizeEvent(QResizeEvent*) override;
  void showEvent(QShowEvent*) override;

  void closeEvent(QCloseEvent *event) override;

signals:
  void pks_generated(std::string);   // table name
  void fks_generated(std::string);   // table name
  void attrs_generated(std::string); // table name
  void closing(std::string);


private:
  Ui::TableInsertGenerator* ui;
  ptr<table> m_table;
  ptr<tables_values> m_all_pks;
  ptr<table_generator> m_table_generator;
  ptr<const table_values> m_pks_values;
  ptr<const table_values> m_fks_values;
  ptr<const table_values> m_attrs_values;
  ShowSql* m_show_sql = nullptr;
  generator_definition_window_factory m_generator_definition_window_factory;

  std::pair<QTableWidget*, int> m_current_params_cell;

  static const QString m_title;

  std::string m_sql;
};

#endif // TABLE_INSERT_GENERATOR_H
