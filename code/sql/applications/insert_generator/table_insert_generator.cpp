#include "table_insert_generator.h"
#include "ui_table_insert_generator.h"

#include <set>

#include <QComboBox>
#include <QDebug>
#include <QListWidget>
#include <QMessageBox>
#include <QStringList>

//#include <sql/business/number_value_generator.h>

#include <sql/business/attribute_columns_generators_factory.h>
#include <sql/business/foreign_keys_columns_generator_factory.h>
#include <sql/business/primary_keys_columns_generator_factory.h>

#define PK_NAME_IDX 0
#define PK_TYPE_IDX 1
#define PK_SIZE_IDX 2
#define PK_UNIQUE_IDX 3
#define PK_GENERATOR_IDX 4
#define PK_PARAMS_IDX 5

#define ATTR_NAME_IDX 0
#define ATTR_TYPE_IDX 1
#define ATTR_SIZE_IDX 2
#define ATTR_UNIQUE_IDX 3
#define ATTR_GENERATOR_IDX 4
#define ATTR_PARAMS_IDX 5

#define FK_NAME_IDX 0
#define FK_GENERATOR_IDX 1
#define FK_PARAMS_IDX 2

using namespace capemisa::sql::entities;
using namespace capemisa::sql::generic;
using namespace capemisa::sql::business;

const QString TableInsertGenerator::m_title("Gerador de 'insert' para ");

TableInsertGenerator::TableInsertGenerator(
  generic::ptr<entities::table> p_table,
  ptr<tables_values> p_all_pks,
  QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::TableInsertGenerator)
  , m_table(p_table)
  , m_all_pks(p_all_pks)
{
  ui->setupUi(this);
  setWindowTitle(m_title + "\"" + m_table->get_name().c_str() + "\"");

  header_pks_definitions();
  fill_pks_definitions();

  header_fks_definitions();
  fill_fks_definitions();

  header_attrs_definitions();
  fill_attrs_definitions();

  //  adjust_grids();
}

TableInsertGenerator::~TableInsertGenerator()
{
  delete ui;
}

void
TableInsertGenerator::set_generator_params(const QString& p_params)
{
  QTableWidget* _tbl = m_current_params_cell.first;
  _tbl->setItem(_tbl->currentRow(),
                m_current_params_cell.second,
                new QTableWidgetItem(p_params));
}

void
TableInsertGenerator::header_pks_definitions()
{
  ui->tblPks->setHorizontalHeaderLabels(
    { "nome", "tipo", "tamanho", "única?", "gerador", "params" });
  ui->tblPks->setColumnHidden(PK_PARAMS_IDX, true);
  ui->tblPks->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void
TableInsertGenerator::header_attrs_definitions()
{
  ui->tblAttrs->setHorizontalHeaderLabels(
    { "nome", "tipo", "tamanho", "única?", "gerador", "params" });
  ui->tblAttrs->setColumnHidden(ATTR_PARAMS_IDX, true);
  ui->tblAttrs->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void
TableInsertGenerator::header_fks_definitions()
{
  ui->tblFks->setHorizontalHeaderLabels({ "nome", "gerador", "params" });
  ui->tblFks->setColumnHidden(FK_PARAMS_IDX, true);
  ui->tblPks->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
      std::string _type = _pk_col->get_type().to_str();
      std::string _size = std::to_string(_pk_col->get_size());
      std::string _is_unique = (_pk_col->is_unique() ? "sim" : "não");
      ui->tblPks->insertRow(ui->tblPks->rowCount());
      int _row = ui->tblPks->rowCount() - 1;
      ui->tblPks->setItem(
        _row, PK_NAME_IDX, new QTableWidgetItem(_name.c_str()));
      ui->tblPks->setItem(
        _row, PK_TYPE_IDX, new QTableWidgetItem(_type.c_str()));
      ui->tblPks->setItem(
        _row, PK_SIZE_IDX, new QTableWidgetItem(_size.c_str()));
      ui->tblPks->setItem(
        _row, PK_UNIQUE_IDX, new QTableWidgetItem(_is_unique.c_str()));

      QComboBox* _generators = new QComboBox();
      _generators->addItem("---");

      std::set<name> _genarators_name =
        primary_keys_columns_generators_factory::list(_pk_col->get_type());

      for (const name& _name : _genarators_name) {
        _generators->addItem(_name.c_str());
      }

      ui->tblPks->setCellWidget(_row, PK_GENERATOR_IDX, _generators);
      ui->tblPks->setColumnWidth(PK_GENERATOR_IDX, _generators->width());
      QObject::connect(_generators,
                       SIGNAL(currentIndexChanged(const QString&)),
                       this,
                       SLOT(on_currentPrimaryKeyIndexChanged(const QString&)));
    }
  }
}

void
TableInsertGenerator::fill_attrs_definitions()
{

  uint16_t _num_cols_attr = m_table->get_num_attrs();
  for (uint16_t _i = 0; _i < _num_cols_attr; ++_i) {

    ptr<attribute_column> _attr_col = m_table->get_attr(_i);

    name _name = _attr_col->get_name();
    std::string _type = _attr_col->get_type().to_str();
    std::string _size = std::to_string(_attr_col->get_size());
    std::string _is_unique = (_attr_col->is_unique() ? "sim" : "não");
    ui->tblAttrs->insertRow(ui->tblAttrs->rowCount());
    int _row = ui->tblAttrs->rowCount() - 1;
    ui->tblAttrs->setItem(
      _row, ATTR_NAME_IDX, new QTableWidgetItem(_name.c_str()));
    ui->tblAttrs->setItem(
      _row, ATTR_TYPE_IDX, new QTableWidgetItem(_type.c_str()));
    ui->tblAttrs->setItem(
      _row, ATTR_SIZE_IDX, new QTableWidgetItem(_size.c_str()));
    ui->tblAttrs->setItem(
      _row, ATTR_UNIQUE_IDX, new QTableWidgetItem(_is_unique.c_str()));

    QComboBox* _generators = new QComboBox();
    _generators->addItem("---");

    std::set<name> _genarators_name =
      primary_keys_columns_generators_factory::list(_attr_col->get_type());

    for (const name& _name : _genarators_name) {
      _generators->addItem(_name.c_str());
    }

    ui->tblAttrs->setCellWidget(_row, ATTR_GENERATOR_IDX, _generators);
    ui->tblAttrs->setColumnWidth(ATTR_GENERATOR_IDX, _generators->width());
    QObject::connect(_generators,
                     SIGNAL(currentIndexChanged(const QString&)),
                     this,
                     SLOT(on_currentAttributeIndexChanged(const QString&)));
  }
}

void
TableInsertGenerator::fill_fks_definitions()
{
  uint16_t _num_fks = m_table->get_num_fks();
  for (uint16_t _count_fks = 0; _count_fks < _num_fks; ++_count_fks) {
    ptr<foreign_key> _fk = m_table->get_foreign_key(_count_fks);
    name _name = _fk->get_name();
    ui->tblFks->insertRow(ui->tblFks->rowCount());
    int _row = ui->tblFks->rowCount() - 1;
    ui->tblFks->setItem(_row, FK_NAME_IDX, new QTableWidgetItem(_name.c_str()));

    QComboBox* _generators = new QComboBox();
    _generators->addItem("---");

    std::set<name> _genarators_name =
      foreign_keys_columns_generator_factory::list();

    for (const name& _name : _genarators_name) {
      _generators->addItem(_name.c_str());
    }

    ui->tblFks->setCellWidget(_row, FK_GENERATOR_IDX, _generators);
    ui->tblFks->setColumnWidth(FK_GENERATOR_IDX, _generators->width());
    QObject::connect(_generators,
                     SIGNAL(currentIndexChanged(const QString&)),
                     this,
                     SLOT(on_currentForeignKeyIndexChanged(const QString&)));
  }
}

void
TableInsertGenerator::setup_table_values()
{
  int _num_rows_values = ui->txtAmountOfLines->text().toInt();
  if (m_table_generator == nullptr) {
    m_table_generator = make_ptr<table_generator>(
      m_table, static_cast<uint16_t>(_num_rows_values));
  }
  if (ui->tblValues->rowCount() != (_num_rows_values)) {
    for (int _i = 0; _i < (_num_rows_values); ++_i) {
      ui->tblValues->insertRow(ui->tblValues->rowCount());
    }
  }
  if (ui->tblValues->columnCount() > 0) {
    ui->tblValues->setCurrentCell(0, 0);
  }
}

bool
TableInsertGenerator::already_generated(const QString& p_col_name) const
{
  int _col_number = ui->tblValues->columnCount();
  for (int _col_count = 0; _col_count < _col_number; ++_col_count) {
    if (ui->tblValues->horizontalHeaderItem(_col_count)->text() == p_col_name) {
      return true;
    }
  }
  return false;
}

void
TableInsertGenerator::adjust_grids()
{
  //  int _tbl_pks_width = ui->tblPks->width();
  //  ui->tblPks->setColumnWidth(PK_NAME_IDX, _tbl_pks_width * 0.2);
  //  ui->tblPks->setColumnWidth(PK_TYPE_IDX, _tbl_pks_width * 0.2);
  //  ui->tblPks->setColumnWidth(PK_SIZE_IDX, _tbl_pks_width * 0.2);
  //  ui->tblPks->setColumnWidth(PK_UNIQUE_IDX, _tbl_pks_width * 0.2);
  //  ui->tblPks->setColumnWidth(PK_GENERATOR_IDX, _tbl_pks_width * 0.2);
  //  ui->tblPks->setColumnWidth(PK_PARAMS_IDX, 1);

  //  int _tbl_attrs_width = ui->tblAttrs->width();
  //  ui->tblAttrs->setColumnWidth(ATTR_NAME_IDX, _tbl_attrs_width * 0.2);
  //  ui->tblAttrs->setColumnWidth(ATTR_TYPE_IDX, _tbl_attrs_width * 0.2);
  //  ui->tblAttrs->setColumnWidth(ATTR_SIZE_IDX, _tbl_attrs_width * 0.2);
  //  ui->tblAttrs->setColumnWidth(ATTR_UNIQUE_IDX, _tbl_attrs_width * 0.2);
  //  ui->tblAttrs->setColumnWidth(ATTR_GENERATOR_IDX, _tbl_attrs_width * 0.2);
  //  ui->tblAttrs->setColumnWidth(ATTR_PARAMS_IDX, 1);

  //  int _tbl_fks_width = ui->tblFks->width();
  //  ui->tblFks->setColumnWidth(FK_NAME_IDX, _tbl_fks_width * 0.3);
  //  ui->tblFks->setColumnWidth(FK_GENERATOR_IDX, _tbl_fks_width * 0.4);
}

void
TableInsertGenerator::add_to_table_values(ptr<column_values> p_column_values)
{
  if (already_generated(p_column_values->get_column()->get_name().c_str())) {
    return;
  }
  ui->tblValues->insertColumn(ui->tblValues->columnCount());
  int _col_number = ui->tblValues->columnCount() - 1;
  ui->tblValues->setHorizontalHeaderItem(
    _col_number,
    new QTableWidgetItem(p_column_values->get_column()->get_name().c_str()));

  int _num_rows_values = ui->txtAmountOfLines->text().toInt();

  for (int _row_count = 0; _row_count < _num_rows_values; ++_row_count) {
    QString _value =
      p_column_values->get_value(static_cast<uint16_t>(_row_count))
        .get_value()
        .c_str();
    ui->tblValues->setItem(
      _row_count, _col_number, new QTableWidgetItem(_value));
  }
}

void
TableInsertGenerator::on_currentPrimaryKeyIndexChanged(const QString& text)
{
  m_current_params_cell = { ui->tblPks, PK_PARAMS_IDX };
  QDialog* _generator_definition =
    m_generator_definition_window_factory.create<primary_key_column>(
      text.toStdString(), this);
  if (_generator_definition) {
    _generator_definition->show();
  }
}

void
TableInsertGenerator::on_currentForeignKeyIndexChanged(const QString& text)
{
  m_current_params_cell = { ui->tblFks, FK_PARAMS_IDX };
  QDialog* _generator_definition =
    m_generator_definition_window_factory.create<foreign_key>(
      text.toStdString(), this);
  if (_generator_definition) {
    _generator_definition->show();
  }
}

void
TableInsertGenerator::on_currentAttributeIndexChanged(const QString& text)
{
  m_current_params_cell = { ui->tblAttrs, ATTR_PARAMS_IDX };
  QDialog* _generator_definition =
    m_generator_definition_window_factory.create<attribute_column>(
      text.toStdString(), this);
  if (_generator_definition) {
    _generator_definition->show();
  }
}

void
TableInsertGenerator::on_btnGenPks_clicked()
{

  setup_table_values();
  int _num_rows_def = ui->tblPks->rowCount();
  if (_num_rows_def == 0) {
    return;
  }
  try {
    for (int _row_def = 0; _row_def < _num_rows_def; ++_row_def) {

      QString _col_name = ui->tblPks->item(_row_def, PK_NAME_IDX)->text();

      ptr<primary_key_column> _col =
        m_table->get_primary_key()->find_pk_column(_col_name.toStdString());

      QComboBox* _combo =
        (QComboBox*)(ui->tblPks->cellWidget(_row_def, PK_GENERATOR_IDX));

      QTableWidgetItem* _item = ui->tblPks->item(_row_def, PK_PARAMS_IDX);
      std::string _params;
      if (_item) {
        _params = _item->text().toStdString();
      }

      ptr<column_generator<primary_key_column>> _generator =
        primary_keys_columns_generators_factory::create(
          _col, _combo->currentText().toStdString(), _params);
      if (_generator) {
        m_table_generator->add_pk_generator(_col_name.toStdString(),
                                            _generator);
      }
    }

    m_pks_values = m_table_generator->generate_pks(m_all_pks);

    uint16_t _num_cols_generated =
      (m_pks_values ? m_pks_values->get_num_cols() : 0);
    for (uint16_t _count_cols_generated = 0;
         _count_cols_generated < _num_cols_generated;
         ++_count_cols_generated) {

      ptr<column_values> _values =
        m_pks_values->get_column_values(_count_cols_generated);
      add_to_table_values(_values);
    }

    if (m_pks_values->get_num_cols() ==
        m_table->get_primary_key()->get_num_pks_cols()) {
      emit pks_generated(m_table->get_name());
    }
  } catch (std::exception& _ex) {
    QMessageBox::critical(this, "ERRO", _ex.what());
  }
}

void
TableInsertGenerator::on_btnGenSql_clicked()
{
  sql_generator _sql_gen;
  std::string _sql = _sql_gen(m_pks_values, m_attrs_values, m_fks_values);
  if (m_show_sql == nullptr) {
    m_show_sql = new ShowSql(this);
  }

  m_show_sql->setSql(_sql);
  m_show_sql->show();
  m_show_sql->raise();
}

void
TableInsertGenerator::on_btnGenFks_clicked()
{

  setup_table_values();
  int _num_rows_def = ui->tblFks->rowCount();
  if (_num_rows_def == 0) {
    return;
  }
  try {
    for (int _row_def = 0; _row_def < _num_rows_def; ++_row_def) {

      QString _col_name = ui->tblFks->item(_row_def, FK_NAME_IDX)->text();
      ptr<foreign_key> _col = m_table->find_fk(_col_name.toStdString());

      QComboBox* _combo =
        (QComboBox*)(ui->tblFks->cellWidget(_row_def, FK_GENERATOR_IDX));

      QTableWidgetItem* _item = ui->tblFks->item(_row_def, FK_PARAMS_IDX);
      std::string _params;
      if (_item) {
        _params = _item->text().toStdString();
      }

      ptr<foreign_key_generator> _generator =
        foreign_keys_columns_generator_factory::create(
          _combo->currentText().toStdString(), _params);
      if (_generator) {
        m_table_generator->add_fk_generator(_col_name.toStdString(),
                                            _generator);
      }
    }

    std::pair<table_generator::table_values_ptr,
              table_generator::tables_names_ptr>
      _res = m_table_generator->generate_fks(m_all_pks);

    if (_res.second) {
      ui->lstMissingPks->clear();
      for (const name& _name : *_res.second) {
        ui->lstMissingPks->addItem(_name.c_str());
      }
    } else {

      m_fks_values = _res.first;
      uint16_t _num_cols = m_fks_values->get_num_cols();
      for (uint16_t _count_cols = 0; _count_cols < _num_cols; ++_count_cols) {
        ptr<column_values> _column_values =
          m_fks_values->get_column_values(_count_cols);
        add_to_table_values(_column_values);
      }
      if (m_fks_values->get_num_cols() == m_table->get_num_all_fks_cols()) {
        emit fks_generated(m_table->get_name());
      }
    }
  } catch (std::exception& _ex) {
    QMessageBox::critical(this, "ERRO", _ex.what());
  }
}

void
TableInsertGenerator::on_btnMain_clicked()
{
  parentWidget()->show();
  parentWidget()->raise();
}

void
TableInsertGenerator::on_pk_generated(std::string p_table_name)
{
  QString _table_name(p_table_name.c_str());
  for (int _count_line = 0; _count_line < ui->lstMissingPks->count();
       ++_count_line) {
    if (ui->lstMissingPks->item(_count_line)->text() == _table_name) {
      delete ui->lstMissingPks->takeItem(_count_line);
    }
  }
}

void
TableInsertGenerator::on_btnGenAttrs_clicked()
{
  setup_table_values();
  int _num_rows_def = ui->tblAttrs->rowCount();
  if (_num_rows_def == 0) {
    return;
  }

  try {
    for (int _row_def = 0; _row_def < _num_rows_def; ++_row_def) {

      QString _col_name = ui->tblAttrs->item(_row_def, ATTR_NAME_IDX)->text();
      ptr<attribute_column> _col =
        m_table->find_attribute(_col_name.toStdString());

      QComboBox* _combo =
        (QComboBox*)(ui->tblAttrs->cellWidget(_row_def, ATTR_GENERATOR_IDX));

      QTableWidgetItem* _item = ui->tblAttrs->item(_row_def, ATTR_PARAMS_IDX);
      std::string _params;
      if (_item) {
        _params = _item->text().toStdString();
      }

      ptr<column_generator<attribute_column>> _generator =
        attribute_columns_generators_factory::create(
          _col, _combo->currentText().toStdString(), _params);
      if (_generator) {
        m_table_generator->add_attr_generator(_col_name.toStdString(),
                                              _generator);
      }
    }

    m_attrs_values = m_table_generator->generate_attrs();

    uint16_t _num_cols_generated =
      (m_attrs_values ? m_attrs_values->get_num_cols() : 0);
    for (uint16_t _count_cols_generated = 0;
         _count_cols_generated < _num_cols_generated;
         ++_count_cols_generated) {

      ptr<column_values> _values =
        m_attrs_values->get_column_values(_count_cols_generated);
      add_to_table_values(_values);
    }
    if (m_attrs_values->get_num_cols() == m_table->get_num_attrs()) {
      emit attrs_generated(m_table->get_name());
    }
  } catch (std::exception& _ex) {
    QMessageBox::critical(this, "ERRO", _ex.what());
  }

  //  if (m_pks_values) {
  //    emit pks_generated(m_table->get_name());
  //  }
}

void
TableInsertGenerator::resizeEvent(QResizeEvent* event)
{
  adjust_grids();
  QMainWindow::resizeEvent(event);
}

void
TableInsertGenerator::showEvent(QShowEvent* event)
{
  adjust_grids();
  QMainWindow::showEvent(event);
}
