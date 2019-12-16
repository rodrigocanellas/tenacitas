#ifndef TEXT_VALUE_GENERATOR_DEFINITION_H
#define TEXT_VALUE_GENERATOR_DEFINITION_H

#include <QDialog>

class TableInsertGenerator;

namespace Ui {
class TextValueGeneratorDefinition;
}

class TextValueGeneratorDefinition : public QDialog
{
  Q_OBJECT

public:
  explicit TextValueGeneratorDefinition(TableInsertGenerator* p_tig,
                                        QWidget* parent = nullptr);
  ~TextValueGeneratorDefinition();

private slots:
  void on_btnOk_clicked();

  void on_btnCancela_clicked();

private:
  Ui::TextValueGeneratorDefinition* ui;
  TableInsertGenerator* m_table_insert_generator;
};

#endif // TEXT_VALUE_GENERATOR_DEFINITION_H
