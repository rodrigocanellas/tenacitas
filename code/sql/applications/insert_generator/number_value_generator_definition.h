#ifndef NUMBER_VALUE_GENERATOR_DEFINITION_H
#define NUMBER_VALUE_GENERATOR_DEFINITION_H

#include <QDialog>

namespace Ui {
class NumberValueGeneratorDefinition;
}

class NumberValueGeneratorDefinition : public QDialog
{
    Q_OBJECT

  public:
    explicit NumberValueGeneratorDefinition(QWidget *parent = nullptr);
    ~NumberValueGeneratorDefinition();

  private:
    Ui::NumberValueGeneratorDefinition *ui;
};

#endif // NUMBER_VALUE_GENERATOR_DEFINITION_H
