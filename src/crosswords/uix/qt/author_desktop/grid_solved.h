#ifndef GRID_SOLVED_H
#define GRID_SOLVED_H

#include <memory>

#include <QColor>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>

#include <tenacitas.crosswords/dom/dispatcher.h>
#include <tenacitas.crosswords/asy/grid_create_solved.h>
#include <tenacitas.crosswords/src/pre/qt/author_desktop/new_word_into_the_grid.h>
#include <tenacitas.crosswords/src/typ/grid.h>
#include <tenacitas.lib/src/log/alg/logger.h>

namespace Ui {
class GridSolved;
}

using namespace tenacitas;

class Char : public QLabel {
  Q_OBJECT
public:
  Char(QChar p_value) : Char() { set(p_value); }

  Char() : QLabel() {
    setFrameShadow(QFrame::Shadow::Plain);
    setFrameStyle(QFrame::NoFrame);
  }

  void set(QChar p_value) {
    setAlignment(Qt::AlignCenter);
    setStyleSheet(lowlight_style);
    setText(QString(p_value).toUpper());
    setMaximumWidth(15);
    adjustSize();
  }

  void highlight() { setStyleSheet(hightlight_style); }
  void lowlight() { setStyleSheet(lowlight_style); }
  void unused() { setStyleSheet(unused_style); }

public:
  static const QString lowlight_style;
  static const QString hightlight_style;
  static const QString unused_style;
};

class Content : public QFrame {
  Q_OBJECT
public:
  Content() : QFrame() {
    setFrameShadow(QFrame::Shadow::Plain);
    setFrameStyle(QFrame::NoFrame);
  }
  virtual bool is_first() const { return false; }
  virtual void highlight() = 0;
  virtual void lowlight() = 0;
  virtual void unused() = 0;
};

class Letter : public Content {
  Q_OBJECT
public:
  Letter(QChar p_value) : Letter() { set(p_value); }

  Letter() : Content() {
    m_char = new Char();
    auto _layout{new QHBoxLayout()};
    _layout->addWidget(m_char);
    setLayout(_layout);
  }

  void set(QChar p_value) { m_char->set(p_value); }

  void highlight() override { m_char->highlight(); }
  void lowlight() override { m_char->lowlight(); }
  void unused() override { m_char->unused(); }

private:
  Char *m_char{nullptr};
};

class Superscript : public QLabel {
  Q_OBJECT
public:
  Superscript(QTableWidget *p_table,
              std::function<void(crosswords::typ::grid::const_layout_ite)>
                  p_letter_id_clicked)
      : QLabel(), m_table(p_table), m_letter_id_clicked(p_letter_id_clicked) {
    setMouseTracking(true);
    setAlignment(Qt::AlignCenter);
    setMaximumWidth(15);
    setFrameShape(QFrame::Shape::NoFrame);
    setFrameShadow(QFrame::Shadow::Plain);
    unsetCursor();
    setCursor(Qt::PointingHandCursor);
    setTextFormat(Qt::RichText);
  }

  void set(crosswords::typ::grid::const_layout_ite p_layout) {
    m_layout = p_layout;
    QString _str{"<sup>" +
                 QString::number(static_cast<int>(m_layout->get_id())) +
                 "</sup>"};
    this->setText(_str);
    this->adjustSize();
  }

  void mousePressEvent(QMouseEvent * /*event*/) {
    m_letter_id_clicked(m_layout);
  }

private:
  QTableWidget *m_table{nullptr};
  std::function<void(crosswords::typ::grid::const_layout_ite)>
      m_letter_id_clicked;
  crosswords::typ::grid::const_layout_ite m_layout;
};

class FirstLetter : public Content {
  Q_OBJECT
public:
  FirstLetter(QTableWidget *p_table,
              std::function<void(crosswords::typ::grid::const_layout_ite)>
                  p_letter_id_clicked)
      : Content(), m_table(p_table), m_char(nullptr),
        m_horizontal(new Superscript(m_table, p_letter_id_clicked)),
        m_vertical(new Superscript(m_table, p_letter_id_clicked)) {
    setFrameShape(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);
  }

  bool is_first() const override { return true; }

  void set_layout(crosswords::typ::grid::const_layout_ite p_layout) {
    auto _lay = layout();
    if (!_lay) {
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

    if (p_layout->get_orientation() == crosswords::typ::orientation::hori) {
      m_horizontal->set(p_layout);
      for (decltype(_size) _i = 1; _i < _size; ++_i) {
        if (!m_table->cellWidget(_row, _col + _i)) {
          m_table->setCellWidget(_row, _col + _i,
                                 new Letter(p_layout->get_word()[_i]));
        }
      }
    } else {
      m_vertical->set(p_layout);
      for (decltype(_size) _i = 1; _i < _size; ++_i) {
        if (!m_table->cellWidget(_row + _i, _col)) {
          m_table->setCellWidget(_row + _i, _col,
                                 new Letter(p_layout->get_word()[_i]));
        }
      }
    }
  }

  void highlight() override { m_char->highlight(); }
  void lowlight() override { m_char->lowlight(); }
  void unused() override { m_char->unused(); }

private:
  QTableWidget *m_table;
  Char *m_char;
  Superscript *m_horizontal;
  Superscript *m_vertical;
};

class UnusedtLetter : public Content {
  Q_OBJECT
public:
  UnusedtLetter(crosswords::mod::dispatcher::ptr p_dispatcher)
      : Content(), m_dispatcher(p_dispatcher) {
    setCursor(Qt::WhatsThisCursor);
    setStyleSheet(Char::unused_style);
  }
  void highlight() override { setStyleSheet(Char::unused_style); }
  void lowlight() override { setStyleSheet(Char::unused_style); }
  void unused() override { setStyleSheet(Char::unused_style); }
  void mousePressEvent(QMouseEvent *event) override {
    if (event->button() == Qt::LeftButton) {
      NewWordIntoTheGrid _newWordIntoTheGrid(m_dispatcher);

      _newWordIntoTheGrid.exec();
    }
  }

private:
  crosswords::mod::dispatcher::ptr m_dispatcher;
};

class GridSolved : public QMainWindow {
  Q_OBJECT

public:
  explicit GridSolved(crosswords::mod::dispatcher::ptr p_dispatcher,
                      QWidget *parent = nullptr);
  ~GridSolved();

  void on_letter_id_click(crosswords::typ::grid::const_layout_ite);

private:
  void on_grid_solved(crosswords::asy::grid_create_solved &&);

  void handle_grid_solved();

  void show_grid();

  void add_word(crosswords::typ::grid::const_layout_ite p_layout);

  void add_word(QTableWidget *p_tbl,
                crosswords::typ::grid::const_layout_ite p_layout);

  void highlight(crosswords::typ::grid::const_layout_ite);
  void lowlight(crosswords::typ::grid::const_layout_ite);

  void on_words_table_cell_clicked(QTableWidget *p_table, int p_row);

signals:
  void grid_solved();

private:
  Ui::GridSolved *ui;

  crosswords::mod::dispatcher::ptr m_dispatcher;

  std::shared_ptr<crosswords::typ::grid> m_grid;

  QColor m_original_background;

  crosswords::typ::grid::const_layout_ite m_first_letter;
};

#endif // GRID_SOLVED_H
