#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

#include <sql/entities/column.h>
#include <sql/entities/database.h>
#include <sql/entities/host.h>
#include <sql/entities/server.h>
#include <sql/entities/table.h>
#include <sql/entities/types.h>

using namespace capemisa::sql::entities;

int
main()
{
  try {
    host _host("host000", "172.310.67.012");
    server _server(&_host, "srv000");
    {
      ptr<database> _db = _server.add_database("db000");

      ptr<table> _table0 = _db->add_table("tb000");
      ptr<column> _pk =
        _table0->add_column("col007", column::type::FIXED_SIZE_CHAR, 20);
      _pk->set_fixed_size_string("string 006");
      _table0->add_to_primary_key(_pk);

      ptr<table> _table1 = _db->add_table("tb001");
      _table1->add_column("col009", column::type::DATE)
        ->set_date(std::time(nullptr));

      _table1->add_column("col005", column::type::LONG_REAL)
        ->set_real(static_cast<double>(46));

      _table1->add_foreign_key("fk001", _table0->get_primary_key());
    }
    {
      ptr<database> _db = _server.add_database("db001");

      ptr<table> _table0 = _db->add_table("tbXPTO");
      ptr<column> _pk =
        _table0->add_column("colABC", column::type::VAR_SIZE_CHAR, 30);
      _pk->set_var_size_string("hello world");
      _table0->add_to_primary_key(_pk);

      ptr<table> _table1 = _db->add_table("tbQWER");
      _table1->add_column("colDtNasc", column::type::DATE)
        ->set_date(std::time(nullptr));
      _table1->add_column("colSalary", column::type::LONG_REAL)
        ->set_real(static_cast<double>(4600));
      _table1->add_foreign_key("fk001", _table0->get_primary_key());
    }

    std::cout << _server << std::endl;
  } catch (std::exception& _ex) {
    std::cerr << "ERROR: " << _ex.what() << std::endl;
  }
}
