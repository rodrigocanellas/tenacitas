#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>

#include <sql/entities/column.h>
#include <sql/entities/traits.h>

using namespace capemisa;

int main() {
  typedef sql::entities::column_t<sql::entities::traits> column;

  column _c000("col000");
  _c000.set_int(static_cast<int8_t>(46));
  std::cout << _c000 << std::endl;

  column _c001("col001");
  _c001.set_int(static_cast<int16_t>(46));
  std::cout << _c001 << std::endl;

  column _c002("col002");
  _c002.set_int(static_cast<int32_t>(46));
  std::cout << _c002 << std::endl;

  column _c003("col003");
  _c003.set_int(static_cast<int64_t>(46));
  std::cout << _c003 << std::endl;

  column _c004("col004");
  _c004.set_real(static_cast<float>(46));
  std::cout << _c004 << std::endl;

  column _c005("col005");
  _c005.set_real(static_cast<double>(46));
  std::cout << _c005 << std::endl;

  column _c006("col006");
  _c006.set_fixed_size_string("string 005");
  std::cout << _c006 << std::endl;

  column _c007("col007");
  _c007.set_var_size_string("string 006");
  std::cout << _c007 << std::endl;

  column _c008("col008");
  _c008.set_blob("00213\\p");
  std::cout << _c008 << std::endl;

  column _c009("col009");
  _c009.set_date(std::time(nullptr));
  std::cout << _c009 << std::endl;

  column _c010("col010");
  _c010.set_char('M');
  std::cout << _c010 << std::endl;

  std::this_thread::sleep_for(std::chrono::seconds(1));

  column _c011("col011");
  _c011.set_datetime(std::time(nullptr));
  std::cout << _c011 << std::endl;

}
