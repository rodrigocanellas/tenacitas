
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>

#include <sql/business/sequential_value_generator.h>
#include <sql/entities/attribute_column.h>
#include <sql/entities/column_values.h>
#include <sql/entities/foreign_key.h>
#include <sql/entities/hosts.h>
#include <sql/entities/primary_key_column.h>
//#include <sql/entities/primary_key_column_sequential_value_generator.h>
#include <sql/entities/table.h>
#include <sql/entities/value.h>
#include <sql/generic/ptr.h>
#include <sql/test/create_hosts.h>

#define day_in_secs 24 * 60 * 60

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

  sequential_value_generator<uint32_t> _asvg(time(nullptr), 2 * day_in_secs);

  ptr<column_values> _values = _asvg(_birthday, 8);

  std::cout << "ATTRIBUTE SEQUENTIAL for " << _values->get_column()->get_name()
            << std::endl;
  uint16_t _counter = 0;
  for (const value& _value : *_values) {
    long _time = atol(_value.get_value().c_str());
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

  sequential_value_generator<uint32_t> _pkcsvg(100, -5);

  ptr<column_values> _values = _pkcsvg(_id, 8);

  std::cout << "PK SEQUENTIAL for " << _values->get_column()->get_name()
            << std::endl;
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
}
