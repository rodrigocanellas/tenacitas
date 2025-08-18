#ifndef NEW_WORD_INTO_THE_GRID_H
#define NEW_WORD_INTO_THE_GRID_H

#include <optional>
#include <tuple>

#include <QDialog>

#include <tenacitas.crosswords/dom/dispatcher.h>
#include <tenacitas.crosswords/src/typ/explanation.h>
#include <tenacitas.crosswords/src/typ/orientation.h>
#include <tenacitas.crosswords/src/typ/word.h>

using namespace tenacitas;

namespace Ui {
class NewWordIntoTheGrid;
}

class NewWordIntoTheGrid : public QDialog {
  Q_OBJECT

public:
  explicit NewWordIntoTheGrid(crosswords::mod::dispatcher::ptr p_dispatcher,
                              QWidget *parent = nullptr);
  ~NewWordIntoTheGrid();

private slots:
  void on_btnOk_clicked();

  void on_btnCancel_clicked();

private:
  Ui::NewWordIntoTheGrid *ui;
  crosswords::mod::dispatcher::ptr m_dispatcher;
};

#endif // NEW_WORD_INTO_THE_GRID_H
