#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tnct/crosswords/dat/grid.h"
#include "tnct/crosswords/dat/index.h"

#include <QKeyEvent>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

using namespace tnct;

template <typename t>
class MainWindow : public QMainWindow
{
  // Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  void on_tblGrid_cellActivated(int row, int column);

  void on_rbbVertical_clicked();

  void on_rdbHorizontal_clicked();

protected:
  void keyPressEvent(QKeyEvent *event) override;

private:
  Ui::MainWindow              *ui;
  int                          m_start_row{crosswords::dat::invalid_index};
  int                          m_start_col{crosswords::dat::invalid_index};
  int                          m_current_row{crosswords::dat::invalid_index};
  int                          m_current_col{crosswords::dat::invalid_index};
  crosswords::mod::orientation m_current_orientation{
      crosswords::mod::orientation::undef};
};

////#include "mainwindow.h"
#include "ui_mainwindow.h"

////using namespace tenacitas;

#include <tenacitas.lib/src //log.h>

template <typename t>
MainWindow<t>::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  connect(ui->rbbVertical, &QRadioButton::clicked, this,
          &MainWindow::on_rbbVertical_clicked);

  connect(ui->rdbHorizontal, &QRadioButton::clicked, this,
          &MainWindow::on_rdbHorizontal_clicked);

  connect(ui->tblGrid, &QTableWidget::cellActivated, this,
          &MainWindow::on_tblGrid_cellActivated);

  connect(ui->tblGrid, &QTableWidget::cellEntered, this,
          &MainWindow::on_tblGrid_cellActivated);

  connect(ui->tblGrid, &QTableWidget::cellClicked, this,
          &MainWindow::on_tblGrid_cellActivated);
}

template <typename t>
MainWindow<t>::~MainWindow()
{
  delete ui;
}

template <typename t>
void MainWindow<t>::on_tblGrid_cellActivated(int row, int column)
{

  m_start_row = m_current_row = row;
  m_start_col = m_current_col = column;

  ui->lblRowValue->setText(QString::number(m_start_row + 1));
  ui->lblColumnValue->setText(QString::number(m_start_col + 1));
}

template <typename t>
void MainWindow<t>::on_rbbVertical_clicked()
{
  m_current_orientation = crosswords::mod::orientation::vert;
}

template <typename t>
void MainWindow<t>::on_rdbHorizontal_clicked()
{
  m_current_orientation = crosswords::mod::orientation::hori;
}

template <typename t>
void MainWindow<t>::keyPressEvent(QKeyEvent *event)
{
  if (m_current_orientation == crosswords::mod::orientation::undef)
  {
    return;
  }

  if (m_current_col > ui->tblGrid->columnCount())
  {
    return;
  }

  if (m_current_row > ui->tblGrid->rowCount())
  {
    return;
  }

  auto _key{event->key()};

  if ((_key >= Qt::Key_A) && (_key <= Qt::Key_Z))
  {
    QChar _char{_key};

    QTableWidgetItem *_cell = ui->tblGrid->item(m_current_row, m_current_col);
    if (!_cell)
    {
      _cell = new QTableWidgetItem;
      ui->tblGrid->setItem(m_current_row, m_current_col, _cell);
    }

    _cell->setText(_char);

    if (m_current_orientation == crosswords::mod::orientation::vert)
    {
      ++m_current_row;
    }
    else
    {
      ++m_current_col;
    }
  }
}

#endif // MAINWINDOW_H
