#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_TRAITS_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_TRAITS_H



#include <sql/entities/table.h>
#include <sql/entities/tables.h>
#include <sql/entities/column.h>
#include <sql/entities/columns.h>
#include <sql/entities/primary_key.h>

namespace capemisa {
namespace sql {
namespace entities {

struct traits {

  typedef column_t<traits> column;
  typedef columns_t<traits> columns;
  typedef typename columns::column_ptr column_ptr;
  typedef typename columns::column_const_ptr column_const_ptr;
  typedef table_t<traits> table;
  typedef tables_t<traits> tables;
  typedef table * table_ptr;
  typedef const table * const table_const_ptr;
  typedef primary_key_t<traits> primary_key;
  typedef const primary_key * const primary_key_const_ptr;


};

} // namespace entities
} // namespace sql
} // namespace capemisa


#endif // TRAITS_H
