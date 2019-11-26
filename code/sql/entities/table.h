#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLE_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLE_H

#include <functional>
#include <iostream>
#include <list>
#include <memory>

#include <sql/entities/internal/types.h>
#include <sql/entities/name.h>
//#include <sql/entities/primary_key.h>

namespace capemisa {
namespace sql {
namespace entities {

struct database;

struct table
{

  friend std::ostream& operator<<(std::ostream& p_out, const table& p_table);

  table() = delete;

  table(const table&) = delete;
  table(table&&) = delete;
  table& operator=(const table&) = delete;
  table& operator=(table&&) = delete;
  ~table() = default;

  explicit inline table(const name& p_name, const database* p_database)
    : m_name(p_name)
    , m_database(p_database)
  {}

  const database& get_database() const;

  inline const name& get_name() const { return m_name; }

//  inline void add_primary_key(ptr<primary_key> p_primary_key)
//  {
//    m_primary_key = p_primary_key;
//  }

//  inline ptr<primary_key> get_primary_key() const { return m_primary_key; }

private:
    name m_name;
  const database*  m_database;

//  ptr<primary_key> m_primary_key;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // TABLE_H
