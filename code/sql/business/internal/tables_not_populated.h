#ifndef CAPEMISA_SQL_BUSINESS_TABLES_NOT_POPULATED_H
#define CAPEMISA_SQL_BUSINESS_TABLES_NOT_POPULATED_H

#include <algorithm>
#include <list>
#include <queue>

#include <sql/entities/foreign_key.h>
#include <sql/entities/tables_values.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

struct tables_not_populated
{

  typedef std::list<generic::name> tables_names;
  typedef generic::ptr<std::list<generic::name>> tables_names_ptr;

  tables_names_ptr operator()(
    generic::ptr<const entities::table> p_table,
    generic::ptr<const entities::tables_values> p_pks);

private:
  bool find(const tables_names& p_tables_names,
            const generic::name& p_table_name) const
  {
    return !(find_if(p_tables_names.begin(),
                     p_tables_names.end(),
                     [&p_table_name](const generic::name& p_name) -> bool {
                       return p_table_name == p_name;
                     }) == p_tables_names.end());
  }

private:
  std::queue<const entities::table*> m_queue;
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // TABLES_NOT_POPULATED_H
