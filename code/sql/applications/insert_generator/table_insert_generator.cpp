#include "table_insert_generator.h"
#include "ui_table_insert_generator.h"

#include <QComboBox>
#include <QDebug>
#include <QListWidget>
#include <QStringList>

#include <sql/business/number_value_generator.h>

#define GERADOR_NUMEROS "gerador de números"

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
TableInsertGenerator::set_number_value_generator_params(
  const QString& p_base,
  const QString& p_limit,
  const QString& p_increment)
{
  ui->tblPks->setItem(
    ui->tblPks->currentRow(),
    5,
    new QTableWidgetItem(p_base + ";" + p_limit + ";" + p_increment));
}

void
TableInsertGenerator::header_pks_definitions()
{
  ui->tblPks->setHorizontalHeaderLabels(
    { "nome", "tipo", "tamanho", "única?", "gerador", "params" });
  ui->tblPks->setColumnHidden(5, true);
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
      _generators->addItem("---");
      switch (_pk_col->get_type()) {
        case column::type::int_1:
        case column::type::int_2:
        case column::type::int_4:
        case column::type::int_8:
        case column::type::date:
        case column::type::date_time:
        case column::type::small_real:
        case column::type::long_real:
          _generators->addItem(GERADOR_NUMEROS);
          break;
        case column::type::var_size_text:
        case column::type::fixed_size_text:
          _generators->addItem("gerador de texto");
          break;
        default:
          _generators->addItem("no generator for this type yet");
      }

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
  if (text == GERADOR_NUMEROS) {
    if (m_number_value_generator_definition == nullptr) {
      m_number_value_generator_definition =
        new NumberValueGeneratorDefinition(this, this);
    }
    m_number_value_generator_definition->show();
    m_number_value_generator_definition->raise();
    // number_value_generator
  }
}

void
TableInsertGenerator::on_btnGenPks_clicked()
{

  int _num_rows_values = ui->txtAmountOfLines->text().toInt();
  if (m_table_generator == nullptr) {
    m_table_generator = make_ptr<table_generator>(
      m_table, static_cast<uint16_t>(_num_rows_values));
  }
  if (m_all_pks == nullptr) {
    m_all_pks = make_ptr<entities::tables_values>();
  }

  if (ui->tblValues->rowCount() != _num_rows_values + 1) {
    for (int _i = 0; _i < (_num_rows_values + 1); ++_i) {
      ui->tblValues->insertRow(_i);
      qDebug() << "i == " << _i
               << ", num row values = " << ui->tblValues->rowCount();
    }
  }
  if (ui->tblValues->columnCount() > 0) {
    ui->tblValues->setCurrentCell(0, 0);
  }

  int _num_rows_def = ui->tblPks->rowCount();
  for (int _row_def = 0; _row_def < (_num_rows_def - 1); ++_row_def) {
    QComboBox* _combo = (QComboBox*)(ui->tblPks->cellWidget(_row_def, 4));
    if (_combo->currentText() == GERADOR_NUMEROS) {

      QStringList _params = ui->tblPks->item(_row_def, 5)->text().split(";");
      int _base = _params[0].toInt();
      int _limit = _params[1].toInt();
      int _increment = _params[2].toInt();
      number_value_generator<int> _nvg(_base, _limit, _increment);

      QString _col_name = ui->tblPks->item(0, 0)->text();
      ptr<primary_key_column> _col =
        m_table->get_primary_key()->find_pk_column(_col_name.toStdString());

      m_table_generator->add_pk_generator(_col_name.toStdString(),
                                          std::move(_nvg));
    }
  }

  m_pks_values = m_table_generator->generate_pks(m_all_pks);

  uint16_t _num_cols_generated = m_pks_values->get_num_cols();
  for (uint16_t _count_cols_generated = 0;
       _count_cols_generated < _num_cols_generated;
       ++_count_cols_generated) {

    ptr<column_values> _values =
      m_pks_values->get_column_values(_count_cols_generated);

    ui->tblValues->insertColumn(ui->tblValues->columnCount());
    int _col_value = ui->tblValues->columnCount() - 1;
    ui->tblValues->setHorizontalHeaderItem(
      _col_value,
      new QTableWidgetItem(_values->get_column()->get_name().c_str()));

    for (int _row_value = 0; _row_value < _num_rows_values; ++_row_value) {
      QString _value = _values->get_value(_row_value).get_value().c_str();
      ui->tblValues->setItem(
        _row_value, _col_value, new QTableWidgetItem(_value));
    }
  }
  //    number_value_generator _nbg
}

void
TableInsertGenerator::on_btnGenSql_clicked()
{
  sql_generator _sql_gen;
  std::string _sql = _sql_gen(
    m_pks_values, ptr<const table_values>(), ptr<const table_values>());
  if (m_show_sql == nullptr) {
    m_show_sql = new ShowSql(this);
  }

  m_show_sql->setSql(_sql);
  m_show_sql->show();
  m_show_sql->raise();
}
