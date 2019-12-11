#include "number_value_generator_definition.h"
#include "ui_number_value_generator_definition.h"

#include <sql/applications/insert_generator/table_insert_generator.h>

NumberValueGeneratorDefinition::NumberValueGeneratorDefinition(
  TableInsertGenerator* p_tig,
  QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::NumberValueGeneratorDefinition)
  , m_table_insert_generator(p_tig)
{
  ui->setupUi(this);
}

NumberValueGeneratorDefinition::~NumberValueGeneratorDefinition()
{
  delete ui;
}

void
NumberValueGeneratorDefinition::on_btnOk_clicked()
{
  m_table_insert_generator->set_number_value_generator_params(
    ui->txtBase->text(), ui->txtLimit->text(), ui->txtIncrement->text());
  close();
}

void
NumberValueGeneratorDefinition::on_btnCancel_clicked()
{
  close();
}
