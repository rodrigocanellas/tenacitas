#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_DATABASE_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_DATABASE_H

#include <iostream>

#include <sql/entities/name.h>
#include <sql/entities/tables.h>

namespace capemisa {
namespace sql {
namespace entities {

struct database
{

  friend std::ostream& operator<<(std::ostream& p_out,
                                  const database& p_database);

  database() = delete;

  inline explicit database(const name& p_name)
    : m_name(p_name)
  {}

  inline tables::const_iterator begin() const { return m_tables.begin(); }

  inline tables::iterator begin() { return m_tables.begin(); }

  inline tables::const_iterator end() const { return m_tables.end(); }

  inline tables::iterator end() { return m_tables.end(); }

  tables::iterator add(table&& p_table)
  {
    return m_tables.add(std::move(p_table));
  }

  tables::iterator find(const name& p_name) const
  {
    return m_tables.find(p_name);
  }

private:
  name m_name;
  tables m_tables;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // DATABASE_H
