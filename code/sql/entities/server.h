#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_SERVER_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_SERVER_H

#include <iostream>

#include <sql/entities/databases.h>
#include <sql/entities/name.h>

namespace capemisa {
namespace sql {
namespace entities {

struct host;

struct server
{

  friend std::ostream& operator<<(std::ostream& p_out, const server& p_server);

  server() = delete;

  inline explicit server(host* p_host, const name& p_name)
    : m_host(p_host)
    , m_name(p_name)
  {}

  inline const name& get_name() const { return m_name; }

  ptr<database> find(const name& p_database_name);

  inline uint16_t get_num_databases() const
  {
    return m_databases.get_size<uint16_t>();
  }

  inline ptr<database> get_database(uint16_t p_index)
  {
    return m_databases[p_index];
  }

  ptr<database> add_database(const name& p_database_name);

  host* get_host() const;

private:
  host* m_host;
  name m_name;
  databases m_databases;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // SERVER_H
