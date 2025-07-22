#include "grid_solved.h"
#include "ui_grid_solved.h"

#include <cctype>
#include <functional>

#include <QBrush>
#include <QCursor>
#include <QVBoxLayout>

#include <tenacitas.lib/src/log/alg/logger.h>

using namespace tenacitas;

const QString Char::lowlight_style =
    "font: 700 12pt \"FreeMono\";color: rgb(0, 0, 0);";
const QString Char::hightlight_style =
    "font: 700 12pt \"FreeMono\";color: rgb(255, 0, 0);";
const QString Char::unused_style = "background-color: rgb(0, 0, 0);";

GridSolved::GridSolved(crosswords::mod::dispatcher::ptr p_dispatcher,
                       QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GridSolved), m_dispatcher(p_dispatcher) {
  ui->setupUi(this);

  m_original_background = QColor{255, 255, 255};

  m_dispatcher->subscribe<crosswords::asy::grid_create_solved>(
      [&](auto p_event) {
        TNCT_LOG_DEB("grid = ", *p_event.grid);
        // m_grid = p_event.grid;

        on_grid_solved(std::move(p_event));
      });

  connect(this, &GridSolved::grid_solved, this,
          &GridSolved::handle_grid_solved);

  connect(ui->tblHorizontalWords, &QTableWidget::cellClicked,
          [&](int p_row, int /*p_col*/) {
            on_words_table_cell_clicked(ui->tblHorizontalWords, p_row);
          });

  connect(ui->tblVerticalWords, &QTableWidget::cellClicked,
          [&](int p_row, int /*p_col*/) {
            on_words_table_cell_clicked(ui->tblVerticalWords, p_row);
          });

  ui->tblHorizontalWords->horizontalHeader()->setStretchLastSection(true);
  ui->tblVerticalWords->horizontalHeader()->setStretchLastSection(true);
  //  ui->tblGrid->setCursor(Qt::PointingHandCursor);
}

void GridSolved::on_words_table_cell_clicked(QTableWidget *p_table, int p_row) {
  auto _id{static_cast<uint16_t>(p_table->item(p_row, 0)->text().toUInt())};
  auto _end{m_grid->end()};
  for (auto _layout = m_grid->begin(); _layout != _end; ++_layout) {
    if (_id == _layout->get_id()) {
      on_letter_id_click(_layout);
      break;
    }
  }
}

GridSolved::~GridSolved() { delete ui; }

void GridSolved::on_grid_solved(crosswords::asy::grid_create_solved &&p_event) {
  m_grid = p_event.grid;
  emit grid_solved(/*p_event.grid*/);
}

void GridSolved::handle_grid_solved() {
  show_grid();
  show();
}

void GridSolved::add_word(QTableWidget *p_tbl,
                          crosswords::typ::grid::const_layout_ite p_layout) {
  p_tbl->setRowCount(p_tbl->rowCount() + 1);
  p_tbl->setItem(p_tbl->rowCount() - 1, 0,
                 new QTableWidgetItem(QString::number(p_layout->get_id())));
  p_tbl->setItem(
      p_tbl->rowCount() - 1, 1,
      new QTableWidgetItem(QString{p_layout->get_word().to_string().c_str()}));
  p_tbl->setItem(p_tbl->rowCount() - 1, 2,
                 new QTableWidgetItem(
                     QString{p_layout->get_explanation().to_string().c_str()}));
}

void GridSolved::add_word(crosswords::typ::grid::const_layout_ite p_layout) {

  auto _row{static_cast<int>(p_layout->get_row())};
  auto _col{static_cast<int>(p_layout->get_col())};

  auto _on_letter_id_click =
      [&](crosswords::typ::grid::const_layout_ite p_layout) {
        on_letter_id_click(p_layout);
      };

  auto _cell{reinterpret_cast<Content *>(ui->tblGrid->cellWidget(_row, _col))};
  if (!_cell) {
    auto _first_letter{new FirstLetter(ui->tblGrid, _on_letter_id_click)};
    ui->tblGrid->setCellWidget(_row, _col, _first_letter);
    _first_letter->set_layout(p_layout);
  } else {
    if (_cell->is_first()) {
      auto _first_letter{reinterpret_cast<FirstLetter *>(_cell)};
      _first_letter->set_layout(p_layout);
    } else {
      auto _first_letter{new FirstLetter(ui->tblGrid, _on_letter_id_click)};
      ui->tblGrid->setCellWidget(_row, _col, _first_letter);
      _first_letter->set_layout(p_layout);
    }
  }

  if (p_layout->get_orientation() == crosswords::typ::orientation::hori) {
    add_word(ui->tblHorizontalWords, p_layout);
  } else {
    add_word(ui->tblVerticalWords, p_layout);
  }
}

void GridSolved::show_grid() {
  ui->tblGrid->setRowCount(0);
  ui->tblGrid->setColumnCount(0);
  ui->tblHorizontalWords->setRowCount(0);
  ui->tblVerticalWords->setRowCount(0);
  m_first_letter = m_grid->end();

  auto _num_rows{m_grid->get_num_rows()};
  ui->tblGrid->setRowCount(_num_rows);

  auto _num_cols{m_grid->get_num_rows()};
  ui->tblGrid->setColumnCount(_num_cols);

  //  for (decltype(_num_cols) _col = 0; _col < _num_cols; ++_col) {
  //    ui->tblGrid->setColumnWidth(_col, 1);
  //  }

  for (auto _ite = m_grid->begin(); _ite != m_grid->end(); ++_ite) {
    add_word(_ite);
  }

  // fill unused cells
  for (decltype(_num_rows) _row = 0; _row < _num_rows; ++_row) {
    for (decltype(_num_cols) _col = 0; _col < _num_cols; ++_col) {
      if (ui->tblGrid->cellWidget(_row, _col) == nullptr) {
        ui->tblGrid->setCellWidget(_row, _col, new UnusedtLetter(m_dispatcher));
      }
    }
  }
  //  auto _col_width{ui->tblGrid->columnWidth(0)};
  //  auto _row_height{ui->tblGrid->rowHeight(0)};

  //  ui->tblGrid->setFixedWidth(_num_cols * _col_width + 1);
  //  ui->tblGrid->setFixedHeight(_num_rows * _row_height + 1);

  //  ui->tblGrid->resizeRowsToContents();
  ui->tblGrid->resizeColumnsToContents();
}

void GridSolved::highlight(crosswords::typ::grid::const_layout_ite p_layout) {
  auto _size{p_layout->get_word().size()};
  auto _row{p_layout->get_row()};
  auto _col{p_layout->get_col()};

  if (p_layout->get_orientation() == crosswords::typ::orientation::vert) {
    for (decltype(_size) _i = 0; _i < _size; ++_i) {
      reinterpret_cast<Content *>(ui->tblGrid->cellWidget(_row + _i, _col))
          ->highlight();
    }
  } else {
    for (decltype(_size) _i = 0; _i < _size; ++_i) {
      reinterpret_cast<Content *>(ui->tblGrid->cellWidget(_row, _col + _i))
          ->highlight();
    }
  }
}

void GridSolved::lowlight(crosswords::typ::grid::const_layout_ite p_layout) {
  auto _size{p_layout->get_word().size()};
  auto _row{p_layout->get_row()};
  auto _col{p_layout->get_col()};

  if (p_layout->get_orientation() == crosswords::typ::orientation::vert) {
    for (decltype(_size) _i = 0; _i < _size; ++_i) {
      reinterpret_cast<Content *>(ui->tblGrid->cellWidget(_row + _i, _col))
          ->lowlight();
    }
  } else {
    for (decltype(_size) _i = 0; _i < _size; ++_i) {
      reinterpret_cast<Content *>(ui->tblGrid->cellWidget(_row, _col + _i))
          ->lowlight();
    }
  }
}

void GridSolved::on_letter_id_click(
    crosswords::typ::grid::const_layout_ite p_first_letter) {
  if (m_first_letter != m_grid->end()) {
    lowlight(m_first_letter);
  }

  m_first_letter = p_first_letter;
  highlight(m_first_letter);

  QTableWidget *_table{nullptr};

  if (m_first_letter->get_orientation() == crosswords::typ::orientation::hori) {
    _table = ui->tblHorizontalWords;
  } else {
    _table = ui->tblVerticalWords;
  }
  {
    QTableWidget *_table{ui->tblVerticalWords};
    auto _rows{_table->rowCount()};
    for (decltype(_rows) _r = 0; _r < _rows; ++_r) {
      _table->item(_r, 0)->setForeground(QBrush(QColor(0, 0, 0)));
      _table->item(_r, 1)->setForeground(QBrush(QColor(0, 0, 0)));
      _table->item(_r, 2)->setForeground(QBrush(QColor(0, 0, 0)));
    }
  }
  {
    QTableWidget *_table{ui->tblHorizontalWords};
    auto _rows{_table->rowCount()};
    for (decltype(_rows) _r = 0; _r < _rows; ++_r) {
      _table->item(_r, 0)->setForeground(QBrush(QColor(0, 0, 0)));
      _table->item(_r, 1)->setForeground(QBrush(QColor(0, 0, 0)));
      _table->item(_r, 2)->setForeground(QBrush(QColor(0, 0, 0)));
    }
  }

  {
    auto _rows{_table->rowCount()};
    for (decltype(_rows) _r = 0; _r < _rows; ++_r) {
      if (_table->item(_r, 0)->text().toUInt() == m_first_letter->get_id()) {
        _table->item(_r, 0)->setForeground(QBrush(QColor(255, 0, 0)));
        _table->item(_r, 1)->setForeground(QBrush(QColor(255, 0, 0)));
        _table->item(_r, 2)->setForeground(QBrush(QColor(255, 0, 0)));
        _table->setCurrentItem(_table->item(_r, 0));
        break;
      }
    }
  }
}
