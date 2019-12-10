#include "number_value_generator_definition.h"
#include "ui_number_value_generator_definition.h"

NumberValueGeneratorDefinition::NumberValueGeneratorDefinition(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::NumberValueGeneratorDefinition)
{
  ui->setupUi(this);
}

NumberValueGeneratorDefinition::~NumberValueGeneratorDefinition()
{
  delete ui;
}
