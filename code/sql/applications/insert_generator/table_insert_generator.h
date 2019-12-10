#ifndef TABLE_INSERT_GENERATOR_H
#define TABLE_INSERT_GENERATOR_H

#include <QMainWindow>
#include <QString>

#include <sql/applications/insert_generator/number_value_generator_definition.h>
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

private:
  void header_pks_definitions();
  void fill_pks_definitions();

private slots:
  void on_currentIndexChanged(const QString& text);

private:
  Ui::TableInsertGenerator* ui;
  generic::ptr<entities::table> m_table;
  NumberValueGeneratorDefinition* m_nvgd = nullptr;

  static const QString m_title;
};

#endif // TABLE_INSERT_GENERATOR_H
