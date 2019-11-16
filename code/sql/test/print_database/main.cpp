#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

#include <sql/entities/column.h>
#include <sql/entities/database.h>
#include <sql/entities/table.h>

using namespace capemisa::sql::entities;

int
main()
{
  database _database("db001");

  table _table0("tb000");
  {
    columns::iterator _c007 = _table0.add_column(column("col007"));
    _c007->set_var_size_string("string 006");

    _table0.add_to_primary_key("col007");
  }

  table _table1("tb001");
  {
    column _c009("col009");
    _c009.set_date(std::time(nullptr));
    _table1.add_column(std::move(_c009));

    column _c005("col005");
    _c005.set_real(static_cast<double>(46));
    _table1.add_column(std::move(_c005));

    foreign_key _fk("fk001", &_table0.get_primary_key());
    _table1.add_foreign_key(std::move(_fk));
  }

  std::cout << _table0 << std::endl;
  std::cout << _table1 << std::endl;

  _database.add(std::move(_table0));
  _database.add(std::move(_table1));

  std::cout << _database << std::endl;
}
