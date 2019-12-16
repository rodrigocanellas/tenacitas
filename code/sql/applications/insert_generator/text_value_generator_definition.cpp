#include "text_value_generator_definition.h"
#include "ui_text_value_generator_definition.h"

#include <sql/applications/insert_generator/table_insert_generator.h>

TextValueGeneratorDefinition::TextValueGeneratorDefinition(
  TableInsertGenerator* p_tig,
  QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::TextValueGeneratorDefinition)
  , m_table_insert_generator(p_tig)
{
  ui->setupUi(this);
}

TextValueGeneratorDefinition::~TextValueGeneratorDefinition()
{
  delete ui;
}

void
TextValueGeneratorDefinition::on_btnOk_clicked()
{
  m_table_insert_generator->set_generator_params(ui->txtBaseText->text() + ";" +
                                                 ui->txtIncrement->text());

  close();
}

void
TextValueGeneratorDefinition::on_btnCancela_clicked()
{
  close();
}
