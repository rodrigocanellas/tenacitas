#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_DATABASE_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_DATABASE_H

#include <cstdint>
#include <iostream>

#include <sql/entities/name.h>
#include <sql/entities/table.h>
#include <sql/entities/tables.h>
#include <sql/entities/types.h>

namespace capemisa {
namespace sql {
namespace entities {

struct server;

struct database
{

  friend std::ostream& operator<<(std::ostream& p_out,
                                  const database& p_database);

  database() = delete;

  inline explicit database(server* p_server, const name& p_name)
    : m_server(p_server)
    , m_name(p_name)
  {}

  inline uint16_t get_num_tables() const
  {
    return m_tables.get_size<uint16_t>();
  }

  inline ptr<table> get_table(uint16_t p_index) { return m_tables[p_index]; }

  inline const name& get_name() const { return m_name; }

  ptr<table> add_table(const name& p_table_name);

  inline ptr<table> find(const name& p_table_name)
  {
    return m_tables.find([&p_table_name](const ptr<table>& p_table) -> bool {
      return p_table->get_name() == p_table_name;
    });
  }

  server* get_server() const;

private:
  server* m_server;
  name m_name;
  tables m_tables;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // DATABASE_H
