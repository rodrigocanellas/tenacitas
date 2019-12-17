#ifndef FIXED_VALUE_GENERATOR_H
#define FIXED_VALUE_GENERATOR_H

#include <QDialog>

namespace Ui {
class FixedValueGenerator;
}

class TableInsertGenerator;

class FixedValueGenerator : public QDialog
{
  Q_OBJECT

public:
  explicit FixedValueGenerator(TableInsertGenerator* p_tig,
                               QWidget* parent = nullptr);
  ~FixedValueGenerator();

private slots:
  void on_btnOk_clicked();

  void on_btnCancela_clicked();

private:
  Ui::FixedValueGenerator* ui;
  TableInsertGenerator* m_table_insert_generator;
};

#endif // FIXED_VALUE_GENERATOR_H
