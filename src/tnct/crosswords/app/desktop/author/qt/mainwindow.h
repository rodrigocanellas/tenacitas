#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColor>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPdfWriter>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>

#include "tnct/crosswords/bus/grid_creator.h"
#include "tnct/crosswords/dat/entries.h"
#include "tnct/crosswords/dat/grid.h"
#include "tnct/crosswords/dat/index.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"

using namespace tnct;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class Char : public QLabel
{
  Q_OBJECT
public:
  Char(QChar p_value) : Char()
  {
    set(p_value);
  }

  Char() : QLabel()
  {
    setFrameShadow(QFrame::Shadow::Plain);
    setFrameStyle(QFrame::NoFrame);
  }

  void set(QChar p_value)
  {
    setAlignment(Qt::AlignCenter);
    setStyleSheet(lowlight_style);
    setText(QString(p_value).toUpper());
    // setMaximumWidth(15);
    // adjustSize();
  }

  void highlight(crosswords::dat::orientation p_orientation)
  {
    setStyleSheet((p_orientation == crosswords::dat::orientation::hori
                       ? hightlight_style_horizontal
                       : hightlight_style_vertical));
  }
  void lowlight()
  {
    setStyleSheet(lowlight_style);
  }
  void unused()
  {
    setStyleSheet(unused_style);
  }

public:
  static const QString lowlight_style;
  static const QString hightlight_style_horizontal;
  static const QString hightlight_style_vertical;
  static const QString unused_style;
};

class Content : public QFrame
{
  Q_OBJECT
public:
  Content() : QFrame()
  {
    setFrameShadow(QFrame::Shadow::Plain);
    setFrameStyle(QFrame::NoFrame);
  }
  virtual bool is_first() const
  {
    return false;
  }
  virtual void highlight(crosswords::dat::orientation) = 0;
  virtual void lowlight()                              = 0;
  virtual void unused()                                = 0;
  virtual bool is_unused() const                       = 0;
};

class Letter : public Content
{
  Q_OBJECT
public:
  Letter(QChar p_value) : Letter()
  {
    set(p_value);
  }

  Letter() : Content()
  {
    m_char = new Char();
    auto _layout{new QHBoxLayout()};
    _layout->addWidget(m_char);
    setLayout(_layout);
  }

  void set(QChar p_value)
  {
    m_char->set(p_value);
  }

  void highlight(crosswords::dat::orientation p_orientation) override
  {
    m_char->highlight(p_orientation);
  }
  void lowlight() override
  {
    m_char->lowlight();
  }
  void unused() override
  {
    m_char->unused();
  }
  bool is_unused() const override
  {
    return false;
  }
  Char *get_char() const
  {
    return m_char;
  }

private:
  Char *m_char{nullptr};
};

class Superscript : public QLabel
{
  Q_OBJECT
public:
  Superscript(QTableWidget *p_table,
              std::function<void(crosswords::dat::grid::const_layout_ite)>
                  p_letter_id_clicked)
      : QLabel(), m_table(p_table), m_letter_id_clicked(p_letter_id_clicked)
  {
    setMouseTracking(true);
    setAlignment(Qt::AlignCenter);
    // setMaximumWidth(15);
    setFrameShape(QFrame::Shape::NoFrame);
    setFrameShadow(QFrame::Shadow::Plain);
    unsetCursor();
    setCursor(Qt::PointingHandCursor);
    setTextFormat(Qt::RichText);
  }

  void set(crosswords::dat::grid::const_layout_ite p_layout)
  {
    m_set    = true;
    m_layout = p_layout;
    QString _str{"<sup>" + QString::number(static_cast<int>(m_layout->get_id()))
                 + "</sup>"};
    this->setText(_str);
    //    this->adjustSize();
  }

  void mousePressEvent(QMouseEvent * /*event*/)
  {
    m_letter_id_clicked(m_layout);
  }

  uint16_t get_id() const
  {
    return (m_set ? m_layout->get_id() : 0);
  }

private:
  QTableWidget *m_table{nullptr};
  std::function<void(crosswords::dat::grid::const_layout_ite)>
                                          m_letter_id_clicked;
  crosswords::dat::grid::const_layout_ite m_layout;
  bool                                    m_set{false};
};

class FirstLetter : public Content
{
  Q_OBJECT
public:
  FirstLetter(QTableWidget *p_table,
              std::function<void(crosswords::dat::grid::const_layout_ite)>
                  p_letter_id_clicked)
      : Content(), m_table(p_table), m_char(nullptr),
        m_horizontal(new Superscript(m_table, p_letter_id_clicked)),
        m_vertical(new Superscript(m_table, p_letter_id_clicked))
  {
    setFrameShape(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);
  }

  bool is_first() const override
  {
    return true;
  }

  std::pair<uint16_t, uint16_t> get_id()
  {
    return {m_horizontal->get_id(), m_vertical->get_id()};
  }

  void set_layout(crosswords::dat::grid::const_layout_ite p_layout)
  {
    auto _lay = layout();
    if (!_lay)
    {
      _lay = new QHBoxLayout();
      _lay->addWidget(m_horizontal);
      m_char = new Char(p_layout->get_word()[0]);
      _lay->addWidget(m_char);
      _lay->addWidget(m_vertical);
      setLayout(_lay);
    }

    auto _size{p_layout->get_word().size()};
    auto _row{p_layout->get_row()};
    auto _col{p_layout->get_col()};

    if (p_layout->get_orientation() == crosswords::dat::orientation::hori)
    {
      m_horizontal->set(p_layout);
      m_orientation = crosswords::dat::orientation::hori;
      for (decltype(_size) _i = 1; _i < _size; ++_i)
      {
        if (!m_table->cellWidget(_row, _col + _i))
        {
          m_table->setCellWidget(_row, _col + _i,
                                 new Letter(p_layout->get_word()[_i]));
        }
      }
    }
    else
    {
      m_vertical->set(p_layout);
      m_orientation = crosswords::dat::orientation::vert;
      for (decltype(_size) _i = 1; _i < _size; ++_i)
      {
        if (!m_table->cellWidget(_row + _i, _col))
        {
          m_table->setCellWidget(_row + _i, _col,
                                 new Letter(p_layout->get_word()[_i]));
        }
      }
    }
  }

  bool is_unused() const override
  {
    return false;
  }

  Char *get_char() const
  {
    return m_char;
  }

  void highlight(crosswords::dat::orientation p_orientation) override
  {
    m_char->highlight(p_orientation);
  }
  void lowlight() override
  {
    m_char->lowlight();
  }
  void unused() override
  {
    m_char->unused();
  }

private:
  QTableWidget                *m_table{nullptr};
  Char                        *m_char{nullptr};
  Superscript                 *m_horizontal{nullptr};
  Superscript                 *m_vertical{nullptr};
  crosswords::dat::orientation m_orientation{
      crosswords::dat::orientation::undef};
};

class UnusedtLetter : public Content
{
  Q_OBJECT
public:
  UnusedtLetter() : Content()
  {
    setStyleSheet(Char::unused_style);
  }
  void highlight(crosswords::dat::orientation) override
  {
    setStyleSheet(Char::unused_style);
  }
  void lowlight() override
  {
    setStyleSheet(Char::unused_style);
  }
  void unused() override
  {
    setStyleSheet(Char::unused_style);
  }
  bool is_unused() const override
  {
    return true;
  }
};

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(log::cerr &p_logger, QWidget *parent = nullptr);

  ~MainWindow();

private:
  void on_add_word_clicked();

  void on_start();

  void on_unresolved();

  void on_import_clicked();

  void on_save_clicked();

  void on_save_as_clicked();

  void on_check_all_clicked();

  void handle_grid_solved();

  void show_grid();

  void add_word_to_grid(crosswords::dat::grid::const_layout_ite p_layout);

  void highlight(crosswords::dat::grid::const_layout_ite);

  void lowlight(crosswords::dat::grid::const_layout_ite);

  void on_letter_id_click(crosswords::dat::grid::const_layout_ite);

  void on_word_selected(int p_row);

  void on_print_clicked();

  void reset();

  void add_word_to_words(int _row, const crosswords::dat::word &p_word,
                         const crosswords::dat::explanation &p_explanation);

  void unselect_words();

  void create_check_box_first_column(int p_row, bool p_checked = true);

  void clear_grid();

  void clear_words();

signals:
  void unresolved();
  void grid_solved();
  void log(QString);

private:
  void solved(std::shared_ptr<crosswords::dat::grid> p_grid,
              std::chrono::seconds p_time, std::uint64_t p_max_permutations);

  void unsolved(crosswords::dat::index p_rows, crosswords::dat::index p_cols);

  void configuration(crosswords::dat::index p_rows,
                     crosswords::dat::index p_cols,
                     std::size_t            p_max_memory_to_be_used,
                     std::size_t            p_memory_available,
                     std::size_t            p_number_of_permutations);

  void permutations(std::size_t p_permutations);

private:
  void configure_dispatcher();

private:
  Ui::MainWindow *ui;

  log::cerr &m_logger;

  crosswords::bus::grid_creator<log::cerr> m_grid_creator;

  crosswords::dat::entries m_entries;

  std::shared_ptr<crosswords::dat::grid> m_grid;

  QColor m_original_background;

  crosswords::dat::grid::const_layout_ite m_first_letter;
  crosswords::dat::index m_current_row{crosswords::dat::invalid_index};

  std::atomic_bool m_solving{false};

  //  std::string m_file_name;
};

// template <typename t_factory>
// MainWindow<t_factory>::MainWindow(QWidget *parent)
//     : QMainWindow(parent), ui(new Ui::MainWindow) {
//   ui->setupUi(this);
// }

// template <typename t_factory> MainWindow<t_factory>::~MainWindow() {
//   delete ui;
// }
#endif // MAINWINDOW_H
