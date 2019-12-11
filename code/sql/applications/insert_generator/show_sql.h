#ifndef SHOW_SQL_H
#define SHOW_SQL_H

#include <QDialog>

namespace Ui {
class ShowSql;
}

class ShowSql : public QDialog
{
  Q_OBJECT

public:
  explicit ShowSql(QWidget* parent = nullptr);
  ~ShowSql();

  void setSql(const std::string& p_sql);

private slots:
  void on_btnClose_clicked();

private:
  Ui::ShowSql* ui;
};

#endif // SHOW_SQL_H
