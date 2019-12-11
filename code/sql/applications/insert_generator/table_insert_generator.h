#ifndef TABLE_INSERT_GENERATOR_H
#define TABLE_INSERT_GENERATOR_H

#include <QMainWindow>
#include <QString>

#include <sql/applications/insert_generator/number_value_generator_definition.h>
#include <sql/applications/insert_generator/show_sql.h>
#include <sql/business/sql_generator.h>
#include <sql/business/table_generator.h>
#include <sql/entities/table.h>
#include <sql/generic/ptr.h>

using namespace capemisa::sql;

namespace Ui {
class TableInsertGenerator;
}

class TableInsertGenerator : public QMainWindow
{
  Q_OBJECT

public:
  explicit TableInsertGenerator(generic::ptr<entities::table> p_table,
                                QWidget* parent = nullptr);
  ~TableInsertGenerator();

  void set_number_value_generator_params(const QString& p_base,
                                         const QString& p_limit,
                                         const QString& p_increment);

private:
  void header_pks_definitions();
  void fill_pks_definitions();

private slots:
  void on_currentIndexChanged(const QString& text);

  void on_btnGenPks_clicked();

  void on_btnGenSql_clicked();

private:
  Ui::TableInsertGenerator* ui;
  generic::ptr<entities::table> m_table;
  generic::ptr<business::table_generator> m_table_generator;
  generic::ptr<entities::tables_values> m_all_pks;
  NumberValueGeneratorDefinition* m_number_value_generator_definition = nullptr;
  generic::ptr<const entities::table_values> m_pks_values;
  ShowSql* m_show_sql = nullptr;

  static const QString m_title;
};

#endif // TABLE_INSERT_GENERATOR_H
