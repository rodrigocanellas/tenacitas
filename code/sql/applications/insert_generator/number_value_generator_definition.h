#ifndef NUMBER_VALUE_GENERATOR_DEFINITION_H
#define NUMBER_VALUE_GENERATOR_DEFINITION_H

#include <QDialog>

namespace Ui {
class NumberValueGeneratorDefinition;
}

class TableInsertGenerator;

class NumberValueGeneratorDefinition : public QDialog
{
  Q_OBJECT

public:
  explicit NumberValueGeneratorDefinition(TableInsertGenerator* p_tig,
                                          QWidget* parent = nullptr);
  ~NumberValueGeneratorDefinition();

private slots:
  void on_btnOk_clicked();

  void on_btnCancel_clicked();

private:
  Ui::NumberValueGeneratorDefinition* ui;
  TableInsertGenerator* m_table_insert_generator;
};

#endif // NUMBER_VALUE_GENERATOR_DEFINITION_H
