#ifndef CAPEMISA_SQL_ENTITIES_TABLE_H
#define CAPEMISA_SQL_ENTITIES_TABLE_H

#include <functional>
#include <iostream>
#include <list>
#include <memory>

#include <sql/entities/attribute_column.h>
#include <sql/entities/foreign_key.h>
#include <sql/entities/internal/collection.h>
#include <sql/entities/internal/types.h>
#include <sql/entities/name.h>
#include <sql/entities/primary_key.h>

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
    , m_primary_key(make_ptr<primary_key>(this))
  {}

  const database& get_database() const;

  inline const name& get_name() const { return m_name; }

  inline ptr<primary_key> get_primary_key() const { return m_primary_key; }

  inline ptr<attribute_column> add_attribute(const name& p_name,
                                             column::type p_type,
                                             size p_size,
                                             bool p_is_null,
                                             bool p_is_unique)
  {
    return m_attributes.add(
      p_name, this, p_type, p_size, p_is_null, p_is_unique);
  }

  inline ptr<attribute_column> add_attribute(const name& p_name,
                                             column::type p_type,
                                             bool p_is_null,
                                             bool p_is_unique)
  {
    return m_attributes.add(p_name, this, p_type, p_is_null, p_is_unique);
  }

  inline ptr<attribute_column> find_attribute(const name& p_attr_name) const
  {
    return m_attributes.find(p_attr_name);
  }

  inline ptr<foreign_key> add_fk(const name& p_fk_name)
  {
    return m_foreigners_keys.add(p_fk_name, this);
  }

  inline ptr<foreign_key> find_fk(const name& p_fk_name) const
  {
    return m_foreigners_keys.find(p_fk_name);
  }

private:
  typedef collection<attribute_column> attributes;
  typedef collection<foreign_key> foreigners_keys;

private:
  name m_name;
  const database* m_database;

  ptr<primary_key> m_primary_key;
  attributes m_attributes;
  foreigners_keys m_foreigners_keys;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // TABLE_H
