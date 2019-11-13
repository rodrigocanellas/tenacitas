#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMNS_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMNS_H

#include <algorithm>
#include <list>

#include <sql/entities/column.h>
#include <sql/entities/name.h>

namespace capemisa {
namespace sql {
namespace entities {

typedef std::list<column> columns;

inline columns::const_iterator find_column(const columns & p_columns,
                                           const name & p_name) {
  return std::find_if(p_columns.begin(),
                      p_columns.end(),
                      [&p_name](const column & p_column)->bool{
    return p_column.get_name() == p_name;
  });

}


} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // COLUMNS_H
