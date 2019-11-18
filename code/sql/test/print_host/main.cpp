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
    {
      ptr<server> _server = _host.add_server("srv000");
      {
        ptr<database> _db0 = _server->add_database("db000");

        ptr<table> _table0 = _db0->add_table("tb000");
        ptr<column> _pk =
          _table0->add_column("col007", column::type::VAR_SIZE_CHAR, 20);
        _pk->set_var_size_string("string 006");
        _table0->add_to_primary_key(_pk);

        ptr<table> _table1 = _db0->add_table("tb001");
        _table1->add_column("col009", column::type::DATE)
          ->set_date(std::time(nullptr));
        _table1->add_column("col005", column::type::LONG_REAL)
          ->set_real(static_cast<double>(46));
        _table1->add_foreign_key("fk001", _table0->get_primary_key());
      }
      {
        ptr<database> _db1 = _server->add_database("db001");

        ptr<table> _table2 = _db1->add_table("tbXPTO");
        ptr<column> _pk =
          _table2->add_column("colABC", column::type::VAR_SIZE_CHAR, 30);
        _pk->set_var_size_string("hello world");
        _table2->add_to_primary_key(_pk);

        ptr<table> _table3 = _db1->add_table("tbQWER");
        _table3->add_column("colDtNasc", column::type::DATE)
          ->set_date(std::time(nullptr));
        _table3->add_column("colSalary", column::type::LONG_REAL)
          ->set_real(static_cast<double>(4600));
        _table3->add_foreign_key("fk001", _table2->get_primary_key());
      }
    }
    {
      ptr<server> _server = _host.add_server("srv001");
      {
        ptr<database> _db0 = _server->add_database("db001");

        ptr<table> _table0 = _db0->add_table("tb021");
        ptr<column> _pk =
          _table0->add_column("col77", column::type::VAR_SIZE_CHAR, 25);
        _pk->set_var_size_string("string UUUU");
        _table0->add_to_primary_key(_pk);

        ptr<table> _table1 = _db0->add_table("tb44");
        _table1->add_column("col109", column::type::DATE)
          ->set_date(std::time(nullptr));
        _table1->add_column("col105", column::type::LONG_REAL)
          ->set_real(static_cast<double>(46));
        _table1->add_foreign_key("fk11", _table0->get_primary_key());
      }
      {
        ptr<database> _db1 = _server->add_database("db901");

        ptr<table> _table2 = _db1->add_table("tabelaZIP");
        ptr<column> _pk =
          _table2->add_column("colZIP", column::type::FIXED_SIZE_CHAR, 8);
        _pk->set_fixed_size_string("222221000");
        _table2->add_to_primary_key(_pk);

        ptr<table> _table3 = _db1->add_table("tbBakers");
        _table3->add_column("colDtNasc", column::type::DATE)
          ->set_date(std::time(nullptr));
        _table3->add_column("col", column::type::LONG_REAL)
          ->set_real(static_cast<double>(4600));
        _table3->add_foreign_key("fk001", _table2->get_primary_key());
      }
    }

    std::cout << _host << std::endl;
  } catch (std::exception& _ex) {
    std::cerr << "ERROR: " << _ex.what() << std::endl;
  }
}
