#include "fixed_value_generator.h"
#include "ui_fixed_value_generator.h"

#include <sql/applications/insert_generator/table_insert_generator.h>

FixedValueGenerator::FixedValueGenerator(TableInsertGenerator* p_tig,
                                         QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::FixedValueGenerator)
  , m_table_insert_generator(p_tig)

{
  ui->setupUi(this);
}

FixedValueGenerator::~FixedValueGenerator()
{
  delete ui;
}

void
FixedValueGenerator::on_btnOk_clicked()
{
  m_table_insert_generator->set_generator_params(ui->txtValue->text());
  close();
}

void
FixedValueGenerator::on_btnCancela_clicked()
{
  close();
}
