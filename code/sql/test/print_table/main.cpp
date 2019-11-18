#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

#include <sql/entities/column.h>
#include <sql/entities/database.h>
#include <sql/entities/host.h>
#include <sql/entities/server.h>
#include <sql/entities/table.h>

using namespace capemisa::sql::entities;

int
main()
{
  host _host("host000", "172.310.67.012");
  server _server(&_host, "srv000");

  database _db000(&_server, "db000");
  table _table0(&_db000, "tb000");
  {
    ptr<column> _c007 =
      _table0.add_column("col007", column::type::VAR_SIZE_CHAR, 20);
    _c007->set_var_size_string("string 006");

    _table0.add_column("col002", column::type::INT_4)
      ->set_int(static_cast<int32_t>(46));

    _table0.add_to_primary_key(_c007);
  }

  table _table1(&_db000, "tb001");
  {
    ptr<column> _c009 = _table1.add_column("col009", column::type::DATE);
    _c009->set_date(std::time(nullptr));

    ptr<column> _c005 = _table1.add_column("col005", column::type::LONG_REAL);
    _c005->set_real(static_cast<double>(46));

    _table1.add_foreign_key("fk001", _table0.get_primary_key());
  }

  std::cout << _table0 << std::endl;
  std::cout << _table1 << std::endl;
}
