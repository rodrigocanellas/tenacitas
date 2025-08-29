#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCheckBox>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QPixmap>

#include <chrono>
#include <fstream>
#include <map>

#include "tnct/async/result.h"
#include "tnct/crosswords/evt/dispatcher.h"
#include "tnct/format/fmt.h"
#include "tnct/log/cerr.h"

using namespace tnct;

static const int USE_COL{0};
static const int WORD_COL{1};
static const int EXPLANATION_COL{2};

static const QBrush vertical_brush(QColor(0, 0, 255));
static const QBrush horizontal_brush(QColor(0, 255, 0));

const QString Char::lowlight_style =
    "font: 700 12pt \"FreeMono\";color: rgb(0, 0, 0);";
const QString Char::hightlight_style_horizontal =
    "font: 700 12pt \"FreeMono\";color: rgb(0, 255, 0);";
const QString Char::hightlight_style_vertical =
    "font: 700 12pt \"FreeMono\";color: rgb(0, 0, 255);";
const QString Char::unused_style = "background-color: rgb(100, 100, 100);";

MainWindow::MainWindow(log::cerr                   &m_logger,
                       crosswords::evt::dispatcher &p_dispatcher,
                       QWidget                     *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_logger(m_logger),
      m_dispatcher(p_dispatcher)
{
  ui->setupUi(this);

  configure_dispatcher();

  m_original_background = QColor{255, 255, 255};

  ui->tblWords->horizontalHeader()->setStretchLastSection(true);

  connect(ui->btnAddWord, &QPushButton::clicked, this,
          &MainWindow::on_add_word_clicked);

  connect(ui->btnRemoveWord, &QPushButton::clicked, this,
          [&]()
          {
            if (m_current_row == tnct::crosswords::dat::invalid_index)
            {
              m_current_row = ui->tblWords->currentRow();
            }
            ui->tblWords->removeRow(m_current_row);
            m_current_row = tnct::crosswords::dat::invalid_index;
            reset();
            // show();
          });

  connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::on_start);

  connect(ui->btnStop, &QPushButton::clicked, this,
          [&]()
          {
            if (auto _result =
                    m_dispatcher
                        .template publish<crosswords::evt::grid_create_stop>()
                    != async::result::OK)
            {
              TNCT_LOG_ERR(m_logger, format::fmt(_result));
            }
          });

  connect(this, &MainWindow::unresolved, this, &MainWindow::on_unresolved);

  connect(ui->btnImport, &QPushButton::clicked, this,
          &MainWindow::on_import_clicked);

  connect(ui->btnSave, &QPushButton::clicked, this,
          &MainWindow::on_save_clicked);

  connect(ui->btnSaveAs, &QPushButton::clicked, this,
          &MainWindow::on_save_as_clicked);

  connect(ui->spbNumRows, &QSpinBox::valueChanged, this,
          [&](int p_value)
          {
            if (ui->spbMaxRows->value() < p_value)
            {
              ui->spbMaxRows->setValue(p_value);
            }
          });

  connect(ui->spbNumRows, &QSpinBox::textChanged, this,
          [&](QString p_value)
          {
            auto _value{p_value.toInt()};
            if (ui->spbMaxRows->value() < _value)
            {
              ui->spbMaxRows->setValue(_value);
            }
          });

  connect(ui->tblWords, &QTableWidget::cellPressed,
          [&](int p_row, int) { on_word_selected(p_row); });

  connect(this, &MainWindow::grid_solved, this,
          &MainWindow::handle_grid_solved);

  connect(ui->btnClear, &QPushButton::clicked,
          [&]()
          {
            clear_words();
            clear_grid();
            ui->chbAllWords->setCheckState(Qt::Unchecked);
            ui->lblFileName->clear();
          });

  connect(ui->chbAllWords, &QCheckBox::clicked,
          [&](bool) { on_check_all_clicked(); });

  connect(ui->btnPrint, &QPushButton::clicked, this,
          &MainWindow::on_print_clicked);

  connect(ui->chbLog, &QCheckBox::clicked, this,
          [&](bool)
          {
            auto _is_checked{ui->chbLog->checkState()};
            if (_is_checked)
            {
              ui->txtLog->setVisible(true);
            }
            else
            {
              ui->txtLog->setVisible(false);
            }
          });

  connect(this, &MainWindow::log, this,
          [&](QString p_txt) { ui->txtLog->append(p_txt); });

  showMaximized();

  reset();

  ui->txtLog->setVisible(false);
  {
    QList<int> sizes;
    sizes << width() * 0.2 << width() * 0.8;
    ui->splVert->setSizes(sizes);
  }
  {
    QList<int> sizes;
    sizes << ui->fraGria->size().height() * 0.1
          << ui->fraGria->size().height() * 0.9;
    ui->splHori->setSizes(sizes);
  }
}

void MainWindow::configure_dispatcher()
{

  m_dispatcher.template add_handling<crosswords::evt::grid_create_unsolved>(
      "grid_create_unsolved", grid_create_unsolved_queue{m_logger},
      [&](crosswords::evt::grid_create_unsolved &&)
      {
        m_solving = false;
        emit unresolved();
      });

  m_dispatcher.template add_handling<crosswords::evt::grid_create_solved>(
      "grid_create_solved", grid_create_solved_queue{m_logger},
      [&](crosswords::evt::grid_create_solved &&p_event)
      {
        TNCT_LOG_DEB(m_logger, format::fmt("grid = ", *p_event.grid));
        // m_grid = p_event.grid;

        on_grid_solved(std::move(p_event));
      });

  m_dispatcher.template add_handling<crosswords::evt::grid_create_stop>(
      "grid_create_stop", grid_create_stop_queue{m_logger},
      [&](crosswords::evt::grid_create_stop &&)
      { emit log({"Montagem interrompida\n"}); });

  m_dispatcher
      .template add_handling<crosswords::evt::grid_attempt_configuration>(
          "grid_attempt_configuration",
          grid_attempt_configuration_queue{m_logger},
          [&](crosswords::evt::grid_attempt_configuration &&p_event)
          {
            emit log(
                QString{"############\n"}
                + QString{"Iniciando tentativa de montagem da grade "}
                + QString::number(p_event.num_rows) + "x"
                + QString::number(p_event.num_cols) + "\n"
                + QString{"Número de permutações: "
                          + QString::number(p_event.number_of_permutations)}
                + "\n"
                + QString{"Memória disponível: "
                          + QString::number(p_event.memory_available / 1024
                                            / 1024)
                          + " MB\n"}
                + QString{"Máximo de memória a ser usada: "
                          + QString::number(p_event.max_memory_to_be_used / 1024
                                            / 1024)
                          + " MB\n"});
          });

  m_dispatcher.template add_handling<crosswords::evt::grid_permutations_tried>(
      "grid_permutations_tried", grid_permutations_tried_queue{m_logger},
      [&](crosswords::evt::grid_permutations_tried &&p_event)
      {
        emit log(QString::number(p_event.permutations)
                 + " permutações tentadas\n");
      });
}

void MainWindow::create_check_box_first_column(int p_row, bool p_checked)
{
  QTableWidgetItem *_item{new QTableWidgetItem()};
  _item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  _item->setTextAlignment(Qt::AlignCenter);

  TNCT_LOG_DEB(m_logger, format::fmt("checked = ", Qt::Checked,
                                     ", unchecked = ", Qt::Unchecked));
  _item->setCheckState((p_checked ? Qt::Checked : Qt::Unchecked));
  ui->tblWords->setItem(p_row, USE_COL, _item);
}

void MainWindow::clear_grid()
{
  auto _num_rows{static_cast<crosswords::dat::index>(ui->spbNumRows->value())};
  auto _num_cols{static_cast<crosswords::dat::index>(ui->spbNumCols->value())};

  ui->tblGrid->setRowCount(_num_rows);
  ui->tblGrid->setColumnCount(_num_cols);

  for (decltype(_num_rows) _r = 0; _r < _num_rows; ++_r)
  {
    for (decltype(_num_cols) _c = 0; _c < _num_cols; ++_c)
    {
      ui->tblGrid->setCellWidget(_r, _c, new UnusedtLetter());
    }
  }
  ui->tblGrid->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_grid.reset();
}

void MainWindow::clear_words()
{
  ui->tblWords->setRowCount(0);
  m_current_row = tnct::crosswords::dat::invalid_index;
}

void MainWindow::add_word_to_words(
    int p_row, const crosswords::dat::word &p_word,
    const crosswords::dat::explanation &p_explanation)
{

  ui->tblWords->insertRow(p_row);

  create_check_box_first_column(p_row);

  ui->tblWords->setItem(p_row, WORD_COL,
                        new QTableWidgetItem(QString{p_word.c_str()}));
  ui->tblWords->setItem(p_row, EXPLANATION_COL,
                        new QTableWidgetItem(QString{p_explanation.c_str()}));
}

void MainWindow::on_word_selected(int p_row)
{
  if (m_grid == nullptr)
  {
    return;
  }

  auto _check_box_item{ui->tblWords->item(p_row, USE_COL)};
  if (_check_box_item == nullptr)
  {
    return;
  }

  if (_check_box_item->checkState() == Qt::Unchecked)
  {
    return;
  }

  auto _word_item = ui->tblWords->item(p_row, WORD_COL);
  if (_word_item == nullptr)
  {
    return;
  }
  auto _word{_word_item->text().toStdString()};
  if (_word.empty())
  {
    return;
  }
  auto _end{m_grid->end()};

  for (auto _layout = m_grid->begin(); _layout != _end; ++_layout)
  {
    if (_word == _layout->get_word())
    {
      on_letter_id_click(_layout);
      break;
    }
  }
}

void MainWindow::reset()
{
  clear_grid();
  unselect_words();
  ui->txtLog->clear();
  m_current_row = tnct::crosswords::dat::invalid_index;
  ui->btnPrint->setVisible(false);
  ui->btnStart->setVisible(true);
  ui->btnStop->setVisible(false);
}

void MainWindow::unselect_words()
{
  const QBrush _not_selected(QColor(0, 0, 0));
  auto         _rows{ui->tblWords->rowCount()};
  for (decltype(_rows) _r = 0; _r < _rows; ++_r)
  {
    if (ui->tblWords->item(_r, WORD_COL))
    {
      ui->tblWords->item(_r, WORD_COL)->setForeground(_not_selected);
    }
    if (ui->tblWords->item(_r, EXPLANATION_COL))
    {
      ui->tblWords->item(_r, EXPLANATION_COL)->setForeground(_not_selected);
    }
  }
}

void MainWindow::on_letter_id_click(
    crosswords::dat::grid::const_layout_ite p_first_letter)
{
  const QBrush _not_selected(QColor(0, 0, 0));

  if (m_first_letter != m_grid->end())
  {
    lowlight(m_first_letter);
    ui->tblWords->item(m_current_row, WORD_COL)->setForeground(_not_selected);
    ui->tblWords->item(m_current_row, EXPLANATION_COL)
        ->setForeground(_not_selected);
  }

  m_first_letter = p_first_letter;
  highlight(m_first_letter);

  const QBrush _brush{m_first_letter->get_orientation()
                              == crosswords::dat::orientation::hori
                          ? horizontal_brush
                          : vertical_brush};

  auto _rows{ui->tblWords->rowCount()};
  for (decltype(_rows) _r = 0; _r < _rows; ++_r)
  {
    if (ui->tblWords->item(_r, WORD_COL)->text().toStdString()
        == m_first_letter->get_word())
    {
      ui->tblWords->item(_r, WORD_COL)->setForeground(_brush);
      ui->tblWords->item(_r, EXPLANATION_COL)->setForeground(_brush);
      ui->tblWords->setCurrentItem(ui->tblWords->item(_r, WORD_COL));
      m_current_row = _r;
      //      ui->tblWords->setCurrentItem(nullptr);
      break;
    }
  }
}

void MainWindow::handle_grid_solved()
{
  show_grid();
  show();
}

void MainWindow::add_word_to_grid(
    crosswords::dat::grid::const_layout_ite p_layout)
{

  auto _row{static_cast<int>(p_layout->get_row())};
  auto _col{static_cast<int>(p_layout->get_col())};

  auto _on_letter_id_click =
      [&](crosswords::dat::grid::const_layout_ite p_layout)
  { on_letter_id_click(p_layout); };

  auto _cell{reinterpret_cast<Content *>(ui->tblGrid->cellWidget(_row, _col))};
  if (!_cell)
  {
    auto _first_letter{new FirstLetter(ui->tblGrid, _on_letter_id_click)};
    ui->tblGrid->setCellWidget(_row, _col, _first_letter);
    _first_letter->set_layout(p_layout);
  }
  else
  {
    if (_cell->is_first())
    {
      auto _first_letter{reinterpret_cast<FirstLetter *>(_cell)};
      _first_letter->set_layout(p_layout);
    }
    else
    {
      auto _first_letter{new FirstLetter(ui->tblGrid, _on_letter_id_click)};
      ui->tblGrid->setCellWidget(_row, _col, _first_letter);
      _first_letter->set_layout(p_layout);
    }
  }
}

void MainWindow::show_grid()
{
  ui->tblGrid->setRowCount(0);
  ui->tblGrid->setColumnCount(0);

  m_first_letter = m_grid->end();

  auto _num_rows{m_grid->get_num_rows()};
  ui->tblGrid->setRowCount(_num_rows);

  auto _num_cols{m_grid->get_num_cols()};
  ui->tblGrid->setColumnCount(_num_cols);

  for (auto _ite = m_grid->begin(); _ite != m_grid->end(); ++_ite)
  {
    add_word_to_grid(_ite);
  }

  // fill unused cells
  for (decltype(_num_rows) _row = 0; _row < _num_rows; ++_row)
  {
    for (decltype(_num_cols) _col = 0; _col < _num_cols; ++_col)
    {
      if (ui->tblGrid->cellWidget(_row, _col) == nullptr)
      {
        ui->tblGrid->setCellWidget(_row, _col, new UnusedtLetter());
      }
    }
  }
  //  ui->tblGrid->resizeColumnsToContents();
  //  ui->tblGrid->
  ui->tblGrid->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  ui->btnStop->setVisible(false);
  ui->btnStart->setVisible(true);
  ui->btnPrint->setVisible(true);
}

void MainWindow::highlight(crosswords::dat::grid::const_layout_ite p_layout)
{
  auto _size{p_layout->get_word().size()};
  auto _row{p_layout->get_row()};
  auto _col{p_layout->get_col()};
  auto _orientation{p_layout->get_orientation()};

  if (_orientation == crosswords::dat::orientation::vert)
  {
    for (decltype(_size) _i = 0; _i < _size; ++_i)
    {
      reinterpret_cast<Content *>(ui->tblGrid->cellWidget(_row + _i, _col))
          ->highlight(_orientation);
    }
  }
  else
  {
    for (decltype(_size) _i = 0; _i < _size; ++_i)
    {
      reinterpret_cast<Content *>(ui->tblGrid->cellWidget(_row, _col + _i))
          ->highlight(_orientation);
    }
  }
}

void MainWindow::lowlight(crosswords::dat::grid::const_layout_ite p_layout)
{
  auto _size{p_layout->get_word().size()};
  auto _row{p_layout->get_row()};
  auto _col{p_layout->get_col()};

  if (p_layout->get_orientation() == crosswords::dat::orientation::vert)
  {
    for (decltype(_size) _i = 0; _i < _size; ++_i)
    {
      reinterpret_cast<Content *>(ui->tblGrid->cellWidget(_row + _i, _col))
          ->lowlight();
    }
  }
  else
  {
    for (decltype(_size) _i = 0; _i < _size; ++_i)
    {
      reinterpret_cast<Content *>(ui->tblGrid->cellWidget(_row, _col + _i))
          ->lowlight();
    }
  }
}

void MainWindow::on_grid_solved(crosswords::evt::grid_create_solved &&p_event)
{
  m_solving = false;
  m_grid    = p_event.grid;
  emit log(QString{"Grade montada em " + QString::number(p_event.time.count())
                   + " segundos\n"});
  emit grid_solved(/*p_event.grid*/);
}

void MainWindow::on_unresolved()
{
  QMessageBox qMessageBox;
  qMessageBox.warning(nullptr, "NÃO RESOLVIDO",
                      "Não foi possível montar a grade");
  ui->btnStart->setVisible(true);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_add_word_clicked()
{
  ui->tblWords->setRowCount(ui->tblWords->rowCount() + 1);
  if (ui->tblWords->rowCount() == 1)
  {
    ui->chbAllWords->setChecked(true);
  }
  //  {
  //    QWidget *pWidget = new QWidget();
  //    QCheckBox *pCheckBox = new QCheckBox();
  //    pCheckBox->setCheckState(Qt::Checked);
  //    QHBoxLayout *pLayout = new QHBoxLayout(pWidget);
  //    pLayout->addWidget(pCheckBox);
  //    pLayout->setAlignment(Qt::AlignCenter);
  //    pLayout->setContentsMargins(0, 0, 0, 0);
  //    pWidget->setLayout(pLayout);
  //    ui->tblWords->setCellWidget(ui->tblWords->rowCount() - 1, USE_COL,
  //    pWidget);
  //  }
  create_check_box_first_column(ui->tblWords->rowCount() - 1);
  ui->tblWords->setCurrentCell(ui->tblWords->rowCount() - 1, WORD_COL);
}

void MainWindow::on_start()
{
  if (m_solving)
  {
    return;
  }

  reset();

  m_entries.clear();

  auto _rows{ui->tblWords->rowCount()};
  for (decltype(_rows) _row = 0; _row < _rows; ++_row)
  {
    auto _item{ui->tblWords->item(_row, USE_COL)};
    if (!_item)
    {
      return;
    }

    auto _checked_state{_item->checkState()};
    TNCT_LOG_DEB(
        m_logger,
        format::fmt("check state is = ",
                    ui->tblWords->item(_row, WORD_COL)->text().toStdString(),
                    " = ", _checked_state));
    if (_checked_state == Qt::Checked)
    {
      QTableWidgetItem *_item_word{ui->tblWords->item(_row, WORD_COL)};
      QTableWidgetItem *_item_explanation{
          ui->tblWords->item(_row, EXPLANATION_COL)};

      if (_item_word && _item_explanation)
      {
        QString _text0{_item_word->text().trimmed()};
        QString _text1{_item_explanation->text().trimmed()};
        if (!_text0.isEmpty() && !_text1.isEmpty())
        {
          m_entries.add_entry(_item_word->text().toStdString(),
                              _item_explanation->text().toStdString());
          TNCT_LOG_DEB(m_logger,
                       format::fmt("word = ", _text0.toStdString(),
                                   ", explanation = ", _text1.toStdString()));
        }
      }
    }
  }

  if (m_entries.empty())
  {
    return;
  }

  auto _num_rows{static_cast<crosswords::dat::index>(ui->spbNumRows->value())};
  auto _num_cols{static_cast<crosswords::dat::index>(ui->spbNumCols->value())};
  auto _max_rows{static_cast<crosswords::dat::index>(ui->spbMaxRows->value())};

  std::chrono::seconds _interval;
  if (ui->rdbSeconds->isChecked())
  {
    _interval = std::chrono::seconds{ui->spbInterval->value()};
  }
  else if (ui->rdbMinutes->isChecked())
  {
    _interval = std::chrono::seconds{ui->spbInterval->value() * 60};
  }
  else
  {
    _interval = std::chrono::seconds{ui->spbInterval->value() * 60 * 24};
  }

  if (auto _result =
          m_dispatcher.template publish<crosswords::evt::grid_create_start>(
              m_entries, _num_rows, _num_cols, _interval, _max_rows)
          != async::result::OK)
  {
    TNCT_LOG_ERR(m_logger, format::fmt(_result));
  }

  ui->btnStop->setVisible(true);
  ui->btnStart->setVisible(false);
}

void MainWindow::on_import_clicked()
{
  auto _file_name{QFileDialog::getOpenFileName(this, tr("Abrir palavras"), "./",
                                               "*.cross")};

  if (_file_name.isEmpty())
  {
    return;
  }

  ui->lblFileName->setText(_file_name);

  std::ifstream _file(ui->lblFileName->text().toStdString());

  std::string _line;

  std::getline(_file, _line);
  int _row{0};
  while (!_file.eof())
  {
    auto        _pos{_line.find('|')};
    std::string _word{_line.substr(0, _pos)};
    ++_pos;
    std::string _explanation{_line.substr(_pos, _line.size())};

    add_word_to_words(_row, _word, _explanation);

    ++_row;

    std::getline(_file, _line);
  }
  ui->chbAllWords->setCheckState(Qt::Checked);
}

void MainWindow::on_save_clicked()
{
  QFileDialog _save_file;

  bool    _add_extension{false};
  QString _file_name{ui->lblFileName->text()};
  if (_file_name.isEmpty())
  {
    _add_extension = true;
  }

  _file_name = _save_file.getSaveFileName(
      this, tr("Salvar palavras"), (_file_name.isEmpty() ? "" : _file_name),
      tr("*.cross"));

  if (_file_name.isEmpty())
  {
    return;
  }

  if (_add_extension)
  {
    _file_name += ".cross";
  }

  std::ofstream _file(_file_name.toStdString());
  if (_file.bad())
  {
    QMessageBox qMessageBox;
    qMessageBox.critical(nullptr, "", "erro salvando arquivo");
    return;
  }

  auto _rows{ui->tblWords->rowCount()};

  for (decltype(_rows) _row = 0; _row < _rows; ++_row)
  {
    QTableWidgetItem *_item_word{ui->tblWords->item(_row, WORD_COL)};
    QTableWidgetItem *_item_explanation{
        ui->tblWords->item(_row, EXPLANATION_COL)};

    if (_item_word && _item_explanation)
    {
      QString _text0{_item_word->text().trimmed()};
      QString _text1{_item_explanation->text().trimmed()};
      _file << _text0.toStdString() << '|' << _text1.toStdString() << '\n';
    }
  }
  ui->lblFileName->setText(_file_name);
}

void MainWindow::on_save_as_clicked()
{
  QString _file_name = QFileDialog::getSaveFileName(this, tr("Salvar palavras"),
                                                    "./", tr("*.cross"));

  if (_file_name.isEmpty())
  {
    return;
  }
  std::ofstream _file(_file_name.toStdString());
  if (_file.bad())
  {
    QMessageBox qMessageBox;
    qMessageBox.critical(nullptr, "", "erro salvando arquivo");
    return;
  }

  auto _rows{ui->tblWords->rowCount()};

  for (decltype(_rows) _row = 0; _row < _rows; ++_row)
  {
    QTableWidgetItem *_item_word{ui->tblWords->item(_row, WORD_COL)};
    QTableWidgetItem *_item_explanation{
        ui->tblWords->item(_row, EXPLANATION_COL)};

    if (_item_word && _item_explanation)
    {
      QString _text0{_item_word->text().trimmed()};
      QString _text1{_item_explanation->text().trimmed()};
      _file << _text0.toStdString() << '|' << _text1.toStdString() << '\n';
    }
  }
  ui->lblFileName->setText(_file_name);
}

void MainWindow::on_check_all_clicked()
{
  auto _rows{ui->tblWords->rowCount()};

  auto _is_checked{ui->chbAllWords->checkState()};

  for (decltype(_rows) _row = 0; _row < _rows; ++_row)
  {
    auto _item{ui->tblWords->item(_row, USE_COL)};
    _item->setCheckState(_is_checked);
  }
}

// void MainWindow::on_print_clicked() {
//   using namespace tnct::crosswords;

//  QString _file_name = QFileDialog::getSaveFileName(this, tr("Imprimir
//  grid"),
//                                                    "./", tr("*.pdf"));

//  if (_file_name.isEmpty()) {
//    return;
//  }

//  const int _cols{ui->tblGrid->columnCount()};
//  const int _rows{ui->tblGrid->rowCount()};

//  QPdfWriter _pdf(_file_name);
//  _pdf.setPageSize(QPageSize::A4);
//  _pdf.setPageOrientation(QPageLayout::Orientation::Portrait);
//  _pdf.setPageMargins(QMargins(10, 10, 10, 10), QPageLayout::Millimeter);

//  QString _html{"<!DOCTYPE html> "
//                "<html> "
//                "<head> "
//                "<style> "
//                "table { border-collapse: collapse; } "
//                "td { border: 1px solid Gray; } "
//                "td.special { border: none; } "
//                "td.alignRight { text-align: right; } "
//                "td.alignLeft { text-align: left; } "
//                "</style> "
//                "</head> "
//                "<body> "
//                "<table style=\"width:100%\"> "};

//  int _col_width = 100 / _rows;

//  _html += "<tr> ";
//  for (int _col = 0; _col < _cols; ++_col) {
//    _html += "<th style=\"width: " + QString::number(_col_width) + "%\"></th>
//    ";
//  }

//  for (int _row = 0; _row < _rows; ++_row) {
//    _html += "<tr style=\"height:90px\"";

//    for (int _col = 0; _col < _cols; ++_col) {
//      Content *_content =
//          reinterpret_cast<Content *>(ui->tblGrid->cellWidget(_row, _col));
//      if (_content->is_unused()) {
//        _html += "<td style=\"width: " + QString::number(_col_width) +
//                 "%\" class=\"special\"></td> ";
//      } else if (_content->is_first()) {
//        FirstLetter *_first_letter{reinterpret_cast<FirstLetter *>(_content)};

//        auto _id_hor{_first_letter->get_id().first};
//        auto _id_ver{_first_letter->get_id().second};
//        if ((_id_hor != 0) && (_id_ver != 0)) {
//          _html += "<td style=\"width: " + QString::number(_col_width) +
//                   "%\" <sup>" + QString::number(_id_hor) +
//                   "</sup> &nbsp; &nbsp; &nbsp;<sup>" +
//                   QString::number(_id_ver) + "</sup></td>";

//        } else if (_id_hor != 0) {
//          _html += "<td style=\"width: " + QString::number(_col_width) +
//                   "%\" <sup> " + QString::number(_id_hor) + " </sup></td> ";
//        } else if (_id_ver != 0) {
//          _html += "<td style=\"width: " + QString::number(_col_width) +
//                   "%\" &nbsp;&nbsp;&nbsp;&nbsp; <sup> " +
//                   QString::number(_id_ver) + "</sup></td> ";
//        }
//      } else {
//        _html +=
//            "<td style=\"width: " + QString::number(_col_width) + "%\" ></td>
//            ";
//      }
//    }
//    _html += "</tr> ";
//  }

//  _html += "</table> "
//           "</body> "
//           "</html> ";

//  {
//    std::ofstream _out("/var/tmp/html.out");
//    _out << _html.toStdString();
//  }
//  QTextDocument _text;
//  _text.setHtml(_html);
//  _text.print(&_pdf);
//}

// void MainWindow::on_print_clicked()
// {
//   using namespace tnct::crosswords;

//   QString _file_name = QFileDialog::getSaveFileName(this, tr("Imprimir
//   grid"),
//                                                     "./", tr("*.pdf"));

//   if (_file_name.isEmpty())
//   {
//     return;
//   }

//   const int _cols{ui->tblGrid->columnCount()};
//   const int _rows{ui->tblGrid->rowCount()};

//   if (_file_name.contains(".pdf", Qt::CaseInsensitive))
//   {
//     _file_name = _file_name.left(_file_name.size() - 4);
//   }

//   QString _png_to_solve_file_name{_file_name + ".png"};
//   QString _png_solution_file_name{_file_name + ".SOLUTION.png"};
//   {
//     const int _x0{10};
//     const int _y0{10};

//     const int _width{250};
//     const int _height{250};

//     QPixmap _paint_to_solve_device(_rows * _height + 400, _cols * _width +
//     400); _paint_to_solve_device.fill(); QPainter
//     _painter_to_solve(&_paint_to_solve_device);

//     QPixmap _paint_solution_device(_rows * _height + 400, _cols * _width +
//     400); _paint_solution_device.fill(); QPainter
//     _painter_solution(&_paint_solution_device);

//     QPen line_pen(Qt::black);
//     line_pen.setWidth(5);
//     _painter_to_solve.setPen(line_pen);
//     _painter_solution.setPen(line_pen);
//     QFont _font;
//     for (int _row = 0; _row < _rows; ++_row)
//     {
//       int _yi = _y0 + (_row * _height);
//       int _yf = _yi + _height;
//       for (int _col = 0; _col < _cols; ++_col)
//       {

//         int _xi = _x0 + (_col * _width);
//         int _xf = _xi + _width;

//         Content *_content =
//             reinterpret_cast<Content *>(ui->tblGrid->cellWidget(_row, _col));
//         if (!_content->is_unused())
//         {
//           _painter_to_solve.drawLine(_xi, _yi, _xf, _yi);
//           _painter_to_solve.drawLine(_xf, _yi, _xf, _yf);
//           _painter_to_solve.drawLine(_xf, _yf, _xi, _yf);
//           _painter_to_solve.drawLine(_xi, _yf, _xi, _yi);
//           _painter_to_solve.fillRect(QRect{QPoint{_xi, _yi}, QPoint{_xf,
//           _yf}},
//                                      QColor{250, 250, 250});

//           _painter_solution.drawLine(_xi, _yi, _xf, _yi);
//           _painter_solution.drawLine(_xf, _yi, _xf, _yf);
//           _painter_solution.drawLine(_xf, _yf, _xi, _yf);
//           _painter_solution.drawLine(_xi, _yf, _xi, _yi);
//           _painter_solution.fillRect(QRect{QPoint{_xi, _yi}, QPoint{_xf,
//           _yf}},
//                                      QColor{250, 250, 250});

//           if (_content->is_first())
//           {
//             _font.setPointSize(50);
//             _painter_solution.setFont(_font);
//             FirstLetter *_first_letter{
//                 reinterpret_cast<FirstLetter *>(_content)};

//             _painter_solution.drawText(_xi + 0.4 * _width, _yi + 0.4 *
//             _height,
//                                        _first_letter->get_char()->text());

//             auto _pair{_first_letter->get_id()};

//             _font.setPointSize(35);
//             _painter_to_solve.setFont(_font);
//             _painter_solution.setFont(_font);
//             if (_pair.first != 0)
//             {
//               _painter_to_solve.drawText(_xi + (0.05 * _width),
//                                          _yi + (0.3 * _height),
//                                          QString::number(_pair.first));
//               _painter_solution.drawText(_xi + (0.05 * _width),
//                                          _yi + (0.3 * _height),
//                                          QString::number(_pair.first));
//             }
//             if (_pair.second != 0)
//             {
//               _painter_to_solve.drawText(_xi + (0.7 * _width),
//                                          _yi + (0.3 * _height),
//                                          QString::number(_pair.second));
//               _painter_solution.drawText(_xi + (0.7 * _width),
//                                          _yi + (0.3 * _height),
//                                          QString::number(_pair.second));
//             }
//           }
//           else
//           {
//             Letter *_letter{reinterpret_cast<Letter *>(_content)};
//             _font.setPointSize(50);
//             _painter_solution.setFont(_font);

//             _painter_solution.drawText(_xi + 0.4 * _width, _yi + 0.4 *
//             _height,
//                                        _letter->get_char()->text());
//           }
//         }
//       }
//     }

//     _paint_to_solve_device.save(_png_to_solve_file_name, "PNG");
//     _paint_solution_device.save(_png_solution_file_name, "PNG");
//   }

//   QString _html{"<!DOCTYPE html> "
//                 "<html> "
//                 "<head> "
//                 "<style> "
//                 "table { "
//                 "font-family: arial, sans-serif; "
//                 "font-size: 80%;"
//                 "border: 1px solid black;"
//                 "border-collapse: collapse; "
//                 "width: 100%; "
//                 "} "
//                 "td, th { "
//                 "border: 1px solid #dddddd;"
//                 "text-align: left; "
//                 "padding: 2px; "
//                 "}"
//                 "</style> "
//                 "</head> "
//                 "<body> "};

//   //  "img{ "
//   //  "max-height:500px;"
//   //  "max-width:500px;"
//   //  "height:auto;"
//   //  "width:auto;"
//   //  "}"

//   {
//     using lines = std::map<uint16_t, std::pair<dat::word, dat::explanation>>;

//     lines _horizontals;
//     lines _verticals;

//     for (dat::grid::const_layout_ite _ite = m_grid->begin();
//          _ite != m_grid->end(); ++_ite)
//     {
//       if (_ite->get_orientation() == dat::orientation::hori)
//       {
//         _horizontals[_ite->get_id()] =
//             std::make_pair(_ite->get_word(), _ite->get_explanation());
//       }
//       else
//       {
//         _verticals[_ite->get_id()] =
//             std::make_pair(_ite->get_word(), _ite->get_explanation());
//       }
//     }

//     auto _print = [&](const QString &p_title, const lines &p_lines)
//     {
//       _html += "<h3>" + p_title + "</h3>";
//       _html += "<table style=\"width:100%\"> ";
//       _html += "<tr>"
//                "<th border-collapse: collapse style=\"width: 2%\" ></th>"
//                "<th border-collapse: collapse style=\"width: 98%\" ></th>"
//                "</tr>";

//       for (const lines::value_type &_value : p_lines)
//       {
//         _html += "<tr> ";
//         _html += "<td><small>" + QString::number(_value.first) +
//         "<small></td>"; _html += "<td> <small>" +
//         QString(_value.second.second.c_str()) + " ("
//                  + QString::number(_value.second.first.size()) + " letras)"
//                  + "</small></td>";
//         _html += "</tr>";
//       }

//       _html += "</table> ";
//       _html += "<p style=\"page-break-after: always;\">&nbsp;</p>";
//     };

//     _print("Horizontais", _horizontals);
//     _print("Verticais", _verticals);
//   }

//   {
//     _html += "<p class=\"aligncenter\"> ";
//     _html += "<img src=\"" + _png_to_solve_file_name
//              + "\" alt=\"grid\" width=\"600\" height=\"600\">";
//     _html += "</p>";
//   }

//   _html += "</body> "
//            "</html> ";

//   QPdfWriter _pdf(_file_name + ".pdf");
//   _pdf.setPageSize(QPageSize::A4);
//   _pdf.setPageOrientation((_cols > _rows ?
//   QPageLayout::Orientation::Landscape
//                                          :
//                                          QPageLayout::Orientation::Portrait));
//   _pdf.setPageMargins(QMargins(10, 10, 10, 10), QPageLayout::Millimeter);

//   QTextDocument _text;
//   _text.setDefaultFont(QFont("Times", 8));
//   _text.setHtml(_html);
//   _text.print(&_pdf);

//   std::filesystem::remove(
//       std::filesystem::path(_png_to_solve_file_name.toStdString()));
// }

void MainWindow::on_print_clicked()
{
  using namespace tnct::crosswords;

  QString _file_name =
      QFileDialog::getSaveFileName(this, tr("Imprimir grid"), "./", "");

  if (_file_name.isEmpty())
  {
    return;
  }

  const int _cols{ui->tblGrid->columnCount()};
  const int _rows{ui->tblGrid->rowCount()};

  if (_file_name.contains(".pdf", Qt::CaseInsensitive))
  {
    _file_name = _file_name.left(_file_name.size() - 4);
  }

  QString _png_to_solve_file_name{_file_name + ".png"};
  QString _png_solution_file_name{_file_name + ".SOLUTION.png"};
  {
    const int _x0{10};
    const int _y0{10};

    const int _width{250};
    const int _height{250};

    QPixmap _paint_to_solve_device(_rows * _height + 400, _cols * _width + 400);
    _paint_to_solve_device.fill();
    QPainter _painter_to_solve(&_paint_to_solve_device);

    QPixmap _paint_solution_device(_rows * _height + 400, _cols * _width + 400);
    _paint_solution_device.fill();
    QPainter _painter_solution(&_paint_solution_device);

    QPen line_pen(Qt::black);
    line_pen.setWidth(5);
    _painter_to_solve.setPen(line_pen);
    _painter_solution.setPen(line_pen);
    QFont _font;
    for (int _row = 0; _row < _rows; ++_row)
    {
      int _yi = _y0 + (_row * _height);
      int _yf = _yi + _height;
      for (int _col = 0; _col < _cols; ++_col)
      {

        int _xi = _x0 + (_col * _width);
        int _xf = _xi + _width;

        Content *_content =
            reinterpret_cast<Content *>(ui->tblGrid->cellWidget(_row, _col));
        if (!_content->is_unused())
        {
          _painter_to_solve.drawLine(_xi, _yi, _xf, _yi);
          _painter_to_solve.drawLine(_xf, _yi, _xf, _yf);
          _painter_to_solve.drawLine(_xf, _yf, _xi, _yf);
          _painter_to_solve.drawLine(_xi, _yf, _xi, _yi);
          _painter_to_solve.fillRect(QRect{QPoint{_xi, _yi}, QPoint{_xf, _yf}},
                                     QColor{250, 250, 250});

          _painter_solution.drawLine(_xi, _yi, _xf, _yi);
          _painter_solution.drawLine(_xf, _yi, _xf, _yf);
          _painter_solution.drawLine(_xf, _yf, _xi, _yf);
          _painter_solution.drawLine(_xi, _yf, _xi, _yi);
          _painter_solution.fillRect(QRect{QPoint{_xi, _yi}, QPoint{_xf, _yf}},
                                     QColor{250, 250, 250});

          if (_content->is_first())
          {
            _font.setPointSize(50);
            _painter_solution.setFont(_font);
            FirstLetter *_first_letter{
                reinterpret_cast<FirstLetter *>(_content)};

            _painter_solution.drawText(_xi + 0.4 * _width, _yi + 0.4 * _height,
                                       _first_letter->get_char()->text());

            auto _pair{_first_letter->get_id()};

            _font.setPointSize(35);
            _painter_to_solve.setFont(_font);
            _painter_solution.setFont(_font);
            if (_pair.first != 0)
            {
              _painter_to_solve.drawText(_xi + (0.05 * _width),
                                         _yi + (0.3 * _height),
                                         QString::number(_pair.first));
              _painter_solution.drawText(_xi + (0.05 * _width),
                                         _yi + (0.3 * _height),
                                         QString::number(_pair.first));
            }
            if (_pair.second != 0)
            {
              _painter_to_solve.drawText(_xi + (0.7 * _width),
                                         _yi + (0.3 * _height),
                                         QString::number(_pair.second));
              _painter_solution.drawText(_xi + (0.7 * _width),
                                         _yi + (0.3 * _height),
                                         QString::number(_pair.second));
            }
          }
          else
          {
            Letter *_letter{reinterpret_cast<Letter *>(_content)};
            _font.setPointSize(50);
            _painter_solution.setFont(_font);

            _painter_solution.drawText(_xi + 0.4 * _width, _yi + 0.4 * _height,
                                       _letter->get_char()->text());
          }
        }
      }
    }

    _paint_to_solve_device.save(_png_to_solve_file_name, "PNG");
    _paint_solution_device.save(_png_solution_file_name, "PNG");
  }

  {
    using lines = std::map<uint16_t, std::pair<dat::word, dat::explanation>>;

    lines _horizontals;
    lines _verticals;

    for (dat::grid::const_layout_ite _ite = m_grid->begin();
         _ite != m_grid->end(); ++_ite)
    {
      if (_ite->get_orientation() == dat::orientation::hori)
      {
        _horizontals[_ite->get_id()] =
            std::make_pair(_ite->get_word(), _ite->get_explanation());
      }
      else
      {
        _verticals[_ite->get_id()] =
            std::make_pair(_ite->get_word(), _ite->get_explanation());
      }
    }

    std::ofstream _tables{std::string(_file_name.toStdString()) + ".txt"};

    for (const auto &_hor : _horizontals)
    {
      _tables << std::setw(2) << std::setfill(' ') << std::right << _hor.first
              << '\t' << ' ' << _hor.second.second << " ("
              << _hor.second.first.size() << " letras)\n";
    }

    for (const auto &_ver : _verticals)
    {
      _tables << std::setw(2) << std::setfill(' ') << std::right << _ver.first
              << '\t' << ' ' << _ver.second.second << " ("
              << _ver.second.first.size() << " letras)\n";
    }
  }
}
