#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>

#include <sql/entities/column.h>
#include <sql/entities/table.h>
#include <sql/entities/traits.h>

  using namespace capemisa;



int main() {
  typedef sql::entities::traits::table table;
  typedef sql::entities::traits::column column;

  table _table_0("table_000");

  {
    column _c000("col000");
    _c000.set_int(static_cast<int8_t>(46));
    _table_0.add_column(std::move(_c000));
  }

  {
    column _c001("col001");
    _c001.set_int(static_cast<int16_t>(46));
    _table_0.add_column(std::move(_c001));
  }

  {
    column _c002("col002");
    _c002.set_int(static_cast<int32_t>(46));
    _table_0.add_column(std::move(_c002));
  }

  {
    column _c003("col003");
    _c003.set_int(static_cast<int64_t>(46));
    _table_0.add_column(std::move(_c003));
  }

  {
    column _c004("col004");
    _c004.set_real(static_cast<float>(46));
    _table_0.add_column(std::move(_c004));
  }

  {
    column _c005("col005");
    _c005.set_real(static_cast<double>(46));
    _table_0.add_column(std::move(_c005));
  }

  {
    column _c006("col006");
    _c006.set_fixed_size_string("string 005");
    _table_0.add_column(std::move(_c006));
  }

  {
    column _c007("col007");
    _c007.set_var_size_string("string 006");
    _table_0.add_column(std::move(_c007));
  }

  {
    column _c008("col008");
    _c008.set_blob("00213p");
    _table_0.add_column(std::move(_c008));
  }

  {
    column _c009("col009");
    _c009.set_date(std::time(nullptr));
    _table_0.add_column(std::move(_c009));
  }


  {
    column _c010("col010");
    _c010.set_char('M');
    _table_0.add_column(std::move(_c010));
  }


  std::this_thread::sleep_for(std::chrono::seconds(1));

  {
    column _c011("col011");
    _c011.set_datetime(std::time(nullptr));
    _table_0.add_column(std::move(_c011));
  }

  _table_0.add_to_primary_key("col004");
  _table_0.add_to_primary_key("col008");

//  sql::entities::table _table_1("table_000");

//  {
//    column _c100("col100");
//    _c100.set_datetime(std::time(nullptr));
//    _table_1.add_column(std::move(_c100));
//  }

//  _table_1.add_to_primary_key("col100");

//  sql::entities::table::foreign_key_ptr _foreign_key =
//      std::make_shared<sql::entities::table::foreign_key>
//      ("FL_001", _table_1.get_primary_key());

//  _table_0.add_foreign_key(_foreign_key);


  std::cout << _table_0 << std::endl;

}

