#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

#include <sql/entities/column.h>
#include <sql/entities/columns.h>
#include <sql/entities/database.h>
#include <sql/entities/host.h>
#include <sql/entities/server.h>
#include <sql/entities/table.h>
#include <sql/entities/types.h>

using namespace capemisa::sql::entities;

int
main()
{

  host _host("host000", "172.310.67.012");

  server _server(&_host, "srv000");

  database _db000(&_server, "db000");
  table _tab000(&_db000, "tab000");

  column _c000(&_tab000, "col000");
  _c000.set_int(static_cast<int8_t>(46));
  std::cout << _c000 << std::endl;

  column _c001(&_tab000, "col001");
  _c001.set_int(static_cast<int16_t>(46));
  std::cout << _c001 << std::endl;

  column _c002(&_tab000, "col002");
  _c002.set_int(static_cast<int32_t>(46));
  std::cout << _c002 << std::endl;

  column _c003(&_tab000, "col003");
  _c003.set_int(static_cast<int64_t>(46));
  std::cout << _c003 << std::endl;

  column _c004(&_tab000, "col004");
  _c004.set_real(static_cast<float>(46));
  std::cout << _c004 << std::endl;

  column _c005(&_tab000, "col005");
  _c005.set_real(static_cast<double>(46));
  std::cout << _c005 << std::endl;

  column _c006(&_tab000, "col006");
  _c006.set_fixed_size_string("string 005");
  std::cout << _c006 << std::endl;

  column _c007(&_tab000, "col007");
  _c007.set_var_size_string("string 006");
  std::cout << _c007 << std::endl;

  column _c008(&_tab000, "col008");
  _c008.set_blob("00213\\p");
  std::cout << _c008 << std::endl;

  column _c009(&_tab000, "col009");
  _c009.set_date(std::time(nullptr));
  std::cout << _c009 << std::endl;

  column _c010(&_tab000, "col010");
  _c010.set_char('M');
  std::cout << _c010 << std::endl;

  std::this_thread::sleep_for(std::chrono::seconds(1));

  column _c011(&_tab000, "col011");
  _c011.set_datetime(std::time(nullptr));
  std::cout << _c011 << std::endl;

  columns _cols;
  _cols.add(make_ptr<column>(_c000));
  _cols.add(make_ptr<column>(_c001));
  std::cout << _cols << std::endl;
}
