#ifndef CAPEMISA_SQL_ENTITIES_DATABASE_H
#define CAPEMISA_SQL_ENTITIES_DATABASE_H

#include <cstdint>
#include <iostream>

#include <sql/entities/table.h>
#include <sql/generic/collection.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace entities {

struct server;

struct database
{

  friend std::ostream& operator<<(std::ostream& p_out,
                                  const database& p_database);

  database() = delete;
  database(const database&) = delete;
  database(database&&) = delete;
  database& operator=(const database&) = delete;
  database& operator=(database&&) = delete;
  ~database() = default;

  inline explicit database(const generic::name& p_name, const server* p_server)
    : m_name(p_name)
    , m_server(p_server)
  {}

  inline uint16_t get_num_tables() const
  {
    return m_tables.get_size<uint16_t>();
  }

  inline generic::ptr<table> get_table(uint16_t p_index)
  {
    return m_tables[p_index];
  }

  inline const generic::name& get_name() const { return m_name; }

  inline generic::ptr<table> add_table(const generic::name& p_table_name)
  {
    return m_tables.add(p_table_name, this);
  }

  inline generic::ptr<table> find(const generic::name& p_table_name)
  {
    return m_tables.find(p_table_name);
  }

  const server& get_server() const;

private:
  typedef generic::collection<table> tables;

private:
  generic::name m_name;
  const server* m_server;

  tables m_tables;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // DATABASE_H
