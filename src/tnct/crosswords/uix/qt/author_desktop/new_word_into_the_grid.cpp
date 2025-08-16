#include "new_word_into_the_grid.h"
#include "ui_new_word_into_the_grid.h"

#include <tenacitas.crosswords/asy/new_word_into_the_grid.h>

NewWordIntoTheGrid::NewWordIntoTheGrid(
    crosswords::mod::dispatcher::ptr p_dispatcher, QWidget *parent)
    : QDialog(parent), ui(new Ui::NewWordIntoTheGrid),
      m_dispatcher(p_dispatcher) {
  ui->setupUi(this);
}

NewWordIntoTheGrid::~NewWordIntoTheGrid() { delete ui; }

void NewWordIntoTheGrid::on_btnOk_clicked() {
  using namespace crosswords::typ;

  word _word{ui->txtWord->text().toStdString()};
  if (_word.size() != 0) {

    m_dispatcher->publish<crosswords::evt::new_word_into_the_grid>(
        _word, crosswords::typ::explanation(
                   ui->txtExplanation->toPlainText().toStdString()));
  }
  close();
}

void NewWordIntoTheGrid::on_btnCancel_clicked() { close(); }
