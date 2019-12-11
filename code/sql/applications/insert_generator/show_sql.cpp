#include "show_sql.h"
#include "ui_show_sql.h"

ShowSql::ShowSql(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::ShowSql)
{
  ui->setupUi(this);
}

ShowSql::~ShowSql()
{
  delete ui;
}

void
ShowSql::setSql(const std::string& p_sql)
{
  ui->txtSql->setText(p_sql.c_str());
}

void
ShowSql::on_btnClose_clicked()
{
  close();
}
