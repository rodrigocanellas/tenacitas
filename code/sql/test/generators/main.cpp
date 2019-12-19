#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <utility>

#include <sql/business/traverse_tables.h>
#include <sql/business/number_value_generator.h>
#include <sql/business/one_pk_all_fks.h>
#include <sql/business/sql_generator.h>
#include <sql/business/table_generator.h>
#include <sql/business/text_value_generator.h>
#include <sql/entities/attribute_column.h>
#include <sql/entities/column_values.h>
#include <sql/entities/foreign_key.h>
#include <sql/entities/hosts.h>
#include <sql/entities/primary_key_column.h>
#include <sql/entities/table.h>
#include <sql/entities/value.h>
#include <sql/generic/ptr.h>
#include <sql/test/create_abstract_model.h>
#include <sql/test/create_hosts.h>

#define day_in_secs 24 * 60 * 60

using namespace capemisa::sql;

void
attribute_sequential()
{
  using namespace capemisa::sql::entities;
  using namespace capemisa::sql::generic;
  using namespace capemisa::sql::business;
  using namespace capemisa::sql::test;

  create_hosts _create_hosts;
  hosts _hosts(_create_hosts());
  ptr<host> _host = _hosts[0];
  ptr<server> _server = _host->get_server(0);
  ptr<database> _db = _server->get_database(0);
  ptr<table> _employee = _db->find("employee");
  ptr<attribute_column> _birthday = _employee->find_attribute("birthday");

  time_t _now = time(nullptr);

  typedef number_value_generator<time_t, attribute_column> birthday_generator;

  ptr<birthday_generator> _asvg(make_ptr<birthday_generator>(
                                  _now, _now + 30 * day_in_secs, 2 * day_in_secs));

  ptr<column_values> _values = (*_asvg)(_birthday, 8);

  std::cout << "ATTRIBUTE SEQUENTIAL for " << _values->get_column()->get_name()
            << std::endl;
  uint16_t _counter = 0;
  for (const value& _value : *_values) {
    time_t _time = atol(_value.get_value().c_str());
    std::cout << "value #" << ++_counter << " = " << _value.get_value() << ", "
              << ctime(&_time);
  }
}

void
pk_sequential()
{
  using namespace capemisa::sql::entities;
  using namespace capemisa::sql::generic;
  using namespace capemisa::sql::business;
  using namespace capemisa::sql::test;

  create_hosts _create_hosts;
  hosts _hosts(_create_hosts());
  ptr<host> _host = _hosts[0];
  ptr<server> _server = _host->get_server(0);
  ptr<database> _db = _server->get_database(0);
  ptr<table> _employee = _db->find("employee");
  ptr<primary_key_column> _id =
      _employee->get_primary_key()->find_pk_column("id");

  typedef number_value_generator<int32_t, primary_key_column> id_generator;

  ptr<id_generator> _pkcsvg(make_ptr<id_generator>(100, 200, 5));

  ptr<column_values> _values = (*_pkcsvg)(_id, 8);

  std::cout << "PK SEQUENTIAL for " << _values->get_column()->get_name()
            << std::endl;
  uint16_t _counter = 0;
  for (const value& _value : *_values) {
    std::cout << "value #" << ++_counter << " = " << _value.get_value()
              << std::endl;
  }
}

void
fk_one_pk()
{
  using namespace capemisa::sql::entities;
  using namespace capemisa::sql::generic;
  using namespace capemisa::sql::business;
  using namespace capemisa::sql::test;

  sql_generator _sql_gen;

  std::cout << std::endl;

  create_hosts _create_hosts;
  hosts _hosts(_create_hosts());
  ptr<host> _host = _hosts[0];
  ptr<server> _server = _host->get_server(0);
  ptr<database> _db = _server->get_database(0);

  ptr<tables_values> _all_pks(make_ptr<tables_values>());

  typedef number_value_generator<int32_t, primary_key_column> id_generator;
  typedef text_value_generator<attribute_column> name_generator;
  typedef number_value_generator<time_t, attribute_column> birthday_generator;

  {

    table_generator _employee_gen(_db->find("employee"), 8);
    _employee_gen.add_pk_generator("id", make_ptr<id_generator>(100, -20, -5));

    _employee_gen.add_attr_generator(
          "name", make_ptr<name_generator>("employee", 30, 90));

    _employee_gen.add_attr_generator(
          "birthday", make_ptr<birthday_generator>(time(nullptr), 2 * day_in_secs));

    ptr<const table_values> _employee_pk_values =
        _employee_gen.generate_pks(_all_pks);

    ptr<const table_values> _employee_attrs_values =
        _employee_gen.generate_attrs();

    std::cout << _sql_gen(_employee_pk_values,
                          _employee_attrs_values,
                          ptr<table_values>())
              << std::endl;
  }
  {
    table_generator _dependent_gen(_db->find("dependent"), 5);
    _dependent_gen.add_pk_generator("id", make_ptr<id_generator>(4, 50, 2));
    _dependent_gen.add_attr_generator("name",
                                      make_ptr<name_generator>("dep", 15, 14));

    time_t _now = time(nullptr);
    _dependent_gen.add_attr_generator(
          "birthday",
          make_ptr<birthday_generator>(
            _now, _now + 25 * day_in_secs, 5 * day_in_secs));

    _dependent_gen.add_fk_generator("employee_fk", make_ptr<one_pk_all_fks>());

    ptr<const table_values> _dependent_pk_values =
        _dependent_gen.generate_pks(_all_pks);
    ptr<const table_values> _dependent_attrs_values =
        _dependent_gen.generate_attrs();

    std::pair<ptr<table_values>, table_generator::tables_names_ptr> _result =
        _dependent_gen.generate_fks(_all_pks);

    if (_result.first) {
      std::cout << _sql_gen(
                     _dependent_pk_values, _dependent_attrs_values, _result.first);
    } else {
      std::cout << "Dados para seguintes tabelas devem ser geradas primeiro: ";
      for (const name& _name : *_result.second) {
        std::cout << _name << std::endl;
      }
    }
  }
}

void
not_all_tables_generated_1()
{
  using namespace capemisa::sql::entities;
  using namespace capemisa::sql::generic;
  using namespace capemisa::sql::business;
  using namespace capemisa::sql::test;

  sql_generator _sql_gen;

  std::cout << std::endl;

  create_hosts _create_hosts;
  hosts _hosts(_create_hosts());
  ptr<host> _host = _hosts[0];
  ptr<server> _server = _host->get_server(0);
  ptr<database> _db = _server->get_database(0);

  ptr<tables_values> _all_pks(make_ptr<tables_values>());

  typedef number_value_generator<int32_t, primary_key_column> id_generator;
  typedef text_value_generator<attribute_column> name_generator;
  typedef number_value_generator<time_t, attribute_column> birthday_generator;

  table_generator _dependent_gen(_db->find("dependent"), 5);
  _dependent_gen.add_pk_generator("id", make_ptr<id_generator>(4, 50, 2));
  _dependent_gen.add_attr_generator("name",
                                    make_ptr<name_generator>("dep", 15, 14));

  time_t _now = time(nullptr);
  _dependent_gen.add_attr_generator(
        "birthday",
        make_ptr<birthday_generator>(
          _now, _now + 25 * day_in_secs, 5 * day_in_secs));

  _dependent_gen.add_fk_generator("employee_fk", make_ptr<one_pk_all_fks>());

  ptr<const table_values> _dependent_pk_values =
      _dependent_gen.generate_pks(_all_pks);
  ptr<const table_values> _dependent_attrs_values =
      _dependent_gen.generate_attrs();

  std::pair<ptr<table_values>, table_generator::tables_names_ptr> _result =
      _dependent_gen.generate_fks(_all_pks);

  if (_result.first) {
    std::cout << _sql_gen(
                   _dependent_pk_values, _dependent_attrs_values, _result.first);
  } else {
    std::cout << "Dados para seguintes tabelas devem ser gerados primeiro:\n ";
    for (const name& _name : *_result.second) {
      std::cout << _name << std::endl;
    }
  }
}

void
text_generator()
{
  using namespace capemisa::sql::entities;
  using namespace capemisa::sql::generic;
  using namespace capemisa::sql::business;
  using namespace capemisa::sql::test;

  create_hosts _create_hosts;
  hosts _hosts(_create_hosts());
  ptr<host> _host = _hosts[0];
  ptr<server> _server = _host->get_server(0);
  ptr<database> _db = _server->get_database(0);
  ptr<table> _employee = _db->find("employee");
  ptr<attribute_column> _name = _employee->find_attribute("name");

  std::cout << std::endl << "TEXT VALUE GENERATOR" << std::endl;

  text_value_generator<attribute_column> _tvg("dep", 15, 14);
  ptr<column_values> _values = _tvg(_name, 6);
  uint16_t _counter = 0;
  for (const value& _value : *_values) {
    std::cout << "value #" << ++_counter << " = " << _value.get_value()
              << std::endl;
  }
}

void
abstract_1()
{
  using namespace capemisa::sql::entities;
  using namespace capemisa::sql::generic;
  using namespace capemisa::sql::business;
  using namespace capemisa::sql::test;
  using namespace std;

  cout << "\nABSTRACT 1" << endl;

  create_abstract_model _cam;

  ptr<hosts> _hosts = _cam();
  ptr<host> _host = (*_hosts)[0];
  ptr<server> _server = _host->get_server(0);
  ptr<database> _db = _server->get_database(0);

  ptr<table> _a = _db->find("A");

  if (_a == nullptr) {
    throw std::runtime_error("não foi possível achar a tabela 'A'");
  }

  table_generator _tb(_a, 5);

  _tb.add_pk_generator(
        "id",
        make_ptr<number_value_generator<uint16_t, primary_key_column>>(4, 50, 2));

  _tb.add_attr_generator(
        "a1",
        make_ptr<number_value_generator<uint16_t, attribute_column>>(
          1000.00, 1600.00, 250.00));

  generic::ptr<entities::tables_values> _all_pks(
        make_ptr<entities::tables_values>());

  _tb.generate_pks(_all_pks);
  _tb.generate_attrs();

  std::pair<ptr<table_values>, table_generator::tables_names_ptr> _result =
      _tb.generate_fks(_all_pks);

  if (_result.first != nullptr) {
    throw std::runtime_error(
          "FKs para 'A' geradas, mas 'D' e 'B' não foram ṕopuladas");
  }

  cout << "As tabelas abaixo devem ser povoadas: " << endl;
  for (const name& _name : *_result.second) {
    cout << _name << endl;
  }
}

void
abstract_2()
{
  using namespace capemisa::sql::entities;
  using namespace capemisa::sql::generic;
  using namespace capemisa::sql::business;
  using namespace capemisa::sql::test;
  using namespace std;

  cout << "\nABSTRACT 2" << endl;

  create_abstract_model _cam;

  ptr<hosts> _hosts = _cam();
  ptr<host> _host = (*_hosts)[0];
  ptr<server> _server = _host->get_server(0);
  ptr<database> _db = _server->get_database(0);

  ptr<table> _table = _db->find("E");

  if (_table == nullptr) {
    throw std::runtime_error("não foi possível achar a tabela 'E'");
  }

  std::list<name> _names;
  traverse_tables _tt;
  _tt(_table.get(), [&_names](const table * const p_table) -> void {
    _names.push_front(p_table->get_name());
  });

  for (const name & _name : _names) {
    std::cout << _name << std::endl;
  }
}

int
main()
{
  attribute_sequential();
  pk_sequential();
  fk_one_pk();
  text_generator();
  not_all_tables_generated_1();
  abstract_1();
  abstract_2();
}
