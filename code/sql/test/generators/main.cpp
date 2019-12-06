#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <utility>

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

  number_value_generator<time_t> _asvg(
    _now, _now + 30 * day_in_secs, 2 * day_in_secs);

  ptr<column_values> _values = _asvg(_birthday, 8);

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

  number_value_generator<int32_t> _pkcsvg(100, 200, 5);

  ptr<column_values> _values = _pkcsvg(_id, 8);

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

  {
    table_generator _employee_gen(_db->find("employee"), 8);
    _employee_gen.add_pk_generator(
      "id", number_value_generator<uint32_t>(100, -20, -5));

    _employee_gen.add_attr_generator("name",
                                     text_value_generator("employee", 30, 90));

    _employee_gen.add_attr_generator(
      "birthday",
      number_value_generator<uint32_t>(time(nullptr), 2 * day_in_secs));

    ptr<const table_values> _employee_pk_values =
      _employee_gen.generate_pks(_all_pks);

    ptr<const table_values> _employee_attrs_values =
      _employee_gen.generate_attrs();

    std::cout << _sql_gen(
      _employee_pk_values, _employee_attrs_values, ptr<table_values>());
  }
  {
    table_generator _dependent_gen(_db->find("dependent"), 5);
    _dependent_gen.add_pk_generator("id",
                                    number_value_generator<uint16_t>(4, 50, 2));
    _dependent_gen.add_attr_generator("name",
                                      text_value_generator("dep", 15, 14));

    time_t _now = time(nullptr);
    _dependent_gen.add_attr_generator(
      "birthday",
      number_value_generator<uint32_t>(
        _now, _now + 25 * day_in_secs, 5 * day_in_secs));

    _dependent_gen.add_fk_generator("employee_fk", one_pk_all_fks());

    ptr<const table_values> _dependent_pk_values =
      _dependent_gen.generate_pks(_all_pks);
    ptr<const table_values> _dependent_attrs_values =
      _dependent_gen.generate_attrs();

    std::pair<ptr<table_values>, table_generator::tables_names_ptr> _result =
      _dependent_gen.generate_fks(_all_pks);

    if (_result.first) {
      std::cout << _sql_gen(
                     _dependent_pk_values, _dependent_attrs_values, _result.first);
    }
    else {
      std::cout << "Dados para seguintes tabelas devem ser geradas primeiro: ";
      for (const name & _name: *_result.second) {
        std::cout << _name << std::endl;
      }

    }
  }
}

void not_all_tables_generated_1() {
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

  table_generator _dependent_gen(_db->find("dependent"), 5);
  _dependent_gen.add_pk_generator("id",
                                  number_value_generator<uint16_t>(4, 50, 2));
  _dependent_gen.add_attr_generator("name",
                                    text_value_generator("dep", 15, 14));

  time_t _now = time(nullptr);
  _dependent_gen.add_attr_generator(
    "birthday",
    number_value_generator<time_t>(
      _now, _now + 25 * day_in_secs, 5 * day_in_secs));

  _dependent_gen.add_fk_generator("employee_fk", one_pk_all_fks());

  ptr<const table_values> _dependent_pk_values =
    _dependent_gen.generate_pks(_all_pks);
  ptr<const table_values> _dependent_attrs_values =
    _dependent_gen.generate_attrs();

  std::pair<ptr<table_values>, table_generator::tables_names_ptr> _result =
    _dependent_gen.generate_fks(_all_pks);

  if (_result.first) {
    std::cout << _sql_gen(
                   _dependent_pk_values, _dependent_attrs_values, _result.first);
  }
  else {
    std::cout << "Dados para seguintes tabelas devem ser gerados primeiro: \n";
    for (const name & _name: *_result.second) {
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

  text_value_generator _tvg("dep", 15, 14);
  ptr<column_values> _values = _tvg(_name, 6);
  uint16_t _counter = 0;
  for (const value& _value : *_values) {
    std::cout << "value #" << ++_counter << " = " << _value.get_value()
              << std::endl;
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
}
