#include "table_insert_generator.h"
#include "ui_table_insert_generator.h"

#include <QComboBox>
#include <QListWidget>

#include <sql/business/number_value_generator.h>

using namespace capemisa::sql::entities;
using namespace capemisa::sql::generic;
using namespace capemisa::sql::business;

const QString TableInsertGenerator::m_title("Gerador de 'insert' para ");

TableInsertGenerator::TableInsertGenerator(
  generic::ptr<entities::table> p_table,
  QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::TableInsertGenerator)
  , m_table(p_table)
{
  ui->setupUi(this);
  setWindowTitle(m_title + "\"" + m_table->get_name().c_str() + "\"");
  header_pks_definitions();
  fill_pks_definitions();
}

TableInsertGenerator::~TableInsertGenerator()
{
  delete ui;
}

void
TableInsertGenerator::header_pks_definitions()
{
  ui->tblPks->setHorizontalHeaderLabels(
    { "nome", "tipo", "tamanho", "única?", "gerador" });
}

void
TableInsertGenerator::fill_pks_definitions()
{
  ptr<primary_key> _pk = m_table->get_primary_key();
  uint16_t _num_cols_pk = _pk->get_num_pks_cols();
  for (uint16_t _i = 0; _i < _num_cols_pk; ++_i) {

    ptr<primary_key_column> _pk_col = _pk->get_pk_column(_i);
    if (!_pk_col->is_autoincrement()) {
      name _name = _pk_col->get_name();
      std::string _type = column::type2str(_pk_col->get_type());
      std::string _size = std::to_string(_pk_col->get_size());
      std::string _is_unique = (_pk_col->is_unique() ? "sim" : "não");
      int _row = ui->tblPks->rowCount() - 1;
      ui->tblPks->setItem(_row, 0, new QTableWidgetItem(_name.c_str()));
      ui->tblPks->setItem(_row, 1, new QTableWidgetItem(_type.c_str()));
      ui->tblPks->setItem(_row, 2, new QTableWidgetItem(_size.c_str()));
      ui->tblPks->setItem(_row, 3, new QTableWidgetItem(_is_unique.c_str()));
      QComboBox* _generators = new QComboBox();
      _generators->addItem("gerador de números");
      _generators->addItem("gerador de texto");
      ui->tblPks->setCellWidget(_row, 4, _generators);
      ui->tblPks->setColumnWidth(4, _generators->width());
      QObject::connect(_generators,
                       SIGNAL(currentIndexChanged(const QString&)),
                       this,
                       SLOT(on_currentIndexChanged(const QString&)));

      //      QPushButton* btn_edit = new QPushButton();
      //      btn_edit = new QPushButton();
      //      btn_edit->setText("Edit");
      //      ui->tblPks->setCellWidget(_row, 4, (QWidget*)btn_edit);

      ui->tblPks->insertRow(ui->tblPks->rowCount());
    }
  }
}

void
TableInsertGenerator::on_currentIndexChanged(const QString& text)
{
  if (text == "gerador de números") {
    if (m_nvgd == nullptr) {
      m_nvgd = new NumberValueGeneratorDefinition(this);
    }
    m_nvgd->show();
    m_nvgd->raise();
    // number_value_generator
  }
}
